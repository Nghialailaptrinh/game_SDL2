#include "constant.h"

// Define the globally used variables here
SDL_Color nameColor {0, 255, 0};  // Màu tên
SDL_Color HPColor {0, 100, 255};  // Màu HP
SDL_Color eHPColor {255, 0, 0};   // Màu HP đối thủ

Mix_Music* gRun = NULL;           // Âm nhạc chạy
Mix_Music* gGo = NULL;            // Âm nhạc đi bộ
Mix_Chunk* gSword = NULL;         // Âm thanh vũ khí

SDL_Window* gWindow = NULL;       // The window we'll be rendering to
SDL_Renderer* gRenderer = NULL;   // The window renderer

TTF_Font* gFont = NULL;           // Globally used font


