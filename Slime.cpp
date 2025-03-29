#include "Slime.h"


LTexture slime::mName;
LTexture slime::gStreamingGo;
LTexture slime::gStreamingAttack;
LTexture slime::gStreamingDie;
int numSlime;
bool slime::isStaticTexturesFreed = false;
slime::slime(int x, int y) : dotSlime(x, y, 1)
{
    dotSlime.setVel(100);
}

bool slime::loadMedia()
{
    bool success = true;

    if (!mName.loadFromRenderedText("Slime mini", eHPColor))
    {
        printf("Failed to create Name texture!\n");
        success = false;
    }

    std::stringstream path;
    path << (GetHP()) << "/100";
    if (!HP.loadFromRenderedText(path.str(), eHPColor))
    {
        printf("Failed to create HP texture!\n");
        success = false;
    }

    if (!gStreamingGo.loadFromFile("image/enemy/1/Walk_00.png"))
    {
        printf("Failed to create gStreamingGo texture!\n");
        success = false;
    }

    if (!gStreamingAttack.loadFromFile("image/enemy/1/Walk_00.png"))
    {
        printf("Failed to create gStreamingAttack texture!\n");
        success = false;
    }

    if (!gStreamingDie.loadFromFile("image/enemy/1/Death_00.png"))
    {
        printf("Failed to create gStreamingDie texture!\n");
        success = false;
    }

    return success;
}

void slime::AiHandleEvent(SDL_Event& e, Tile* tiles[], std::pair<int, int>* position)
{
    dotSlime.AiHandleEvent(e, tiles, position);
}

void slime::render(SDL_Rect& camera)
{
    int width = 50;
    int X = dotSlime.GetX() + dotSlime.DOT_WIDTH / 2 - width / 2 - camera.x;
    int Y = dotSlime.GetY() + dotSlime.DOT_HEIGHT - width - camera.y + 5;
    int Frame;

    if ((!isDead()) && checkDistance(dotSlime.GetX(), dotSlime.GetY(), gCharacter.GetDot()->GetX(), gCharacter.GetDot()->GetY(), 150))
    {
        mName.render(X + width / 2 - mName.getWidth() / 2, Y - 25);

        std::stringstream path;
        path << (GetHP()) << "/" << dotSlime.GetMaxHP();
        if (!HP.loadFromRenderedText(path.str(), eHPColor))
        {
            printf("Failed to create HP texture!\n");
        }
        HP.render(X + width / 2 - HP.getWidth() / 2, Y - 15);
    }

    if (isDie()) {}
    else if (isDead())
    {
        Frame = 6;

        if (mFrame >= 5.8)
        {
            dotSlime.SetDie(1);
        }
        int x = (int)mFrame;
        int y = 0;
        mFrame = fmod(double(mFrame + 0.2), double(Frame));

        SDL_Rect clip = {x * width, y * width, width, width};

        gStreamingDie.render(X, Y, &clip, 0, NULL, SDL_FLIP_NONE);
    }
    else
    {
        Frame = 6;
        int x = mFrame;
        int y = 0;
        mFrame = (mFrame + 0.1);
        if (mFrame >= Frame) mFrame = mFrame - Frame;
        SDL_Rect clip = {x * width, y * width, width, width};

        gStreamingGo.render(X, Y, &clip, 0, NULL, SDL_FLIP_NONE);
    }
}

void slime::setBlendMode(SDL_BlendMode blending)
{
    gStreamingGo.setBlendMode(blending);
    gStreamingAttack.setBlendMode(blending);
    gStreamingDie.setBlendMode(blending);
}

void slime::free()
{
    HP.free();
}
