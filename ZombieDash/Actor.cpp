#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

void Penelope::doSomething(){
    int movement;
    
    if(!getWorld()->getKey(movement))
        return;
    switch (movement) {
        case KEY_PRESS_LEFT:
            setDirection(left);
            if(getWorld()->notblocked(getX()-4, getY(),this))
                moveTo(getX()-4, getY());
            break;
        case KEY_PRESS_RIGHT:
            setDirection(right);
            if(getWorld()->notblocked(getX()+4, getY(),this))
                moveTo(getX()+4, getY());
            break;
        case KEY_PRESS_UP:
            setDirection(up);
            if(getWorld()->notblocked(getX(), getY()+4,this))
                moveTo(getX(), getY()+4);
            break;
        case  KEY_PRESS_DOWN:
            setDirection(down);
            if(getWorld()->notblocked(getX(), getY()-4,this))
                moveTo(getX(), getY()-4);
            break;
        default:
            break;}
    
    return;
}

bool Penelope::exit(){
    if (getWorld()->citizenCount()==0)
        return true;
    return false;
}

bool Citizen::exit(){
    setDead();
    return true;
}

void Exit::doSomething(){
    if(getWorld()->overlapExit(getX(), getY())){
        if(getWorld()->citizenCount()!=0){
            getWorld()->increaseScore(500);
            getWorld()->playSound(SOUND_CITIZEN_SAVED);
        }
        else{
            getWorld()->toNextLevel();
        }
    }
}
