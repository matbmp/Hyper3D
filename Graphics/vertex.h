#ifndef VERTEX_H
#define VERTEX_H

#include "Math/Vector4.h"
#include "Color.h"

template<typename T>
struct IVertex{
    virtual void Interpolate(const T &A, const T &B, const T &C, T &D, float u, float v, float w) = 0;
};

struct PNCVertex : IVertex<PNCVertex>
{
public:
    Vector4 position;
    Vector4 normal;
    Color color;

    PNCVertex(): position(Vector4()), normal(Vector4()), color(Color()){}
    PNCVertex(Vector3 position, Vector3 normal, Color color):position(Vector4(position)), normal(normal), color(color){}
    PNCVertex(Vector4 position, Vector4 normal, Color color):position(position), normal(normal), color(color){}


    PNCVertex operator*(float f){
        return PNCVertex(position*f, normal*f, color*f);
    }
    PNCVertex operator+(PNCVertex o){
        return PNCVertex(position + o.position, normal+o.normal, color+o.color);
    }
    void Interpolate(const PNCVertex &A, const PNCVertex &B, const PNCVertex &C, PNCVertex &D, float u, float v, float w){
        /*
        D.position = A.position * u + B.position * v + C.position * w;
        D.normal = A.normal * u + B.normal * v + C.normal * w;
        D.color = A.color * u + B.color * v + C.color * w;
        */

        D.position.x = u * A.position.x + v * B.position.x + w * C.position.x;
        D.position.y = u * A.position.y + v * B.position.y + w * C.position.y;
        D.position.z = u * A.position.z + v * B.position.z + w * C.position.z;
        D.position.w = 1.0F;

        D.normal.x = u * A.normal.x + v * B.normal.x + w * C.normal.x;
        D.normal.y = u * A.normal.y + v * B.normal.y + w * C.normal.y;
        D.normal.z = u * A.normal.z + v * B.normal.z + w * C.normal.z;
        D.normal.w = 1.0F;

        D.color.r = u * A.color.r + v * B.color.r + w * C.color.r;
        D.color.g = u * A.color.g + v * B.color.g + w * C.color.g;
        D.color.b = u * A.color.b + v * B.color.b + w * C.color.b;

    }
};

struct PVertex{
    Vector4 position;

    PVertex():position(Vector4()){}
    PVertex(Vector3 position):position(Vector4(position)){}
    PVertex(Vector4 position):position(position){}
};

struct PCVertex{
    Vector4 position;
    Vector4 normal;
    Color color;
    Vector2 texUV;


    PCVertex():position(Vector4()){}
    PCVertex(Vector3 position, Color color):position(Vector4(position)), color(color){}
    PCVertex(Vector3 position, Color color, Vector2 texUV):position(Vector4(position)), normal(normal), color(color), texUV(texUV){}
    PCVertex(Vector4 position, Vector4 normal, Color color, Vector2 texUV):position(position), normal(normal), color(color), texUV(texUV){}
};

struct PCPVertex{
    Vector4 original;
    Vector4 position;
    Vector4 normal;
    Vector4 projected;
    Color color;
    Vector2 texUV;
};

struct PPVertex{
    Vector4 position;
    Vector4 projected;
};

#endif // VERTEX_H
