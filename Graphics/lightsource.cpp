#include "lightsource.h"

LightSource::LightSource(){}

LightSource::LightSource(Vector3 Position, float Strength)
    :originalPosition(Position), strength(Strength){}

LightSource::LightSource(Vector3 Position, float Strength, Color lightColor)
    :originalPosition(Position), strength(Strength), lightColor(lightColor){}


HyperbolicLightSource::HyperbolicLightSource(){}

HyperbolicLightSource::HyperbolicLightSource(GyroVector Position, float Strength)
    :originalPosition(Position), strength(Strength){}

HyperbolicLightSource::HyperbolicLightSource(GyroVector Position, float Strength, Color lightColor)
    :originalPosition(Position), strength(Strength), lightColor(lightColor){}
