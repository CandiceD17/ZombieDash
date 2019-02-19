#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

void Penelope::doSomething(){
    if(StateInfection())
        infecting();
    if(CountInfection()==500){
        setDead();
        getWorld()->decLives();
    }
    
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
        case KEY_PRESS_SPACE:
            getWorld()->playerFire(getX(), getY(), getDirection());
        default:
            break;}
    
    return;
}

bool Penelope::exit(){
    if (getWorld()->citizenCount()==0)
        return true;
    return false;
}

void Penelope::flamming(){
    setDead();
    getWorld()->decLives();
    getWorld()->playSound(SOUND_PLAYER_DIE);
}

void Citizen::doSomething(){
    if(StateInfection())
        infecting();
    if(CountInfection()==500){
        setDead();
    //create a zombie
    }
}

bool Citizen::exit(){
    setDead();
    return true;
}

void Citizen::flamming(){
    setDead();
    getWorld()->increaseScore(-1000);
    getWorld()->playSound(SOUND_CITIZEN_DIE);
}

void Exit::doSomething(){
    if(getWorld()->overlapExit(getX(), getY(), this)){
        if(getWorld()->citizenCount()!=0){
            getWorld()->increaseScore(500);
            getWorld()->playSound(SOUND_CITIZEN_SAVED);
        }
        else{
            getWorld()->toNextLevel();
        }
    }
}

void Pit::doSomething(){
    getWorld()->overlapFlame(getX(), getY(), this);
}

void Flame::doSomething(){
    if(myTick()==0){
        setDead();
        return;
    }
    minus();
    getWorld()->overlapFlame(getX(), getY(), this);
}

void Vomit::doSomething(){
    if(myTick()==0){
        setDead();
        return;
    }
    minus();
    getWorld()->overlapVomit(getX(), getY(), this);
}

void Vaccine::doSomething(){
    if(!alive())
        return;
    if(getWorld()->overlapGoodies(getX(), getY())){
        getWorld()->increaseScore(50);
        setDead();
        getWorld()->playSound(SOUND_GOT_GOODIE);
        getWorld()->setVaccine(1);
    }
}

void GasCan::doSomething(){
    if(!alive())
        return;
    if(getWorld()->overlapGoodies(getX(), getY())){
        getWorld()->increaseScore(50);
        setDead();
        getWorld()->playSound(SOUND_GOT_GOODIE);
        getWorld()->setGasCan(5);
    }
}

void LandmineGoodies::doSomething(){
    if(!alive())
        return;
    if(getWorld()->overlapGoodies(getX(), getY())){
        getWorld()->increaseScore(50);
        setDead();
        getWorld()->playSound(SOUND_GOT_GOODIE);
        getWorld()->setLandmine(2);
    }
}
