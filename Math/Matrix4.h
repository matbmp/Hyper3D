#ifndef MATRIX4_H
#define MATRIX4_H

struct Vector4;
struct Vector3;
#include "Vector4.h"
#include "Vector3.h"
#include <cmath>

struct Matrix4{
public :
        float x11 = 0, x12 = 0, x13 = 0, x14 = 0,
              x21 = 0, x22 = 0, x23 = 0, x24 = 0,
              x31 = 0, x32 = 0, x33 = 0, x34 = 0,
              x41 = 0, x42 = 0, x43 = 0, x44 = 0;

        Matrix4 operator*(Matrix4 o);
        Matrix4 operator*(float o);
        Matrix4 operator+(Matrix4 o);
        Matrix4 operator-();
        Matrix4 operator-(Matrix4 o);
        Vector4 operator*(Vector4 o);
        Vector4 HMult(Vector4 o);
        Vector4 HMult2(Vector4 o);
        Matrix4 Inverse();
        Matrix4 Transpose();
        static Matrix4 identity();
        static Matrix4 scale(float x, float y, float z);
        static Matrix4 translation(float x, float y, float z);
        static Matrix4 translation(Vector3 v);
        static Matrix4 translationh(Vector4 v);
        static Matrix4 rotationOX(float x);
        static Matrix4 rotationOY(float x);
        static Matrix4 rotationOZ(float x);
        static Matrix4 translationhOX(float x);
        static Matrix4 translationhOY(float x);
        static Matrix4 translationhOZ(float x);
        static Matrix4 invScale(float x, float y, float z);
        static Matrix4 invTranslation(float x, float y, float z);
        static Matrix4 invRotationOX(float x);
        static Matrix4 invRotationOY(float x);
        static Matrix4 invRotationOZ(float x);
        static Matrix4 Perspective(float FOV, float size, float nearPlane, float farPlane);
};

#endif // MATRIX4_H
