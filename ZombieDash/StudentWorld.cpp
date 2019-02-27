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


StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath)
{
    citizenNum = 0;
    nextLevel = false;
    m_vac= 0;
    m_gas = 0;
    m_land = 0;
    deletePene = false;
    m_init = false;
}

StudentWorld::~StudentWorld(){
    cleanUp();
}

int StudentWorld::init()
{
    playSound(SOUND_THEME);
    
    citizenNum = 0;
    nextLevel = false;
    m_vac= 0;
    m_gas = 0;
    m_land = 0;
    deletePene = false;
    m_init = false;
    
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
        default:
            return GWSTATUS_PLAYER_WON;
    }
    Level::LoadResult result = lev.loadLevel("level04.txt");
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
                    case Level::pit:
                        m_member.push_back(new Pit(i*SPRITE_WIDTH, j*SPRITE_HEIGHT,this));
                        break;
                    case Level::vaccine_goodie:
                        m_member.push_back(new Vaccine(i*SPRITE_WIDTH, j*SPRITE_HEIGHT,this));
                        break;
                    case Level::gas_can_goodie:
                        m_member.push_back(new GasCan(i*SPRITE_WIDTH, j*SPRITE_HEIGHT,this));
                        break;
                    case Level::landmine_goodie:
                        m_member.push_back(new LandmineGoodies(i*SPRITE_WIDTH, j*SPRITE_HEIGHT,this));
                        break;
                    case Level::dumb_zombie:
                        m_member.push_back(new DumbZombie(i*SPRITE_WIDTH, j*SPRITE_HEIGHT,this));
                        break;
                    case Level::smart_zombie:
                        m_member.push_back(new SmartZombie(i*SPRITE_WIDTH, j*SPRITE_HEIGHT,this));
                        break;
                    case Level::citizen:
                        m_member.push_back(new Citizen(i*SPRITE_WIDTH, j*SPRITE_HEIGHT,this));
                        citizenNum++;
                        break;
                    default:
                        break;
                }
            }
        }
    }
    m_init = true;
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    if(m_Pene->alive())
        m_Pene->doSomething();
    list<Actor*>::iterator it;
    for(it=m_member.begin(); it!=m_member.end();it++){
        if((*it)->alive())
            (*it)->doSomething();
        if(!m_Pene->alive()){
            return GWSTATUS_PLAYER_DIED;}
        if(nextLevel){
            nextLevel=false;
            return GWSTATUS_FINISHED_LEVEL;}
    }
    for(it=m_member.begin(); it!=m_member.end();it++){
        if(!(*it)->alive()){
            delete *it;
            m_member.erase(it);
        }
    }
    setGameStatText("Score: "+std::to_string(getScore())+"  "+
                    "Level: "+std::to_string(getLevel())+"  "+
                    "Lives: "+std::to_string(getLives())+"  "+
                    "Vacc: "+std::to_string(m_vac)+"  "+
                    "Flames: "+std::to_string(m_gas)+"  "+
                    "Mines: "+std::to_string(m_land)+"  "+
                 //   "C: "+std::to_string(citizenNum)+"  "+
                    "Infected: "+std::to_string(m_Pene->CountInfection()));
    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
    if(!m_init)
        return;
    if(!deletePene){
        delete m_Pene;
        deletePene = true;}
    list<Actor*>::iterator it;
    for(it=m_member.begin(); it!=m_member.end();it++){
        delete *it;
        m_member.erase(it);
    }
}

bool StudentWorld::notblocked(double x, double y, Actor* moving){
    if(moving!=m_Pene)
        if(abs((m_Pene)->getX()-x)<SPRITE_WIDTH && abs((m_Pene)->getY()-y)<SPRITE_HEIGHT)
            return false;
    list<Actor*>::iterator it;
    for(it=m_member.begin();it!=m_member.end();it++){
        if(moving!=(*it)){
            if(abs((*it)->getX()-x)<SPRITE_WIDTH && abs((*it)->getY()-y)<SPRITE_HEIGHT)
                if(!(*it)->pass())
                   return false;}}
    return true;
}

bool StudentWorld::overlap(double x, double y, Actor* me){
    if(pow(me->getX()-x,2)+pow(me->getY()-y,2)<=100)
        return true;
    return false;
}

bool StudentWorld::overlapExit(double x, double y, Actor* thisOne){
    list<Actor*>::iterator it;
    for(it=m_member.begin();it!=m_member.end();it++){
        if(thisOne!=*it)
            if(overlap(x, y, *it))
                return (*it)->exit();}
    
    if(overlap(x, y, m_Pene))
        return m_Pene->exit();
    
    return false;
}

void StudentWorld::overlapFlame(double x, double y, Actor* thisOne){
    if(overlap(x, y, m_Pene))
        m_Pene->flamming();
    
    list<Actor*>::iterator it;
    for(it=m_member.begin();it!=m_member.end();it++){
        if(thisOne!=*it)
            if(overlap(x, y, *it))
                (*it)->flamming();}
}

void StudentWorld::overlapVomit(double x, double y, Actor* thisOne){
    if(overlap(x, y, m_Pene))
        m_Pene->infecting();
    
    list<Actor*>::iterator it;
    for(it=m_member.begin();it!=m_member.end();it++){
        if(thisOne!=*it)
            if(overlap(x, y, *it))
                (*it)->infecting();}
}

bool StudentWorld::overlapGoodies(double x, double y){
    if(overlap(x, y, m_Pene))
        return true;
    return false;
}

bool StudentWorld::overlapLandmine(double x, double y, Actor* thisOne){
    if(overlap(x, y, m_Pene))
        return true;
    
    list<Actor*>::iterator it;
    for(it=m_member.begin();it!=m_member.end();it++){
        if(thisOne!=*it)
            if(overlap(x, y, *it))
                return ((*it)->isHuman()||(*it)->isZombie());
    }
    return false;
}

bool StudentWorld::canFire(double x, double y){
    list<Actor*>::iterator it;
    for(it=m_member.begin();it!=m_member.end();it++){
        if(overlap(x, y, *it))
            if((*it)->blockFlame())
                return false;}
    return true;
}

void StudentWorld::playerFire(double x, double y, int direction){
    if(m_gas<=0)
        return;
    m_gas--;
    for(int i=1; i<=3; i++){
        playSound(SOUND_PLAYER_FIRE);
        if(direction==GraphObject::up){
            if(!canFire(x, y+i*SPRITE_HEIGHT))
                return;
            m_member.push_front(new Flame(x,y+i*SPRITE_HEIGHT,direction,this));}
        else if(direction==GraphObject::down){
            if(!canFire(x, y-i*SPRITE_HEIGHT))
                return;
            m_member.push_front(new Flame(x,y-i*SPRITE_HEIGHT,direction,this));}
        else if (direction==GraphObject::left){
            if(!canFire(x-i*SPRITE_HEIGHT,y))
                return;
            m_member.push_front(new Flame(x-i*SPRITE_HEIGHT,y,direction,this));}
        else if (direction==GraphObject::right){
            if(!canFire(x+i*SPRITE_HEIGHT,y))
                    return;
            m_member.push_front(new Flame(x+i*SPRITE_HEIGHT,y,direction,this));}
    }
}

void StudentWorld::playerLandmine(double x, double y){
    if(m_land<=0)
        return;
    m_land--;
    m_member.push_back(new Landmine(x,y,this));
}

void StudentWorld::LandmineExplode(double x, double y){
    m_member.push_front(new Pit(x,y,this));
    if(canFire(x, y))
        m_member.push_front(new Flame(x,y,GraphObject::up,this));
    
    if(canFire(x+SPRITE_WIDTH, y))
        m_member.push_front(new Flame(x+SPRITE_WIDTH,y,GraphObject::up,this));
    if(canFire(x-SPRITE_WIDTH, y))
        m_member.push_front(new Flame(x-SPRITE_WIDTH,y,GraphObject::up,this));
    if(canFire(x, y+SPRITE_HEIGHT))
        m_member.push_front(new Flame(x,y+SPRITE_HEIGHT,GraphObject::up,this));
    if(canFire(x, y-SPRITE_HEIGHT))
        m_member.push_front(new Flame(x,y-SPRITE_HEIGHT,GraphObject::up,this));

    if(canFire(x+SPRITE_WIDTH, y+SPRITE_HEIGHT))
        m_member.push_front(new Flame(x+SPRITE_WIDTH,y+SPRITE_HEIGHT,GraphObject::up,this));
    if(canFire(x+SPRITE_WIDTH, y-SPRITE_HEIGHT))
        m_member.push_front(new Flame(x+SPRITE_WIDTH,y-SPRITE_HEIGHT,GraphObject::up,this));
    if(canFire(x-SPRITE_WIDTH, y-SPRITE_HEIGHT))
        m_member.push_front(new Flame(x-SPRITE_WIDTH,y-SPRITE_HEIGHT,GraphObject::up,this));
    if(canFire(x-SPRITE_WIDTH, y+SPRITE_HEIGHT))
        m_member.push_front(new Flame(x-SPRITE_WIDTH,y+SPRITE_HEIGHT,GraphObject::up,this));
}

bool StudentWorld::noOverlap(double x, double y){
    if(overlap(x, y, m_Pene))
        return false;
    
    list<Actor*>::iterator it;
    for(it=m_member.begin();it!=m_member.end();it++){
            if(overlap(x, y, *it))
                return false;}
    return true;
}



void StudentWorld::moreVaccine(double x, double y){
    int dir = randInt(1, 4);
    switch (dir) {
        case 1: //up
            if(noOverlap(x, y+SPRITE_HEIGHT))
                m_member.push_front(new Vaccine(x, y+SPRITE_HEIGHT, this));
            return;
        case 2: //down
            if(noOverlap(x, y-SPRITE_HEIGHT))
                m_member.push_front(new Vaccine(x, y-SPRITE_HEIGHT, this));
            return;
        case 3: //right
            if(noOverlap(x+SPRITE_WIDTH, y))
                m_member.push_front(new Vaccine(x+SPRITE_WIDTH, y, this));
            return;
        case 4: //left
            if(noOverlap(x-SPRITE_WIDTH, y))
                m_member.push_front(new Vaccine(x-SPRITE_WIDTH, y, this));
            return;
    }
}

bool StudentWorld::toVomit(double x, double y, int direction){
    bool isNear = false;
    
    int chance = randInt(1, 3);
    if(chance==1 || chance==2)
        return false;
    
    if(direction == GraphObject::left){
        if(overlap(x-SPRITE_WIDTH, y, m_Pene)){
            isNear = true;}
        list<Actor*>::iterator it;
        for(it=m_member.begin();it!=m_member.end();it++){
            if(overlap(x-SPRITE_WIDTH, y, *it))
                if((*it)->isHuman())
                    isNear = true;}
        if(isNear == true){
            m_member.push_back(new Vomit(x-SPRITE_WIDTH, y, GraphObject::left,this));
            return true;}}
 
    else if(direction == GraphObject::right){
        if(overlap(x+SPRITE_WIDTH, y, m_Pene)){
            isNear = true;}
        list<Actor*>::iterator it;
        for(it=m_member.begin();it!=m_member.end();it++){
            if(overlap(x+SPRITE_WIDTH, y, *it))
                if((*it)->isHuman())
                    isNear = true;}
        if(isNear == true){
            m_member.push_back(new Vomit(x+SPRITE_WIDTH, y, GraphObject::right,this));
            return true;}}

    else if(direction == GraphObject::up){
        if(overlap(x, y+SPRITE_HEIGHT, m_Pene)){
            isNear = true;}
        list<Actor*>::iterator it;
        for(it=m_member.begin();it!=m_member.end();it++){
            if(overlap(x, y+SPRITE_HEIGHT, *it))
                if((*it)->isHuman())
                    isNear = true;}
        if(isNear == true){
            m_member.push_back(new Vomit(x, y+SPRITE_HEIGHT, GraphObject::up,this));
            return true;}}
  
    else if(direction == GraphObject::down){
        if(overlap(x, y-SPRITE_HEIGHT, m_Pene)){
            isNear = true;}
        list<Actor*>::iterator it;
        for(it=m_member.begin();it!=m_member.end();it++){
            if(overlap(x, y-SPRITE_HEIGHT, *it))
                if((*it)->isHuman())
                    isNear = true;}
        if(isNear == true){
            m_member.push_back(new Vomit(x, y-SPRITE_HEIGHT, GraphObject::down,this));
            return true;}}
    return false;
}

int StudentWorld::findDirection(double x, double y){
    double minDistance=sqrt(pow((m_Pene)->getX()-x,2)+pow((m_Pene)->getY()-y,2));
    int dir = -1;
    Actor* closet = m_Pene;
    list<Actor*>::iterator it;
    for(it=m_member.begin();it!=m_member.end();it++){
        if(sqrt(pow((*it)->getX()-x,2)+pow((*it)->getY()-y,2))<minDistance){
            if((*it)->isHuman()){
                minDistance=sqrt(pow((*it)->getX()-x,2)+pow((*it)->getY()-y,2));
                closet = *it;}
        }
    }
    if(minDistance>80)
        return dir;
    if(x==closet->getX()){
        if(y>closet->getY())
            dir = GraphObject::down;
        else
            dir = GraphObject::up;}
    else if (y==closet->getY()){
        if(x>closet->getX())
            dir = GraphObject::left;
        else
            dir = GraphObject::right;}
    else if(randInt(1, 2)==1){ //move up and down
        if(y>closet->getY())
            dir = GraphObject::down;
        else
            dir = GraphObject::up;}
    else{
        if(x>closet->getX())
            dir = GraphObject::left;
        else
            dir = GraphObject::right;}

    return dir;
}

void StudentWorld::newZombie(double x, double y){
    int random = randInt(1, 10);
    if(random<=3)
        m_member.push_back(new SmartZombie(x,y,this));
    else
        m_member.push_back(new DumbZombie(x,y,this));
}

double StudentWorld::distancePene(double x, double y){
    return sqrt(pow((m_Pene)->getX()-x,2)+pow((m_Pene)->getY()-y,2));
}

int StudentWorld::findDirectionPene(double x, double y, Actor* moving){
    if(x==m_Pene->getX()){
        if(y>m_Pene->getY()){
            if(notblocked(x, y-2, moving))
                return GraphObject::down;}
        else{
            if(notblocked(x, y+2, moving))
                return GraphObject::up;}}
    else if(y==m_Pene->getY()){
        if(x>m_Pene->getX()){
            if(notblocked(x-2, y, moving))
                return GraphObject::left;}
        else{
            if(notblocked(x+2, y, moving))
                return GraphObject::right;}}
    else if(randInt(1, 2)==1){ //up and down
        if(y>m_Pene->getY()){
            if(notblocked(x, y-2, moving))
                return GraphObject::down;}
        else{
            if(notblocked(x, y+2, moving))
                return GraphObject::up;}}
    else{if(x>m_Pene->getX()){
            if(notblocked(x-2, y, moving))
                return GraphObject::left;}
        else{
            if(notblocked(x+2, y, moving))
                return GraphObject::right;}}
    return -1;
}


double StudentWorld::distanceZombie(double x, double y){
    double minDistance = 10000;
    list<Actor*>::iterator it;
    for(it=m_member.begin();it!=m_member.end();it++){
        if((*it)->isZombie()){
            if(sqrt(pow((*it)->getX()-x,2)+pow((*it)->getY()-y,2))<minDistance){
                minDistance=sqrt(pow((*it)->getX()-x,2)+pow((*it)->getY()-y,2));}
        }
    }
    return minDistance;
}

int StudentWorld::findDirectionZombie(double x, double y, Actor* moving){
    double minDistance=10000;
    bool hasPointer = false;
    Actor* closet = m_member.front();
    list<Actor*>::iterator it;
    for(it=m_member.begin();it!=m_member.end();it++){
        if(sqrt(pow((*it)->getX()-x,2)+pow((*it)->getY()-y,2))<minDistance){
            if((*it)->isZombie()){
                minDistance=sqrt(pow((*it)->getX()-x,2)+pow((*it)->getY()-y,2));
                closet = *it;
                hasPointer=true;
            }
        }
    }
    int dir=-1;
    if(!hasPointer)
        return dir;
    if(sqrt(pow((closet)->getX()-(x+2),2)+pow((closet)->getY()-y,2))>minDistance){//move right
        if(notblocked(x+2, y, moving)){
            minDistance = sqrt(pow((closet)->getX()-(x+2),2)+pow((closet)->getY()-y,2));
            dir = GraphObject::right;
        }}
    if (sqrt(pow((closet)->getX()-(x-2),2)+pow((closet)->getY()-y,2))>minDistance){//move left
        if(notblocked(x-2, y, moving)){
            minDistance = sqrt(pow((closet)->getX()-(x-2),2)+pow((closet)->getY()-y,2));
            dir = GraphObject::left;
        }}
    if (sqrt(pow((closet)->getX()-x,2)+pow((closet)->getY()-(y+2),2))>minDistance){//move up
        if(notblocked(x, y+2, moving)){
            minDistance = sqrt(pow((closet)->getX()-x,2)+pow((closet)->getY()-(y+2),2));
            dir = GraphObject::up;
        }}
    if (sqrt(pow((closet)->getX()-x,2)+pow((closet)->getY()-(y-2),2))>minDistance){//move down
        if(notblocked(x, y-2, moving)){
            minDistance = sqrt(pow((closet)->getX()-x,2)+pow((closet)->getY()-(y-2),2));
            dir = GraphObject::down;
        }}
    return dir;
}

