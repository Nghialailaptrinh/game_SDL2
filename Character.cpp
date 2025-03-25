#include "Character.h"


Character gCharacter;
Dot* dotCharacter = gCharacter.GetDot(); // Tạo con trỏ chỉ đến đối tượng Dot
Dot* dotCharacter0[1] = {dotCharacter}; // Gán trực tiếp con trỏ vào mảng


Character::Character()
    : gDataStreamGo(1), gDataStreamStand(1), gDataStreamRun(1), gDataStreamDie(1), gDataStreamAttack(1)
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

    if (!gStreamingGo.createBlank(480, 320))
    {
        printf("Failed to create streamingGo texture!\n");
        success = false;
    }
    if (!gStreamingStand.createBlank(960, 320))
    {
        printf("Failed to create streamingStand texture!\n");
        success = false;
    }
    if (!gStreamingRun.createBlank(640, 320))
    {
        printf("Failed to create streamingRun texture!\n");
        success = false;
    }
    if (!gStreamingDie.createBlank(560, 320))
    {
        printf("Failed to create streamingDie texture!\n");
        success = false;
    }
    if (!gStreamingAttack.createBlank(640, 320))
    {
        printf("Failed to create streamingAttack texture!\n");
        success = false;
    }

    // Load data stream
    if (!gDataStreamGo.loadMedia("image/character/Walk_full_"))
    {
        printf("Unable to load data streamGo!\n");
        success = false;
    }
    else {
        gStreamingGo.lockTexture();
        gStreamingGo.copyRawPixels32(gDataStreamGo.getBuffer());
        gStreamingGo.unlockTexture();
    }

    if (!gDataStreamStand.loadMedia("image/character/Sword_Idle_full_"))
    {
        printf("Unable to load data streamStand!\n");
        success = false;
    }
    else {
        gStreamingStand.lockTexture();
        gStreamingStand.copyRawPixels32(gDataStreamStand.getBuffer());
        gStreamingStand.unlockTexture();
    }

    if (!gDataStreamRun.loadMedia("image/character/Sword_Run_full_"))
    {
        printf("Unable to load data streamRun!\n");
        success = false;
    }
    else {
        gStreamingRun.lockTexture();
        gStreamingRun.copyRawPixels32(gDataStreamRun.getBuffer());
        gStreamingRun.unlockTexture();
    }

    if (!gDataStreamDie.loadMedia("image/character/Sword_Death_full_"))
    {
        printf("Unable to load data streamDie!\n");
        success = false;
    }
    else {
        gStreamingDie.lockTexture();
        gStreamingDie.copyRawPixels32(gDataStreamDie.getBuffer());
        gStreamingDie.unlockTexture();
    }

    if (!gDataStreamAttack.loadMedia("image/character/Sword_Attack_full_"))
    {
        printf("Unable to load data streamAttack!\n");
        success = false;
    }
    else {
        gStreamingAttack.lockTexture();
        gStreamingAttack.copyRawPixels32(gDataStreamAttack.getBuffer());
        gStreamingAttack.unlockTexture();
    }

    return success;
}

void Character::handleEvent(SDL_Event& e)
{
    dotCharacter.handleEvent(e);
}

bool Character::attackEnemy(Dot* dotEnemy[], int numEnemies, int weapon)  // demo weapon 1 laf kiến; mỗi vũ khí có tầm đánh và sát thương khác nhau
{     if(weapon==1)dotCharacter.SetDameSword(50);
    int attackRange ;if(weapon==1)attackRange =50;
    if (gDataStreamAttack.FgetCurrentFrame() >= 2.9 && gDataStreamAttack.FgetCurrentFrame() <= 3)
        Mix_PlayChannel(-1, gSword, 0);
    return dotCharacter.attackEnemy(dotEnemy, numEnemies, attackRange, gDataStreamAttack.FgetCurrentFrame() >= 4.85 && gDataStreamAttack.FgetCurrentFrame() <= 5.05);
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
    if (GetHP() <= dotCharacter.GetMaxHP() * 1.0 / 8) hp = 0;
    else if (GetHP() <= dotCharacter.GetMaxHP() * 2 * 1.0 / 8) hp = 1;
    else if (GetHP() <= dotCharacter.GetMaxHP() * 3 * 1.0 / 8) hp = 2;
    else if (GetHP() <= dotCharacter.GetMaxHP() * 4 * 1.0 / 8) hp = 3;
    else if (GetHP() <= dotCharacter.GetMaxHP() * 5 * 1.0 / 8) hp = 4;
    else if (GetHP() <= dotCharacter.GetMaxHP() * 6 * 1.0 / 8) hp = 5;
    else if (GetHP() <= dotCharacter.GetMaxHP() * 7 * 1.0 / 8) hp = 6;
    else if (GetHP() <= dotCharacter.GetMaxHP() * 8 * 1.0 / 8) hp = 7;
    SDL_Rect Clip = { 0, hp * 10, 80, 10 };

    HPTexture.render(X + 40 - HPTexture.getWidth() / 2, Y + 15, &Clip);

    int i;
    if (dotCharacter.isRight()) i = 2;
    else if (dotCharacter.isLeft()) i = 1;
    else if (dotCharacter.isUp()) i = 3;
    else i = 0;

    if (isDead()) {
        Frame = 16;
        gDataStreamDie.upCurrentFrame(Frame);
        if (gDataStreamDie.FgetCurrentFrame() >= Frame - 0.2) {
            dotCharacter.SetDie(1);
            printf("died \n");
        }
        int x = (gDataStreamDie.getCurrentFrame() % Frame) / 2;
        int y = gDataStreamDie.getCurrentFrame() / Frame + i;

        SDL_Rect clip = { x * 80, y * 80, 80, 80 };

        gStreamingDie.render(
            X,
            Y + 5,
            &clip,
            0,
            NULL,
            SDL_FLIP_NONE
        );
    }
    else if (isAttacking()) {
        Frame = 8;
        gDataStreamAttack.upCurrentFrame(Frame);
        gDataStreamAttack.upCurrentFrame(Frame);
        if (gDataStreamAttack.FgetCurrentFrame() >= Frame - 0.2) {
            dotCharacter.SetAttacking(isAttack());
        }
        int x = gDataStreamAttack.getCurrentFrame() % Frame;
        int y = gDataStreamAttack.getCurrentFrame() / Frame + i;

        SDL_Rect clip = { x * 80, y * 80, 80, 80 };

        gStreamingAttack.render(
            X,
            Y + 5,
            &clip,
            0,
            NULL,
            SDL_FLIP_NONE
        );
    }
    else if (dotCharacter.isRun() && dotCharacter.isWalk()) {
        Frame = 8;
        gDataStreamRun.upCurrentFrame(Frame);
        gDataStreamRun.upCurrentFrame(Frame);

        int x = gDataStreamRun.getCurrentFrame() % Frame;
        int y = gDataStreamRun.getCurrentFrame() / Frame + i;

        SDL_Rect clip = { x * 80, y * 80, 80, 80 };

        gStreamingRun.render(
            X,
            Y + 5,
            &clip,
            0,
            NULL,
            SDL_FLIP_NONE
        );
    }
    else if (dotCharacter.isWalk()) {
        Frame = 6;
        gDataStreamGo.upCurrentFrame(Frame);

        int x = gDataStreamGo.getCurrentFrame() % Frame;
        int y = gDataStreamGo.getCurrentFrame() / Frame + i;

        SDL_Rect clip = { x * 80, y * 80, 80, 80 };

        gStreamingGo.render(
            X,
            Y + 5,
            &clip,
            0,
            NULL,
            SDL_FLIP_NONE
        );
    }
    else {
        Frame = (dotCharacter.isUp() ? 4 : 12);
        gDataStreamStand.upCurrentFrame(Frame);

        int x = gDataStreamStand.getCurrentFrame() % Frame;
        int y = gDataStreamStand.getCurrentFrame() / Frame + i;

        SDL_Rect clip = { x * 80, y * 80, 80, 80 };

        gStreamingStand.render(
            X,
            Y + 5,
            &clip,
            0,
            NULL,
            SDL_FLIP_NONE
        );
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
}

void Character::free()
{
    mName.free();
    HP.free();
    HPTexture.free();
    gStreamingGo.free();
    gStreamingStand.free();
    gStreamingRun.free();
    gStreamingDie.free();
    gStreamingAttack.free();
    gDataStreamGo.free();
    gDataStreamStand.free();
    gDataStreamRun.free();
    gDataStreamDie.free();
    gDataStreamAttack.free();
}
