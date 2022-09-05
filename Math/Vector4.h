#ifndef VECTOR4_H
#define VECTOR4_H

struct Vector3;
struct Vector2;
struct Matrix4;
#include "Vector3.h"

struct Vector4{
public:
    float x, y, z, w;
    Vector4();
    Vector4(Vector3 v);
    Vector4(float X, float Y, float Z);
    Vector4(float X, float Y, float Z, float W);
    Vector4 operator-();
    Vector4 operator-(Vector4 v);
    Vector4 operator+(Vector4 v);
    Vector4 operator+(float f);
    Vector4 operator*(float f) const;
    Matrix4 multiplyToMatrix(Vector4 v);
    float Normalize();
    float Length();
    float LengthSquared();
    float Hw();
    Vector4 moveH(Vector4 v);
    static Vector4 Lerp(Vector4 a, Vector4 b, float t);
    static float Dot(Vector4 v1, Vector4 v2);
};


#endif // VECTOR4_H
