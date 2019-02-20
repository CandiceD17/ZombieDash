#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Level.h"
#include <string>
#include <list>
using namespace std;

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp
class Actor;
class Penelope;
class Citizen;

class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetPath);
    virtual int init();
    virtual int move();
    virtual void cleanUp();
    void toNextLevel() {nextLevel = true;}
    
    bool notblocked(double x, double y, Actor* moving);
    bool overlapExit(double x,double y, Actor* thisOne);
    void overlapFlame(double x, double y, Actor* thisOne);
    void overlapVomit(double x, double y, Actor* thisOne);
    bool overlapGoodies(double x, double y);
    bool overlapLandmine(double x, double y, Actor* thisOne);
    bool toVomit(double x, double y, int direction);

    
    void playerFire(double x, double y, int direction);
    void playerLandmine(double x, double y);
    void LandmineExplode(double x, double y);
    void moreVaccine(double x, double y);
    
    int citizenCount() {return citizenNum;}
    void removeCitizen() {citizenNum--;}
    
    void setVaccine(int m) {m_vac+=m;}
    void setGasCan(int m) {m_gas+=m;}
    void setLandmine(int m) {m_land+=m;}
    int myVaccine() {return m_vac;}

private:
    Penelope* m_Pene;
    list<Actor*> m_member;
    int citizenNum;
    bool nextLevel;
    bool overlap(double x, double y, Actor* me);
    int m_vac;
    int m_gas;
    int m_land;
};

#endif // STUDENTWORLD_H_
