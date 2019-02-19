#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

class StudentWorld;

//base class

class Actor: public GraphObject {
public:
    Actor(int imageID, double startX, double startY,
          Direction startDirection, int depth, double size, StudentWorld* world)
    :GraphObject(imageID, startX, startY, startDirection, depth, size)
    {isAlive = true;
    m_world=world;}
    virtual void doSomething()=0;
    virtual bool exit()=0; //can pass thru the exit
    virtual bool pass()=0; //allows other object to pass thru
    virtual bool isHuman()=0;
    virtual bool isZombie()=0;
    virtual bool flamming()=0;
    virtual bool infecting()=0;
    virtual bool blockFlame()=0;
    void setDead() {isAlive = false;}
    bool alive() {return isAlive;}
    StudentWorld* getWorld() {return m_world;}
private:
    bool isAlive;
    StudentWorld* m_world;
};


//Humam Class (citizen & Penelope)z
class Human: public Actor{
public:
    Human(int imageID, double startX, double startY,
          Direction startDirection, int depth, double size,StudentWorld* world)
    :Actor(imageID, startX, startY, startDirection, depth, size, world)
    {};
    virtual void doSomething()=0;
    virtual bool exit()=0;
    virtual bool pass() {return false;}
    virtual bool isHuman() {return true;}
    virtual bool isZombie() {return false;}
    virtual bool flamming() {return true;}
    virtual bool infecting() {return true;}
    virtual bool blockFlame() {return false;}
};

class Penelope: public Human{
public:
    Penelope(double startX, double startY, StudentWorld* world)
    :Human(IID_PLAYER, startX, startY, right, 0, 1.0, world)
    {}
    virtual void doSomething();
    virtual bool exit();
};

class Citizen: public Human{
public:
    Citizen(int imageID, double startX, double startY,
            Direction startDirection, int depth, double size, StudentWorld* world)
    :Human(IID_CITIZEN, startX, startY, right, 0, 1.0, world)
    {}
    virtual void doSomething() {return;}
    virtual bool exit();
};


//objects that wait for others to overlap
class Wall: public Actor{
public:
    Wall(double startX, double startY, StudentWorld* world)
    :Actor(IID_WALL, startX, startY, right, 0, 1.0, world)
    {}
    virtual void doSomething() {return;}
    virtual bool pass() {return false;}
    virtual bool exit() {return false;}
    virtual bool isHuman() {return false;}
    virtual bool isZombie() {return false;}
    virtual bool flamming() {return false;}
    virtual bool infecting() {return false;}
    virtual bool blockFlame() {return true;}
};

class Exit: public Actor{
public:
    Exit(double startX, double startY, StudentWorld* world)
    :Actor(IID_EXIT, startX, startY, right, 1, 1.0, world)
    {}
    virtual void doSomething();
    virtual bool pass() {return true;}
    virtual bool exit() {return false;}
    virtual bool isHuman() {return false;}
    virtual bool isZombie() {return false;}
    virtual bool flamming() {return false;}
    virtual bool infecting() {return false;}
    virtual bool blockFlame() {return true;}
};

class Pit: public Actor{
public:
    Pit(double startX, double startY, StudentWorld* world)
    :Actor(IID_PIT, startX, startY, right, 0, 1.0, world)
    {}
    virtual void doSomething();
    virtual bool pass() {return true;}
    virtual bool exit() {return false;}
    virtual bool isHuman() {return false;}
    virtual bool isZombie() {return false;}
    virtual bool flamming() {return false;}
    virtual bool infecting() {return false;}
    virtual bool blockFlame() {return false;}
};
//
//class Goodies: public Actor{
//public:
//    void flamming();
//};
//
//class Landmine: public Actor{
//public:
//private:
//    int safeTick;
//};
//
//class Zombies:public Actor{
//public:
//};
//


//
//class props: public Actor{
//public:
//    
//private:
//};
//vomit & flame


#endif // ACTOR_H_
