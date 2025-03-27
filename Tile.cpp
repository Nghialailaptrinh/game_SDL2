#include "Tile.h"
#include <SDL.h>
#include <fstream>


int CantMove[18]={64,67,68,69,74,75,76,77,78,79,86,88,91,93,94,97,98,101}; // các loại gạch ko đi qua được
bool canMove(int tileType){
    bool Cmove=true;
for(int i=0;i<18;i++){
    if(tileType==CantMove[i] ||tileType>=101){Cmove=false;break;}
}
   return Cmove;
}


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
        if(mType>=104)gTileTexture[37].render(mBox.x - camera.x, mBox.y - camera.y);
        gTileTexture[mType].render(mBox.x - camera.x, mBox.y - camera.y);
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
         if( !canMove(tiles[i]->getType()))   // nếu là loại gạch không thể đi qua
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

bool setTiles( Tile* tiles[],int level )
 {
 	//Success flag
 	bool tilesLoaded = true;

     //The tile offsets
     int x = 0, y = 0;

     //Open the map

     std::ifstream map;
     if(level==1)map.open( "map/map1.map" );
     if(level==2)map.open( "map/map2.map" );


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

 	}

     //Close the file
     map.close();

     //If the map was loaded fine
     return tilesLoaded;
 }
