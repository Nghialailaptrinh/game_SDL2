#ifndef NEED_H
#define NEED_H

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <string>
#include <sstream>
#include <iomanip>
#include <cmath>

//Screen dimension constants
 const int SCREEN_WIDTH = 800;
 const int SCREEN_HEIGHT = 500;

 //The dimensions of the level
 const int LEVEL_WIDTH = 1280;
 const int LEVEL_HEIGHT = 960;

 //Tile constants
 const int TILE_WIDTH = 80;
 const int TILE_HEIGHT = 80;
 const int TOTAL_TILES = 192;
 const int TOTAL_TILE_SPRITES = 12;

 //The different tile sprites
 const int TILE_RED = 0;
 const int TILE_GREEN = 1;
 const int TILE_BLUE = 2;
 const int TILE_CENTER = 3;
 const int TILE_TOP = 4;
 const int TILE_TOPRIGHT = 5;
 const int TILE_RIGHT = 6;
 const int TILE_BOTTOMRIGHT = 7;
 const int TILE_BOTTOM = 8;
 const int TILE_BOTTOMLEFT = 9;
 const int TILE_LEFT = 10;
 const int TILE_TOPLEFT = 11;
////////// hàm hỗ trợ mạnh


class LTexture
{
public:
    LTexture();
    ~LTexture();

    bool loadFromFile(std::string path);
    bool loadPixelsFromFile(std::string path);
    bool loadFromPixels();
    #if defined(SDL_TTF_MAJOR_VERSION)
 	//Creates image from font string
 	bool loadFromRenderedText( std::string textureText, SDL_Color textColor );
    #endif
    bool createBlank(int width, int height);
    void free();
    void setColor(Uint8 red, Uint8 green, Uint8 blue);
    void setBlendMode(SDL_BlendMode blending);
    void setAlpha(Uint8 alpha);
    void render(int x, int y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);
    int getWidth();
    int getHeight();
    Uint32* getPixels32();
    Uint32 getPixel32(Uint32 x, Uint32 y);
    Uint32 getPitch32();
    Uint32 mapRGBA(Uint8 r, Uint8 g, Uint8 b, Uint8 a);
    bool lockTexture();
    bool unlockTexture();

private:
    SDL_Texture* mTexture;
    SDL_Surface* mSurfacePixels;
    void* mRawPixels;
    int mRawPitch;
    int mWidth;
    int mHeight;
};

class Tile
{
public:
    Tile(int x, int y, int tileType);
    void render(SDL_Rect& camera);
    int getType();
    SDL_Rect getBox();

private:
    SDL_Rect mBox;
    int mType;
};

class DataStream
{
public:
    DataStream(int Frame);
    bool loadMedia(std::string path1);
    void free();
    void* getBuffer();
    int getFrame();
    int getCurrentImage();
    int getCurrentFrame();
    void upCurrentFrame(int n = 6);
    void upCurrentImage();

private:
    SDL_Surface* mImages[24];
    int mCurrentImage;
    float mCurrentFrame;
    int mDelayFrames;
    int mFrame;
};

class LTimer
{
public:
    LTimer();
    void start();
    void stop();
    void pause();
    void unpause();
    Uint32 getTicks();
    bool isStarted();
    bool isPaused();

private:
    Uint32 mStartTicks;
    Uint32 mPausedTicks;
    bool mPaused;
    bool mStarted;
};

class Dot
{
public:
    static const int DOT_WIDTH = 20;
    static const int DOT_HEIGHT = 20;
    static const int DOT_VEL = 100;

    Dot();
    void handleEvent(SDL_Event& e);
    void move(Tile* tiles[], float timeStep);
    void setCamera(SDL_Rect& camera);
    void render(SDL_Rect& camera);
    int getHP();
    int GetX();
    int GetY();
    bool isRight();
    bool isLeft();
    bool isUp();
    bool isDown();
    bool isWalk();
    bool isRun();

private:
    SDL_Rect mBox;
    int mHP;
    int mVelX, mVelY;
    bool walk;
    bool run;
    bool goRight;
    bool goLeft;
    bool goUp;
    bool goDown;
};

class Character
{
public:
    Character();
    bool loadMedia();
    void handleEvent(SDL_Event& e);
    void move(Tile* tiles[], float timeStep);
    void setCamera(SDL_Rect& camera);
    void render(SDL_Rect& camera);
    int GetX();
    int GetY();
    bool isRight();
    bool isLeft();
    bool isUp();
    bool isDown();
    bool isWalk();
    bool isRun();
    void setBlendMode(SDL_BlendMode blending);
    void free();
    int GetHP();
    bool isDead();

private:
    Dot dotCharacter;
    LTexture mName;
    LTexture HP;
    LTexture gStreamingGo;
    LTexture gStreamingStand;
    LTexture gStreamingRun;
    DataStream gDataStreamGo;
    DataStream gDataStreamStand;
    DataStream gDataStreamRun;
    bool dead;
    int mHP;
};

bool init();
bool loadMedia(Tile* tiles[]);
void close(Tile* tiles[]);
bool checkCollision(SDL_Rect a, SDL_Rect b);
bool touchesWall(SDL_Rect box, Tile* tiles[]);
bool setTiles(Tile* tiles[]);

extern SDL_Window* gWindow;
extern SDL_Renderer* gRenderer;
extern TTF_Font* gFont;
extern Character gCharacter;
extern SDL_Color nameColor;
extern SDL_Color HPColor;
extern LTexture gDotTexture;
extern LTexture gTileTexture;
extern SDL_Rect gTileClips[TOTAL_TILE_SPRITES];

#endif // NEED_H
