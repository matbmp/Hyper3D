#ifndef INTERPOLATION_H
#define INTERPOLATION_H

#include <functional>
#include "Math/Point2.h"
#include "Math/INumeric.h"
#include "vertex.h"
#include "Shader.h"

class AbstractBarycentricInterpolator{
public:
    std::function<bool(int, int)> acceptPoint;
protected:
    Point2 A, B, C, D;
    float invW;
    int Wv, Ww;
    float v, w, u;
    Point2 Pp;
    bool swap1 = false, swap2 = false, swap3 = false;
    int cmay, cmax, bmax, bmay;


    void init();
    void CalculateUVW();
    void topTriangle(Point2 b1, Point2 b2, Point2 t);
    void bottomTriangle(Point2 t1, Point2 t2, Point2 b);
    void doInterpolate();
    virtual void interp(){};

public:
    AbstractBarycentricInterpolator();
    void setTarget(Point2 A, Point2 B, Point2 C);
};

template<typename InVertex, typename T>
class BarycentricInterpolator : public AbstractBarycentricInterpolator{
protected:
    T Ap1, Bp1, Cp1;
    T result;
    void interp(){
        shader->Interpolate(Ap1, Bp1, Cp1, result, u, v, w);
        shader->Draw(Pp, result);
    }
    T interp1(){ return Ap1*u + Bp1 * v + Cp1 * w;}
    void init1(T Ap1, T Bp1, T Cp1){
        if(swap1) std::swap(Ap1, Bp1);
        if(swap2) std::swap(Ap1, Cp1);
        if(swap3) std::swap(Bp1, Cp1);
        this->Ap1 = Ap1;
        this->Bp1 = Bp1;
        this->Cp1 = Cp1;
    }
public:
    Shader<InVertex, T> *shader;
    BarycentricInterpolator(){
    }

    void interpolate(T Ap1, T Bp1, T Cp1){
        init1(Ap1, Bp1, Cp1);
        doInterpolate();
    }
};


#endif // INTERPOLATION_H
