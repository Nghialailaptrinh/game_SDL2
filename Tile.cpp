#include "Tile.h"
#include <SDL.h>
#include <fstream>


Tile::Tile(int x, int y, int tileType)
{
    mBox.x = x;
    mBox.y = y;
    mBox.w = TILE_WIDTH;
    mBox.h = TILE_HEIGHT;

    mType = tileType;
}

void Tile::render(SDL_Rect& camera)
{
    if (checkCollision(camera, mBox))
    {
        gTileTexture.render(mBox.x - camera.x, mBox.y - camera.y, &gTileClips[mType]);
    }
}

int Tile::getType()
{
    return mType;
}

SDL_Rect Tile::getBox()
{
    return mBox;
}

bool touchesWall( SDL_Rect box, Tile* tiles[] )
 {
     //Go through the tiles
     for( int i = 0; i < TOTAL_TILES; ++i )
     {
         //If the tile is a wall type tile
         if( ( tiles[ i ]->getType() >= TILE_CENTER ) && ( tiles[ i ]->getType() <= TILE_TOPLEFT ) )
         {
             //If the collision box touches the wall tile
             if( checkCollision( box, tiles[ i ]->getBox() ) )
             {
                 return true;
             }
         }
     }

     //If no wall tiles were touched
     return false;
 }

bool setTiles( Tile* tiles[] )
 {
 	//Success flag
 	bool tilesLoaded = true;

     //The tile offsets
     int x = 0, y = 0;

     //Open the map
     std::ifstream map( "map/map1.map" );

     //If the map couldn't be loaded
     if( map.fail() )
     {
 		printf( "Unable to load map file!\n" );
 		tilesLoaded = false;
     }
 	else
 	{
 		//Initialize the tiles
 		for( int i = 0; i < TOTAL_TILES; ++i )
 		{
 			//Determines what kind of tile will be made
 			int tileType = -1;

 			//Read tile from map file
 			map >> tileType;

 			//If the was a problem in reading the map
 			if( map.fail() )
 			{
 				//Stop loading map
 				printf( "Error loading map: Unexpected end of file!\n" );
 				tilesLoaded = false;
 				break;
 			}

 			//If the number is a valid tile number
 			if( ( tileType >= 0 ) && ( tileType < TOTAL_TILE_SPRITES ) )
 			{
 				tiles[ i ] = new Tile( x, y, tileType );
 			}
 			//If we don't recognize the tile type
 			else
 			{
 				//Stop loading map
 				printf( "Error loading map: Invalid tile type at %d!\n", i );
 				tilesLoaded = false;
 				break;
 			}

 			//Move to next tile spot
 			x += TILE_WIDTH;

 			//If we've gone too far
 			if( x >= LEVEL_WIDTH )
 			{
 				//Move back
 				x = 0;

 				//Move to the next row
 				y += TILE_HEIGHT;
 			}
 		}

 		//Clip the sprite sheet
 		if( tilesLoaded )
 		{
 			gTileClips[ TILE_RED ].x = 0;
 			gTileClips[ TILE_RED ].y = 0;
 			gTileClips[ TILE_RED ].w = TILE_WIDTH;
 			gTileClips[ TILE_RED ].h = TILE_HEIGHT;

 			gTileClips[ TILE_GREEN ].x = 0;
 			gTileClips[ TILE_GREEN ].y = 80;
 			gTileClips[ TILE_GREEN ].w = TILE_WIDTH;
 			gTileClips[ TILE_GREEN ].h = TILE_HEIGHT;

 			gTileClips[ TILE_BLUE ].x = 0;
 			gTileClips[ TILE_BLUE ].y = 160;
 			gTileClips[ TILE_BLUE ].w = TILE_WIDTH;
 			gTileClips[ TILE_BLUE ].h = TILE_HEIGHT;

 			gTileClips[ TILE_TOPLEFT ].x = 80;
 			gTileClips[ TILE_TOPLEFT ].y = 0;
 			gTileClips[ TILE_TOPLEFT ].w = TILE_WIDTH;
 			gTileClips[ TILE_TOPLEFT ].h = TILE_HEIGHT;

 			gTileClips[ TILE_LEFT ].x = 80;
 			gTileClips[ TILE_LEFT ].y = 80;
 			gTileClips[ TILE_LEFT ].w = TILE_WIDTH;
 			gTileClips[ TILE_LEFT ].h = TILE_HEIGHT;

 			gTileClips[ TILE_BOTTOMLEFT ].x = 80;
 			gTileClips[ TILE_BOTTOMLEFT ].y = 160;
 			gTileClips[ TILE_BOTTOMLEFT ].w = TILE_WIDTH;
 			gTileClips[ TILE_BOTTOMLEFT ].h = TILE_HEIGHT;

 			gTileClips[ TILE_TOP ].x = 160;
 			gTileClips[ TILE_TOP ].y = 0;
 			gTileClips[ TILE_TOP ].w = TILE_WIDTH;
 			gTileClips[ TILE_TOP ].h = TILE_HEIGHT;

 			gTileClips[ TILE_CENTER ].x = 160;
 			gTileClips[ TILE_CENTER ].y = 80;
 			gTileClips[ TILE_CENTER ].w = TILE_WIDTH;
 			gTileClips[ TILE_CENTER ].h = TILE_HEIGHT;

 			gTileClips[ TILE_BOTTOM ].x = 160;
 			gTileClips[ TILE_BOTTOM ].y = 160;
 			gTileClips[ TILE_BOTTOM ].w = TILE_WIDTH;
 			gTileClips[ TILE_BOTTOM ].h = TILE_HEIGHT;

 			gTileClips[ TILE_TOPRIGHT ].x = 240;
 			gTileClips[ TILE_TOPRIGHT ].y = 0;
 			gTileClips[ TILE_TOPRIGHT ].w = TILE_WIDTH;
 			gTileClips[ TILE_TOPRIGHT ].h = TILE_HEIGHT;

 			gTileClips[ TILE_RIGHT ].x = 240;
 			gTileClips[ TILE_RIGHT ].y = 80;
 			gTileClips[ TILE_RIGHT ].w = TILE_WIDTH;
 			gTileClips[ TILE_RIGHT ].h = TILE_HEIGHT;

 			gTileClips[ TILE_BOTTOMRIGHT ].x = 240;
 			gTileClips[ TILE_BOTTOMRIGHT ].y = 160;
 			gTileClips[ TILE_BOTTOMRIGHT ].w = TILE_WIDTH;
 			gTileClips[ TILE_BOTTOMRIGHT ].h = TILE_HEIGHT;
 		}
 	}

     //Close the file
     map.close();

     //If the map was loaded fine
     return tilesLoaded;
 }
