#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp
void Penelope::doSomething(){
    int movement;
    
    if(!m_world->getKey(movement))
        return;
    switch (movement) {
        case KEY_PRESS_LEFT:
            setDirection(left);
            if(m_world->notblocked(getX()-4, getY(),left))
                moveTo(getX()-4, getY());
            break;
        case KEY_PRESS_RIGHT:
            setDirection(right);
            if(m_world->notblocked(getX()+4, getY(),right))
                moveTo(getX()+4, getY());
            break;
        case KEY_PRESS_UP:
            setDirection(up);
            if(m_world->notblocked(getX(), getY()+4,up))
                moveTo(getX(), getY()+4);
            break;
        case  KEY_PRESS_DOWN:
            setDirection(down);
            if(m_world->notblocked(getX(), getY()-4,down))
                moveTo(getX(), getY()-4);
            break;
        default:
            break;}
    
    return;
}
