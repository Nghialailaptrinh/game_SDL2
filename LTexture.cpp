#include "LTexture.h"
#include <cstdio>
#include <cstring>



 //Scene textures
 LTexture gDotTexture; //thuận tiện test thay vì dùng hoạt ảnh
 LTexture gPauseTexture;
 LTexture gOverTexture;
 LTexture gPassTexture;
 LTexture gRainTexture; // thời tiết;
 LTexture gTileTexture[TOTAL_TILE_SPRITES];
 LTexture OpenTexture;
 LTexture OpenTexture1;
 LTexture texture1;
 LTexture texture2;
 LTexture texture3;
 LTexture texture4;
 LTexture HandBook1Texture;
 LTexture HandBook2Texture;
 LTexture Q;


LTexture::LTexture()
    : mTexture(nullptr), mSurfacePixels(nullptr), mRawPixels(nullptr), mRawPitch(0),
      mWidth(0), mHeight(0) {}

LTexture::~LTexture() {
    free();
}

bool LTexture::loadFromFile(std::string path, Uint8 red, Uint8 green, Uint8 blue) {
    if (!loadPixelsFromFile(path)) {
        printf("Failed to load pixels for %s!\n", path.c_str());
        return false;
    }

    if (!loadFromPixels(red, green, blue)) {
        printf("Failed to create texture from pixels from %s!\n", path.c_str());
        return false;
    }

    return true;
}

bool LTexture::loadPixelsFromFile(std::string path) {
    free();

    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    if (!loadedSurface) {
        printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
        return false;
    }

    mSurfacePixels = SDL_ConvertSurfaceFormat(loadedSurface, SDL_GetWindowPixelFormat(gWindow), 0);
    if (!mSurfacePixels) {
        printf("Unable to convert loaded surface! SDL Error: %s\n", SDL_GetError());
        SDL_FreeSurface(loadedSurface);
        return false;
    }

    mWidth = mSurfacePixels->w;
    mHeight = mSurfacePixels->h;

    SDL_FreeSurface(loadedSurface);
    return true;
}

bool LTexture::loadFromPixels(Uint8 red, Uint8 green, Uint8 blue) {
    if (!mSurfacePixels) {
        printf("No pixels loaded!\n");
        return false;
    }

    SDL_SetColorKey(mSurfacePixels, SDL_TRUE, SDL_MapRGB(mSurfacePixels->format, red, green, blue));
    mTexture = SDL_CreateTextureFromSurface(gRenderer, mSurfacePixels);
    if (!mTexture) {
        printf("Unable to create texture! SDL Error: %s\n", SDL_GetError());
        SDL_FreeSurface(mSurfacePixels);
        mSurfacePixels = nullptr;
        return false;
    }

    return true;
}

#if defined(SDL_TTF_MAJOR_VERSION)
bool LTexture::loadFromRenderedText(std::string textureText, SDL_Color textColor) {
    free();

    SDL_Surface* textSurface = TTF_RenderText_Solid(gFont, textureText.c_str(), textColor);
    if (!textSurface) {
        printf("Unable to render text! SDL_ttf Error: %s\n", TTF_GetError());
        return false;
    }

    mTexture = SDL_CreateTextureFromSurface(gRenderer, textSurface);
    if (!mTexture) {
        printf("Unable to create texture from text! SDL Error: %s\n", SDL_GetError());
        SDL_FreeSurface(textSurface);
        return false;
    }

    mWidth = textSurface->w;
    mHeight = textSurface->h;
    SDL_FreeSurface(textSurface);
    return true;
}
#endif

bool LTexture::createBlank(int width, int height) {
    free();
    mTexture = SDL_CreateTexture(gRenderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, width, height);
    if (!mTexture) {
        printf("Unable to create blank texture! SDL Error: %s\n", SDL_GetError());
        return false;
    }
    mWidth = width;
    mHeight = height;
    return true;
}

void LTexture::free() {
    if (mTexture != nullptr) {
        SDL_DestroyTexture(mTexture);  // Giải phóng texture
        mTexture = nullptr;  // Đặt lại con trỏ về nullptr để tránh lỗi trỏ đến bộ nhớ đã giải phóng
    }
    if (mSurfacePixels != nullptr) {
        SDL_FreeSurface(mSurfacePixels);  // Giải phóng surface
        mSurfacePixels = nullptr;  // Đặt lại con trỏ về nullptr
    }
    mWidth = 0;  // Đặt lại chiều rộng
    mHeight = 0;  // Đặt lại chiều cao
}


void LTexture::setColor(Uint8 red, Uint8 green, Uint8 blue) {
    SDL_SetTextureColorMod(mTexture, red, green, blue);
}

void LTexture::setBlendMode(SDL_BlendMode blending) {
    SDL_SetTextureBlendMode(mTexture, blending);
}

void LTexture::setAlpha(Uint8 alpha) {
    SDL_SetTextureAlphaMod(mTexture, alpha);
}

void LTexture::render(int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip) {
    SDL_Rect renderQuad = { x, y, mWidth, mHeight };
    if (clip) {
        renderQuad.w = clip->w;
        renderQuad.h = clip->h;
    }
    SDL_RenderCopyEx(gRenderer, mTexture, clip, &renderQuad, angle, center, flip);
}

int LTexture::getWidth() {
    return mWidth;
}

int LTexture::getHeight() {
    return mHeight;
}

Uint32* LTexture::getPixels32() {
    return mSurfacePixels ? static_cast<Uint32*>(mSurfacePixels->pixels) : nullptr;
}

Uint32 LTexture::getPixel32(Uint32 x, Uint32 y) {
    Uint32* pixels = static_cast<Uint32*>(mSurfacePixels->pixels);
    return pixels[(y * getPitch32()) + x];
}

Uint32 LTexture::getPitch32() {
    return mSurfacePixels ? (mSurfacePixels->pitch / 4) : 0;
}

Uint32 LTexture::mapRGBA(Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
    return mSurfacePixels ? SDL_MapRGBA(mSurfacePixels->format, r, g, b, a) : 0;
}

bool LTexture::lockTexture() {
    if (mRawPixels) {
        printf("Texture already locked!\n");
        return false;
    }
    return SDL_LockTexture(mTexture, nullptr, &mRawPixels, &mRawPitch) == 0;
}

bool LTexture::unlockTexture() {
    if (!mRawPixels) {
        printf("Texture not locked!\n");
        return false;
    }
    SDL_UnlockTexture(mTexture);
    mRawPixels = nullptr;
    mRawPitch = 0;
    return true;
}

void LTexture::copyRawPixels32(void* pixels) {
    if (mRawPixels)
        memcpy(mRawPixels, pixels, mRawPitch * mHeight);
}
