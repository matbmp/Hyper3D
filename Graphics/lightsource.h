#ifndef LIGHTSOURCE_H
#define LIGHTSOURCE_H

#include "Math/Vector3.h"
#include "Graphics/Color.h"
#include "Game/HMath.h"
#include "rendertarget.h"
#include "Sampler.h"

class LightSource
{
public:
    Vector3 originalPosition;
    Vector3 position;
    float strength;
    Color lightColor = Color(1,1,1);
    Color specularColor = Color(1,1,1);

    LightSource();
    LightSource(Vector3 Position, float Strength);
    LightSource(Vector3 Position, float Strength, Color lightColor);
};

class HyperbolicLightSource
{
public:
    GyroVector originalPosition;
    GyroVector position;
    GyroVector minusPosition; // pixel shader optimalization
    float strength;
    Color lightColor = Color(1,1,1);
    Color specularColor = Color(1,1,1);

    RenderTarget lightMapFront;
    RenderTarget lightMapBack;
    Sampler<float> lightMapFrontSampler;
    Sampler<float> lightMapBackSampler;

    HyperbolicLightSource();
    HyperbolicLightSource(GyroVector Position, float Strength);
    HyperbolicLightSource(GyroVector Position, float Strength, Color lightColor);
};

#endif // LIGHTSOURCE_H
