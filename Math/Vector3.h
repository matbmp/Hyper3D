#ifndef VECTOR3_H
#define VECTOR3_H

struct Vector2;
struct Vector4;
struct Quaternion;
#include "Vector2.h"
#include "Vector4.h"
#include "quaternion.h"

struct Vector3{
public:
    float x, y, z;

    Vector3();
    Vector3(float X, float Y, float Z);
    Vector3(Vector4 v);
    Vector3 operator+(Vector3 o) const;
    Vector3 operator-(Vector3 o);
    Vector3 operator-();
    Vector3 operator*(float o);
    friend Vector3 operator*(float o, Vector3);

    Vector2 Perspective(float d);
    float Normalize();
    float Length();
    float LengthSquared();
    static float Dot(const Vector3 &a, const Vector3 &b);
};

#endif // VECTOR3_H
