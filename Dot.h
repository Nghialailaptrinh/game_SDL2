#ifndef DOT_H_INCLUDED
#define DOT_H_INCLUDED

#include <SDL.h>
#include <utility>
#include <cstdlib>
#include "Tile.h"


class Dot
{
public:
    static const int DOT_WIDTH = 20;
    static const int DOT_HEIGHT = 20;

    int DOT_VEL = 100;

    Dot(int x = 50, int y = 50, int typeE = 0);
    ~Dot(){};

    void handleEvent(SDL_Event& e);
    void AiHandleEvent(SDL_Event& e, Tile* tiles[], std::pair<int, int>* position = nullptr);
    void move(Tile* tiles[], float timeStep);

    bool attackEnemy(Dot* dotEnemy[] = nullptr, int numEnemies = 0, int attackRange = 0, bool inThisFrame = 0);

    void setVel(int Vel) { DOT_VEL = Vel; }
    void setCamera(SDL_Rect& camera);

    void SetDie(bool Die) { die = Die; }
    void SetDead(bool Dead) { dead = Dead; }
    void SetAttack(bool Attack) { attack = Attack; }
    void SetAttacking(bool Attacking) { attacking = Attacking; }

    void updatePosition(std::pair<int, int> position[20]);

    int GetHP() { return mHP; }
    int GetX() { return mBox.x; }
    int GetY() { return mBox.y; }
    int GetDameSword() { return dameSword; }
    int GetMaxHP() { return maxHP; }
    int GetMana(){return mana;}
    int GetMaxMana(){return maxMana;}
    void SetX(int X){mBox.x=X;}
    void SetY (int Y) {mBox.y=Y;}              // demo kĩ năng dịch chuyển
    void SetHP(int HP) { mHP = HP; }
    void SetMaxHP(int MaxHP) { maxHP = MaxHP; }
    void SetMana(int Mana){mana=Mana;}
    void SetMaxMana(int MaxMana){maxMana=MaxMana;}
    void SetDameSword(int dame) { dameSword = dame; }
    void SetHurt(bool Hurt) { hurt = Hurt; }
    void SetTimeHurt(int TimeHurt) { timeHurt = TimeHurt; }
    void SetTimePois(int TimePois){timePois=TimePois;}

    bool isRight() { return goRight; }
    bool isLeft() { return goLeft; }
    bool isUp() { return goUp; }
    bool isDown() { return goDown; }
    bool isHurt() { return hurt;}
    bool isWalk() { return walk; }
    bool isRun() { return run; }
    bool isDead() { return dead; }
    bool isDie() { return die; }
    bool isAttack() { return attack; }
    bool isAttacking() { return attacking; }
    bool isPois(){return (timePois >0);}
    int typeEnemy;

private:
    SDL_Rect mBox;
    int mVelX, mVelY;
    int lastDirection;

    bool die, dead, run, walk;
    bool goRight, goLeft, goUp, goDown;
    bool touch, attack, attacking;
    bool touchX,touchY;
    bool hurt;
    int timeHurt;
    int timePois=0;
    int mHP, maxHP, dameSword;
    int mana=0,maxMana=0;

};

extern Dot** dotSlime;
extern Dot** dotWolve;
extern Dot** dotGoblin;
extern Dot** dotBee;
#endif // DOT_H_INCLUDED
