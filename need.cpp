#include "need.h"
#include <sstream>


void SetColor(SDL_Surface* &surface, Uint8 r, Uint8 g, Uint8 b, Uint8 a,
              Uint8 r2, Uint8 g2, Uint8 b2, Uint8 a2) {
    if (surface == NULL) {
        printf("Surface is NULL! Cannot set color key.\n");
        return;
    }

    if (surface->format->format != SDL_PIXELFORMAT_RGBA8888) {
        printf("Surface format is not RGBA8888! Cannot set color key.\n");
        return;
    }

    SDL_PixelFormat* format = surface->format;
    Uint32 color1 = SDL_MapRGBA(format, r, g, b, a);
    Uint32 color2;

    if (r2 == 0 && g2 == 0 && b2 == 0 && a2 == 0) {
        color2 = SDL_MapRGBA(format, r, g, b, 0);
    } else {
        color2 = SDL_MapRGBA(format, r2, g2, b2, a2);
    }

    Uint32* pixels = static_cast<Uint32*>(surface->pixels);
    int pixelCount = surface->w * surface->h;

    for (int i = 0; i < pixelCount; ++i) {
        if (pixels[i] == color1) {
            pixels[i] = color2;
        }
    }
}

SDL_Surface* resizeSurface(SDL_Surface* original, int newWidth, int newHeight) {
    SDL_Surface* resizedSurface = SDL_CreateRGBSurface(
        0, newWidth, newHeight, original->format->BitsPerPixel,
        original->format->Rmask, original->format->Gmask,
        original->format->Bmask, original->format->Amask
    );

    if (resizedSurface == nullptr) {
        std::printf("Unable to create resized surface! SDL_Error: %s\n", SDL_GetError());
        return nullptr;
    }

    if (SDL_BlitScaled(original, NULL, resizedSurface, NULL) != 0) {
        std::printf("Unable to resize surface! SDL_Error: %s\n", SDL_GetError());
        SDL_FreeSurface(resizedSurface);
        return nullptr;
    }

    return resizedSurface;
}

bool resizeTexture(SDL_Renderer* renderer, SDL_Texture* texture, int newWidth, int newHeight) {
    if (texture == NULL) {
        printf("Texture is NULL!\n");
        return false;
    }

    int width, height;
    if (SDL_QueryTexture(texture, NULL, NULL, &width, &height) != 0) {
        printf("Failed to query texture: %s\n", SDL_GetError());
        return false;
    }

    SDL_Rect destRect = { 0, 0, newWidth, newHeight };

    if (SDL_RenderCopy(renderer, texture, NULL, &destRect) != 0) {
        printf("Failed to render texture with new size: %s\n", SDL_GetError());
        return false;
    }

    return true;
}

int checkDistance(float x1, float y1, float x2, float y2, float d) {
    float distance = sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
    return distance < d ? 1 : 0;
}

int checkDiRect(float x1, float y1, float x2, float y2) {
    float diffX = x2 - x1;
    float diffY = y2 - y1;

    if (fabs(diffX) > fabs(diffY)) {
        return diffX > 0 ? 0 : 2;
    } else {
        return diffY > 0 ? 3 : 1;
    }
}

float getDistance(float x1, float y1, float x2, float y2) {
    return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
}

std::pair<int, int>* createPositionArray(int size) {
    std::pair<int, int>* position = new std::pair<int, int>[size];
    for (int i = 0; i < size; ++i) {
        position[i] = std::make_pair(50, 50);
    }
    return position;
}

bool checkCollision( SDL_Rect a, SDL_Rect b )
 {
     //The sides of the rectangles
     int leftA, leftB;
     int rightA, rightB;
     int topA, topB;
     int bottomA, bottomB;

     //Calculate the sides of rect A
     leftA = a.x;
     rightA = a.x + a.w;
     topA = a.y;
     bottomA = a.y + a.h;

     //Calculate the sides of rect B
     leftB = b.x;
     rightB = b.x + b.w;
     topB = b.y;
     bottomB = b.y + b.h;

     //If any of the sides from A are outside of B
     if( bottomA <= topB )
     {
         return false;
     }

     if( topA >= bottomB )
     {
         return false;
     }

     if( rightA <= leftB )
     {
         return false;
     }

     if( leftA >= rightB )
     {
         return false;
     }

     //If none of the sides from A are outside B
     return true;
 }

