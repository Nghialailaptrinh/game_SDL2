#include "need.h"
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <cstdio>
#include <string>
#include <sstream>
#include <cmath>
#include <iostream>

SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;
TTF_Font* gFont = NULL;
Character gCharacter;
SDL_Color nameColor = {0, 0, 255};
SDL_Color HPColor = {0, 100, 255};

LTexture gDotTexture;
LTexture gTileTexture;
SDL_Rect gTileClips[TOTAL_TILE_SPRITES];

// Implementation of LTexture class, Tile class, DataStream class, LTimer class, Dot class, and Character class goes here.

bool init()
{
    bool success = true;
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
        success = false;
    }
    else
    {
        if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
        {
            printf("Warning: Linear texture filtering not enabled!\n");
        }

        gWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if (gWindow == NULL)
        {
            printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
            success = false;
        }
        else
        {
            gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
            if (gRenderer == NULL)
            {
                printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
                success = false;
            }
            else
            {
                SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

                int imgFlags = IMG_INIT_PNG;
                if (!(IMG_Init(imgFlags) & imgFlags))
                {
                    printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
                    success = false;
                }

                if (TTF_Init() == -1)
                {
                    printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
                    success = false;
                }
            }
        }
    }
    return success;
}

bool loadMedia(Tile* tiles[])
{
    bool success = true;

    gFont = TTF_OpenFont("font/lazy.ttf", 12);
    if (gFont == NULL)
    {
        printf("Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError());
        success = false;
    }

    if (!gDotTexture.loadFromFile("image/dot.bmp"))
    {
        printf("Failed to load dot texture!\n");
        success = false;
    }

    if (!gCharacter.loadMedia())
    {
        printf("Failed to load character!\n");
        success = false;
    }

    if (!gTileTexture.loadFromFile("image/tiles.png"))
    {
        printf("Failed to load tile set texture!\n");
        success = false;
    }

    if (!setTiles(tiles))
    {
        printf("Failed to load tile set!\n");
        success = false;
    }

    return success;
}

void close(Tile* tiles[])
{
    for (int i = 0; i < TOTAL_TILES; ++i)
    {
        if (tiles[i] != NULL)
        {
            delete tiles[i];
            tiles[i] = NULL;
        }
    }

    gDotTexture.free();
    gTileTexture.free();
    gCharacter.free();
    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    gWindow = NULL;
    gRenderer = NULL;

    TTF_CloseFont(gFont);
    gFont = NULL;

    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

bool checkCollision(SDL_Rect a, SDL_Rect b)
{
    int leftA, leftB;
    int rightA, rightB;
    int topA, topB;
    int bottomA, bottomB;

    leftA = a.x;
    rightA = a.x + a.w;
    topA = a.y;
    bottomA = a.y + a.h;

    leftB = b.x;
    rightB = b.x + b.w;
    topB = b.y;
    bottomB = b.y + b.h;

    if (bottomA <= topB)
    {
        return false;
    }

    if (topA >= bottomB)
    {
        return false;
    }

    if (rightA <= leftB)
    {
        return false;
    }

    if (leftA >= rightB)
    {
        return false;
    }

    return true;
}

bool touchesWall(SDL_Rect box, Tile* tiles[])
{
    for (int i = 0; i < TOTAL_TILES; ++i)
    {
        if ((tiles[i]->getType() >= TILE_CENTER) && (tiles[i]->getType() <= TILE_TOPLEFT))
        {
            if (checkCollision(box, tiles[i]->getBox()))
            {
                return true;
            }
        }
    }
    return false;
}

bool setTiles(Tile* tiles[])
{
    bool tilesLoaded = true;
    int x = 0, y = 0;

    std::ifstream map("map/lazy.map");

    if (map.fail())
    {
        printf("Unable to load map file!\n");
        tilesLoaded = false;
    }
    else
    {
        for (int i = 0; i < TOTAL_TILES; ++i)
        {
            int tileType = -1;
            map >> tileType;
            if (map.fail())
            {
                printf("Error loading map: Unexpected end of file!\n");
                tilesLoaded = false;
                break;
            }

            if ((tileType >= 0) && (tileType < TOTAL_TILE_SPRITES))
            {
                tiles[i] = new Tile(x, y, tileType);
            }
            else
            {
                printf("Error loading map: Invalid tile type at %d!\n", i);
                tilesLoaded = false;
                break;
            }

            x += TILE_WIDTH;

            if (x >= LEVEL_WIDTH)
            {
                x = 0;
                y += TILE_HEIGHT;
            }
        }
    }

    map.close();
    return tilesLoaded;
}

int main(int argc, char* args[])
{
    if (!init())
    {
        printf("Failed to initialize!\n");
    }
    else
    {
        Tile* tileSet[TOTAL_TILES];

        if (!loadMedia(tileSet))
        {
            printf("Failed to load media!\n");
        }
        else
        {
            bool quit = false;
            SDL_Event e;
            LTimer stepTimer;
            SDL_Rect camera = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};

            while (!quit)
            {
                while (SDL_PollEvent(&e) != 0)
                {
                    if (e.type == SDL_QUIT)
                    {
                        quit = true;
                    }

                    gCharacter.handleEvent(e);
                    if (gCharacter.isDead())
                    {
                        quit = true;
                    }
                }

                float timeStep = stepTimer.getTicks() / 1000.f;
                gCharacter.move(tileSet, timeStep);
                stepTimer.start();
                gCharacter.setCamera(camera);

                gCharacter.setBlendMode(SDL_BLENDMODE_BLEND);
                SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
                SDL_RenderClear(gRenderer);

                for (int i = 0; i < TOTAL_TILES; ++i)
                {
                    tileSet[i]->render(camera);
                }

                gCharacter.render(camera);

                SDL_RenderPresent(gRenderer);
            }

            close(tileSet);
        }
    }

    return 0;
}
