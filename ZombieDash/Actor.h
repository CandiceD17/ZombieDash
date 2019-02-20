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
    virtual void flamming()=0;
    virtual void infecting()=0;
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
    {infectCount=0;
    infectState = false;
    };
    virtual void doSomething()=0;
    virtual bool exit()=0;
    virtual bool pass() {return false;}
    virtual bool isHuman() {return true;}
    virtual bool isZombie() {return false;}
    virtual void flamming()=0;
    virtual void infecting(){
        infectCount++;
        infectState=true;}
    virtual bool blockFlame() {return false;}
    int CountInfection() {return infectCount;}
    bool StateInfection() {return infectState;}
    void noInfection() {infectState = false;}
    void resetInfection() {infectCount = 0;}
private:
    int infectCount;
    bool infectState;
};

class Penelope: public Human{
public:
    Penelope(double startX, double startY, StudentWorld* world)
    :Human(IID_PLAYER, startX, startY, right, 0, 1.0, world)
    {}
    virtual void doSomething();
    virtual bool exit();
    virtual void flamming();
};

class Citizen: public Human{
public:
    Citizen(double startX, double startY, StudentWorld* world)
    :Human(IID_CITIZEN, startX, startY, right, 0, 1.0, world)
    {}
    virtual void doSomething();
    virtual bool exit();
    virtual void flamming();
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
    virtual void flamming() {return;}
    virtual void infecting() {return;}
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
    virtual void flamming() {return;}
    virtual void infecting() {return;}
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
    virtual void flamming() {return;}
    virtual void infecting() {return;}
    virtual bool blockFlame() {return false;}
};

//class for flame and vomit
class Props: public Actor{
public:
    Props(int imageID, double startX, double startY, Direction dir, StudentWorld* world)
    :Actor(imageID, startX, startY, dir, 0, 1.0, world)
    {tickLeft=2;}
    virtual void doSomething()=0;
    virtual bool pass() {return true;}
    virtual bool exit() {return false;}
    virtual bool isHuman() {return false;}
    virtual bool isZombie() {return false;}
    virtual void flamming() {return;}
    virtual void infecting() {return;}
    virtual bool blockFlame() {return false;}
    int myTick() {return tickLeft;}
    void minus() {tickLeft--;}
private:
    int tickLeft;
};

class Flame:public Props{
public:
    Flame(double startX, double startY, Direction dir, StudentWorld* world)
    :Props(IID_FLAME, startX, startY, dir, world)
    {}
    virtual void doSomething();
};

class Vomit:public Props{
public:
    Vomit(double startX, double startY, Direction dir, StudentWorld* world)
    :Props(IID_VOMIT, startX, startY, dir, world)
    {}
    virtual void doSomething();
};

class Goodies: public Actor{
public:
    Goodies(int imageID, double startX, double startY, StudentWorld* world)
    :Actor(imageID, startX, startY, right, 1, 1.0, world)
    {}
    virtual void doSomething()=0;
    virtual bool exit() {return false;}
    virtual bool pass() {return true;}
    virtual bool isHuman() {return false;}
    virtual bool isZombie() {return false;}
    virtual void flamming() {setDead();}
    virtual void infecting() {return;}
    virtual bool blockFlame() {return false;}
};

class Vaccine: public Goodies{
public:
    Vaccine(double startX, double startY, StudentWorld* world)
    :Goodies(IID_VACCINE_GOODIE, startX, startY, world)
    {}
    virtual void doSomething();
};

class GasCan: public Goodies{
public:
    GasCan(double startX, double startY, StudentWorld* world)
    :Goodies(IID_GAS_CAN_GOODIE, startX, startY, world)
    {}
    virtual void doSomething();
};

class LandmineGoodies: public Goodies{
public:
    LandmineGoodies(double startX, double startY, StudentWorld* world)
    :Goodies(IID_LANDMINE_GOODIE, startX, startY, world)
    {}
    virtual void doSomething();
};

class Landmine: public Goodies{
public:
    Landmine(double startX, double startY, StudentWorld* world)
    :Goodies(IID_LANDMINE, startX, startY,world)
    {safeTick=30;}
    virtual void doSomething();
    virtual void flamming();
    void decSafeTick() {safeTick--;}
private:
    int safeTick;
};

class Zombies:public Actor{
public:
    Zombies(double startX, double startY, StudentWorld* world)
    :Actor(IID_ZOMBIE, startX, startY, right, 0, 1.0, world)
    {}
    virtual void doSomething()=0;
    virtual bool exit() {return false;}
    virtual bool pass() {return false;}
    virtual bool isHuman() {return false;}
    virtual bool isZombie() {return true;}
    virtual void flamming()=0;
    virtual void infecting() {return;}
    virtual bool blockFlame() {return false;}
    void addTime() {m_tick++;}
    int tick() {return m_tick;}
    Direction randomDirection();
    void moving(Direction dir, Zombies* thisOne);
    int movement() {return m_movement;}
    void setMovement(int n) {m_movement = n;}
private:
    int m_tick;
    int m_movement;
};

class DumbZombie:public Zombies{
public:
    DumbZombie(double startX, double startY, StudentWorld* world)
    :Zombies(startX, startY, world)
    {}
    virtual void doSomething();
    virtual void flamming();
};

class SmartZombie:public Zombies{
public:
    SmartZombie(double startX, double startY, StudentWorld* world)
    :Zombies(startX, startY, world)
    {}
    virtual void doSomething();
    virtual void flamming();
};







#endif // ACTOR_H_
