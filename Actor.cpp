#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp
/**************************** Actor Implementation *****************************/
Actor:: Actor(int imageID, int startX, int startY,StudentWorld* world, Direction dir, double size,unsigned int depth)
      :GraphObject(imageID, startX, startY, dir, size, depth),m_world(world),m_alive(true),m_ID(imageID){
    
    setVisible(true);
    
}

Actor::~Actor(){
    
    setVisible(false);
}

bool Actor::isAlive(){
    
    return m_alive;
}
void Actor::setDead(){
    
    m_alive=false;
}

StudentWorld* Actor::getWorld() const
{
    return m_world;
}
unsigned int Actor::getID() const
{
    return m_ID;
}
void Actor::playSound(int soundID){
    
    m_world->playSound(soundID);
};
Actor::Direction Actor::randDir(){
    
    Direction dir[4]={ up, down, left, right };
    return dir[rand()%4];
}

void Actor::moveInDir(Direction dir){
    
    switch (dir) {
        case left:
            if (getX()-1>=0&&!getWorld()->dirtThere(getX()-1, getY())&&!getWorld()->actorThere(getX()-1,getY(),IID_BOULDER)) {
                moveTo(getX()-1, getY()); // move one square to the left
            }
            break;
        case right:
            if (getX()+1<=60&&!getWorld()->dirtThere(getX()+1, getY())&&!getWorld()->actorThere(getX()+1,getY(),IID_BOULDER)) {
                moveTo(getX()+1, getY()); // move one square to the right
            }
            break;
        case up:
            if (getY()+1<=60&&!getWorld()->dirtThere(getX(), getY()+1)&&!getWorld()->actorThere(getX(),getY()+1,IID_BOULDER)) {
                moveTo(getX(), getY()+1); // move one square to the up
            }
            break;
        case down:
            if (getY()-1>=0&&!getWorld()->dirtThere(getX(), getY()-1)&&!getWorld()->actorThere(getX(),getY()-1,IID_BOULDER)) {
                moveTo(getX(), getY()-1); // move one square to the down
            }
            break;
        default:break;
    }
    
}
bool Actor::canMoveInDir(Direction dir){
    
    switch (dir) {
        case left:
            if (getX()-1>=0&&!getWorld()->dirtThere(getX()-1, getY())&&!getWorld()->actorThere(getX()-1,getY(),IID_BOULDER)) {// can move one square to the left
                return true;
            }
            break;
        case right:
            if (getX()+1<=60&&!getWorld()->dirtThere(getX()+1, getY())&&!getWorld()->actorThere(getX()+1,getY(),IID_BOULDER)) { // can move one square to the right
                return true;
            }
            break;
        case up:
            if (getY()+1<=60&&!getWorld()->dirtThere(getX(), getY()+1)&&!getWorld()->actorThere(getX(),getY()+1,IID_BOULDER)) {// can move one square to the up
                return true;
            }
            break;
        case down:
            if (getY()-1>=0&&!getWorld()->dirtThere(getX(), getY()-1)&&!getWorld()->actorThere(getX(),getY()-1,IID_BOULDER)) {// can move one square to the down
                return true;
            }
            break;
        default:break;
    }
    return false;
    
}
bool Actor::isAtIntersection(Direction dir){
    switch (dir) {
        case left:
            
        case right:
            if (getY()+1<=60&&!getWorld()->dirtThere(getX(), getY()+1)&&!getWorld()->actorThere(getX(),getY()+1,IID_BOULDER)) //can move one square to the up
                return true;
           
            if (getY()-1>=0&&!getWorld()->dirtThere(getX(), getY()-1)&&!getWorld()->actorThere(getX(),getY()-1,IID_BOULDER)) // can move one square to the down
                return true;
            break;
            
        case up:
           
        case down:
            if (getX()-1>=0&&!getWorld()->dirtThere(getX()-1, getY())&&!getWorld()->actorThere(getX()-1,getY(),IID_BOULDER))  // can move one square to the left
                return true;
           
            if (getX()+1<=60&&!getWorld()->dirtThere(getX()+1, getY())&&!getWorld()->actorThere(getX()+1,getY(),IID_BOULDER)) // can move one square to the right
                return true;
            break;
            
        default:break;
    }
    return false;
}
Actor::Direction Actor::pickViablePerpendicularDir(Direction dir){
    bool l=false, r=false, u=false, d=false;
    Direction d1[2]={ up, down };
    Direction d2[2]={ left, right };

    switch (dir) {
        case left:
            
        case right:
            if (getY()+1<=60&&!getWorld()->dirtThere(getX(), getY()+1)&&!getWorld()->actorThere(getX(),getY()+1,IID_BOULDER)) //can move one square to the up
                u=true;
            
            if (getY()-1>=0&&!getWorld()->dirtThere(getX(), getY()-1)&&!getWorld()->actorThere(getX(),getY()-1,IID_BOULDER)) // can move one square to the down
                d=true;
            break;
            
        case up:
            
        case down:
            if (getX()-1>=0&&!getWorld()->dirtThere(getX()-1, getY())&&!getWorld()->actorThere(getX()-1,getY(),IID_BOULDER))  // can move one square to the left
                l=true;
            
            if (getX()+1<=60&&!getWorld()->dirtThere(getX()+1, getY())&&!getWorld()->actorThere(getX()+1,getY(),IID_BOULDER)) // can move one square to the right
                r=true;
            break;
            
        default:break;
    }
    if(u&&d)
        return d1[rand()%2];
    else if(u)
        return up;
    else if(d)
        return down;
    if(l&&r)
        return d2[rand()%2];
    else if(l)
        return left;
    else if(r)
        return right;
    return none;
}
/**************************** Dirt Implementation *****************************/
Dirt::Dirt(int X, int Y, StudentWorld* world): Actor(IID_DIRT, X, Y, world,right,0.25,3){
}

Dirt:: ~Dirt(){
}
/**************************** Frackman Implementation *****************************/
FrackMan::FrackMan(StudentWorld* world): Actor(IID_PLAYER, 30, 60, world,right,1.0,0){
    hit_points=10;
    water_units=5;
    sonar_charge=1;
    gold_nuggets=0;
}

FrackMan:: ~FrackMan(){
}

int FrackMan::getHealth(){
    float health=hit_points/10.0*100;
    return health;
}
int FrackMan::getWaterUnits(){
    return water_units;
}
int FrackMan::getSonarCharge(){
    return sonar_charge;
}
int FrackMan::getGoldNuggets(){
    return gold_nuggets;
}
void FrackMan::incGold(){
    gold_nuggets++;
}
void FrackMan::incSonar(){
    sonar_charge+=2;
}
void FrackMan::incWater(){
    water_units+=5;
}
void FrackMan::getAnnoyed(int points){
    hit_points=hit_points-points;
    if (hit_points<=0) {
        setDead();
        getWorld()->decLives();
    }
}
void FrackMan::doSomething(){
    int ch;
    
    if (!isAlive())
        return;
    
    if (getWorld()->getKey(ch) == true) {
            // user hit a key this tick!
        
            switch (ch)
            {
            case KEY_PRESS_LEFT:
                if(getDirection()!=left)
                    setDirection(left);
                else{
                    if (getX()-1>=0&&!getWorld()->actorThere(getX()-1,getY(),IID_BOULDER)) {
                        moveTo(getX()-1, getY()); // move one square to the left
                        if(getWorld()->dirtThere(getX(), getY())){
                            getWorld()->deleteDirt(getX(), getY());
                            playSound(SOUND_DIG);
                        }
                    }
                }
                
                break;
            case KEY_PRESS_RIGHT:
                if(getDirection()!=right)
                    setDirection(right);
                else{
                    if (getX()+1<=60&&!getWorld()->actorThere(getX()+1,getY(),IID_BOULDER)) {
                        moveTo(getX()+1, getY()); // move one square to the right
                        if(getWorld()->dirtThere(getX()+3, getY())){
                            getWorld()->deleteDirt(getX()+3, getY());
                            playSound(SOUND_DIG);
                        }
                    }
                }
                
                break;
            case KEY_PRESS_UP:
                if(getDirection()!=up)
                    setDirection(up);
                else{
                    if (getY()+1<=60&&!getWorld()->actorThere(getX(),getY()+1,IID_BOULDER)) {
                        moveTo(getX(), getY()+1); // move one square to the up
                        if(getWorld()->dirtThere(getX(), getY()+3)){
                            getWorld()->deleteDirt(getX(), getY()+3);
                            playSound(SOUND_DIG);
                        }
                    }
                }
                
                    break;
            case KEY_PRESS_DOWN:
                if(getDirection()!=down)
                    setDirection(down);
                else{
                    if (getY()-1>=0&&!getWorld()->actorThere(getX(),getY()-1,IID_BOULDER)) {
                        moveTo(getX(), getY()-1); // move one square to the down
                        if(getWorld()->dirtThere(getX(), getY())){
                            getWorld()->deleteDirt(getX(), getY());
                            playSound(SOUND_DIG);
                        }
                    }
                }
                    break;
            case KEY_PRESS_SPACE:
                    if(water_units>0){
                        switch (getDirection()) {
                            case right:
                                getWorld()->addSquirt(getX() + 4, getY(), right);
                                break;
                            case left:
                                getWorld()->addSquirt(getX() - 4, getY(), left);
                                break;
                            case up:
                                getWorld()->addSquirt(getX(), getY() + 4, up);
                                break;
                            case down:
                                getWorld()->addSquirt(getX(), getY() - 4, down);
                                break;
                            default:break;
                        }
                    getWorld()->playSound(SOUND_PLAYER_SQUIRT);
                    water_units--;
                    }
                break;
            case KEY_PRESS_TAB:
                    if(gold_nuggets>0){
                        getWorld()->addTempGold(getX(), getY());
                        gold_nuggets--;
                    }
                    break;
            case 'Z':
            case 'z':
                    if (sonar_charge > 0) {
                        sonar_charge--;
                        getWorld()->useSonar(getX(), getY());
                        getWorld()->playSound(SOUND_SONAR);
                    }
                    break;
            case KEY_PRESS_ESCAPE:
                    setDead();
                    break;
            
            }
        }
}
/**************************** Boulders Implementation *****************************/
Boulders::Boulders(int X, int Y, StudentWorld* world): Actor(IID_BOULDER, X, Y, world,down,1.0,1){
    wait_time=30;
    is_falling = true;
}
Boulders:: ~Boulders(){
}

void Boulders::doSomething(){
    if (!isAlive())
        return;
    if(getWorld()->dirtBelow(getX(), getY()))
        return;
    wait_time--;
    if(wait_time<=0){
        if(is_falling){
            playSound(SOUND_FALLING_ROCK);
            is_falling=false;
        }
        if (getWorld()->boulderCollision(getX(), getY()-1)){
            setDead(); //set the boulder to dead
            return;
        } else
        moveTo(getX(), getY()-1); // move one square down
    
        getWorld()->boulderAnnoyActor(getX(), getY());
    }
}
/**************************** Squirt Implementation *****************************/
Squirt::Squirt(int X, int Y,  StudentWorld* world, Direction dir): Actor(IID_WATER_SPURT, X, Y, world,dir,1.0,1){
    travel_distance=4;
}
Squirt::~Squirt(){
}

void Squirt::doSomething(){
    if(!isAlive())
        return;
    
    if(travel_distance>0){
        switch (getDirection())
        {
            case GraphObject::left:
                if (getX()-1>=0&&!getWorld()->dirtThere(getX()-1, getY())&&!getWorld()->actorThere(getX()-1, getY(), IID_BOULDER)) {
                    moveTo(getX()-1, getY()); // move one square to the left
                    travel_distance--;
                }else
                    setDead();
                break;
            case GraphObject::right:
                if (getX()+1<=60&&!getWorld()->dirtThere(getX()+1, getY())&&!getWorld()->actorThere(getX()+1, getY(), IID_BOULDER)) {
                    moveTo(getX()+1, getY()); // move one square to the right
                    travel_distance--;
                }else
                    setDead();
                break;
            case GraphObject::up:
                if (getY()+1<=60&&!getWorld()->dirtThere(getX(), getY()+1)&&!getWorld()->actorThere(getX(), getY()+1, IID_BOULDER)) {
                    moveTo(getX(), getY()+1); // move one square to the up
                    travel_distance--;
                }else
                    setDead();
                break;
               
            case GraphObject::down:
                if (getY()-1>=0&&!getWorld()->dirtThere(getX(), getY()-1)&&!getWorld()->actorThere(getX(), getY()-1, IID_BOULDER)) {
                    moveTo(getX(), getY()-1); // move one square to the down
                    travel_distance--;
                }else
                    setDead();
                break;
                
            default:break;
        }
    }else
    {
        getWorld()->attackProtester(getX(), getY());
        setDead();
    }
    
    return;
}
/**************************** Barrel of Oil Implementation *****************************/
BarrelsOfOil::BarrelsOfOil(int X, int Y, StudentWorld* world):Actor(IID_BARREL, X, Y, world,right,1.0,2){    
    setVisible(false);
}
BarrelsOfOil::~BarrelsOfOil(){
}
void BarrelsOfOil::doSomething(){
    if(!isAlive())
        return;
    if(!isVisible()&&getWorld()->frackmanThere(getX(), getY(),4)){
        setVisible(true);
        return;
    }
    if(getWorld()->frackmanThere(getX(), getY(),3)){
        setDead();
        playSound(SOUND_FOUND_OIL);
        getWorld()->increaseScore(1000);
        getWorld()->decreaseOil();
    }
    
}
/**************************** Gold Nuggets Implementation *****************************/
GoldNuggets::GoldNuggets(int X, int Y, StudentWorld* world, bool permanent):Actor(IID_GOLD, X, Y, world,right,1.0,2){
    life_time=100;
    is_permanent=permanent;
    if (is_permanent)
        setVisible(false);
    else
        setVisible(true);
}
GoldNuggets::~GoldNuggets(){
}
void GoldNuggets::doSomething(){
    if(!isAlive())
        return;
    if(is_permanent){
        if(!isVisible()&&getWorld()->frackmanThere(getX(), getY(),4)){
            setVisible(true);
            return;
        }
        if(getWorld()->frackmanThere(getX(), getY(),3)){
            setDead();
            playSound(SOUND_GOT_GOODIE);
            getWorld()->increaseScore(10);
            getWorld()->increaseGold();
        }
    }
    else{
        if (life_time>0) {
            if(getWorld()->pickUpGold(getX(),getY()))
                setDead();
            life_time--;
        }else
            setDead();
    }
}


/**************************** Sonar Kit Implementation *****************************/
SonarKit::SonarKit(StudentWorld* world):Actor(IID_SONAR, 0, 60, world,right,1.0,2){
    life_time=100>(300-10*getWorld()->getLevel())?100:300-10*getWorld()->getLevel();
    
}
SonarKit::~SonarKit(){
}
void SonarKit::doSomething(){
    if(!isAlive())
        return;
    if (life_time>0) {
        if(getWorld()->frackmanThere(getX(), getY(),3)){
            setDead();
            playSound(SOUND_GOT_GOODIE);
            getWorld()->increaseSonar();
            getWorld()->increaseScore(75);
        }
        life_time--;
    }else
        setDead();
}
/**************************** Water Pool Implementation *****************************/
WaterPool::WaterPool(int X, int Y, StudentWorld* world):Actor(IID_WATER_POOL, X, Y, world,right,1.0,2){
    life_time=100>(300-10*getWorld()->getLevel())?100:300-10*getWorld()->getLevel();
    
}
WaterPool::~WaterPool(){
}
void WaterPool::doSomething(){
    if(!isAlive())
        return;
    if (life_time>0) {
        if(getWorld()->frackmanThere(getX(), getY(),3)){
            setDead();
            playSound(SOUND_GOT_GOODIE);
            getWorld()->increaseWater();
            getWorld()->increaseScore(100);
        }
        life_time--;
    }else
        setDead();
}
/**************************** Regular Protester Implementation *****************************/
RegularProtester::RegularProtester(int IID,StudentWorld* world): Actor(IID, 60, 60, world,left,1.0,0){
    if (IID==IID_PROTESTER) {
        hit_points=5;
    }else
        if (IID==IID_HARD_CORE_PROTESTER) {
            hit_points=20;
        }
    
    leaveOilField=false;
    ticksToWaitBetweenMoves = 0>int(3-getWorld()->getLevel()/4)? 0:int(3-getWorld()->getLevel()/4);
    ticksToWaitBetweenShouts = 15;
    ticksToMakePerpendicularTurn= 200;
    ticksToBeFreezed=0;
    calLeavePath=false;
    numSquaresToMoveInCurrentDirection=getWorld()->randInt(8, 60);
}

RegularProtester:: ~RegularProtester(){
}

void RegularProtester::doSomething(){
    if(ticksToBeFreezed<=0)
    {
      if(ticksToWaitBetweenMoves<0)
     {
        if(!isAlive())
            return;
        if(leaveOilField)
        {
            if(getX()==60&&getY()==60)
            {
                setDead();
                return;
            }else
            {
               if(!calLeavePath)
               {
                leavepath = getWorld()->calLeavePath(getX(), getY());
                calLeavePath=true;
               }
               
                if(leavepath.size()>0)
                {
                    vector<Coord>::reverse_iterator rp=leavepath.rbegin();
                    int nx=(*rp).c();
                    int ny=(*rp).r();
                    if(getX()==nx)
                    {
                        if(getY()<ny)
                        {
                            setDirection(up);
                        }else
                        {
                            setDirection(down);
                        }
                    }
                    if(getY()==ny)
                    {
                        if(getX()<nx)
                        {
                            setDirection(right);
                        }else
                        {
                            setDirection(left);
                        }
                    }
                    moveTo(nx, ny);
                    leavepath.pop_back();
                    setTicksToWait();
                    return;
                }
               
            }
        }else
        {
            if(getWorld()->frackmanThere(getX(), getY(), 4))
               //&&getDirection()==getWorld()->directionToFrackMan(getX(), getY()))
            {
                if(ticksToWaitBetweenShouts<0)
                {
                    playSound(SOUND_PROTESTER_YELL);
                    getWorld()->annoyFrackman(2);
                    ticksToWaitBetweenShouts=15;
                    setTicksToWait();
                    return;
                }else
                    ticksToWaitBetweenShouts--;
            }
            
            if(trackFrackman())
                return;
            if(getWorld()->lineOfSightToFrackMan(getX(), getY())&&!getWorld()->frackmanThere(getX(), getY(), 4))
            {
                    setDirection(getWorld()->directionToFrackMan(getX(), getY()));
                    moveInDir(getDirection());
                    numSquaresToMoveInCurrentDirection=0;
                    setTicksToWait();
                    return;
            }
            
            if(numSquaresToMoveInCurrentDirection<=0)
            {
                if(!getWorld()->lineOfSightToFrackMan(getX(), getY()))
                {
                    setDirection(randDir());
                    while (!canMoveInDir(getDirection()))
                        setDirection(randDir());
                    numSquaresToMoveInCurrentDirection=getWorld()->randInt(8, 60);
                }
            }
            
            if(ticksToMakePerpendicularTurn<=0&&isAtIntersection(getDirection()))
            {
                setDirection(pickViablePerpendicularDir(getDirection()));
                ticksToMakePerpendicularTurn=200;
                numSquaresToMoveInCurrentDirection=getWorld()->randInt(8, 60);
            }else
                ticksToMakePerpendicularTurn--;
            
        
                if (canMoveInDir(getDirection()))
                {
                    moveInDir(getDirection());
                    numSquaresToMoveInCurrentDirection--;
                }else
                    numSquaresToMoveInCurrentDirection=0;
            

          }
         
         setTicksToWait();
     }else
        ticksToWaitBetweenMoves--;
     
    }else
        ticksToBeFreezed--;
  
}
void RegularProtester::getAnnoyed(int points){
    
    hit_points=hit_points-points;
    
    if (hit_points>0) {
        getWorld()->playSound(SOUND_PROTESTER_ANNOYED);
        getFreezed();
    }else
    {
        leaveOilField=true;
        getWorld()->playSound(SOUND_PROTESTER_GIVE_UP);
        ticksToWaitBetweenMoves=-1;
    }
}

void RegularProtester::pickUpGold(){
    leaveOilField=true;
    getWorld()->playSound(SOUND_PROTESTER_FOUND_GOLD);
    getWorld()->increaseScore(25);

}
bool RegularProtester::trackFrackman(){
    return false;
}
void RegularProtester::getFreezed(){
    ticksToBeFreezed = 50>(100-getWorld()->getLevel() * 10)?50:100-getWorld()->getLevel() * 10;
}
void RegularProtester::setTicksToWait(){
    ticksToWaitBetweenMoves = 0>int(3-getWorld()->getLevel()/4)? 0:int(3-getWorld()->getLevel()/4);
}
/**************************** Hardcore Protester Implementation *****************************/
HardcoreProtester::HardcoreProtester(int IID,StudentWorld* world): RegularProtester(IID,world){

}

HardcoreProtester:: ~HardcoreProtester(){
}

void HardcoreProtester::pickUpGold(){
    getFreezed();
    getWorld()->playSound(SOUND_PROTESTER_FOUND_GOLD);
    getWorld()->increaseScore(50);
    
}
bool HardcoreProtester::trackFrackman(){
    int M = 16 + getWorld()->getLevel() * 2;
    if(!getWorld()->frackmanThere(getX(), getY(), 4)&&getWorld()->senseFrackman(getX(), getY(), M))
    {
        Coord trackpath = getWorld()->calTrackPath(getX(), getY());
        int nx=trackpath.c();
        int ny=trackpath.r();
        if(getX()==nx)
        {
            if(getY()<ny)
            {
                setDirection(up);
            }else
            {
                setDirection(down);
            }
        }
        if(getY()==ny)
        {
            if(getX()<nx)
            {
                setDirection(right);
            }else
            {
                setDirection(left);
            }
        }
        moveTo(nx, ny);
        setTicksToWait();
        return true;
    }
    return false;
}



