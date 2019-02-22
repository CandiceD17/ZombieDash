#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

//vaccine when zombie die

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
            getWorld()->playSound(SOUND_PLAYER_FIRE);
            break;
        case KEY_PRESS_TAB: //landmine
            getWorld()->playerLandmine(getX(), getY());
            break;
        case KEY_PRESS_ENTER: //vaccine
            if(getWorld()->myVaccine()>0){
                getWorld()->setVaccine(-1);
                noInfection();
                resetInfection();}
            break;
        default:
            break;}
    
    return;
}

bool Penelope::exit(){
    if (getWorld()->citizenCount()<=0){
        getWorld()->removeCitizen();
        return true;}
    return false;
}

void Penelope::flamming(){
    setDead();
    getWorld()->decLives();
    getWorld()->playSound(SOUND_PLAYER_DIE);
}

void Citizen::makeMove(Direction dir){
    switch (dir) {
        case right:
            moveTo(getX()+2, getY());
            break;
        case left:
            moveTo(getX()-2, getY());
            break;
        case up:
            moveTo(getX(), getY()+2);
            break;
        case down:
            moveTo(getX(), getY()-2);
            break;
        default:
            break;
    }
}

void Citizen::doSomething(){
    if(!alive())
        return;
    if(StateInfection())
        infecting();
    if(CountInfection()==500){
        setDead();
        getWorld()->playSound(SOUND_ZOMBIE_BORN);
        getWorld()->newZombie(getX(),getY());
        getWorld()->removeCitizen();
        getWorld()->increaseScore(-1000);
        return;
    }
    addTime();
    if(tick()%2==0)
        return;
    double dist_p = getWorld()->distancePene(getX(), getY());
    double dist_z = getWorld()->distanceZombie(getX(), getY());
    if(dist_p<dist_z && dist_p<=80){
        int dir = getWorld()->findDirectionPene(getX(), getY(),this);
        if(dir != -1){
            makeMove(dir);
            return;}
    }
    if(dist_z<=80){
        int dir =getWorld()->findDirectionZombie(getX(), getY(), this);
        if(dir==-1)
            return;
        makeMove(dir);
    }
    else
        return;
}

bool Citizen::exit(){
    setDead();
    getWorld()->removeCitizen();
    return true;
}

void Citizen::flamming(){
    if(!alive())
        return;
    setDead();
    getWorld()->removeCitizen();
    getWorld()->increaseScore(-1000);
    getWorld()->playSound(SOUND_CITIZEN_DIE);
}

void Exit::doSomething(){
    if(getWorld()->overlapExit(getX(), getY(), this)){
        if(getWorld()->citizenCount()>=0){
            getWorld()->increaseScore(500);
            getWorld()->playSound(SOUND_CITIZEN_SAVED);
        }
        else{
            getWorld()->toNextLevel();
            getWorld()->playSound(SOUND_LEVEL_FINISHED);
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

void Landmine::doSomething(){
    if(!alive())
        return;
    decSafeTick();
    if(safeTick==0)
        return;
    if(safeTick<0){
        if(getWorld()->overlapLandmine(getX(), getY(), this)){
            setDead();
            getWorld()->playSound(SOUND_LANDMINE_EXPLODE);
            getWorld()->LandmineExplode(getX(), getY());}
    }
}

void Landmine::flamming(){
    setDead();
    getWorld()->playSound(SOUND_LANDMINE_EXPLODE);
    getWorld()->LandmineExplode(getX(), getY());
}

Direction Zombies::randomDirection(){
    int direction = randInt(1, 4);
    switch (direction) {
        case 1:
            return right;
        case 2:
            return up;
        case 3:
            return left;
        default:
            return down;
    }
}

void Zombies::moving(Direction dir, Zombies* thisOne){
    switch (dir) {
        case up:
            if(getWorld()->notblocked(thisOne->getX(), thisOne->getY()+1, this)){
                thisOne->moveTo(thisOne->getX(), thisOne->getY()+1);
                m_movement--;
            }
            else m_movement=0;
            break;
        case down:
            if(getWorld()->notblocked(thisOne->getX(), thisOne->getY()-1, this)){
                thisOne->moveTo(thisOne->getX(), thisOne->getY()-1);
                m_movement--;
            }
            else m_movement=0;
            break;
        case left:
            if(getWorld()->notblocked(thisOne->getX()-1, thisOne->getY(), this)){
                thisOne->moveTo(thisOne->getX()-1, thisOne->getY());
                m_movement--;
            }
            else m_movement=0;
            break;
        case right:
            if(getWorld()->notblocked(thisOne->getX()+1, thisOne->getY(), this)){
                moveTo(thisOne->getX()+1, thisOne->getY());
                m_movement--;
            }
            else m_movement=0;
            break;
        default:
            break;
    }
}


void DumbZombie::doSomething(){
    if(!alive())
        return;
    addTime();
    if(tick()%2==0)
        return;
    if(getWorld()->toVomit(getX(),getY(),getDirection())){
        getWorld()->playSound(SOUND_ZOMBIE_VOMIT);
        return;}
    if(movement()==0){
        setMovement(randInt(3, 10));
        setDirection(randomDirection());}
    moving(getDirection(), this);
}

void DumbZombie::flamming(){
    if(!alive())
        return;
    setDead();
    getWorld()->increaseScore(1000);
    getWorld()->playSound(SOUND_ZOMBIE_DIE);
    if(randInt(1, 10)==1)
        getWorld()->moreVaccine(getX(), getY());
}

void SmartZombie::doSomething(){
    if(!alive())
        return;
    addTime();
    if(tick()%2==0)
        return;
    if(getWorld()->toVomit(getX(),getY(),getDirection())){
        getWorld()->playSound(SOUND_ZOMBIE_VOMIT);
        return;}
    if(movement()==0){
        setMovement(randInt(3, 10));
        if(getWorld()->findDirection(getX(),getY()) == -1)
            setDirection(randomDirection());
        else
            setDirection(getWorld()->findDirection(getX(),getY()));
    }
    moving(getDirection(), this);
}

void SmartZombie::flamming(){
    setDead();
    getWorld()->increaseScore(2000);
    getWorld()->playSound(SOUND_ZOMBIE_DIE);
}
