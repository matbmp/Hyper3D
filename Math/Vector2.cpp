#include "Vector2.h"

Vector2::Vector2():x(0), y(0) {}
Vector2::Vector2(float X, float Y): x(X), y(Y){}
Vector2::Vector2(Point2 p): x(p.x), y(p.y){}

Vector2 Vector2::operator*(float o)const{ return Vector2(x*o, y*o);}
Vector2 Vector2::operator*(Vector2 o){ return Vector2(x*o.x, y*o.y);}
Vector2 Vector2::operator+(Vector2 o){ return Vector2(x+o.x, y+o.y);}
Vector2 Vector2::operator+(float o){ return Vector2(x+o, y+o);}
Vector2 Vector2::operator-(Vector2 o){ return Vector2(x-o.x, y-o.y);}
Vector2 Vector2::operator-(){ return Vector2(-x, -y);}
Vector2 Vector2::Lerp(Vector2 a, Vector2 b, float t){
    return (a + (b+(-a))*t);
}

