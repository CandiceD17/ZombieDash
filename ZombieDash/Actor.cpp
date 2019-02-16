#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp
bool Actor::notBlocked(Direction dir){
    return true;
}

void Penelope::doSomething(){
    int movement;
    
    if(!m_world->getKey(movement))
        return;
   
    if(notBlocked(movement)){
     
        switch (movement) {
            case KEY_PRESS_LEFT:
                setDirection(left);
                moveTo(getX()-1, getY());
                break;
            case KEY_PRESS_RIGHT:
                setDirection(right);
                moveTo(getX()+1, getY());
                break;
            case KEY_PRESS_UP:
                setDirection(up);
                moveTo(getX(), getY()+1);
                break;
            case  KEY_PRESS_DOWN:
                setDirection(down);
                moveTo(getX(), getY()-1);
                break;
            default:
                break;}
    }
    
    return;
}
