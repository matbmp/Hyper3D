#ifndef MATRIX3_H
#define MATRIX3_H

#include "Vector3.h"

struct Matrix3{
public :
        float x11 = 0, x12 = 0, x13 = 0,
              x21 = 0, x22 = 0, x23 = 0,
              x31 = 0, x32 = 0, x33 = 0;
        Matrix3 operator*(Matrix3 o);
        Vector3 operator*(Vector3 o);
        static Matrix3 identity();
        static Matrix3 rotation(float alpha);
        static Matrix3 translation(float x, float y);
        static Matrix3 scale(float x, float y);
        static Matrix3 shearingX(float x);
        static Matrix3 shearingY(float x);
        static Matrix3 invRotation(float alpha);
        static Matrix3 invTranslation(float x, float y);
        static Matrix3 invScale(float x, float y);
        static Matrix3 invShearingX(float x);
        static Matrix3 invShearingY(float x);
};

#endif // MATRIX3_H
