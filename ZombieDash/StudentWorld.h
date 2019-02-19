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
    bool overlapExit(double x,double y);
    int citizenCount() {return citizenNum;}
    void removeCitizen() {citizenNum--;}

private:
    Penelope* m_Pene;
    list<Actor*> m_member;
    int citizenNum;
    bool nextLevel;
    bool overlap(double x, double y, Actor* me);
};

#endif // STUDENTWORLD_H_
