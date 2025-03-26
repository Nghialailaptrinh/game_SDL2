#ifndef SLIME_H_INCLUDED
#define SLIME_H_INCLUDED

#include <SDL.h>
#include <sstream>
#include "Character.h"

class slime
{
public:
    slime(int x = 50, int y = 50);
    ~slime() { free(); }

    bool loadMedia();
    void AiHandleEvent(SDL_Event& e, Tile* tiles[], std::pair<int, int>* position = nullptr);
    void render(SDL_Rect& camera);

    int GetX() { return dotSlime.GetX(); }
    int GetY() { return dotSlime.GetY(); }
    bool isRight() { return dotSlime.isRight(); }
    bool isLeft() { return dotSlime.isLeft(); }
    bool isUp() { return dotSlime.isUp(); }
    bool isDown() { return dotSlime.isDown(); }

    bool isWalk() { return dotSlime.isWalk(); }
    bool isRun() { return dotSlime.isRun(); }
    void setBlendMode(SDL_BlendMode blending);
    void free();

    static void freeStaticTextures() {
        if (!isStaticTexturesFreed) { // Kiểm tra xem đã giải phóng chưa
            mName.free();
            gStreamingGo.free();
            gStreamingAttack.free();
            gStreamingDie.free();
            isStaticTexturesFreed = true; // Đánh dấu đã giải phóng
        }
    }

    int GetHP() { return dotSlime.GetHP(); }
    bool isDead() { return dotSlime.isDead(); }
    bool isDie() { return dotSlime.isDie(); }
    Dot* GetDot() { return &dotSlime; }

private:
    Dot dotSlime;
    float mFrame = 0;

    // Scene textures
    static LTexture mName;
    LTexture HP;
    static LTexture gStreamingGo;
    static LTexture gStreamingAttack;
    static LTexture gStreamingDie;

    static bool isStaticTexturesFreed;  // Cờ kiểm tra trạng thái đã giải phóng
};

extern int numSlime;

#endif // SLIME_H_INCLUDED
