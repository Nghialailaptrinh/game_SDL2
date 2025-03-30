#ifndef LTEXTURE_H_INCLUDED
#define LTEXTURE_H_INCLUDED


#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <string>
#include "constant.h"
#include "need.h"


class LTexture
{
public:
    LTexture();
    ~LTexture();

    bool loadFromFile(std::string path, Uint8 red = 0, Uint8 green = 255, Uint8 blue = 0);
    bool loadPixelsFromFile(std::string path);
    bool loadFromPixels(Uint8 red, Uint8 green, Uint8 blue);

#if defined(SDL_TTF_MAJOR_VERSION)
    bool loadFromRenderedText(std::string textureText, SDL_Color textColor);
#endif

    bool createBlank(int width, int height);
    void free();

    void setColor(Uint8 red, Uint8 green, Uint8 blue);
    void setBlendMode(SDL_BlendMode blending);
    void setAlpha(Uint8 alpha);

    void render(int x, int y, SDL_Rect* clip = nullptr, double angle = 0.0, SDL_Point* center = nullptr, SDL_RendererFlip flip = SDL_FLIP_NONE);

    int getWidth();
    int getHeight();

    Uint32* getPixels32();
    Uint32 getPixel32(Uint32 x, Uint32 y);
    Uint32 getPitch32();
    Uint32 mapRGBA(Uint8 r, Uint8 g, Uint8 b, Uint8 a);
    void copyRawPixels32(void* pixels);
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


 extern LTexture gDotTexture; //thuận tiện test thay vì dùng hoạt ảnh
 extern LTexture gPauseTexture; // kết thúc;
 extern LTexture gOverTexture;
 extern LTexture gPassTexture;
 extern LTexture gRainTexture; // thời tiết;

 extern LTexture gTileTexture[TOTAL_TILE_SPRITES]; // gạch;

 extern LTexture OpenTexture;
 extern LTexture OpenTexture1;
 extern LTexture texture1;
 extern LTexture texture2;
 extern LTexture texture3;
 extern LTexture texture4;
 extern LTexture HandBook1Texture;
 extern LTexture HandBook2Texture;
 extern LTexture Q;
#endif // LTEXTURE_H_INCLUDED
