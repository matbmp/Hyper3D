#include "Math.h"
#include <cmath>
#include "Game/HMath.h"

Vector4::Vector4(): x(0), y(0), z(0), w(1){}
Vector4::Vector4(float X, float Y, float Z, float W): x(X), y(Y), z(Z), w(W){}
Vector4::Vector4(float X, float Y, float Z): x(X), y(Y), z(Z), w(1){}
Vector4::Vector4(Vector3 v): x(v.x), y(v.y), z(v.z), w(1){}

Vector4 Vector4::operator-(){ return Vector4(-x, -y, -z, 1);}
Vector4 Vector4::operator-(Vector4 v){ return Vector4(x-v.x, y-v.y, z-v.z, 1);}
Vector4 Vector4::operator+(Vector4 v){ return Vector4(x+v.x, y+v.y, z+v.z, 1);}
Vector4 Vector4::operator+(float f){ return Vector4(x+f, y+f, z+f, 1);}
Vector4 Vector4::operator*(float f)const{ return Vector4(x*f, y*f, z*f, 1);}

Matrix4 Vector4::multiplyToMatrix(Vector4 v){
    Matrix4 m;
    m.x11 = x*v.x; m.x12 = x*v.y; m.x13 = x*v.z; m.x14 = x*v.w;
    m.x21 = y*v.x; m.x22 = y*v.y; m.x23 = y*v.z; m.x24 = y*v.w;
    m.x31 = z*v.x; m.x32 = z*v.y; m.x33 = z*v.z; m.x34 = z*v.w;
    m.x41 = w*v.x; m.x42 = w*v.y; m.x43 = w*v.z; m.x44 = w*v.w;
    return m;
}


float Vector4::Dot(Vector4 v1, Vector4 v2){
    return v1.x*v2.x + v1.y * v2.y + v1.z * v2.z + v1.w *v2.w;
}

float Vector4::Length(){
    return std::sqrt(x*x + y*y +z*z);
}
float Vector4::LengthSquared(){
    return (x*x + y*y +z*z);
}
float Vector4::Normalize(){
    w = 1.0F;
    float l = Length();
    if(l != 0){
        float il = (1.0F/l);
        x*=il;
        y*=il;
        z*=il;
    }
    return l;
}

float Vector4::Hw(){
    return std::sqrt(1 + LengthSquared());
}

Vector4 Vector4::moveH(Vector4 v){
    return Matrix4::translationh(v).HMult(*this);
}

Vector4 Vector4::Lerp(Vector4 a, Vector4 b, float t){
    return (a + (b+(-a))*t);
}
