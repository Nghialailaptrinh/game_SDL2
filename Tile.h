#ifndef TILE_H_INCLUDED
#define TILE_H_INCLUDED


#include <SDL.h>
#include "LTexture.h"


class Tile
{
public:
    Tile(int x, int y, int tileType);
    ~Tile(){}

    void render(SDL_Rect& camera);
    int getType();
    SDL_Rect getBox();

private:
    SDL_Rect mBox;
    int mType;
};
bool setTiles( Tile *tiles[] );

bool touchesWall(SDL_Rect box,Tile* tiles[]);




#endif // TILE_H_INCLUDED
