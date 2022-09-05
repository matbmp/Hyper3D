#ifndef SAMPLER_H
#define SAMPLER_H

#include <QImage>
#include <algorithm>

template<typename T>
class Sampler{
private:
    T *data;
    int width;
    int height;
    int w1;
    int h1;
    T samp(int x, int y){
        return data[y*width + x];
    }
public:
    Sampler(){}
    Sampler(T *data, int width, int height){
        this->data = data;
        this->width = width;
        this->height = height;
        this->w1 = width-1;
        this->h1 = height-1;
    }
    Sampler(QImage *img){
        this->data = (uint*)img->bits();
        this->width = img->width();
        this->height = img->height();
        this->w1 = width-1;
        this->h1 = height-1;
    }
    T flatSample(float x, float y){
        int xx = int(x * width);
        int yy = int(y * height);
        xx = std::min(w1, std::max(xx, 0));
        yy = std::min(h1, std::max(yy, 0));
        return data[int(yy*width + xx)];
    }
    T bilinearShadowSample(float x, float y, float cutoff){
        int x1 = int(x * width);
        int y1 = int(y * height);
        int x2 = x1+1;
        int y2 = y1+1;
        float a = x*width - x1;
        float b = y*height - y1;

        x1 = std::min(w1, std::max(x1, 0));
        y1 = std::min(h1, std::max(y1, 0));
        x2 = std::min(w1, std::max(x2, 0));
        y2 = std::min(h1, std::max(y2, 0));

        /*
        int f1 = (samp(x1, y1) > cutoff) == 1 ? 1 : 0;
        int f2 = (samp(x1, y2) > cutoff) == 1 ? 1 : 0;
        int f3 = (samp(x2, y1) > cutoff) == 1 ? 1 : 0;
        int f4 = (samp(x2, y2) > cutoff) == 1 ? 1 : 0;
        */

        return (1 + (1-a) * ((1-b)*(samp(x1, y1) > cutoff)+ b * (samp(x1, y2) > cutoff))
                + a * ((1-b)*(samp(x2, y1) > cutoff) + b * (samp(x2,y2) > cutoff)))/5.0F;

    }
    T softshadowSample(float X, float Y, float cutoff){
        int count = 0;
        int xx = int(X * width);
        int yy = int(Y * height);
        int x, y;

        for(int i = -1; i <= 1; i++){
            y = yy + i;
            y = std::min(h1, std::max(y, 0));
            for(int j = -1; j <= 1; j++){
                x = xx + j;
                x = std::min(w1, std::max(x, 0));
                if(samp(x, y) > cutoff) count+= 3 - (std::abs(i) + std::abs(j));
            }

        }
        return count/27.0F;
    }
};

#endif // SAMPLER_H
