#ifndef CPUGRAPHICSDEVICE_H
#define CPUGRAPHICSDEVICE_H

#include <functional>
#include <QImage>
#include <thread>
#include <queue>
#include <functional>
#include <mutex>
#include <typeindex>
#include <map>
#include <algorithm>

#include "interpolation.h"
#include "Shader.h"
#include "Math.h"
#include "rendertarget.h"



class CPUGraphicsDevice
{
private:
    RenderTarget *renderTarget;
public:
    RenderTarget defaultRenderTarget;
    int maxVertices;
    int numWorkers;
    bool enableCulling;

    std::map<std::type_index, void*> transformedMap;
    std::vector<bool> discarded;

    std::atomic<int> workersWorking;
    std::vector<std::thread> workers;
    std::mutex task_mutex;
    std::queue<std::function<void()> > tasks;
    std::chrono::nanoseconds sleep_time = std::chrono::nanoseconds(100000);

    std::mutex running;

    CPUGraphicsDevice(){}

    CPUGraphicsDevice(int width, int height, int numWorkers = 6)
    {
        this->defaultRenderTarget = RenderTarget(width, height, RenderTargetType::DRAWDEPTH);
        this->renderTarget = &defaultRenderTarget;
        this->maxVertices = 2000;
        this->numWorkers = numWorkers;

        this->discarded = std::vector<bool>(maxVertices, false);

        workersWorking = numWorkers;
        for(int i = 0; i < numWorkers; i++){
            this->workers.push_back(std::thread([this](){
                std::function<void()> task;
                task = nullptr;
                while(true){
                    task_mutex.lock();
                    if(!tasks.empty()){
                        task = tasks.front();
                        tasks.pop();
                    }
                    task_mutex.unlock();
                    if(task != nullptr){
                        task();
                    }else{
                        workersWorking--;
                        std::this_thread::sleep_for(sleep_time);
                        workersWorking++;
                    }
                    task = nullptr;
                }
            }));
        }
    }

    void setRenderTarget(RenderTarget *target){
        if(target == NULL){
            renderTarget = &defaultRenderTarget;
        } else {
            renderTarget = target;
        }
    }

    void ClearZBuffer(){
        int n = renderTarget->height / numWorkers;
        for(int i = 0; i < numWorkers-1; i++){
            ClearZBufferTask( n*i, n*(i+1));
        }
        ClearZBufferTask(n*(numWorkers-1), renderTarget->height);
        Join();
    }

    void Fill(uint color){
        int n = renderTarget->height / numWorkers;
        for(int i = 0; i < numWorkers-1; i++){
            FillTask(color, n*i, n*(i+1));
        }
        FillTask(color, n*(numWorkers-1), renderTarget->height);
        Join();
    }
    template<typename InVertex, typename InPixel>
    void DrawTriangles(const std::vector<InVertex> &vertices, const std::vector<short> &indices, Shader<InVertex, InPixel> shader){
        addTask([this, vertices, indices, shader](){DrawTraingles(vertices, indices, shader);});
    }

    void addTask(std::function<void()> task){
        task_mutex.lock();
        tasks.push(task);
        task_mutex.unlock();
    }

    void Join(){
        while(true){
            if(workersWorking == 0){
                task_mutex.lock();
                if(tasks.empty()){
                    task_mutex.unlock();
                    return;
                }
                task_mutex.unlock();
            }
            std::this_thread::sleep_for(sleep_time);
        }
    }

    void FillTask(uint color, int lineStart, int lineEnd){
        addTask([this, color, lineStart, lineEnd](){PrivateFill(color, lineStart, lineEnd);});
    }
    void PrivateFill(uint color, int lineStart, int lineEnd){
        uint *pos, *end;
        pos = renderTarget->backbuffer + (lineStart*renderTarget->width);
        end = renderTarget->backbuffer + (lineEnd*renderTarget->width);
        while(pos != end){
            *(pos++) = color;
        }
    }
    void ClearZBufferTask(int lineStart, int lineEnd){
        addTask([this, lineStart, lineEnd](){PrivateClearZBuffer(lineStart, lineEnd);});
    }

    void PrivateClearZBuffer(int lineStart, int lineEnd){
        float *pos, *end;
        pos = renderTarget->zbuffer + (lineStart*renderTarget->width);
        end = renderTarget->zbuffer + (lineEnd*renderTarget->width);
        while(pos != end){
            *(pos++) = 10e4F;
        }
    }

    template<typename InVertex, typename InPixel>
    void TransformTask(std::vector<InPixel> *transformed, const std::vector<InVertex> &vertices, Shader<InVertex, InPixel> *shader, int startIndex, int endIndex){
        std::vector<bool> *discarded = &(this->discarded);
        addTask([discarded, transformed, vertices, shader, startIndex, endIndex](){
            for(int i = startIndex; i < endIndex; i++){
                shader->TransformVertex(vertices[i], transformed->operator[](i));
                float invW = (1.0F/transformed->operator[](i).projected.w);
                transformed->operator[](i).projected.x *= invW;
                transformed->operator[](i).projected.y *= invW;
                transformed->operator[](i).projected.w *= invW;
                if(transformed->at(i).projected.z < 0.0F || transformed->at(i).projected.z > 1.0F) discarded->operator[](i) = true;
                else discarded->operator[](i) = false;
            }
        });
    }

    template<typename InVertex, typename InPixel>
    void DrawTraingles(const std::vector<InVertex> &vertices, const std::vector<short> &indices, Shader<InVertex, InPixel> *shader){
        if(vertices.size() > maxVertices){
            throw std::invalid_argument("This CPUGraphicsDevice supports rendering meshes with at most "
                                        + std::to_string(maxVertices) + " vertices,"
                                         "but received " + std::to_string(vertices.size()) + " vertices");
        }

        const std::type_info &type = typeid(InPixel);
        if(transformedMap.find(type) == transformedMap.end()){
            transformedMap[type] = new std::vector<InPixel>(maxVertices, InPixel());
        }
        std::vector<InPixel> *transformed = (std::vector<InPixel>*)transformedMap[type];

        shader->zbuffer = renderTarget->zbuffer;
        shader->backbuffer = renderTarget->backbuffer;
        shader->width = renderTarget->width;
        shader->target = renderTarget;

        // przeksztalcenie wszystkich vertexow
        if(false){
            int n = vertices.size() / numWorkers;
            for(int i = 0; i < numWorkers-1; i++){
                TransformTask(transformed, vertices, shader, n*i, n*(i+1));
            }
            TransformTask(transformed, vertices, shader, n*(numWorkers-1), vertices.size());
            Join();
        } else{
            for(uint i = 0; i < vertices.size(); i++){
                shader->TransformVertex(vertices[i], transformed->operator[](i));
                float invW = (1.0F/transformed->operator[](i).projected.w);
                transformed->operator[](i).projected.x *= invW;
                transformed->operator[](i).projected.y *= invW;
                transformed->operator[](i).projected.w *= invW;
                if(transformed->at(i).projected.z < 0.0F || transformed->at(i).projected.z > 1.0F) discarded[i] = true;
                else discarded[i] = false;
            }
        }
        int size = indices.size();
        int batch = size/(numWorkers * 3);
        if(batch == 0){
            batch = 3;
        }else{
            while(batch % 3 != 0) batch++;
        }
        int b = 0;
        while(b < size){
            TaskDrawTriangles(transformed, indices, shader, b, std::min(b+batch, size));
            b+=batch;
        }
        Join();

    }

    template<typename InVertex, typename InPixel>
    void TaskDrawTriangles(std::vector<InPixel> *transformed ,const std::vector<short> &indices,
                             Shader<InVertex, InPixel> *shader,
                           int startIndex, int endIndex){
        addTask([this, transformed, indices, shader, startIndex, endIndex](){
           InnerDrawTriangles2(transformed, indices, shader, startIndex, endIndex);
        });
    }

    template<typename InVertex, typename InPixel>
    void InnerDrawTriangles2(std::vector<InPixel> *transformed ,const std::vector<short> &indices,
                       Shader<InVertex, InPixel> *shader,
                       int startIndex, int endIndex){
        int width = renderTarget->width, height = renderTarget->height;
        BarycentricInterpolator<InVertex, InPixel> bi = BarycentricInterpolator<InVertex, InPixel>();
        bi.shader = shader;
        bi.acceptPoint = [width, height](int x, int y){
            return (x >= 0) && (y >= 0) && (x<width) && (y<height);
        };
        int i1, i2, i3;
        Vector2 v1Proj, v2Proj, v3Proj;
        InPixel v1, v2, v3;
        Vector2 halfBuffDim = renderTarget->halfBuffDim;
        for(int i = startIndex; i < endIndex; i+=3){
            i1 = indices[i];
            i2 = indices[i+1];
            i3 = indices[i+2];

            if(discarded[i1] && discarded[i2] && discarded[i3]) continue;

            v1 = transformed->at(i1);
            v2 = transformed->at(i2);
            v3 = transformed->at(i3);

            v1Proj = Vector2(v1.projected.x, v1.projected.y);
            v2Proj = Vector2(v2.projected.x, v2.projected.y);
            v3Proj = Vector2(v3.projected.x, v3.projected.y);

            if(enableCulling){
                Vector3 iloczyn = Math::IloczynWektorowy(v2.projected-v1.projected, v3.projected-v1.projected);
                if(iloczyn.z > 0) continue;
            }

            shader->ForTriangle(v1, v2, v3);

            // czy trojkat na ekranie
            if(v1Proj.x < -1.0F && v2Proj.x < -1.0F && v3Proj.x < -1.0F) continue;
            if(v1Proj.x > 1.0F && v2Proj.x > 1.0F && v3Proj.x > 1.0F) continue;
            if(v1Proj.y < -1.0F && v2Proj.y < -1.0F && v3Proj.y < -1.0F) continue;
            if(v1Proj.y > 1.0F && v2Proj.y > 1.0F && v3Proj.y > 1.0F) continue;

            if(std::abs(v1Proj.x) > 10.0F || std::abs(v1Proj.y) > 10.0F) continue;
            if(std::abs(v2Proj.x) > 10.0F || std::abs(v2Proj.y) > 10.0F) continue;
            if(std::abs(v3Proj.x) > 10.0F || std::abs(v3Proj.y) > 10.0F) continue;

            // przeksztalcenie do wspolrzednych ekranowych
            v1Proj.y = -v1Proj.y;
            v2Proj.y = -v2Proj.y;
            v3Proj.y = -v3Proj.y;
            v1Proj = (v1Proj*halfBuffDim)+halfBuffDim;
            v2Proj = (v2Proj*halfBuffDim)+halfBuffDim;
            v3Proj = (v3Proj*halfBuffDim)+halfBuffDim;

            // interpolacja trojkata
            bi.setTarget(Point2(v1Proj), Point2(v2Proj), Point2(v3Proj));
            bi.interpolate(v1 ,v2, v3);
        }
    }

    void rysujLinie(QImage *image, int x1, int y1, int x2, int y2, int color){
        double a, b;
        int i, d, f;

        if(x1 == x2){
            // pionowa linia
            d = (y1 < y2)? 1 : -1;


            for(i = y1; i != y2 + d; i +=d){
                rysujPixel(image, x1, i, color);
            }
        }else{
            a = (y1-y2)/((double)(x1-x2));
            b = y1 - (a*x1);

            if(a >= -1 && a <= 1){
                // bardziej plaska niz stroma linia
                d = (x1 < x2) ? 1 : -1;

                for(i = x1; i != x2; i+=d){
                    f = (int)(a*i + b);
                    rysujPixel(image, i, f, color);
                }
            }else{
                // bardziej stroma niz plaska linia
                a = (x1-x2)/((double)(y1-y2));
                b = x1 - (a*y1);
                d = (y1 < y2) ? 1 : -1;

                for(i = y1; i != y2; i+=d){
                    f = (int)(a*i + b);
                    rysujPixel(image, f, i, color);
                }
            }
        }
    }

    void rysujPixel(QImage *img, int x, int y, int color){
        if(naObrazie(img, x, y)){
            ((int*)img->bits())[y * img->width() + x] = color;
        }
    }
    bool naObrazie(QImage *img, int x, int y){
        return x>=0 && x < img->width() && y >= 0  && y < img->height();
    }
};

#endif // CPUGRAPHICSDEVICE_H
