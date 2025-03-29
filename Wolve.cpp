#include "Wolve.h"

LTexture wolve::mName;
LTexture wolve::gStreamingGo;
LTexture wolve::gStreamingAttack;
LTexture wolve::gStreamingDie;

int numWolve;
wolve::wolve(int x, int y) : dotWolve(x, y, 2)
{
    dotWolve.setVel(100);
    dotWolve.SetMaxHP(100);
    dotWolve.SetDameSword(20);
}

bool wolve::loadMedia()
{
    bool success = true;

    if (!mName.loadFromRenderedText("wild Wolve", eHPColor))
    {
        printf("Failed to create Name texture!\n");
        success = false;
    }

    std::stringstream path;
    path << (GetHP()) << "/" << dotWolve.GetMaxHP();
    if (!HP.loadFromRenderedText(path.str(), eHPColor))
    {
        printf("Failed to create HP texture!\n");
        success = false;
    }

    if (!gStreamingGo.loadFromFile("image/enemy/2/Walk_00.png"))
    {
        printf("Failed to create W walk texture!\n");
        success = false;
    }

    if (!gStreamingDie.loadFromFile("image/enemy/2/Death_00.png"))
    {
        printf("Failed to create W death texture!\n");
        success = false;
    }

    if (!gStreamingAttack.loadFromFile("image/enemy/2/Attack_00.png"))
    {
        printf("Failed to create W attack texture!\n");
        success = false;
    }

    return success;
}

void wolve::AiHandleEvent(SDL_Event& e, Tile* tiles[], std::pair<int, int>* position)
{
    dotWolve.AiHandleEvent(e, tiles, position);
}

void wolve::render(SDL_Rect& camera)
{
    if(GetHP()<=0)dotWolve.SetDead(1);
    else if (isAttack()) { dotWolve.SetAttacking(1); }
    int width = 50;
    int X = dotWolve.GetX() + dotWolve.DOT_WIDTH / 2 - width / 2 - camera.x;
    int Y = dotWolve.GetY() + dotWolve.DOT_HEIGHT - width - camera.y + 5;
    int Frame;

    if ((!isDead()) && checkDistance(dotWolve.GetX(), dotWolve.GetY(), gCharacter.GetDot()->GetX(), gCharacter.GetDot()->GetY(), 150))
    {
        mName.render(X + width / 2 - mName.getWidth() / 2, Y - 25);

        std::stringstream path;
        path << (GetHP()) << "/" << dotWolve.GetMaxHP();
        if (!HP.loadFromRenderedText(path.str(), eHPColor))
        {
            printf("Failed to create HP texture!\n");
        }
        HP.render(X + width / 2 - HP.getWidth() / 2, Y - 15);
    }

    int i;
    if (dotWolve.isRight()) i = 2;
    else if (dotWolve.isLeft()) i = 1;
    else if (dotWolve.isUp()) i = 3;
    else i = 0;

    if (isDie()) {}
    else if (isDead())
    {
        Frame = 6;

        if (mFrame >= 5.7) { dotWolve.SetDie(1);  }

        int x = (int)mFrame;
        int y = i;
        mFrame = fmod(double(mFrame + 0.2), double(Frame));

        SDL_Rect clip = { x * width, y * width, width, width };

        gStreamingDie.render(X, Y, &clip, 0, NULL, SDL_FLIP_NONE);
    }
    else if (isAttacking())
    {
        Frame = 6;

        if (mFrame >= 5.85) { dotWolve.SetAttacking(isAttack()); }
        int x = (int)mFrame;
        int y = i;
        mFrame = fmod(double(mFrame + 0.1), double(Frame));

        SDL_Rect clip = { x * width, y * width, width, width };

        gStreamingAttack.render(X, Y, &clip, 0, NULL, SDL_FLIP_NONE);
    }
    else
    {
        Frame = 6;
        int x = mFrame;
        int y = i;
        mFrame = fmod(double(mFrame + 0.2), double(Frame));
        SDL_Rect clip = { x * width, y * width, width, width };

        gStreamingGo.render(X, Y, &clip, 0, NULL, SDL_FLIP_NONE);
    }
}

void wolve::setBlendMode(SDL_BlendMode blending)
{
    gStreamingGo.setBlendMode(blending);
    gStreamingAttack.setBlendMode(blending);
    gStreamingDie.setBlendMode(blending);
}

void wolve::free()
{
    HP.free();
}

bool wolve::attackEnemy(){
return dotWolve.attackEnemy(dotCharacter0,1,25,mFrame>=3.8 && mFrame<=3.95);
}
