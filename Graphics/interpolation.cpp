#include "interpolation.h"

void AbstractBarycentricInterpolator::setTarget(Point2 A, Point2 B, Point2 C){
    this->A = A;
    this->B = B;
    this->C = C;
    init();
}

void AbstractBarycentricInterpolator::init(){
    if(A.y > B.y){
        std::swap(A,B);
        swap1 = true;
    } else{
        swap1 = false;
    }
    if(A.y > C.y){
        std::swap(A,C);
        swap2 = true;
    } else {
        swap2 = false;
    }
    if(B.y > C.y){
        std::swap(B,C);
        swap3 = true;
    } else{
        swap3 = false;
    }
    float m, n;
    if(A.y == C.y){
        D = Point2(A.x, B.y);
    } else{
        m = (C.x-A.x)/(float)(C.y-A.y);
        n = C.x - m*C.y;
        D = Point2(m*B.y+n, B.y);
    }
    int W = (B.x - A.x) * (C.y-A.y) - (B.y-A.y) * (C.x-A.x);
    invW = W == 0 ? 0 : 1.0F/W;
    cmax = C.x - A.x;
    cmay = C.y - A.y;
    bmax = B.x - A.x;
    bmay = B.y - A.y;
}

void AbstractBarycentricInterpolator::CalculateUVW(){
    //Wv = (P.x - A.x) * (C.y-A.y) - (P.y-A.y) * (C.x-A.x);
    //Ww = (B.x-A.x) * (P.y-A.y) - (B.y - A.y) * (P.x-A.x);
    Wv = (Pp.x - A.x) * cmay - (Pp.y-A.y) * cmax;
    Ww = bmax * (Pp.y-A.y) - bmay * (Pp.x-A.x);
    v = Wv * invW;
    w = Ww * invW;
    u = 1 - v - w;
}

void AbstractBarycentricInterpolator::topTriangle(Point2 b1, Point2 b2, Point2 t){
    if(b2.x < b1.x){
        std::swap(b1, b2);
    }

    b1.x-=1;
    b2.x+=1;

    float slope1 = (b1.x-t.x)/(float)(b1.y-t.y);
    float slope2 = (b2.x-t.x)/(float)(b2.y-t.y);

    float x1 = t.x, x2 = t.x;

    for(int i = t.y; i <= b1.y; i++){
        for(int j = x1; j < x2; j++){
            if(acceptPoint(j, i)){
                Pp = Point2(j,i);
                CalculateUVW();
                if(v >= -0.0001F && v <=1.0001F && w >=-0.0001F && w <= 1.0001F){ // uwzględniamy możliwe błędy w interpolacji barycentrycznej
                                        interp();
                                    }
            }
        }
        x1 += slope1;
        x2 += slope2;
    }
}

void AbstractBarycentricInterpolator::bottomTriangle(Point2 t1, Point2 t2, Point2 b){
    if(t2.x < t1.x){
        std::swap(t1, t2);
    }

    t1.x-=1;
    t2.x+=1;

    float slope1 = (b.x-t1.x)/(float)(b.y-t1.y);
    float slope2 = (b.x-t2.x)/(float)(b.y-t2.y);

    float x1 = b.x, x2 = b.x;

    for(int i = b.y; i >= t1.y; i--){
        for(int j = x1; j < x2; j++){
            if(acceptPoint(j, i)){
                Pp = Point2(j,i);
                CalculateUVW();
                if(v >= -0.03F && v <=1.03F && w >=-0.03F && w <= 1.03F){ // uwzględniamy możliwe błędy w interpolacji barycentrycznej
                                        interp();
                                    }
            };
        }
        x1 -= slope1;
        x2 -= slope2;
    }
}
void AbstractBarycentricInterpolator::doInterpolate(){
    this->topTriangle(B,D,A);
    this->bottomTriangle(D,B,C);
}

AbstractBarycentricInterpolator::AbstractBarycentricInterpolator(){

}

// --------------------------------------------------------------------------------




