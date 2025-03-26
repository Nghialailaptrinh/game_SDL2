#ifndef WOLVE_H_INCLUDED
#define WOLVE_H_INCLUDED

#include <SDL.h>
#include <sstream>
#include "Character.h"


class wolve
{
public:
    wolve(int x = 50, int y = 50);
    ~wolve() { free();}

    bool loadMedia();
    void AiHandleEvent(SDL_Event& e, Tile* tiles[], std::pair<int, int>* position = nullptr);
    void render(SDL_Rect& camera);

    int GetX() { return dotWolve.GetX(); }
    int GetY() { return dotWolve.GetY(); }
    bool isRight() { return dotWolve.isRight(); }
    bool isLeft() { return dotWolve.isLeft(); }
    bool isUp() { return dotWolve.isUp(); }
    bool isDown() { return dotWolve.isDown(); }

    bool isWalk() { return dotWolve.isWalk(); }
    bool isRun() { return dotWolve.isRun(); }
    void setBlendMode(SDL_BlendMode blending);
    bool attackEnemy();
    void free();
    static void freeStaticTextures() {
        mName.free();
        gStreamingGo.free();
        gStreamingAttack.free();
        gStreamingDie.free();
    }
    int GetHP() { return dotWolve.GetHP(); }
    bool isDead() { return dotWolve.isDead(); }
    bool isDie() { return dotWolve.isDie(); }
    bool isAttack() { return dotWolve.isAttack(); }
    bool isAttacking() { return dotWolve.isAttacking(); }
    Dot* GetDot() { return &dotWolve; }

private:
    Dot dotWolve;
    float mFrame = 0;

    // Scene textures
    static LTexture mName;
    LTexture HP;
    static LTexture gStreamingGo;
    static LTexture gStreamingAttack;
    static LTexture gStreamingDie;
};


extern int numWolve;
#endif // WOLVE_H_INCLUDED
