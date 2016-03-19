#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "GameConstants.h"
#include <vector>
class StudentWorld;

using namespace std;


// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

class Coord
{
public:
    Coord(int cc, int rr,int ppc, int ppr) :  m_c(cc), m_r(rr),m_pc(ppc), m_pr(ppr) {}
    int r() const { return m_r; }
    int c() const { return m_c; }
    int pc() const { return m_pc; }
    int pr() const { return m_pr; }
    
private:
    int m_c;
    int m_r;
    int m_pc;
    int m_pr;
};

class Actor: public GraphObject{
public:
    Actor(int imageID, int startX, int startY, StudentWorld* world, Direction dir = right, double size = 1.0, unsigned int depth = 0);
    virtual ~Actor();
    bool isAlive();
    void setDead();
    virtual void doSomething()=0;
    Direction randDir();
    unsigned int getID() const;
    StudentWorld* getWorld() const;
    void playSound(int soundID);
    virtual void getAnnoyed(int points){}
    void moveInDir(Direction dir);
    bool canMoveInDir(Direction dir);
    bool isAtIntersection(Direction dir);
    Direction pickViablePerpendicularDir(Direction dir);
    virtual void pickUpGold(){}
    
private:
    StudentWorld* m_world;
    bool m_alive;
    unsigned int m_ID;
};

class Dirt: public Actor {
public:
    Dirt(int X, int Y, StudentWorld* world);
    virtual void doSomething(){}
    virtual ~Dirt();
};

class FrackMan: public Actor {
public:
    FrackMan(StudentWorld* world);
    virtual ~FrackMan();
    virtual void doSomething();
    int getHealth();
    int getWaterUnits();
    int getSonarCharge();
    int getGoldNuggets();
    void incGold();
    void incSonar();
    void incWater();
    virtual void getAnnoyed(int points);
    
private:
    int hit_points;
    int water_units;
    int sonar_charge;
    int gold_nuggets;
    
};

class Boulders: public Actor {
public:
     Boulders(int X, int Y, StudentWorld* world);
     virtual ~Boulders();
     virtual void doSomething();
private:
     int wait_time;
     bool is_falling;
    
};

class Squirt: public Actor {
public:
    Squirt(int X, int Y,  StudentWorld* world, Direction dir);
    virtual ~Squirt();
    virtual void doSomething();
private:
    int travel_distance;
};

class BarrelsOfOil: public Actor {
public:
    BarrelsOfOil(int X, int Y, StudentWorld* world);
    virtual ~BarrelsOfOil();
    void doSomething();
    
};

class GoldNuggets: public Actor {
public:
    GoldNuggets(int X, int Y, StudentWorld* world,  bool permanent);
    virtual ~GoldNuggets();
    void doSomething();
private:
    int life_time;
    bool is_permanent;
};

class SonarKit: public Actor {
public:
    SonarKit(StudentWorld* world);
    virtual ~SonarKit();
    void doSomething();
private:
    int life_time;
};

class WaterPool: public Actor {
public:
    WaterPool(int X, int Y, StudentWorld* world);
    virtual ~WaterPool();
    void doSomething();
private:
    int life_time;
};

class RegularProtester: public Actor {
public:
    RegularProtester(int IID, StudentWorld* world);
    virtual ~RegularProtester();
    virtual void doSomething();
    virtual void getAnnoyed(int points);
    
protected:
    void getFreezed();
    void setTicksToWait();
    
private:
    int hit_points;
    int numSquaresToMoveInCurrentDirection;
    bool leaveOilField;
    int ticksToWaitBetweenMoves;
    int ticksToWaitBetweenShouts;
    int ticksToMakePerpendicularTurn;
    int ticksToBeFreezed;
    bool calLeavePath;
    vector<Coord> leavepath;
    
    virtual void pickUpGold();
    virtual bool trackFrackman();
    
};

class HardcoreProtester: public RegularProtester {
public:
    HardcoreProtester(int IID, StudentWorld* world);
    virtual ~HardcoreProtester();

private:
    virtual void pickUpGold();
    virtual bool trackFrackman();
};


#endif // ACTOR_H_
