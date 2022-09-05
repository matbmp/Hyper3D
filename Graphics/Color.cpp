#include "Graphics.h"
#include <functional>

Color::Color():r(0),g(0),b(0){}
Color::Color(float R, float G, float B): r(R), g(G), b(B){}
Color::Color(uint packed){
    r = ((packed &0xff0000) >> 16)*(1/255.0F);
    g = ((packed &0xff00) >> 8)*(1/255.0F);
    b = ((packed &0xff) >> 0)*(1/255.0F);
}

Color Color::operator*(float f) const{ return Color(r*f,g*f,b*f);}
Color Color::operator*(float f) { return Color(r*f,g*f,b*f);}
Color Color::operator*(Color o){ return Color(r*o.r,g*o.g,b*o.b);}
Color Color::operator+(Color o){ return Color(r+o.r,g+o.g,b+o.b);}

Color operator*(float f, Color c){
    return Color(c.r*f, c.g*f, c.b*f);
}

uint Color::ToUint() const{ return (std::min(uint(255), uint(255*r))<<16)+
            (std::min(uint(255), uint(255*g))<<8)+
            (std::min(uint(255), uint(255*b)));}
int Color::ToInt(){ return ((int(255*r))<<16) + ((int(255*g))<<8) + (int(255*b));}

Color& Color::ReinhardTonemap(){
    float lum = (r + g + b)/3.0F;
    float mapped = lum/(lum+1);
    r *= mapped;
    g *= mapped;
    b *= mapped;
    return *this;
}

