#include "StudentWorld.h"
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
#include <queue>
#include <vector>

using namespace std;


GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}

// Students:  Add code to this file (if you wish), StudentWorld.h, Actor.h and Actor.cpp

StudentWorld::StudentWorld(string assetDir): GameWorld(assetDir)
{
}
int StudentWorld::randInt(int lowest, int highest)
{
    if (highest < lowest)
        swap(highest, lowest); //organize the numbers in times of size
    return lowest + (rand() % (highest - lowest + 1)); //return the corresponding random integer
}

 int StudentWorld ::init()
{
    addDirt();
    addBoulders();
    addOil();
    addGoldNuggets();
    ticksToAddProtester=0;
    addProtester();
    m_frackman=new FrackMan(this);
    ticksToAddProtester=25>( 200-getLevel())?25: 200-getLevel();
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld ::move()
{
    
    setDisplayText();// Update the Game Status Line
    addSonarAndWater();
    addProtester();
   
    if(m_frackman->isAlive()) // Give each Actor a chance to do something
    {
        m_frackman->doSomething();
        int i;
        for(i=0; i<m_actor.size(); i++)
        {
            m_actor.at(i)->doSomething();
        }
        removeDeadActors();
        if (oil_left <= 0) {
            playSound(SOUND_FINISHED_LEVEL);
            return GWSTATUS_FINISHED_LEVEL;
        }
    }
    else{
        return GWSTATUS_PLAYER_DIED;
    }
    
    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld ::cleanUp()
{
    for (int i=0; i<64; i++) {
        for(int j=0;j<60;j++){
            if(m_dirt[i][j]!=nullptr){
                delete m_dirt[i][j];
                m_dirt[i][j] = nullptr;
            }
        }
    }
    delete m_frackman;
    for(vector<Actor*>::iterator p=m_actor.begin();p!=m_actor.end();)
    {                                                           //if any actors are left, delete them
            Actor* killme=*p;
            p=m_actor.erase(p);
            delete killme;
    }
    m_actor.clear(); //clear the vector
}

 StudentWorld::~StudentWorld()
{
    cleanUp();

}

void StudentWorld::removeDeadActors(){
    for(vector<Actor*>::iterator p=m_actor.begin();p!=m_actor.end();){
        if(!(*p)->isAlive()){
            Actor* killme=*p;
            p=m_actor.erase(p);
            delete killme;
        }
        else
            p++;
    }
}

void StudentWorld::setDisplayText()
{
    int score = getScore();
    int level = getLevel();
    int lives = getLives();
    int health = m_frackman->getHealth();
    int squirts = m_frackman->getWaterUnits();
    int gold = m_frackman->getGoldNuggets();
    int sonar = m_frackman->getSonarCharge();
    int barrelsLeft = oil_left;
    // create a string from your statistics, of the form:
    // “Scr: 0321000  Lvl: 52  Lives: 3  Hlth: 80%  Water: 20  Gld: 3  Sonar: 1  Oil Left: 2”
    string s_score=to_string(score);
    int zero_size=8-(int)s_score.size();
    for(int i=0;i<zero_size;i++)
        s_score="0"+s_score;
    string s="Scr: "+s_score+"  Lvl: "+to_string(level)+"  Lives: "+to_string(lives)+"  Hlth: "+to_string(health)+"%  Water: "+to_string(squirts)+"  Gld: "+to_string(gold)+"  Sonar: "+to_string(sonar)+"  Oil Left: "+to_string(barrelsLeft);
    setGameStatText(s);//update the display text at the top of the screen with newly created stats
}

bool StudentWorld::withinRadius(int x, int y){
    int i;

    for(i=0; i<m_actor.size(); i++)
    {
        if(sqrt(pow(m_actor.at(i)->getX()-x, 2)+pow(m_actor.at(i)->getY()-y, 2))<=6)
            return true;
    }
    return false;
}


void StudentWorld::addDirt(){
    int i,j;
    for (i=0; i<64; i++) {
        for(j=0; j<60; j++){
            if(i>=30&&i<34&&j>=4)
                continue;
            m_dirt[i][j]=new Dirt(i,j,this);
        }
    }
    
}
void StudentWorld::addBoulders(){
    
    int B = (getLevel()/ 2 + 2)< 6? getLevel()/ 2 + 2: 6;
   
    for (int i=0; i<B; i++) {
        int x = randInt(0, 60);
        int y = randInt(20, 56);
        while ((x > 26 && x < 34) || withinRadius(x, y)){
            x = randInt(0, 60);
            y = randInt(20, 56);
        }
        
        Boulders* b=new Boulders(x,y,this);
        m_actor.push_back(b);
        for (int i=x; i<x+4; i++) {
            for(int j=y;j<y+4;j++){
                if(m_dirt[i][j]!=nullptr)
                {
                    delete m_dirt[i][j];
                    m_dirt[i][j] = nullptr;
                }
            }
        }
    }
    
}
void StudentWorld::addSquirt(int x, int y, Actor::Direction dir){
    
    Squirt* s=new Squirt(x,y,this,dir);
    m_actor.push_back(s);
    
}
void StudentWorld::addOil(){
    
    oil_left=(2+getLevel())< 20? 2+getLevel(): 20;
    for (int i=0; i<oil_left; i++) {
        int x = randInt(0, 60);
        int y = randInt(20, 56);
        while ((x > 26 && x < 34) || withinRadius(x, y)){
            x = randInt(0, 60);
            y = randInt(20, 56);
        }
        
        BarrelsOfOil* o=new BarrelsOfOil(x,y,this);
        m_actor.push_back(o);
        /*for (int i=x; i<x+4; i++) {
            for(int j=y;j<y+4;j++){
                if(m_dirt[i][j]!=nullptr)
                {
                    delete m_dirt[i][j];
                    m_dirt[i][j] = nullptr;
                }
            }
        }*/
    }
}
void StudentWorld::addGoldNuggets(){
    
    int G = (5-getLevel()/ 2)> 2? 5-getLevel()/ 2: 2;
    
    for (int i=0; i<G; i++) {
        int x = randInt(0, 60);
        int y = randInt(20, 56);
        while ((x > 26 && x < 34) || withinRadius(x, y)){
            x = randInt(0, 60);
            y = randInt(20, 56);
        }
        
        GoldNuggets* g=new GoldNuggets(x,y,this,true);
        m_actor.push_back(g);
       /* for (int i=x; i<x+4; i++) {
            for(int j=y;j<y+4;j++){
                if(m_dirt[i][j]!=nullptr)
                {
                    delete m_dirt[i][j];
                    m_dirt[i][j] = nullptr;
                }
            }
        }*/
    }
}
void StudentWorld::addSonarAndWater(){
    int G = getLevel() * 25 + 300;
    if(rand()%G==0){
        if (rand()%5==0) {
            SonarKit* s=new SonarKit(this);
            m_actor.push_back(s);
        }
        else{
            int x = randInt(0, 60);
            int y = randInt(0, 56);
            while (dirtThere(x, y)|| withinRadius(x, y)){
                x = randInt(0, 60);
                y = randInt(0, 56);
            }
            WaterPool* w=new WaterPool(x,y,this);
            m_actor.push_back(w);
        }
    }
}

void StudentWorld::addTempGold(int x, int y){
    GoldNuggets* g=new GoldNuggets(x,y,this,false);
    m_actor.push_back(g);
}

void StudentWorld::addProtester(){
    int targetNumOfProtester = 15<(2+getLevel() * 1.5)?15:(2+getLevel() * 1.5);
    int numOfProtester=0;
    int probabilityOfHardcore = 90<(getLevel() * 10 + 30)?90:(getLevel() * 10 + 30);
    for(int i=0; i<m_actor.size(); i++)
    {
        if(m_actor.at(i)->getID()==IID_PROTESTER||m_actor.at(i)->getID()==IID_HARD_CORE_PROTESTER)
        {
            numOfProtester++;
        }
    }

    if (ticksToAddProtester<=0&&numOfProtester<targetNumOfProtester) {
        RegularProtester* p;
        if((rand()%100)<probabilityOfHardcore)
            p=new HardcoreProtester(IID_HARD_CORE_PROTESTER,this);
        else
            p=new RegularProtester(IID_PROTESTER,this);
        m_actor.push_back(p);
        ticksToAddProtester=25>(200-getLevel())?25: 200-getLevel();
    }else
        ticksToAddProtester--;
}

bool StudentWorld::dirtThere(int x, int y)
{
    int i,j;
    for(i=x;i<x+4;i++)
        for (j=y; j<y+4; j++) {
            if(m_dirt[i][j]!=nullptr)
                return true;
        }
    return false;
}

void StudentWorld::deleteDirt(int x, int y)
{
    int i;
    Actor::Direction direction=m_frackman->getDirection();
        switch(direction)
    {
        case GraphObject::right:
           
        case GraphObject::left:
            for(i=y;i<y+4;i++)
            {
                if(m_dirt[x][i]!=nullptr){
                    delete m_dirt[x][i];
                    m_dirt[x][i] = nullptr;
                }
            }
            break;
        case GraphObject::up:
           
        case GraphObject::down:
            for(i=x;i<x+4;i++)
            {
                if(m_dirt[i][y]!=nullptr){
                    delete m_dirt[i][y];
                    m_dirt[i][y] = nullptr;
                }
            }
            break;
        default:break;

    }
}
bool StudentWorld::dirtBelow(int x, int y){
    int i;
    for(i=x;i<x+4;i++){
        if(m_dirt[i][y-1]!=nullptr)
            return true;
    }
        
    return false;
}
bool StudentWorld::boulderCollision(int x, int y){
    int i;
    if (y<=0) {
        return true;
    }
    for(i=x; i<x+4; i++){
        if(m_dirt[i][y-1]!=nullptr)
            return true;
    }
    for(i=0; i<m_actor.size(); i++)
    {
        if(m_actor.at(i)->getID()==IID_BOULDER)//if there is another boulder, return true
        {
            if(abs(m_actor.at(i)->getX()-x)<=3&&y==m_actor.at(i)->getY()+3)
                return true;
        }
    }
    return false;
}

void StudentWorld::boulderAnnoyActor(int x, int y){
    int i;
    if(abs(m_frackman->getX()-x)<=3 && y-m_frackman->getY()<=3){
        m_frackman->setDead();
        decLives();
    }

    for(i=0; i<m_actor.size(); i++)
    {
        if(m_actor.at(i)->getID()==IID_PROTESTER||m_actor.at(i)->getID()==IID_HARD_CORE_PROTESTER)
        {
            if(abs(m_actor.at(i)->getX()-x)<=3&&y-m_actor.at(i)->getY()<=3)
                m_actor.at(i)->getAnnoyed(10);
            increaseScore(500);
        }
    }

}

void StudentWorld::attackProtester(int x,int y){
    int i;
    for(i=0; i<m_actor.size(); i++)
    {
        if(m_actor.at(i)->getID()==IID_PROTESTER)
        {
            if(abs(m_actor.at(i)->getX()-x)<=3&&y-m_actor.at(i)->getY()<=3){
                m_actor.at(i)->getAnnoyed(2);
                increaseScore(100);
               
            }
        }
        if(m_actor.at(i)->getID()==IID_HARD_CORE_PROTESTER)
        {
            if(abs(m_actor.at(i)->getX()-x)<=3&&y-m_actor.at(i)->getY()<=3){
                m_actor.at(i)->getAnnoyed(2);
                increaseScore(250);
                
            }
        }
    }
 
}

bool StudentWorld::actorThere(int x,int y, int IID){
    int i;
    
    for(i=0; i<m_actor.size(); i++)
    {
        if(m_actor.at(i)->getID()==IID)
        {
            //if(abs(m_actor.at(i)->getX()-x)<=3&&abs(m_actor.at(i)->getY()-y)<=3)
            if(sqrt(pow((m_actor.at(i)->getX()-x),2)+pow((m_actor.at(i)->getY()-y),2))<=3)
            {
                return true;
            }
        }
    }
    return false;
}

bool StudentWorld::frackmanThere(int x,int y, int distance){
    if(abs(m_frackman->getX()-x)<=distance&&abs(m_frackman->getY()-y)<=distance)
        return true;
    return false;
}

bool StudentWorld::senseFrackman(int x,int y,int distance){
    if(abs(m_frackman->getX()-x)+abs(m_frackman->getY()-y)<=distance)
        return true;
    return false;
}

void StudentWorld::decreaseOil(){
    oil_left--;
}

void StudentWorld::increaseGold(){
    m_frackman->incGold();
}

void StudentWorld::increaseSonar(){
    m_frackman->incSonar();
}
void StudentWorld::increaseWater(){
    m_frackman->incWater();
}

void StudentWorld::useSonar(int x, int y){
    int i;
    
    for(i=0; i<m_actor.size(); i++)
    {
        if (m_actor.at(i)->getID()==IID_GOLD||m_actor.at(i)->getID()==IID_BARREL) {
            if(sqrt(pow(m_actor.at(i)->getX()-x, 2)+pow(m_actor.at(i)->getY()-y, 2))<=12){
                m_actor.at(i)->setVisible(true);
            }
        }
    }
}
Actor::Direction StudentWorld::getFrackmanDirection(){
    return m_frackman->getDirection();
}

void StudentWorld::annoyFrackman(int points){
    m_frackman->getAnnoyed(points);
}

bool StudentWorld::lineOfSightToFrackMan(int x, int y){
    if(m_frackman->getX()==x){
        if(m_frackman->getY()>y)
        {
            for(int i=y;i<m_frackman->getY();i++)
            {
                if(dirtThere(x, i))
                    return false;
            }
        }else
           {
               for(int i=y;i>m_frackman->getY();i--)
               {
                   if(dirtThere(x, i))
                       return false;
               }
   
           }
        return true;
    }
    if(m_frackman->getY()==y){
        if(m_frackman->getX()>x)
        {
            for(int i=x;i<m_frackman->getX();i++)
            {
                if(dirtThere(i, y))
                    return false;
            }
        }else
        {
            for(int i=x;i>m_frackman->getX();i--)
            {
                if(dirtThere(i, y))
                    return false;
            }
            
        }
        return true;
    }
    return false;
}
Actor::Direction StudentWorld::directionToFrackMan(int x, int y){
    if(abs(m_frackman->getX()-x)<4){
        if(m_frackman->getY()>y)
        {
            return Actor::up;
        }else
        {
            return Actor::down;
        }
    }
    if(abs(m_frackman->getY()-y)<4){
        if(m_frackman->getX()>x)
        {
            return Actor::right;
        }else
        {
           return Actor::left;
        }
    }
    return Actor::none;
}

bool StudentWorld::pickUpGold(int x, int y){
    int i;
    for(i=0; i<m_actor.size(); i++)
    {
        if(m_actor.at(i)->getID()==IID_PROTESTER)
        {
            if(abs(m_actor.at(i)->getX()-x)<=3&&abs(y-m_actor.at(i)->getY())<=3){
                m_actor.at(i)->pickUpGold();
                return true;
            }
        }
        if(m_actor.at(i)->getID()==IID_HARD_CORE_PROTESTER)
        {
            if(abs(m_actor.at(i)->getX()-x)<=3&&abs(y-m_actor.at(i)->getY())<=3){
                m_actor.at(i)->pickUpGold();
                return true;
            }
        }
    }
    return false;    
}

vector<Coord> StudentWorld::calLeavePath(int x, int y){
    vector<Coord> leavePath;
    vector<Coord> coordVector;
    queue<Coord> coordQueue; // declare a queue of Coords
    Coord start(x,y,-1,-1);
    coordQueue.push(start); //enqueue item(x,y) at back of queue
    int maze[64][64]={0};
    int c,r;
    
    while (!coordQueue.empty()) {
        
        Coord current = coordQueue.front(); //look at front item in the queue
        coordQueue.pop();  //remove the front item from queue
        coordVector.push_back(current);
        r = current.r();
        c = current.c();
        
        if (c==60&&r==60)  //if the current position is equal to the exit
        {
            
            int px,py;
            vector<Coord>::reverse_iterator rp=coordVector.rbegin();
            while(x!=(*rp).c()||y!=(*rp).r())
            {
                leavePath.push_back(*rp);
                px=(*rp).pc();
                py=(*rp).pr();
                
                while (px!=(*rp).c()||py!=(*rp).r()) {
                    rp++;
                    if (rp==coordVector.rend())
                        break;
                }
                if (rp==coordVector.rend())
                    break;
            }
        }
        
        
        if ((r+1)<=60&&!dirtThere(c, r+1)&&!actorThere(c, r+1, IID_BOULDER)&&!maze[c][r+1])
        {  //Check the up position
            coordQueue.push(Coord(c, r+1,c,r));
            maze[c][r+1]=1;
        }
        if ((c+1)<=60&&!dirtThere(c+1, r)&&!actorThere(c+1, r, IID_BOULDER)&&!maze[c+1][r])
        { //Check the right position
            coordQueue.push(Coord(c+1, r,c,r));
            maze[c+1][r]=1;
        }
        if (r-1>=0&&!dirtThere(c, r-1)&&!actorThere(c, r-1, IID_BOULDER)&&!maze[c][r-1])
        {  //Check the down position
            coordQueue.push(Coord(c,r-1,c,r));
            maze[c][r-1]=1;
        }
        if (c-1>=0&&!dirtThere(c-1, r)&&!actorThere(c-1, r, IID_BOULDER)&&!maze[c-1][r])
        {  //Check the left position
            coordQueue.push(Coord(c-1,r,c,r));
            maze[c-1][r]=1;
        }
        
    }
    
    return leavePath;
}

Coord StudentWorld::calTrackPath(int x, int y){
    vector<Coord> coordVector;
    queue<Coord> coordQueue; // declare a queue of Coords
    Coord start(x,y,-1,-1);
    coordQueue.push(start); //enqueue item(x,y) at back of queue
    int maze[64][64]={0};
    int c,r;
    
    while (!coordQueue.empty()) {
        
        Coord current = coordQueue.front(); //look at front item in the queue
        coordQueue.pop();  //remove the front item from queue
        coordVector.push_back(current);
        r = current.r();
        c = current.c();
        
        if (c==m_frackman->getX()&&r==m_frackman->getY())  //if the current position is equal to the exit
        {
            
            int px,py;
            vector<Coord>::reverse_iterator rp=coordVector.rbegin();
            while(x!=(*rp).pc()||y!=(*rp).pr())
            {
                px=(*rp).pc();
                py=(*rp).pr();
           
                while (px!=(*rp).c()||py!=(*rp).r()) {
                    rp++;
                    if (rp==coordVector.rend())
                        break;
                }
                if (rp==coordVector.rend())
                    break;
            }
            return (*rp);
        }
        
        
        if ((r+1)<=60&&!dirtThere(c, r+1)&&!actorThere(c, r+1, IID_BOULDER)&&!maze[c][r+1])
        {  //Check the up position
            coordQueue.push(Coord(c, r+1,c,r));
            maze[c][r+1]=1;
        }
        if ((c+1)<=60&&!dirtThere(c+1, r)&&!actorThere(c+1, r, IID_BOULDER)&&!maze[c+1][r])
        { //Check the right position
            coordQueue.push(Coord(c+1, r,c,r));
            maze[c+1][r]=1;
        }
        if (r-1>=0&&!dirtThere(c, r-1)&&!actorThere(c, r-1, IID_BOULDER)&&!maze[c][r-1])
        {  //Check the down position
            coordQueue.push(Coord(c,r-1,c,r));
            maze[c][r-1]=1;
        }
        if (c-1>=0&&!dirtThere(c-1, r)&&!actorThere(c-1, r, IID_BOULDER)&&!maze[c-1][r])
        {  //Check the left position
            coordQueue.push(Coord(c-1,r,c,r));
            maze[c-1][r]=1;
        }
        
    }
    return start;
}


