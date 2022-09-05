#ifndef RENDERTARGET_H
#define RENDERTARGET_H

#include <algorithm>
#include <QImage>
#include "Math/Vector2.h"

enum RenderTargetType{
    NONE = 0,
    DRAW = 1,
    DEPTH = 2,
    DRAWDEPTH = 3
};

struct RenderTarget{
public:
    int width, height;
    Vector2 halfBuffDim;
    QImage *backbufferImage;
    float *zbuffer;
    uint *backbuffer;
    RenderTargetType renderType;

    RenderTarget(){}

    RenderTarget(int width, int height, RenderTargetType renderType){
        this->width = width;
        this->height = height;
        this->halfBuffDim = Vector2(width/2.0F, height/2.0F);
        if((renderType & 1) == 1)
        {
            this->backbufferImage = new QImage(width, height, QImage::Format_RGB32);
            this->backbuffer = (uint*)(this->backbufferImage->bits());
        }
        if((renderType>>1 & 1) == 1)
        this->zbuffer = new float[width*height];
        this->renderType = renderType;
    }
};

#endif // RENDERTARGET_H
