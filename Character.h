#ifndef CHARACTER_H_INCLUDED
#define CHARACTER_H_INCLUDED

#include "Dot.h"
#include "DataStream.h"
#include <SDL.h>
#include <sstream>

class Character
{
public:
    Character();
    ~Character() { free(); }

    bool loadMedia();
    void handleEvent(SDL_Event& e);
    bool attackEnemy(Dot* dotEnemy[] = nullptr, int numEnemies = 0, int weapon = 1);
    void move(Tile* tiles[], float timeStep);
    void setCamera(SDL_Rect& camera);
    void render(SDL_Rect& camera);

    int GetX() { return dotCharacter.GetX(); }
    int GetY() { return dotCharacter.GetY(); }
    bool isRight() { return dotCharacter.isRight(); }
    bool isLeft() { return dotCharacter.isLeft(); }
    bool isUp() { return dotCharacter.isUp(); }
    bool isDown() { return dotCharacter.isDown(); }

    bool isWalk() { return dotCharacter.isWalk(); }
    bool isRun() { return dotCharacter.isRun(); }
    void setBlendMode(SDL_BlendMode blending);
    void free();
    int GetHP() { return dotCharacter.GetHP(); }
    bool isDead() { return dotCharacter.isDead(); }
    bool isDie() { return dotCharacter.isDie(); }
    bool isAttack() { return dotCharacter.isAttack(); }
    bool isAttacking() { return dotCharacter.isAttacking(); }
    Dot* GetDot() { return &dotCharacter; }

private:
    Dot dotCharacter;

    // Scene textures
    LTexture mName;
    LTexture HP;
    LTexture HPTexture;
    LTexture gStreamingGo;
    LTexture gStreamingStand;
    LTexture gStreamingRun;
    LTexture gStreamingDie;
    LTexture gStreamingAttack;

    // Animation stream
    DataStream gDataStreamGo;
    DataStream gDataStreamStand;
    DataStream gDataStreamRun;
    DataStream gDataStreamDie;
    DataStream gDataStreamAttack;
};

extern Character gCharacter;

#endif // CHARACTER_H_INCLUDED
