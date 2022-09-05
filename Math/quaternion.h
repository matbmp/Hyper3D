#ifndef QUATERNION_H
#define QUATERNION_H

#include "cmath"

struct Vector3;
#include "Vector3.h"

struct Quaternion
{
public:
    float x = 0, y = 0, z = 0, w = 1;

    Quaternion();
    Quaternion(float x, float y, float z, float w);

    static Quaternion CreateFromAxisAngle(Vector3 axis, float angle);
    static Quaternion CreateFromYawPitchRoll(float yaw, float pitch, float roll);
    void Normalize();
    float Length();
    Quaternion operator *(Quaternion b) const;
    static Quaternion Inverse(const Quaternion &q);
    Vector3 operator*(Vector3 value);
};

#endif // QUATERNION_H
