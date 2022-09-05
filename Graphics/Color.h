#ifndef COLOR_H
#define COLOR_H

#include <algorithm>
#include "Math/INumeric.h"

struct Color{
public:
    float r,g,b;

    Color();
    Color(float R, float G, float B);
    Color(uint packed);
    Color operator*(float f) const;
    Color operator*(float f);
    Color operator*(Color o);
    Color operator+(Color o);
    friend Color operator*(float f, Color c);
    unsigned int ToUint() const;
    int ToInt();
    Color &ReinhardTonemap();
    static float pomRGBHSV(int n, float H, float S, float V){
        float k = (n + H*6.0) - (int(n + H*6.0)/6)*6;
        return (V - V * S * std::max(std::min(std::min(k, 4-k), 1.0F), 0.0F));
    }
    static Color FromHSV(float H, float S, float V){
        return Color(pomRGBHSV(5, H, S, V), pomRGBHSV(3, H, S, V), pomRGBHSV(1, H, S, V));
    }
};

#endif // COLOR_H
