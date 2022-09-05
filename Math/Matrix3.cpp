#include <cmath>
#include "Matrix3.h"

Matrix3 Matrix3::operator*(Matrix3 o){
    Matrix3 r;
    r.x11 = x11*o.x11 + x12*o.x21 + x13*o.x31;
    r.x12 = x11*o.x12 + x12*o.x22 + x13*o.x32;
    r.x13 = x11*o.x13 + x12*o.x23 + x13*o.x33;
    r.x21 = x21*o.x11 + x22*o.x21 + x23*o.x31;
    r.x22 = x21*o.x12 + x22*o.x22 + x23*o.x32;
    r.x23 = x21*o.x13 + x22*o.x23 + x23*o.x33;
    r.x31 = x31*o.x11 + x32*o.x21 + x33*o.x31;
    r.x32 = x31*o.x12 + x32*o.x22 + x33*o.x32;
    r.x33 = x31*o.x13 + x32*o.x23 + x33*o.x33;
    return r;
}
Vector3 Matrix3::operator*(Vector3 o){
    return Vector3(x11*o.x + x12*o.y + x13*o.z, x21*o.x + x22*o.y + x23 * o.z, x31*o.x + x32*o.y + x33*o.z);
}
Matrix3 Matrix3::identity(){
    Matrix3 m;
    m.x11 = 1;
    m.x22 = 1;
    m.x33 = 1;
    return m;
}
Matrix3 Matrix3::rotation(float alpha){
    Matrix3 m;
    m.x11 = cos(alpha);
    m.x12 = -sin(alpha);
    m.x22 = cos(alpha);
    m.x21 = sin(alpha);
    m.x33 = 1;
    return m;
}
Matrix3 Matrix3::translation(float x, float y){
    Matrix3 m = identity();
    m.x13 = x;
    m.x23 = y;
    return m;
}
Matrix3 Matrix3::scale(float x, float y){
    Matrix3 m;
    m.x11 = x;
    m.x22 = y;
    m.x33 = 1;
    return m;
}
Matrix3 Matrix3::shearingX(float x){
    Matrix3 m = identity();
    m.x12 = x;
    return m;
}
Matrix3 Matrix3::shearingY(float x){
    Matrix3 m = identity();
    m.x21 = x;
    return m;
}
Matrix3 Matrix3::invRotation(float alpha){
    return rotation(-alpha);
}
Matrix3 Matrix3::invTranslation(float x, float y){
    Matrix3 m = identity();
    m.x13 = -x;
    m.x23 = -y;
    return m;
}
Matrix3 Matrix3::invScale(float x, float y){
    Matrix3 m;
    if(x == 0) x = 10e-6;
    if(y == 0) y = 10e-6;
    m.x11 = 1/x;
    m.x22 = 1/y;
    m.x33 = 1;
    return m;
}
Matrix3 Matrix3::invShearingX(float x){
    Matrix3 m = identity();
    m.x12 = -x;
    return m;
}
Matrix3 Matrix3::invShearingY(float x){
    Matrix3 m = identity();
    m.x21 = -x;
    return m;
}

