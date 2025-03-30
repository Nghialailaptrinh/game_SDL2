#include "Bee.h"

LTexture bee::mName;
LTexture bee::gStreamingGo;
LTexture bee::gStreamingDie;

int numBee;
bee::bee(int x, int y) : dotBee(x, y, 4)
{
    dotBee.setVel(100);
    dotBee.SetMaxHP(80);
    dotBee.SetHP(80);
    dotBee.SetDameSword(10);
}

bool bee::loadMedia()
{
    bool success = true;

    if (!mName.loadFromRenderedText("bee", eHPColor))
    {
        printf("Failed to create Name texture!\n");
        success = false;
    }

    std::stringstream path;
    path << (GetHP()) << "/" << dotBee.GetMaxHP();
    if (!HP.loadFromRenderedText(path.str(), eHPColor))
    {
        printf("Failed to create HP texture!\n");
        success = false;
    }

    if (!gStreamingGo.loadFromFile("image/enemy/4/Walk_00.png"))
    {
        printf("Failed to create B walk texture!\n");
        success = false;
    }

    if (!gStreamingDie.loadFromFile("image/enemy/4/Death_00.png"))
    {
        printf("Failed to create B death texture!\n");
        success = false;
    }

    return success;
}

void bee::AiHandleEvent(SDL_Event& e, Tile* tiles[], std::pair<int, int>* position)
{
    dotBee.AiHandleEvent(e, tiles, position);
}

void bee::render(SDL_Rect& camera)
{
    if(GetHP()<=0)dotBee.SetDead(1);
    else if (isAttack()) { dotBee.SetAttacking(1); }
    int width = 50;
    int X = dotBee.GetX() + dotBee.DOT_WIDTH / 2 - width / 2 - camera.x;
    int Y = dotBee.GetY() + dotBee.DOT_HEIGHT - width - camera.y + 5;
    int Frame;

    if ((!isDead()) && checkDistance(dotBee.GetX(), dotBee.GetY(), gCharacter.GetDot()->GetX(), gCharacter.GetDot()->GetY(), 150))
    {
        mName.render(X + width / 2 - mName.getWidth() / 2, Y - 25);

        std::stringstream path;
        path << (GetHP()) << "/" << dotBee.GetMaxHP();
        if (!HP.loadFromRenderedText(path.str(), eHPColor))
        {
            printf("Failed to create HP texture!\n");
        }
        HP.render(X + width / 2 - HP.getWidth() / 2, Y - 15);
    }

    int i;
    if (dotBee.isRight()) i = 2;
    else if (dotBee.isLeft()) i = 1;
    else if (dotBee.isUp()) i = 3;
    else i = 0;

    if (isDie()) {}
    else if (isDead())
    {
        Frame = 6;

        if (mFrame >= 5.7) { dotBee.SetDie(1);  }

        int x = (int)mFrame;
        int y = i;
        mFrame = fmod(double(mFrame + 0.2), double(Frame));

        SDL_Rect clip = { x * width, y * width, width, width };

        gStreamingDie.render(X, Y, &clip, 0, NULL, SDL_FLIP_NONE);
    }
    else if (isAttacking())
    {
        Frame = 6;

        if (mFrame >= 5.85) { dotBee.SetAttacking(isAttack()); }
        int x = (int)mFrame;
        int y = i;
        mFrame = fmod(double(mFrame + 0.1), double(Frame));

        SDL_Rect clip = { x * width, y * width, width, width };

        gStreamingGo.render(X, Y, &clip, 0, NULL, SDL_FLIP_NONE);
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

void bee::setBlendMode(SDL_BlendMode blending)
{
    gStreamingGo.setBlendMode(blending);
    gStreamingDie.setBlendMode(blending);
}

void bee::free()
{
    HP.free();
}

bool bee::attackEnemy(){
return dotBee.attackEnemy(dotCharacter0,1,40,mFrame>=3.8 && mFrame<=3.95);
}
