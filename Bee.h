#ifndef BEE_H_INCLUDED
#define BEE_H_INCLUDED

#include <SDL.h>
#include <sstream>
#include "Character.h"


class bee
{
public:
    bee(int x = 50, int y = 50);
    ~bee() { free();}

    bool loadMedia();
    void AiHandleEvent(SDL_Event& e, Tile* tiles[], std::pair<int, int>* position = nullptr);
    void render(SDL_Rect& camera);

    int GetX() { return dotBee.GetX(); }
    int GetY() { return dotBee.GetY(); }
    bool isRight() { return dotBee.isRight(); }
    bool isLeft() { return dotBee.isLeft(); }
    bool isUp() { return dotBee.isUp(); }
    bool isDown() { return dotBee.isDown(); }

    bool isWalk() { return dotBee.isWalk(); }
    bool isRun() { return dotBee.isRun(); }
    void setBlendMode(SDL_BlendMode blending);
    bool attackEnemy();
    void free();
    static void freeStaticTextures() {
        mName.free();
        gStreamingGo.free();
        gStreamingDie.free();
    }
    int GetHP() { return dotBee.GetHP(); }
    bool isDead() { return dotBee.isDead(); }
    bool isDie() { return dotBee.isDie(); }
    bool isAttack() { return dotBee.isAttack(); }
    bool isAttacking() { return dotBee.isAttacking(); }
    Dot* GetDot() { return &dotBee; }

private:
    Dot dotBee;
    float mFrame = 0;

    // Scene textures
    static LTexture mName;
    LTexture HP;
    static LTexture gStreamingGo;
    static LTexture gStreamingDie;
};


extern int numBee;



#endif // BEE_H_INCLUDED
