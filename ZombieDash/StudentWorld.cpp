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
    m_Pene = nullptr;
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
        default:
            levelFile = "level06.txt";
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
                        m_member.push_back(new Wall(i*SPRITE_WIDTH,j*SPRITE_HEIGHT));
                        block.push_back(new Wall(i*SPRITE_WIDTH,j*SPRITE_HEIGHT));
                        break;
                    default:
                        break;}
            }
        }
    }
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    // This code is here merely to allow the game to build, run, and terminate after you hit enter.
    // Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.
    m_Pene->doSomething();
    list<Actor*>::iterator it;
    for(it=m_member.begin(); it!=m_member.end();it++)
        (*it)->doSomething();
 //   decLives();
    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
    delete m_Pene;
    list<Actor*>::iterator it;
    for(it=m_member.begin(); it!=m_member.end();it++)
        delete *it;
}

bool StudentWorld::notblocked(double x, double y, int dir){
    list<Actor*>::iterator it;
    for(it=block.begin();it!=block.end();it++){
        if(dir == GraphObject::up || dir == GraphObject::down){
            if(abs((*it)->getX()-x)<16)
                if (abs(y-(*it)->getY())<16)
                    return false;
        }
        else
        {if(abs((*it)->getY()-y)<16)
            if(abs(x-(*it)->getX())<16)
                return false;}
    }
    return true;
}
