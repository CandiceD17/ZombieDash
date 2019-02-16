#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
class Actor: public GraphObject {
public:
    Actor(int imageID, double startX, double startY,
          Direction startDirection, int depth, double size)
    :GraphObject(imageID, startX, startY, startDirection, depth, size)
    {}
    virtual ~Actor();
    virtual void doSomething()=0;
};

class Penelope: public Actor{
public:
    Penelope(double startX, double startY)
    :Actor(0, startX, startY, 0, 0, 1.0)
    {}
    virtual ~Penelope();
    virtual void doSomething();
};

class Wall: public Actor{
    Wall();
    virtual ~Wall();
    virtual void doSomething();
private:
    int m_x;
    int m_y;
};


#endif // ACTOR_H_
