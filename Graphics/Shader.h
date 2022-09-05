#ifndef SHADER_H
#define SHADER_H

#include <functional>
#include <vector>
#include <algorithm>
#include "vertex.h"
#include "lighting.h"
#include "Math/Matrix4.h"
#include "Game/HMath.h"

template <typename InVertex, typename InPixel>
class VertexShader{
public:
    virtual void TransformVertex(const InVertex &vertex, InPixel &o){};
    virtual void ForTriangle(InPixel &v1, InPixel &v2, InPixel &v3){}
    void init(){
    }
};

template <typename InPixel>
class PixelShader{
public:
    virtual uint DrawPixel(const InPixel &vertex){};
    virtual void Interpolate(const InPixel &A, const InPixel &B, const InPixel &C, InPixel &D, float u, float v, float w){};
};

template <typename InVertex, typename InPixel>
class Shader: public VertexShader<InVertex, InPixel>, public PixelShader<InPixel>{
public:
    float *zbuffer;
    uint *backbuffer;
    int width;
    RenderTarget *target;


    void Draw(const Point2 screenPos, InPixel &vertex){
        uint i = screenPos.y * width + screenPos.x;
        float z = vertex.position.z;
        if(z < zbuffer[i]){
            zbuffer[i] = z;
            backbuffer[i] = this->DrawPixel(vertex);
        }
    }
};

class HyperbolicPCShader: public Shader<PCVertex, PCPVertex>{
public:

    Matrix4 projectionMatrix = Matrix4::Perspective(90.0F, 0.1F, 0.00001F, 2.0F);
    GyroVector objectPos;
    GyroVector rotatedCamera;
    HyperbolicLighting lighting;
    Sampler<uint> textureSampler;
    bool enableTexture;
    float fBK = 1.0F;
    float shadow_eps = 0.005F;
    float depth_shadow_eps = 0.01F;
    int shadow_opt = 0;
    HyperbolicPCShader(){
        init();
    }
    void TransformVertex(const PCVertex &vertex, PCPVertex &o){
        GyroVector p = objectPos + GyroVector(Vector3(vertex.position));
        o.original = p.vec;
        o.normal = (GyroVector(Vector3(), objectPos.gyr) + GyroVector(vertex.normal)).vec;
        p = -rotatedCamera + p;
        p.vec = HMath2::Any_Any(p.vec, 1.0F, fBK);
        o.position = Vector4(p.vec);
        o.projected = (projectionMatrix)* o.position;
        o.color = vertex.color;
        o.texUV = vertex.texUV;
    }
    void ForTriangle(PCPVertex &v1, PCPVertex &v2, PCPVertex &v3){
        v1.color = lighting.CalculateWOspecular(v1.color, v1.normal, v1.original);
        v2.color = lighting.CalculateWOspecular(v2.color, v2.normal, v2.original);
        v3.color = lighting.CalculateWOspecular(v3.color, v3.normal, v3.original);
    }

    uint DrawPixel(const PCPVertex &vertex){
        float light = 1.0F;
        Color c = Color(1,1,1);
        float illum = 1.0F;
        if(shadow_opt != 0){
            Vector3 ls;
            Vector3 p; // pixel in Poincare camera view
            p = HMath2::Any_Any(vertex.position, fBK, 1.0F);

            // gyrovector addition optimized
            ls = HMath::MobiusAddition(lighting.lights[0].minusPosition.vec, lighting.lights[0].position.gyr * p);
            //GyroVector lightSpace = lighting.lights[0].position + p;  // Poincare light-to-pixel (light.position is in Poincare camera view)

            ls = HMath2::Poincare_BK(ls);
            float sceneDepth = ls.Normalize();
            if(shadow_opt == 3){
                float innerRing = 0.01F;
                float outerRing = 0.03F;
                Vector3 v = Vector3(0, 0, 1.0F);
                float l = (ls-v).LengthSquared();
                if(l < innerRing){
                    illum = 1.0F;
                } else if(l < outerRing){
                    illum = (innerRing-outerRing)/(l-outerRing);
                } else{
                    illum = 10e5F;
                }
                illum = 3.0F / (illum * sceneDepth);
            }
            float z1;
            if(ls.z >= 0.0F){
                z1 = 1.0F + ls.z;
            }else{
                z1 = 1.0F - ls.z;
            }
            if(shadow_opt == 2){
                c = Color::FromHSV(std::atan2(ls.x, z1), 1, 1);
            }

            ls.x = (ls.x/z1) * 0.5F + 0.5F;
            ls.y = 1.0F - ((ls.y/z1) * 0.5F + 0.5F);

            if(ls.z >= 0.0F){
                light = lighting.lights[0].lightMapFrontSampler.softshadowSample(ls.x, ls.y, sceneDepth - shadow_eps - sceneDepth*depth_shadow_eps);
            } else{
                light = lighting.lights[0].lightMapBackSampler.softshadowSample(ls.x, ls.y, sceneDepth - shadow_eps - sceneDepth*depth_shadow_eps);
            }

        }
        Color final;
        final = (c * vertex.color) * (lighting.ambient + std::max(illum, 0.0F) * light);
        if(enableTexture){
            Color texColor = Color(uint(textureSampler.flatSample(vertex.texUV.x, vertex.texUV.y)));
            final = final * texColor;
        }
        return (final).ToUint();
    }

    void TransformLight(const GyroVector &original, GyroVector &o){
        GyroVector v = (-rotatedCamera + original);
        o = v;
    }

    void Interpolate(const PCPVertex &A, const PCPVertex &B, const PCPVertex &C, PCPVertex &D, float u, float v, float w){
        D.position.x = u * A.position.x + v * B.position.x + w * C.position.x;
        D.position.y = u * A.position.y + v * B.position.y + w * C.position.y;
        D.position.z = u * A.position.z + v * B.position.z + w * C.position.z;

        D.color.r = u * A.color.r + v * B.color.r + w * C.color.r;
        D.color.g = u * A.color.g + v * B.color.g + w * C.color.g;
        D.color.b = u * A.color.b + v * B.color.b + w * C.color.b;

        D.texUV.x = u * A.texUV.x + v * B.texUV.x + w * C.texUV.x;
        D.texUV.y = u * A.texUV.y + v * B.texUV.y + w * C.texUV.y;
    };

};


class HyperbolicShadowShader : public Shader<PCVertex, PCPVertex>{
public:
     GyroVector rotatedCamera;
     GyroVector objectPos;
     float dir; // -1 or 1 for two sheets of paraboloid
     void TransformVertex(const PCVertex &vertex, PCPVertex &o){
         GyroVector p = objectPos + GyroVector(Vector3(vertex.position));
         p = -rotatedCamera + p;
         o.position = Vector4(HMath2::Poincare_BK(p.vec));
         o.position.z *= dir;
         float l = o.position.Normalize();
         o.projected.z = o.position.z;
         float z1 = o.position.z + 1.0F;
         o.position.x /= z1;
         o.position.y /= z1;
         o.position.z = l; //depth
         o.projected.x = o.position.x;
         o.projected.y = o.position.y;
     }
     void ForTriangle(PCPVertex &v1, PCPVertex &v2, PCPVertex &v3){
     }

     uint DrawPixel(const PCPVertex &vertex){
         return 0;
     }

     void Interpolate(const PCPVertex &A, const PCPVertex &B, const PCPVertex &C, PCPVertex &D, float u, float v, float w){
         D.position.z = u * A.position.z + v * B.position.z + w * C.position.z;
     };
};

#endif // SHADER_H
