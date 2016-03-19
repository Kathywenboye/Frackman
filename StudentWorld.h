#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include <string>
#include <vector>

using namespace std;


// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
    StudentWorld(string assetDir);

    virtual int init();
    virtual int move();
    virtual void cleanUp();
    
    virtual ~StudentWorld();
    int randInt(int lowest, int highest);
    void addDirt();
    void addBoulders();
    void addOil();
    void addGoldNuggets();
    void addTempGold(int x, int y);
    void addSquirt(int x, int y, Actor::Direction dir);
    void addSonarAndWater();
    void addProtester();
    bool dirtThere(int x, int y);
    void deleteDirt(int x, int y);
    bool dirtBelow(int x, int y);
    bool boulderCollision(int x, int y);
    void boulderAnnoyActor(int x, int y);
    void attackProtester(int x,int y);
    bool actorThere(int x,int y, int IID);
    bool frackmanThere(int x,int y, int distance);
    bool senseFrackman(int x,int y,int distance);
    void decreaseOil();
    void increaseGold();
    void increaseSonar();
    void increaseWater();
    void useSonar(int x,int y);
    Actor::Direction getFrackmanDirection();
    void annoyFrackman(int points);
    bool lineOfSightToFrackMan(int x, int y);
    Actor::Direction directionToFrackMan(int x, int y);
    bool pickUpGold(int x, int y);
    vector<Coord> calLeavePath(int x, int y);
    Coord calTrackPath(int x, int y);
private:
    Dirt* m_dirt[VIEW_WIDTH][VIEW_HEIGHT];
    FrackMan* m_frackman;
    vector<Actor*> m_actor;
    int oil_left;
    int ticksToAddProtester;
    

    void removeDeadActors(); 
    void setDisplayText();
    bool withinRadius(int x, int y);
    
};

#endif // STUDENTWORLD_H_
