#ifndef NEED_H_INCLUDED
#define NEED_H_INCLUDED

#include <SDL.h>
#include <cstdio>
#include <cmath>
#include <utility>



void SetColor(SDL_Surface* &surface, Uint8 r, Uint8 g, Uint8 b, Uint8 a,
              Uint8 r2 = 0, Uint8 g2 = 0, Uint8 b2 = 0, Uint8 a2 = 0);

SDL_Surface* resizeSurface(SDL_Surface* original, int newWidth, int newHeight);

bool resizeTexture(SDL_Renderer* renderer, SDL_Texture* texture, int newWidth, int newHeight);

int checkDistance(float x1, float y1, float x2, float y2, float d);

int checkDiRect(float x1, float y1, float x2, float y2);

float getDistance(float x1, float y1, float x2, float y2);

std::pair<int, int>* createPositionArray(int size = 20);

bool checkCollision( SDL_Rect a, SDL_Rect b );

#endif // NEED_H_INCLUDED
