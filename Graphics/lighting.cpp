#include "lighting.h"
#include <cmath>

Lighting::Lighting(){}
Lighting::Lighting(float ambient, float distanceConst, int specular)
    :ambient(std::max(0.0F,ambient)), distanceConst(distanceConst), specularExp(specular){}
void Lighting::addLight(LightSource light){
    lights.push_back(light);
}

Color Lighting::CalculateColor(Color color, Vector3 N, Vector3 vertex, Vector3 observator) const{
    Color result = (color*ambient);
   Vector3 V = observator - vertex;
   Color b;
   V.Normalize();

   for(ulong i = 0; i < lights.size(); i++){
       LightSource ls = lights[i];
       Vector3 L = ls.position - vertex;
       float l = L.Normalize();

       float fatt = 1.0F/(distanceConst + l);

       float a = fatt*ls.strength;

       float diffuse = (std::max(0.0F, (Math::IloczynSkalarny(N, L))));

       Vector3 R = (2.0F*Math::IloczynSkalarny(L, N)*N) - L;

       float specular = Math::IloczynSkalarny(R,V);
       if(specular > 1e-2){
           specular = std::max(0.0F, specular);
           specular = std::pow(specular, specularExp);
           b = ls.lightColor*(color*diffuse + ls.specularColor*specular);
       } else{
           b = ls.lightColor*(color*diffuse);
       }

       result = result + (a*b);
   }
   return result;
}

Color Lighting::CalculateWOspecular(Color color, Vector3 N, Vector3 vertex) const{
    Color result = (color*ambient);
    Color b;
    N.Normalize();

    for(ulong i = 0; i < lights.size(); i++){
        LightSource ls = lights[i];
        Vector3 L = ls.position - vertex;
        float l = L.Normalize();

        float fatt = 1.0F/(distanceConst + l);

        float a = fatt*ls.strength;

        float diffuse = (std::max(0.0F, (Math::IloczynSkalarny(N, L))));

        b = ls.lightColor*(color*diffuse);
        result = result + (a*b);
    }
    return result;
}



HyperbolicLighting::HyperbolicLighting(){}
HyperbolicLighting::HyperbolicLighting(float ambient, float distanceConst, int specular)
    :ambient(std::max(0.0F,ambient)), distanceConst(distanceConst), specularExp(specular){}
void HyperbolicLighting::addLight(HyperbolicLightSource light){
    lights.push_back(light);
}

Color HyperbolicLighting::CalculateWOspecular(Color color, Vector3 N, Vector3 vertex) const{
    Color result = (color*ambient);
    Color b;

    for(ulong i = 0; i < lights.size(); i++){
        HyperbolicLightSource ls = lights[i];
        //GyroVector L =  GyroVector(vertex) - ls.originalPosition;
        Vector3 LL = vertex - ls.originalPosition.vec;
        float l = (vertex - ls.originalPosition.vec).LengthSquared() / ((-1 + vertex.LengthSquared()) * (-1 + ls.originalPosition.vec.LengthSquared()));

        float fatt = 1.0F/(distanceConst + std::exp(l));

        float a = fatt*ls.strength;

        LL.Normalize();
        float diffuse = (std::max(0.0F, (Math::IloczynSkalarny(N, -LL))));

        b = ls.lightColor*(color*diffuse);
        result = result + (a*b);
    }
    return result;
}
