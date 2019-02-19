#include "StudentWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include <string>
#include <cmath>
using namespace std;

GameWorld* createStudentWorld(string assetPath)
{
    return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h and Actor.cpp

StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath)
{
    citizenNum = 0;
    nextLevel = false;
}

int StudentWorld::init()
{
    Level lev(assetPath());
    int level = getLevel();
    string levelFile;
    switch (level) {
        case 1:
            levelFile = "level01.txt"; break;
        case 2:
            levelFile = "level02.txt"; break;
        case 3:
            levelFile = "level03.txt"; break;
        case 4:
            levelFile = "level04.txt"; break;
        case 5:
            levelFile = "level05.txt"; break;
        case 6:
            levelFile = "level06.txt"; break;
    }
    Level::LoadResult result = lev.loadLevel(levelFile);
    if (result == Level::load_success){
        for(int i=0; i<LEVEL_WIDTH;i++){
            for(int j=0; j<LEVEL_HEIGHT;j++){
                Level::MazeEntry thisOne = lev.getContentsOf(i,j);
                switch (thisOne) {
                    case Level::player:
                        m_Pene = new Penelope(i*SPRITE_WIDTH,j*SPRITE_HEIGHT,this);
                        break;
                    case Level::wall:
                        m_member.push_back(new Wall(i*SPRITE_WIDTH,j*SPRITE_HEIGHT,this));
                        break;
                    case Level::exit:
                        m_member.push_back(new Exit(i*SPRITE_WIDTH, j*SPRITE_HEIGHT,this));
                        break;
                    default:
                        break;
                }
            }
        }
    }
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    // This code is here merely to allow the game to build, run, and terminate after you hit enter.
    // Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.
    if(m_Pene->alive())
        m_Pene->doSomething();
    list<Actor*>::iterator it;
    for(it=m_member.begin(); it!=m_member.end();it++){
        if((*it)->alive())
            (*it)->doSomething();
    }
    if(!m_Pene->alive())
        return GWSTATUS_PLAYER_DIED;
    if(nextLevel){
        nextLevel=false;
        return GWSTATUS_FINISHED_LEVEL;}
    for(it=m_member.begin(); it!=m_member.end();it++){
        if(!(*it)->alive()){
            delete *it;
            m_member.erase(it);
        }
    }
    //text here
    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
    delete m_Pene;
    list<Actor*>::iterator it;
    for(it=m_member.begin(); it!=m_member.end();it++){
        delete *it;
        m_member.erase(it);
    }
}

bool StudentWorld::notblocked(double x, double y, Actor* moving){
    if(moving!=m_Pene)
        if(abs((m_Pene)->getX()-x)<SPRITE_WIDTH && abs(y-(m_Pene)->getY())<SPRITE_HEIGHT)
            return false;
    list<Actor*>::iterator it;
    for(it=m_member.begin();it!=m_member.end();it++){
        if(moving!=(*it)){
            if(abs((*it)->getX()-x)<SPRITE_WIDTH && abs(y-(*it)->getY())<SPRITE_HEIGHT)
                return (*it)->pass();}}
    return true;
}

bool StudentWorld::overlap(double x, double y, Actor* me){
    if(pow(me->getX()-x,2)+pow(me->getY()-y,2)<=100)
        return true;
    return false;
}

bool StudentWorld::overlapExit(double x, double y){
    if(overlap(x, y, m_Pene))
        return m_Pene->exit();
    
    list<Actor*>::iterator it;
    for(it=m_member.begin();it!=m_member.end();it++){
        if(overlap(x, y, *it))
            return (*it)->exit();}
    return false;
}

