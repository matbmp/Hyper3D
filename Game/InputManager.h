#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#include <QMouseEvent>
#include "Math/Vector2.h"

class InputManager{
public:
    Vector2 mouse;
    std::vector<bool> pressed;

    InputManager(){
        pressed = std::vector<bool>(2000, false);
    }
    void keyPressEvent(QKeyEvent *event){
        int key = event->key();
        pressed[key] = true;
    }
    void keyReleaseEvent(QKeyEvent *event){
        int key = event->key();
        pressed[key] = false;
    }
    void setMousePos(Vector2 pos){
        mouse = pos;
    }
    bool isPressed(int key){
        return pressed[key];
    }
};


#endif // INPUTMANAGER_H
