#include <QtMath>
#include "Matrix4.h"
#include "Game/HMath.h"

Matrix4 Matrix4::operator*(Matrix4 o){
    Matrix4 r;
    r.x11 = x11*o.x11 + x12*o.x21 + x13*o.x31 + x14*x41;
    r.x12 = x11*o.x12 + x12*o.x22 + x13*o.x32 + x14*x42;
    r.x13 = x11*o.x13 + x12*o.x23 + x13*o.x33 + x14*x43;
    r.x14 = x11*o.x14 + x12*o.x24 + x13*o.x34 + x14*x44;
    r.x21 = x21*o.x11 + x22*o.x21 + x23*o.x31 + x24*x41;
    r.x22 = x21*o.x12 + x22*o.x22 + x23*o.x32 + x24*x42;
    r.x23 = x21*o.x13 + x22*o.x23 + x23*o.x33 + x24*x43;
    r.x24 = x21*o.x14 + x22*o.x24 + x23*o.x34 + x24*x44;
    r.x31 = x31*o.x11 + x32*o.x21 + x33*o.x31 + x34*x41;
    r.x32 = x31*o.x12 + x32*o.x22 + x33*o.x32 + x34*x42;
    r.x33 = x31*o.x13 + x32*o.x23 + x33*o.x33 + x34*x43;
    r.x34 = x31*o.x14 + x32*o.x24 + x33*o.x34 + x34*x44;
    r.x41 = x41*o.x11 + x42*o.x21 + x43*o.x31 + x44*x41;
    r.x42 = x41*o.x12 + x42*o.x22 + x43*o.x32 + x44*x42;
    r.x43 = x41*o.x13 + x42*o.x23 + x43*o.x33 + x44*x43;
    r.x44 = x41*o.x14 + x42*o.x24 + x43*o.x34 + x44*x44;
    return r;
}

Matrix4 Matrix4::operator*(float o){
    Matrix4 r;
    r.x11 = x11 * o; r.x12 = x12 * o; r.x13 = x13 * o; r.x14 = x14 * o;
    r.x21 = x21 * o; r.x22 = x22 * o; r.x23 = x23 * o; r.x24 = x24 * o;
    r.x31 = x31 * o; r.x32 = x32 * o; r.x33 = x33 * o; r.x34 = x34 * o;
    r.x41 = x41 * o; r.x42 = x42 * o; r.x43 = x43 * o; r.x44 = x44 * o;
    return r;
}
Matrix4 Matrix4::operator+(Matrix4 o){
    Matrix4 m;
    m.x11 = x11+o.x11, m.x12 = x12+o.x12, m.x13 = x13+o.x13, m.x14 = x14+o.x14;
    m.x21 = x21+o.x21, m.x22 = x22+o.x22, m.x23 = x23+o.x23, m.x24 = x24+o.x24;
    m.x31 = x31+o.x31, m.x32 = x32+o.x32, m.x33 = x33+o.x33, m.x34 = x34+o.x34;
    m.x41 = x41+o.x41, m.x42 = x42+o.x42, m.x43 = x43+o.x43, m.x44 = x44+o.x44;
    return m;
}

Matrix4 Matrix4::operator-(){
    Matrix4 m;
    m.x11 = -x11, m.x12 = -x12, m.x13 = -x13, m.x14 = -x14;
    m.x21 = -x21, m.x22 = -x22, m.x23 = -x23, m.x24 = -x24;
    m.x31 = -x31, m.x32 = -x32, m.x33 = -x33, m.x34 = -x34;
    m.x41 = -x41, m.x42 = -x42, m.x43 = -x43, m.x44 = -x44;
    return m;
}

Matrix4 Matrix4::operator-(Matrix4 o){
    return (*this + (-o));
}
Vector4 Matrix4::operator*(Vector4 o){
    return Vector4(x11*o.x + x12*o.y + x13*o.z + x14*o.w,
                   x21*o.x + x22*o.y + x23*o.z + x24*o.w,
                   x31*o.x + x32*o.y + x33*o.z + x34*o.w,
                   x41*o.x + x42*o.y + x43*o.z + x44*o.w);
}

Vector4 Matrix4::HMult(Vector4 o){
    o.w = o.Hw();
    return Vector4( x21*o.w + x22*o.x + x23*o.y + x24*o.z,
                    x31*o.w + x32*o.x + x33*o.y + x34*o.z,
                    x41*o.w + x42*o.x + x43*o.y + x44*o.z,
                    x11*o.w + x12*o.x + x13*o.y + x14*o.z);
}

Vector4 Matrix4::HMult2(Vector4 o){
    o.w = o.Hw();
    return Vector4( x11*o.x + x12*o.y + x13*o.z + x14*o.w,
                    x21*o.x + x22*o.y + x23*o.z + x24*o.w,
                    x31*o.x + x32*o.y + x33*o.z + x34*o.w,
                    x41*o.x + x42*o.y + x43*o.z + x44*o.w);
}

// https://stackoverflow.com/questions/1148309/inverting-a-4x4-matrix
Matrix4 Matrix4::Inverse(){
    float A2323 = x33 * x44 - x34 * x43 ;
    float A1323 = x32 * x44 - x34 * x42 ;
    float A1223 = x32 * x43 - x33 * x42 ;
    float A0323 = x31 * x44 - x34 * x41 ;
    float A0223 = x31 * x43 - x33 * x41 ;
    float A0123 = x31 * x42 - x32 * x41 ;
    float A2313 = x23 * x44 - x24 * x43 ;
    float A1313 = x22 * x44 - x24 * x42 ;
    float A1213 = x22 * x43 - x23 * x42 ;
    float A2312 = x23 * x34 - x24 * x33 ;
    float A1312 = x22 * x34 - x24 * x32 ;
    float A1212 = x22 * x33 - x23 * x32 ;
    float A0313 = x21 * x44 - x24 * x41 ;
    float A0213 = x21 * x43 - x23 * x41 ;
    float A0312 = x21 * x34 - x24 * x31 ;
    float A0212 = x21 * x33 - x23 * x31 ;
    float A0113 = x21 * x42 - x22 * x41 ;
    float A0112 = x21 * x32 - x22 * x31 ;

    float det = x11 * ( x22 * A2323 - x23 * A1323 + x24 * A1223 )
        - x12 * ( x21 * A2323 - x23 * A0323 + x24 * A0223 )
        + x13 * ( x21 * A1323 - x22 * A0323 + x24 * A0123 )
        - x14 * ( x21 * A1223 - x22 * A0223 + x23 * A0123 ) ;
    det = 1 / det;

    Matrix4 m;
    m.x11 = det * ( x22 * A2323 - x23 * A1323 + x24 * A1223 );
    m.x12 = det * - ( x12 * A2323 - x13 * A1323 + x14 * A1223 );
    m.x13 = det *   ( x12 * A2313 - x13 * A1313 + x14 * A1213 );
    m.x14 = det * - ( x12 * A2312 - x13 * A1312 + x14 * A1212 );
    m.x21 = det * - ( x21 * A2323 - x23 * A0323 + x24 * A0223 );
    m.x22 = det *   ( x11 * A2323 - x13 * A0323 + x14 * A0223 );
    m.x23 = det * - ( x11 * A2313 - x13 * A0313 + x14 * A0213 );
    m.x24 = det *   ( x11 * A2312 - x12 * A0312 + x14 * A0212 );
    m.x31 = det *   ( x21 * A1323 - x22 * A0323 + x24 * A0123 );
    m.x32 = det * - ( x11 * A1323 - x12 * A0323 + x14 * A0123 );
    m.x33 = det *   ( x11 * A1313 - x12 * A0313 + x14 * A0113 );
    m.x34 = det * - ( x11 * A1312 - x12 * A0312 + x14 * A0112 );
    m.x31 = det * - ( x21 * A1223 - x22 * A0223 + x23 * A0123 );
    m.x42 = det *   ( x11 * A1223 - x12 * A0223 + x13 * A0123 );
    m.x43 = det * - ( x11 * A1213 - x12 * A0213 + x13 * A0113 );
    m.x44 = det *   ( x11 * A1212 - x12 * A0212 + x13 * A0112 );
    return m;
}

Matrix4 Matrix4::Transpose(){
    Matrix4 m;
    m.x11 = x11, m.x12 = x21, m.x13 = x31, m.x14 = x41;
    m.x21 = x12, m.x22 = x22, m.x23 = x32, m.x24 = x42;
    m.x31 = x13, m.x32 = x23, m.x33 = x33, m.x34 = x43;
    m.x41 = x14, m.x42 = x24, m.x43 = x34, m.x44 = x44;
    return m;
}

Matrix4 Matrix4::identity(){
    Matrix4 m;
    m.x11 = m.x22 = m.x33 = m.x44 = 1;
    return m;
}
Matrix4 Matrix4::scale(float x, float y, float z){
    Matrix4 m;
    m.x11 = x;
    m.x22 = y;
    m.x33 = z;
    m.x44 = 1;
    return m;
}
Matrix4 Matrix4::translation(float x, float y, float z){
    Matrix4 m;
    m.x11 = m.x22 = m.x33 = m.x44 = 1;
    m.x14 = x;
    m.x24 = y;
    m.x34 = z;
    return m;
}

Matrix4 Matrix4::translation(Vector3 v){
    Matrix4 m;
    m.x11 = m.x22 = m.x33 = m.x44 = 1;
    m.x14 = v.x;
    m.x24 = v.y;
    m.x34 = v.z;
    return m;
}

//https://en.wikipedia.org/wiki/Hyperboloid_model
Matrix4 Matrix4::translationh(Vector4 v){
    Matrix4 m;
    float w = v.Hw();
    float w1 = w+1;
    m.x11 = w,     m.x12 = v.x,               m.x13 = v.y,              m.x14 = v.z;
    m.x21 = v.x,   m.x22 = ((v.x*v.x)/w1)+1,  m.x23 = (v.y*v.x)/w1,     m.x24 = (v.z*v.x)/w1;
    m.x31 = v.y,   m.x32 = m.x23,             m.x33 = ((v.y*v.y)/w1)+1, m.x34 = (v.z*v.y)/w1;
    m.x41 = v.z,   m.x42 = m.x24,             m.x43 = m.x34,            m.x44 = ((v.z*v.z)/w1)+1;
    return m;
}
Matrix4 Matrix4::rotationOX(float x){
    Matrix4 m;
    m.x11 = m.x44 = 1;
    m.x33 = m.x22 = cos(x);
    m.x23 = sin(x);
    m.x32 = -m.x23;
    return m;
}
Matrix4 Matrix4::rotationOY(float x){
    Matrix4 m;
    m.x22 = m.x44 = 1;
    m.x11 = m.x33 = cos(x);
    m.x13 = sin(x);
    m.x31 = -m.x13;
    return m;
}
Matrix4 Matrix4::rotationOZ(float x){
    Matrix4 m;
    m.x33 = m.x44 = 1;
    m.x11 = m.x22 = cos(x);
    m.x12 = sin(x);
    m.x21 = -m.x12;
    return m;
}
Matrix4 Matrix4::translationhOX(float x){
    Matrix4 m = identity();
    m.x11 = m.x44 =  HMath2::cosh(x);
    m.x41 = m.x14 = HMath2::sinh(x);
    return m;
}
Matrix4 Matrix4::translationhOY(float x){
    Matrix4 m = identity();
    m.x22 = m.x44 =  HMath2::cosh(x);
    m.x42 = m.x24 = HMath2::sinh(x);
    return m;
}
Matrix4 Matrix4::translationhOZ(float x){
    Matrix4 m = identity();
    m.x33 = m.x44 =  HMath2::cosh(x);
    m.x43 = m.x34 = HMath2::sinh(x);
    return m;
}

Matrix4 Matrix4::invScale(float x, float y, float z){
    return Matrix4::scale(1.0F/x, 1.0F/y, 1.0F/z);
}
Matrix4 Matrix4::invTranslation(float x, float y, float z){
    return Matrix4::translation(-x, -y, -z);
}
Matrix4 Matrix4::invRotationOX(float x){
    return Matrix4::rotationOX(-x);
}
Matrix4 Matrix4::invRotationOY(float x){
    return Matrix4::rotationOY(-x);
}
Matrix4 Matrix4::invRotationOZ(float x){
    return Matrix4::rotationOZ(-x);
}
Matrix4 Matrix4::Perspective(float FOV, float size, float nearPlane, float farPlane){
    float s = size / std::tan(FOV * M_PI / 360);
    Matrix4 m = identity();
    m.x11 = 1/s;
    m.x22 = 1/s;
    m.x33 = 1.0F/(farPlane-nearPlane);
    m.x34 = -nearPlane/(farPlane-nearPlane);
    m.x43 = 1;
    return m;
}
