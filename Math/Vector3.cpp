
#include "Vector3.h"
#include <cmath>

Vector3::Vector3(): x(0),y(0),z(0){}
Vector3::Vector3(float X, float Y, float Z): x(X), y(Y), z(Z){}
Vector3::Vector3(Vector4 v): x(v.x),y(v.y),z(v.z){}

Vector3 Vector3::operator+(Vector3 o)const{
    return Vector3(x+o.x, y+o.y, z+o.z);
}
Vector3 Vector3::operator-(Vector3 o){
    return Vector3(x-o.x, y-o.y, z-o.z);
}
Vector3 Vector3::operator-(){
    return Vector3(-x, -y, -z);
}
Vector3 Vector3::operator*(float o){
    return Vector3(x*o,y*o,z*o);
}
Vector3 operator*(float o, Vector3 v){
    return Vector3(v.x*o, v.y*o, v.z*o);
}

Vector2 Vector3::Perspective(float d){
    return Vector2((d*x)/(d+z), (d*y)/(d+z));
}

float Vector3::Length(){
    return std::sqrt(x*x + y*y +z*z);
}
float Vector3::LengthSquared(){
    return x*x + y*y +z*z;
}

float Vector3::Normalize(){
    float l = Length();
    if(l != 0){
        float il = (1.0F/l);
        x*=il;
        y*=il;
        z*=il;
    }
    return l;
}

float Vector3::Dot(const Vector3 &a, const Vector3 &b){
    return a.x*b.x + a.y*b.y + a.z*b.z;
}
