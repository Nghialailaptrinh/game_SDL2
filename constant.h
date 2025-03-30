#ifndef CONSTANT_H_INCLUDED
#define CONSTANT_H_INCLUDED

#include <SDL.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>

// Forward declaration of Dot and other classes if necessary


// Screen dimension constants
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 500;

// The dimensions of the level
const int LEVEL_WIDTH = 1600;
const int LEVEL_HEIGHT = 1600;

// Tile constants
const int TILE_WIDTH = 32;
const int TILE_HEIGHT = 32;
const int TOTAL_TILES = 2500;
const int TOTAL_TILE_SPRITES = 243;

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


// Store the frame rate target
const int FPS = 60;
const int frameDelay = 1000 / FPS;  // Delay per frame to reach 60 FPS (in milliseconds)

extern SDL_Color nameColor ;
extern SDL_Color HPColor ;
extern SDL_Color eHPColor ;

extern Mix_Music* gRun ;       // Âm nhạc chạy
extern Mix_Music* gGo ;        // Âm nhạc đi bộ
extern Mix_Chunk* gSword ;     // Âm thanh vũ khí

// The window we'll be rendering to
extern SDL_Window* gWindow ;

// The window renderer
extern SDL_Renderer* gRenderer ;

//Globally used font
extern TTF_Font* gFont ;

const int MaxLevel=3; // game chỉ mới có 3 màn

#endif // CONSTANT_H_INCLUDED
