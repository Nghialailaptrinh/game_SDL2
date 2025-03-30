#include "Character.h"


Character gCharacter;
Dot* dotCharacter = gCharacter.GetDot(); // Tạo con trỏ chỉ đến đối tượng Dot
Dot* dotCharacter0[1] = {dotCharacter}; // Gán trực tiếp con trỏ vào mảng


Character::Character(int x,int y)
    : dotCharacter(x,y)
{
    dotCharacter.SetMaxHP(200);
    dotCharacter.SetHP(dotCharacter.GetMaxHP());
    dotCharacter.SetDameSword(20);
    dotCharacter.setVel(150);
}

bool Character::loadMedia()
{
    bool success = true;
    if (!mName.loadFromRenderedText("Nghia sad thu", nameColor))
    {
        printf("Failed to create Name texture!\n");
        success = false;
    }

    std::stringstream path;
    path << (GetHP()) << "/" << dotCharacter.GetMaxHP();
    if (!HP.loadFromRenderedText(path.str(), HPColor))
    {
        printf("Failed to create HP texture!\n");
        success = false;
    }

    if (!HPTexture.loadFromFile("image/character/HP.png"))
    {
        printf("Failed to create HPTexture texture!\n");
        success = false;
    }

    std::stringstream path2;
    path2 << (dotCharacter.GetMana()) << "/" << dotCharacter.GetMaxMana();
    if (!Mana.loadFromRenderedText(path2.str(), HPColor))
    {
        printf("Failed to create Mana texture!\n");
        success = false;
    }

    if (!ManaTexture.loadFromFile("image/character/mana.png"))
    {
        printf("Failed to create ManaTexture texture!\n");
        success = false;
    }

    if (!gHurt.loadFromFile("image/character/Sword_Hurt_full.png"))
    {
        printf("Failed to create Hurt texture!\n");
        success = false;
    }

    if (!gStreamingGo.loadFromFile("image/character/Walk_full_00.png"))
    {
        printf("Failed to create streamingGo texture!\n");
        success = false;
    }
    if (!gStreamingStand.loadFromFile("image/character/Sword_Idle_full_00.png"))
    {
        printf("Failed to create streamingStand texture!\n");
        success = false;
    }
    if (!gStreamingRun.loadFromFile("image/character/Sword_Run_full_00.png"))
    {
        printf("Failed to create streamingRun texture!\n");
        success = false;
    }
    if (!gStreamingDie.loadFromFile("image/character/Sword_Death_full_00.png"))
    {
        printf("Failed to create streamingDie texture!\n");
        success = false;
    }
    if (!gStreamingAttack.loadFromFile("image/character/Sword_Attack_full_00.png"))
    {
        printf("Failed to create streamingAttack texture!\n");
        success = false;
    }


    return success;
}

void Character::handleEvent(SDL_Event& e)
{
    dotCharacter.handleEvent(e);
}

bool Character::attackEnemy(Dot* dotEnemy[], int numEnemies, int weapon)  // demo weapon 1 laf kiến; mỗi vũ khí có tầm đánh và sát thương khác nhau
{     if(weapon==1){dotCharacter.SetDameSword(dotCharacter.GetDameSword());}   //  kiếm là loại có sát thương cơ bản nhất
    int attackRange ;if(weapon==1)attackRange =50;
    if (mFrame >= 2.85 && mFrame <= 3.05&& isAttacking())
    Mix_PlayChannel(-1, gSword, 0);
    return dotCharacter.attackEnemy(dotEnemy, numEnemies, attackRange, mFrame >= 4.85 && mFrame <= 5.05 && isAttacking());
}

void Character::move(Tile* tiles[], float timeStep)
{
     dotCharacter.move(tiles, timeStep);
    std::stringstream path;
    path << (GetHP()) << "/" << dotCharacter.GetMaxHP();
    HP.free();
    if (!HP.loadFromRenderedText(path.str(), HPColor))
    {
        printf("Failed to create HP texture!\n");
    }

    std::stringstream path2;
    path2 << (dotCharacter.GetMana()) << "/" << dotCharacter.GetMaxMana();
    if (!Mana.loadFromRenderedText(path2.str(), HPColor))
    {
        printf("Failed to create Mana texture!\n");
    }



    static bool isRunMusicPlaying = false;
    if (dotCharacter.isRun() && dotCharacter.isWalk()) {
        if (Mix_PlayingMusic() == 0 || (Mix_PlayingMusic() && !isRunMusicPlaying)) {
            Mix_PlayMusic(gRun, -1);
            isRunMusicPlaying = true;

        }
    }
    else if (dotCharacter.isWalk()) {
        if (Mix_PlayingMusic() == 0 || (Mix_PlayingMusic() && isRunMusicPlaying)) {
            Mix_PlayMusic(gGo, -1);
            isRunMusicPlaying = false;

        }
    }
    else {
        if (Mix_PlayingMusic() != 0) {
            Mix_HaltMusic();
            isRunMusicPlaying = false;

        }
    }
}

void Character::setCamera(SDL_Rect& camera)
{
    dotCharacter.setCamera(camera);
}

void Character::render(SDL_Rect& camera)
{
    if (isAttack()) {
        dotCharacter.SetAttacking(1);
    }

    int X = dotCharacter.GetX() + dotCharacter.DOT_WIDTH / 2 - 40 - camera.x;
    int Y = dotCharacter.GetY() + dotCharacter.DOT_HEIGHT - 80 + 10 - camera.y;
    int Frame;

    mName.render(X + 40 - mName.getWidth() / 2, Y);
    HP.render(X + 40 - HP.getWidth() / 2 - HPTexture.getWidth() + 10, Y + 15);

    int hp;
    if(GetHP()==0)hp=0;
    else if (GetHP() <= dotCharacter.GetMaxHP() * 1.0 / 8) hp = 1;
    else if (GetHP() <= dotCharacter.GetMaxHP() * 2 * 1.0 / 8) hp = 2;
    else if (GetHP() <= dotCharacter.GetMaxHP() * 3 * 1.0 / 8) hp = 3;
    else if (GetHP() <= dotCharacter.GetMaxHP() * 4 * 1.0 / 8) hp = 4;
    else if (GetHP() <= dotCharacter.GetMaxHP() * 5 * 1.0 / 8) hp = 5;
    else if (GetHP() <= dotCharacter.GetMaxHP() * 6 * 1.0 / 8) hp = 6;
    else if (GetHP() <= dotCharacter.GetMaxHP() * 7 * 1.0 / 8) hp = 7;
    else if (GetHP() <= dotCharacter.GetMaxHP() * 8 * 1.0 / 8) hp = 8;
    SDL_Rect Clip = { 0, hp * 10, 80, 10 };

    HPTexture.render(X + 40 - HPTexture.getWidth() / 2, Y + 15, &Clip);
    //////////////////////////////////////////////////////////
    Mana.render(35,12);

    int mana=dotCharacter.GetMana();
    int maxMana=dotCharacter.GetMaxMana();
    int mn;
    if(mana==0)mn=0;
    else if (mana <= maxMana * 1.0 / 10) mn = 1;
    else if (mana <= maxMana * 2 * 1.0 / 10) mn = 2;
    else if (mana <= maxMana * 3 * 1.0 / 10) mn = 3;
    else if (mana <= maxMana * 4 * 1.0 / 10) mn = 4;
    else if (mana <= maxMana * 5 * 1.0 / 10) mn = 5;
    else if (mana <= maxMana * 6 * 1.0 / 10) mn = 6;
    else if (mana <= maxMana * 7 * 1.0 / 10) mn = 7;
    else if (mana <= maxMana * 8 * 1.0 / 10) mn = 8;
    else if (mana <= maxMana * 9 * 1.0 / 10) mn = 9;
    else if (mana <= maxMana * 10 * 1.0 / 10) mn = 10;

     Clip = { 0, mn * 40, 400, 40 };

    ManaTexture.render(100, 0, &Clip);

/////////////////////////////////////////////////////////////////////
    int i;
    if (dotCharacter.isRight()) i = 2;
    else if (dotCharacter.isLeft()) i = 1;
    else if (dotCharacter.isUp()) i = 3;
    else i = 0;

//////////////////////////////////////////////////////////////////////
    if (isDead()) {
        Frame = 16;


        if (mFrame >= 15.85) { dotCharacter.SetDie(1); }
        mFrame = fmod(double(mFrame + 0.1), double(Frame));
        SDL_Rect clip = { (int)mFrame * 80, i * 80, 80, 80 };

        gStreamingDie.render(X, Y + 5, &clip, 0, NULL, SDL_FLIP_NONE);



    }
    else if (isHurt()){
         dotCharacter.SetAttacking(0);        // thứ này vô cùng đáng sợ; sự tấn công của đối thr có thể hủy tấn công của bạn
         Frame = 6;

        if (mFrame >= 5.85) { dotCharacter.SetHurt(0);dotCharacter.SetTimeHurt(0); mFrame=5.9;}
        mFrame = fmod(double(mFrame + 0.1), double(Frame));
        SDL_Rect clip = { (int)mFrame * 80, i * 80, 80, 80 };

        gHurt.render(X, Y + 5, &clip, 0, NULL, SDL_FLIP_NONE);
    }
    else if (isAttacking()) {
        Frame = 8;

        if (mFrame >= 7.75) { dotCharacter.SetAttacking(isAttack()); mFrame=7.8;}
        mFrame = fmod(double(mFrame + 0.2), double(Frame));
        SDL_Rect clip = { (int)mFrame * 80, i * 80, 80, 80 };

        gStreamingAttack.render(X, Y + 5, &clip, 0, NULL, SDL_FLIP_NONE);

    }
    else if (dotCharacter.isRun() && dotCharacter.isWalk()) {
        Frame = 8;
        mFrame = fmod(double(mFrame + 0.1), double(Frame));
        SDL_Rect clip = { (int)mFrame * 80, i * 80, 80, 80 };

        gStreamingRun.render(X, Y + 5, &clip, 0, NULL, SDL_FLIP_NONE);
        if(mFrame>=7.85){mFrame=7.9;}

    }
    else if (dotCharacter.isWalk()) {
        Frame = 6;

        mFrame = fmod(double(mFrame + 0.1), double(Frame));
        SDL_Rect clip = { (int)mFrame * 80, i * 80, 80, 80 };

        gStreamingGo.render(X, Y + 5, &clip, 0, NULL, SDL_FLIP_NONE);
        if(mFrame>=5.85){mFrame=5.9;}

    }
    else {
        Frame = (dotCharacter.isUp() ? 4 : 12);
        if(Frame==4)mFrame = fmod(double(mFrame + 0.025), double(Frame));
        else mFrame = fmod(double(mFrame + 0.05), double(Frame));
        SDL_Rect clip = { (int)mFrame * 80, i * 80, 80, 80 };

        gStreamingStand.render(X, Y + 5, &clip, 0, NULL, SDL_FLIP_NONE);
        if(mFrame>=Frame-0.06){mFrame=0;}

    }
}

void Character::setBlendMode(SDL_BlendMode blending)
{
    HPTexture.setBlendMode(blending);
    gStreamingGo.setBlendMode(blending);
    gStreamingStand.setBlendMode(blending);
    gStreamingRun.setBlendMode(blending);
    gStreamingDie.setBlendMode(blending);
    gStreamingAttack.setBlendMode(blending);
    gHurt.setBlendMode(blending);
}

void Character::free()
{
    mName.free();
    HP.free();
    HPTexture.free();
    Mana.free();
    ManaTexture.free();
    gStreamingGo.free();
    gStreamingStand.free();
    gStreamingRun.free();
    gStreamingDie.free();
    gStreamingAttack.free();
    gHurt.free();

}
