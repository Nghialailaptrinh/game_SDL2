#include "Goblin.h"

LTexture goblin::mName;
LTexture goblin::gStreamingGo;
LTexture goblin::gStreamingAttack;
LTexture goblin::gStreamingDie;

int numGoblin;
goblin::goblin(int x, int y) : dotGoblin(x, y, 3)
{
    dotGoblin.setVel(100);
    dotGoblin.SetMaxHP(300);
    dotGoblin.SetHP(300);
    dotGoblin.SetDameSword(20);
}

bool goblin::loadMedia()
{
    bool success = true;

    if (!mName.loadFromRenderedText("big Goblin", eHPColor))
    {
        printf("Failed to create Name texture!\n");
        success = false;
    }

    std::stringstream path;
    path << (GetHP()) << "/" << dotGoblin.GetMaxHP();
    if (!HP.loadFromRenderedText(path.str(), eHPColor))
    {
        printf("Failed to create HP texture!\n");
        success = false;
    }

    if (!gStreamingGo.loadFromFile("image/enemy/3/Walk_00.png"))
    {
        printf("Failed to create W walk texture!\n");
        success = false;
    }

    if (!gStreamingDie.loadFromFile("image/enemy/3/Death_00.png"))
    {
        printf("Failed to create W death texture!\n");
        success = false;
    }

    if (!gStreamingAttack.loadFromFile("image/enemy/3/Attack_00.png"))
    {
        printf("Failed to create W attack texture!\n");
        success = false;
    }

    return success;
}

void goblin::AiHandleEvent(SDL_Event& e, Tile* tiles[], std::pair<int, int>* position)
{
    dotGoblin.AiHandleEvent(e, tiles, position);

}

void goblin::render(SDL_Rect& camera)
{
    if(GetHP()<=0)dotGoblin.SetDead(1);
    else if (isAttack()) { dotGoblin.SetAttacking(1); }
    int width = 70;
    int X = dotGoblin.GetX() + dotGoblin.DOT_WIDTH / 2 - width / 2 - camera.x;
    int Y = dotGoblin.GetY() + dotGoblin.DOT_HEIGHT - width - camera.y + 5;
    int Frame;

    if ((!isDead()) && checkDistance(dotGoblin.GetX(), dotGoblin.GetY(), gCharacter.GetDot()->GetX(), gCharacter.GetDot()->GetY(), 150))
    {
        mName.render(X + width / 2 - mName.getWidth() / 2, Y - 25);

        std::stringstream path;
        path << (GetHP()) << "/" << dotGoblin.GetMaxHP();
        if (!HP.loadFromRenderedText(path.str(), eHPColor))
        {
            printf("Failed to create HP texture!\n");
        }
        HP.render(X + width / 2 - HP.getWidth() / 2, Y - 15);
    }

    int i;
    if (dotGoblin.isRight()) i = 2;
    else if (dotGoblin.isLeft()) i = 1;
    else if (dotGoblin.isUp()) i = 3;
    else i = 0;

    if (isDie()) {}
    else if (isDead())
    {
        Frame = 6;

        if (mFrame >= 5.7) { dotGoblin.SetDie(1);  }

        int x = (int)mFrame;
        int y = i;
        mFrame = fmod(double(mFrame + 0.2), double(Frame));

        SDL_Rect clip = { x * width, y * width, width, width };

        gStreamingDie.render(X, Y, &clip, 0, NULL, SDL_FLIP_NONE);
    }
    else if (isAttacking())
    {
        Frame = 6;

        if (mFrame >= 5.85) { dotGoblin.SetAttacking(isAttack()); }
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

void goblin::setBlendMode(SDL_BlendMode blending)
{
    gStreamingGo.setBlendMode(blending);
    gStreamingAttack.setBlendMode(blending);
    gStreamingDie.setBlendMode(blending);
}

void goblin::free()
{
    HP.free();
}

bool goblin::attackEnemy(){
return dotGoblin.attackEnemy(dotCharacter0,1,30,mFrame>=3.8 && mFrame<=3.95);
}
