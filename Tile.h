#ifndef TILE_H_INCLUDED
#define TILE_H_INCLUDED


#include <SDL.h>
#include "LTexture.h"


class Tile
{
public:
    Tile(int x, int y, int tileType, bool canmove);
    ~Tile(){}

    void render(SDL_Rect& camera);
    int getType();
    SDL_Rect getBox();
    bool Can_move(){return can_move;}

private:
    SDL_Rect mBox;
    int mType;
    bool can_move;
};
bool setTiles( Tile *tiles[],int level=1 );

bool touchesWall(SDL_Rect box,Tile* tiles[]);




#endif // TILE_H_INCLUDED
