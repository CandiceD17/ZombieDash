#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
class StudentWorld;

class Actor: public GraphObject {
public:
    Actor(int imageID, double startX, double startY,
          Direction startDirection, int depth, double size)
    :GraphObject(imageID, startX, startY, startDirection, depth, size)
    {}
    virtual void doSomething()=0;
};

class Penelope: public Actor{
public:
    Penelope(double startX, double startY, StudentWorld* world)
    :Actor(IID_PLAYER, startX, startY, right, 0, 1.0)
    {
        m_world = world;
    }
    virtual void doSomething();
private:
    StudentWorld* m_world;
};

class Wall: public Actor{
public:
    Wall(double startX, double startY)
    :Actor(IID_WALL, startX, startY, right, 0, 1.0)
    {}
    virtual void doSomething() {return;}
};


#endif // ACTOR_H_
