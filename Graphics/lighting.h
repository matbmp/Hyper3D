#ifndef LIGHTING_H
#define LIGHTING_H

#include <vector>

class LightSource;
struct Color;
struct Vector3;

#include "lightsource.h"

class Lighting
{
public:

    float ambient;
    float distanceConst;
    float specularExp;
    std::vector<LightSource> lights;

    Lighting();
    Lighting(float ambient, float distanceConst, int specular);
    void addLight(LightSource light);
    Color CalculateColor(Color color, Vector3 N, Vector3 vertex, Vector3 observator) const;
    Color CalculateWOspecular(Color color, Vector3 N, Vector3 vertex) const;
};

class HyperbolicLighting{
public:
    float ambient;
    float distanceConst;
    float specularExp;
    std::vector<HyperbolicLightSource> lights;

    HyperbolicLighting();
    HyperbolicLighting(float ambient, float distanceConst, int specular);
    void addLight(HyperbolicLightSource light);
    Color CalculateColor(Color color, Vector3 N, Vector3 vertex, Vector3 observator) const;
    Color CalculateWOspecular(Color color, Vector3 N, Vector3 vertex) const;
};

#endif // LIGHTING_H
