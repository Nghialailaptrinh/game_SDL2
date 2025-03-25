#ifndef LTIMER_H_INCLUDED
#define LTIMER_H_INCLUDED

#include <SDL.h>

class LTimer
{
public:
    LTimer();
    ~LTimer(){};

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


#endif // LTIMER_H_INCLUDED
