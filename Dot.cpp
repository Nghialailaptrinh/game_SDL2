#include "Dot.h"
#include "Character.h"
//class Character;

Dot** dotSlime;
Dot** dotWolve;


 Dot::Dot(int x,int y,int typeE)
 {
     //Initialize the collision box
     mBox.x = x;
     mBox.y = y;
 	mBox.w = DOT_WIDTH;
 	mBox.h = DOT_HEIGHT;

     //Initialize the velocity
     mVelX = 0;
     mVelY = 0;
     lastDirection=rand()%3;
    walk=0;
    run=0;
    die=0;
    dead=0;
    goRight = 1;
    goLeft = 0;
    goUp =0;
    goDown = 0;
    touch=0;
    attack=0;
    hurt=0;
    timeHurt=0;
    mHP=100;
    maxHP=100;
    dameSword=0;
    typeEnemy=typeE;

 }

 void Dot::handleEvent( SDL_Event& e )
 {
     //If a key was pressed
 	if( e.type == SDL_KEYDOWN && e.key.repeat == 0 )
     {
         //Adjust the velocity
         switch( e.key.keysym.sym )
         {
             case SDLK_UP: case SDLK_w: mVelY -= DOT_VEL;break;
             case SDLK_DOWN: case SDLK_s : mVelY += DOT_VEL;  break;
             case SDLK_LEFT: case SDLK_a : mVelX -= DOT_VEL; break;
             case SDLK_RIGHT: case SDLK_d : mVelX += DOT_VEL;  break;
             case SDLK_LSHIFT: run=1; break;
             case SDLK_j: attack=1; break;
         }

     }


     //If a key was released
     else if( e.type == SDL_KEYUP && e.key.repeat == 0 )
     {
         //Adjust the velocity
         switch( e.key.keysym.sym )
         {   case SDLK_LSHIFT: run=0; break;
             case SDLK_UP: case SDLK_w: mVelY += DOT_VEL; break;
             case SDLK_DOWN: case SDLK_s : mVelY -= DOT_VEL; break;
             case SDLK_LEFT: case SDLK_a : mVelX += DOT_VEL; break;
             case SDLK_RIGHT: case SDLK_d :  mVelX -= DOT_VEL; break;
             case SDLK_j: attack=0;break;
         }
     }


      if(mVelX!=0 || mVelY!=0){walk=1;}
      else walk =0;

      if(mVelX==0 && mVelY==0){}
      else{goUp = 0;goDown =0; goRight=0; goLeft =0;
      if(mVelX>0)goRight=1;
      else if(mVelX<0)goLeft=1;
      else if(mVelY<0)goUp=1;
      else goDown=1;}


 }

void Dot::updatePosition(std::pair<int,int>* position) {
    // Di chuyển bóng các vị trí còn lại về trước (đẩy lùi các bóng)
    for (int i = 19; i > 0; --i) {
        position[i] = position[i - 1];  // Đẩy lùi các bóng
    }

    // Cập nhật bóng số 0 là vị trí hiện tại của nhân vật
    position[0] = std::make_pair(mBox.x, mBox.y);  // Cập nhật bóng với vị trí hiện tại
}

void Dot::AiHandleEvent(SDL_Event& e, Tile* tiles[], std::pair<int, int>* position) {
    bool hasMoved = false;
    bool isAngry = false;

    if (timeHurt == 4 && typeEnemy == 2) {
        setVel(200);
    }

    if (hurt != 0) {
        timeHurt -= 4;
    }

    if (timeHurt <= 0) {
        hurt = 0;
        timeHurt = 0;
    }

    if (hurt && (typeEnemy > 1) && getDistance((float)mBox.x, (float)mBox.y, (float)gCharacter.GetX(), (float)gCharacter.GetY()) < 50) {
        attack = 1;
    } else {
        attack = 0;
        if (hurt && (typeEnemy > 1)) {
            isAngry = true;
        }

        if (isAngry && (position != nullptr)) {
            for (int i = 0; i < 20; ++i) {
                if (position[i].first > mBox.x)
                    mVelX = DOT_VEL;
                else if (position[i].first < mBox.x)
                    mVelX = -DOT_VEL;

                if (position[i].second > mBox.y)
                    mVelY = DOT_VEL;
                else if (position[i].second < mBox.y)
                    mVelY = -DOT_VEL;

                move(tiles, 0.01);

                if (touch == 1) {
                    touch = 0;
                } else {
                    hasMoved = true;
                    break;
                }
            }
        } else if (!hasMoved) {
            int moveCode = rand() % 600;

            if (moveCode >=20) {
                // Giữ nguyên hướng di chuyển
            } else {
                if (moveCode < 2)
                    lastDirection = (lastDirection + 2) % 4;
                else if (moveCode <= 10)
                    lastDirection = (lastDirection + 1) % 4;
                else if (moveCode <= 19)
                    lastDirection = (lastDirection - 1 + 4) % 4;

                switch (lastDirection) {
                    case 0: mVelX = DOT_VEL; mVelY = 0; break;
                    case 1: mVelX = 0; mVelY = -DOT_VEL; break;
                    case 2: mVelX = -DOT_VEL; mVelY = 0; break;
                    case 3: mVelX = 0; mVelY = DOT_VEL; break;
                }
            }

            touch = 0;
            move(tiles, 0.01);
            if (touch == 1) {
                touch = 0;
                mVelX = -mVelX;
                mVelY = -mVelY;
            }
        }
    }

    goUp = goDown = goRight = goLeft = 0;
    if (mVelX > 0) { goRight = 1; lastDirection = 0; }
    else if (mVelX < 0) { goLeft = 1; lastDirection = 2; }

    if (mVelY > 0) { goDown = 1; lastDirection = 3; }
    else if (mVelY < 0) { goUp = 1; lastDirection = 1; }
}

void Dot::move( Tile *tiles[], float timeStep )
 {

     //Move the dot left or right
     float i=(isRun()?1.5:1);
     float j=(isAttacking()?0.5:1); // nếu đang tấn công; tốc độ giảm đi
     int X= mVelX * timeStep *i*j;
     mBox.x += X;

     //If the dot went too far to the left or right or touched a wall
     if( ( mBox.x < 0 ) || ( mBox.x + DOT_WIDTH > LEVEL_WIDTH ) || touchesWall( mBox, tiles ) )
     {
         //move back
         mBox.x -= X;
         touch=1;
     }

     //Move the dot up or down
     int Y=mVelY * timeStep *i*j;
     mBox.y += Y;

     //If the dot went too far up or down or touched a wall
     if( ( mBox.y < 0 ) || ( mBox.y + DOT_HEIGHT > LEVEL_HEIGHT ) || touchesWall( mBox, tiles ) )
     {
         //move back
         mBox.y -=Y;
         touch=1;
     }

    if(mHP<=0)dead=1;
 }

bool Dot::attackEnemy(Dot* dotEnemy[], int numEnemies, int attackRange,bool inThisFrame) {
    bool damage=0;
        if (isAttacking() && inThisFrame) {
            for (int i = 0; i < numEnemies; ++i) {

                // Kiểm tra nếu kẻ thù nằm trong phạm vi tấn công
                if (checkDistance((float)GetX(),(float)GetY(),(float)dotEnemy[i]->GetX(),(float)dotEnemy[i]->GetY(),attackRange)) {
                        int diRect=checkDiRect((float)GetX(),(float)GetY(),(float)dotEnemy[i]->GetX(),(float)dotEnemy[i]->GetY());
                    // Trừ máu kẻ thù (giảm HP)

                    if( (goRight&&!(diRect==2))||(goUp&&!(diRect==3))||(goLeft&&!(diRect==0))||(goRight==0&&goLeft==0&&goUp==0&& !(diRect==1))){
                            damage =1; // đánh trúng
                    int newHP = dotEnemy[i]->GetHP() - dameSword;
                    dotEnemy[i]->SetHP((newHP>0)?newHP:0);
                    dotEnemy[i]->SetHurt(1);
                    dotEnemy[i]->SetTimeHurt(5000);
                    if(dotEnemy[i]->typeEnemy==2)dotEnemy[i]->setVel(200);     // sói chuyển sang trái thái thần tốc;
                    else if(dotEnemy[i]->typeEnemy==3)dotEnemy[i]->SetDameSword(100);       // goblin chuyển sang trạng thái cuồng bạo
                    else{}
                    }
                }
            }
        }
       return damage;
    }


 void Dot::setCamera( SDL_Rect& camera )
 {
 	//Center the camera over the dot
 	camera.x = ( mBox.x + DOT_WIDTH / 2 ) - SCREEN_WIDTH / 2;
 	camera.y = ( mBox.y + DOT_HEIGHT / 2 ) - SCREEN_HEIGHT / 2;

 	//Keep the camera in bounds
 	if( camera.x < 0 )
 	{
 		camera.x = 0;
 	}
 	if( camera.y < 0 )
 	{
 		camera.y = 0;
 	}
 	if( camera.x > LEVEL_WIDTH - camera.w )
 	{
 		camera.x = LEVEL_WIDTH - camera.w;
 	}
 	if( camera.y > LEVEL_HEIGHT - camera.h )
 	{
 		camera.y = LEVEL_HEIGHT - camera.h;
 	}
 }


