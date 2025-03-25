#include "DataStream.h"
#include <cstdio>

DataStream::DataStream(int Frame)
{
    mFrame = Frame;
    for (int i = 0; i < 24; ++i) {
        mImages[i] = nullptr;
    }

    mCurrentFrame = 0.0f;
    mCurrentImage = 0;
    mDelayFrames = 4;
}

bool DataStream::loadMedia(std::string path1)
{
    bool success = true;

    for (int i = 0; i < mFrame; ++i)
    {
        std::stringstream path;
        path << path1 << std::setw(2) << std::setfill('0') << i << ".png";

        SDL_Surface* loadedSurface = IMG_Load(path.str().c_str());
        if (loadedSurface == nullptr)
        {
            printf("Unable to load %s! SDL_image error: %s\n", path.str().c_str(), IMG_GetError());
            success = false;
        }
        else
        {
            if (mFrame != 1) {
                loadedSurface = resizeSurface(loadedSurface, 80, 80);
            }
            mImages[i] = SDL_ConvertSurfaceFormat(loadedSurface, SDL_PIXELFORMAT_RGBA8888, 0);
            SetColor(mImages[i], 255, 255, 255, 0);
        }

        SDL_FreeSurface(loadedSurface);
    }

    return success;
}

void DataStream::free()
{
    for (int i = 0; i < mFrame; ++i)
    {
        if (mImages[i] != nullptr)
        {
            SDL_FreeSurface(mImages[i]);
            mImages[i] = nullptr;
        }
    }
}

void* DataStream::getBuffer(int n)
{
    if (n == 0) {
        --mDelayFrames;
        if (mDelayFrames == 0)
        {
            upCurrentImage();
            mDelayFrames = 2;
        }

        return mImages[mCurrentImage]->pixels;
    }
    else {
        return mImages[n - 1]->pixels;
    }
}
