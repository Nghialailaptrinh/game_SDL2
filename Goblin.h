#ifndef GOBLIN_H_INCLUDED
#define GOBLIN_H_INCLUDED

#include <SDL.h>
#include <sstream>
#include "Character.h"


class goblin
{
public:
    goblin(int x = 50, int y = 50);
    ~goblin() { free();}

    bool loadMedia();
    void AiHandleEvent(SDL_Event& e, Tile* tiles[], std::pair<int, int>* position = nullptr);
    void render(SDL_Rect& camera);

    int GetX() { return dotGoblin.GetX(); }
    int GetY() { return dotGoblin.GetY(); }
    bool isRight() { return dotGoblin.isRight(); }
    bool isLeft() { return dotGoblin.isLeft(); }
    bool isUp() { return dotGoblin.isUp(); }
    bool isDown() { return dotGoblin.isDown(); }

    bool isWalk() { return dotGoblin.isWalk(); }
    bool isRun() { return dotGoblin.isRun(); }
    void setBlendMode(SDL_BlendMode blending);
    bool attackEnemy();
    void free();
    static void freeStaticTextures() {
        mName.free();
        gStreamingGo.free();
        gStreamingAttack.free();
        gStreamingDie.free();
    }
    int GetHP() { return dotGoblin.GetHP(); }
    bool isDead() { return dotGoblin.isDead(); }
    bool isDie() { return dotGoblin.isDie(); }
    bool isAttack() { return dotGoblin.isAttack(); }
    bool isAttacking() { return dotGoblin.isAttacking(); }
    Dot* GetDot() { return &dotGoblin; }

private:
    Dot dotGoblin;
    float mFrame = 0;

    // Scene textures
    static LTexture mName;
    LTexture HP;
    static LTexture gStreamingGo;
    static LTexture gStreamingAttack;
    static LTexture gStreamingDie;
};


extern int numGoblin;



#endif // GOBLIN_H_INCLUDED
