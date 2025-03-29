#ifndef DATASTREAM_H_INCLUDED
#define DATASTREAM_H_INCLUDED

#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include <sstream>
#include <iomanip>
#include <cmath>

#include "need.h"
///////////////////////////////////////// tạm thời không sử  dụng thứ này do không có nhu cầu cải tiến đồ họa/////////////////////////
class DataStream
{
public:
    DataStream(int Frame);
    ~DataStream() { free(); }

    bool loadMedia(std::string path1);
    void free();

    void* getBuffer(int n = 0);

    int getFrame() { return mFrame; }
    int getCurrentImage() { return mCurrentImage; }
    int getCurrentFrame() { return static_cast<int>(mCurrentFrame); }
    float FgetCurrentFrame() { return mCurrentFrame; }

    void upCurrentFrame(int n) {
        mCurrentFrame += 0.1f;
        mCurrentFrame = fmod(mCurrentFrame, n);
    }

    void upCurrentImage() { mCurrentImage = (mCurrentImage + 1) % mFrame; }

private:
    SDL_Surface* mImages[24];
    int mCurrentImage;
    float mCurrentFrame;
    int mDelayFrames;
    int mFrame;
};


#endif // DATASTREAM_H_INCLUDED
