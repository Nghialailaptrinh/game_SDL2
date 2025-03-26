
 // Code được tham khảo từ lazyfoo.net

 #include <SDL.h>
 #include <SDL_image.h>
 #include <SDL_ttf.h>
 #include <SDL_mixer.h>
 #include <stdio.h>
 #include <string>
 #include <fstream>
 #include <sstream>
 #include <iomanip>
 #include <cmath>

 #include "LTimer.h"
 #include "Slime.h"
 #include "Wolve.h"





 //Starts up SDL and creates window
 bool init();

 //Loads media
 bool loadMedia( Tile* tiles[] );

 //Frees media and shuts down SDL
 void close( Tile* tiles[] );



 bool init()
 {
 	//Initialization flag
 	bool success = true;

 	//Initialize SDL
 	if( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_AUDIO ) < 0 )
 	{
 		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
 		success = false;
 	}
 	else
 	{
 		//Set texture filtering to linear
 		if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
 		{
 			printf( "Warning: Linear texture filtering not enabled!" );
 		}

 		//Create window
 		gWindow = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
 		if( gWindow == NULL )
 		{
 			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
 			success = false;
 		}
 		else
 		{
 			//Create renderer for window
 			gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
 			if( gRenderer == NULL )
 			{
 				printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
 				success = false;
 			}
 			else
 			{
 				//Initialize renderer color
 				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );

 				//Initialize PNG loading
 				int imgFlags = IMG_INIT_PNG;
 				if( !( IMG_Init( imgFlags ) & imgFlags ) )
 				{
 					printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
 					success = false;
 				}
 				//Initialize SDL_ttf
				if( TTF_Init() == -1 )
				{
					printf( "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError() );
					success = false;
				}
				//Initialize SDL_mixer
				int flags = MIX_INIT_MP3;
                if ((Mix_Init(flags) & flags) != flags) {
                    printf("Mix_Init failed: %s\n", Mix_GetError());
                    SDL_Quit();
                    return false;
                }
				if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 )
				{
					printf( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
					success = false;
				}
 			}
 		}
 	}

 	return success;
 }

 bool loadMedia( Tile* tiles[] )
 {
 	//Loading success flag
 	bool success = true;

 	//Open the font
	gFont = TTF_OpenFont( "font/VNLUCIDA.TTF", 12 );
	if( gFont == NULL )
	{
		printf( "Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError() );
		success = false;
	}

 	if( !gPauseTexture.loadFromFile( "image/Game_Pause.png" ) )
 	{
 		printf( "Failed to load tile Pause texture!\n" );
 		success = false;
 	}
 	if( !gPassTexture.loadFromFile( "image/Game_Pass.png" ) )
 	{
 		printf( "Failed to load tile Pass texture!\n" );
 		success = false;
 	}
 	else{
        gPassTexture.setBlendMode(SDL_BLENDMODE_BLEND);
        gPassTexture.setAlpha(0);
 	}
 	if( !gRainTexture.loadFromFile( "image/rain.png" ,255,255,255) )
 	{
 		printf( "Failed to load tile rain texture!\n" );
 		success = false;
 	}
 	else{
        gRainTexture.setBlendMode(SDL_BLENDMODE_BLEND);
        gRainTexture.setAlpha(0);
 	}


 	//Load dot Charracter & enemy
 	if( !gCharacter.loadMedia() )
 	{
 		printf( "Failed to load character!\n" );
 		success = false;
 	}

 	//Load tile texture
 	for (int i = 0; i < TOTAL_TILE_SPRITES ; ++i) {
        std::ostringstream filename;
        filename << std::setw(3) << std::setfill('0') << (i + 1) << ".png";  // Tạo tên file từ 001.png đến 104.png

        // Gọi hàm loadFromFile cho từng texture
        if (!gTileTexture[i].loadFromFile("image/objects/1 Tiles/" + filename.str())) {
            printf("Failed to load texture: %s\n", filename.str().c_str());
            success = false;
        }
    }

 	//Load tile map
 	if( !setTiles( tiles ) )
 	{
 		printf( "Failed to load tile set!\n" );
 		success = false;
 	}

 	gGo = Mix_LoadMUS( "audio/go.mp3" );
	if( gGo == NULL )
	{
		printf( "Failed to load go music! SDL_mixer Error: %s\n", Mix_GetError() );
		success = false;
	}
	gRun = Mix_LoadMUS( "audio/run.mp3" );
	if( gRun == NULL )
	{
		printf( "Failed to load run music! SDL_mixer Error: %s\n", Mix_GetError() );
		success = false;
	}
	gRain = Mix_LoadWAV( "audio/rain.mp3" );
    if( gRain == NULL )
	{
		printf( "Failed to load rain music! SDL_mixer Error: %s\n", Mix_GetError() );
		success = false;
	}
	gSword = Mix_LoadWAV( "audio/sword_attack.mp3" );
	if( gSword == NULL )
	{
		printf( "Failed to load sword sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
		success = false;
	}


 	return success;
 }

 void close(Tile* tiles[]) {
    // Giải phóng bộ nhớ cho từng Tile trong mảng
    for (int i = 0; i < TOTAL_TILES; ++i) {
        if (tiles[i] != nullptr) {
            delete tiles[i];  // Giải phóng bộ nhớ của Tile[i]
            tiles[i] = nullptr;  // Đặt lại con trỏ về nullptr để tránh sử dụng lại
        }
    }

    // Giải phóng các tài nguyên khác (như texture, âm thanh)
    gPauseTexture.free();
    gPassTexture.free();
    gRainTexture.free();
    gCharacter.free();

    // Giải phóng bộ nhớ cho các đối tượng khác như gTileTexture, gFont, v.v.
    for (int i = 0; i < TOTAL_TILE_SPRITES; ++i) {
        gTileTexture[i].free();
    }

    // Giải phóng các tài nguyên SDL
    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    gWindow = nullptr;
    gRenderer = nullptr;
    TTF_CloseFont(gFont);
    gFont = nullptr;
    Mix_FreeChunk(gSword);
    gSword = nullptr;
    Mix_FreeMusic(gRun);
    Mix_FreeMusic(gGo);
    Mix_FreeChunk(gRain);
    gRun = nullptr;
    gGo = nullptr;
    gRain = nullptr;

    // Quit SDL subsystems
    Mix_Quit();
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}


 bool handleEvent(SDL_Event &e, Character &character,  bool &quit) {        // ta dùng bool để check xem sự kiện có phải sự kiện thóat không
    static bool waitingForQuit = false;
    // Xử lý sự kiện của Character
    character.handleEvent(e);

    // Xử lý sự kiện thoát
    if (e.type == SDL_QUIT) {
        waitingForQuit = true;
    }

    if (waitingForQuit) {
        if (e.type == SDL_KEYDOWN) {
            if (e.key.keysym.sym == SDLK_y) {
                printf("Exiting game...\n");
                quit = true;
            } else if (e.key.keysym.sym == SDLK_n) {
                printf("Returning to game...\n");
                waitingForQuit = false;
            }
        }
    }
    printf("%d\n",(bool)waitingForQuit);
    return waitingForQuit;
}

 bool renderPass(Dot** dotEnemy, int numEnemies) {
    bool render = true;

    // Kiểm tra xem có kẻ địch nào còn sống không
    for (int i = 0; i < numEnemies; i++) {
        if (dotEnemy[i]->isDie() != 1) {
            render = false;
            break;
        }
    }

    if (render) {
        gPassTexture.render(0, 0);

    }

    return render;
}

// Trong hàm createSlimesFromFile
slime** createSlimesFromFile(const std::string& filename, int& numSlime, Dot**& dotSlime) {
    std::ifstream file(filename);
    if (!file) {
        printf("Không thể mở file!\n");
        return nullptr;
    }

    // Đọc số lượng slime
    file >> numSlime;

    // Tạo mảng chứa các con trỏ slime* và mảng chứa Dot* cho slime
    slime** Slime = new slime*[numSlime];
    dotSlime = new Dot*[numSlime];  // Cấp phát mảng Dot* tương ứng

    // Đọc vị trí của các slime từ file và tạo các đối tượng slime
    int x, y;
    for (int i = 0; i < numSlime; ++i) {
        file >> x >> y;
        Slime[i] = new slime(x, y);  // Tạo slime mới tại vị trí (x, y)
        if (i == 0) {
            if (!Slime[i]->loadMedia()) {
                // Giải phóng tài nguyên đã cấp phát nếu loadMedia thất bại
                for (int j = 0; j < i; ++j) {
                    delete Slime[j];
                }
                delete[] Slime;
                delete[] dotSlime;
                return nullptr; // Trả về nullptr để chỉ ra có lỗi
            }
        }
        dotSlime[i] = Slime[i]->GetDot();  // Cập nhật Dot* của slime vào mảng dotSlime
    }

    // Đóng file
    file.close();

    return Slime;
}


// Trong hàm freeSlimes
void freeSlimes(slime**& Slime, Dot**& dotSlime, int& numSlime) {
    // Giải phóng bộ nhớ cho từng slime và dotSlime
    for (int i = 0; i < numSlime; ++i) {
        if (Slime[i] != nullptr) {
            // Không giải phóng dotSlime[i] vì nó được quản lý bởi slime
            Slime[i]->free();  // Giải phóng tài nguyên của slime (sẽ tự giải phóng Dot nếu cần)
            delete Slime[i];   // Giải phóng bộ nhớ cho đối tượng slime
            Slime[i] = nullptr; // Đặt lại con trỏ về nullptr
        }
    }

    // Giải phóng texture static của slime (chỉ cần giải phóng một lần)
    slime::freeStaticTextures();
    // Giải phóng mảng chứa các con trỏ slime
    delete[] Slime;

    // Không cần giải phóng dotSlime vì mỗi Dot thuộc về một slime và sẽ được giải phóng khi slime bị hủy
    delete[] dotSlime;

    // Đặt lại số lượng slime
    numSlime = 0;
}

wolve** createWolvesFromFile(const std::string& filename, int& numWolve, Dot**& dotWolve) {
    std::ifstream file(filename);
    if (!file) {
        printf("Không thể mở file!\n");
        return nullptr;
    }

    // Đọc số lượng wolve
    file >> numWolve;

    // Tạo mảng chứa các con trỏ wolve* và mảng chứa Dot* cho wolve
    wolve** Wolve = new wolve*[numWolve];
    dotWolve = new Dot*[numWolve];  // Cấp phát mảng Dot* tương ứng

    // Đọc vị trí của các wolve từ file và tạo các đối tượng wolve
    int x, y;
    for (int i = 0; i < numWolve; ++i) {
        file >> x >> y;
        Wolve[i] = new wolve(x, y);  // Tạo wolve mới tại vị trí (x, y)
        if (i == 0) Wolve[i]->loadMedia();
        dotWolve[i] = Wolve[i]->GetDot();  // Cập nhật Dot* của wolve vào mảng dotWolve
    }

    // Đóng file
    file.close();

    return Wolve;
}

void freeWolves(wolve**& Wolve, Dot**& dotWolve, int& numWolve) {
    // Giải phóng bộ nhớ cho từng wolve và dotWolve
    for (int i = 0; i < numWolve; ++i) {
        if (Wolve[i] != nullptr) {
            // Không giải phóng dotWolve[i] vì nó được quản lý bởi wolve
            Wolve[i]->free();  // Giải phóng tài nguyên của wolve (sẽ tự giải phóng Dot nếu cần)
            delete Wolve[i];   // Giải phóng bộ nhớ cho đối tượng wolve
            Wolve[i] = nullptr; // Đặt lại con trỏ về nullptr
        }
    }

    // Giải phóng texture static của wolve (chỉ cần giải phóng một lần)
    wolve::freeStaticTextures();
    // Giải phóng mảng chứa các con trỏ wolve
    delete[] Wolve;

    // Không cần giải phóng dotWolve vì mỗi Dot thuộc về một wolve và sẽ được giải phóng khi wolve bị hủy
    delete[] dotWolve;

    // Đặt lại số lượng wolve
    numWolve = 0;
}

// Hàm xử lý mưa
void handleRain(Uint32 &frameStart) {
    static bool isRaining = false;  // Biến theo dõi trạng thái mưa
    static float frameRain = 0;

    // Kiểm tra nếu đang trong thời gian mưa (20 đến 40 giây)
    if ((frameStart / 20000) % 2) {
        if (!isRaining) {  // Nếu nhạc mưa chưa được phát
            if (((frameStart / 100) % 100) > 20) {
                Mix_PlayChannel(1, gRain, -1);  // Phát nhạc mưa (lặp vô hạn)
                isRaining = true;  // Đánh dấu là đang mưa
            }
        }

        // Tạo hiệu ứng mưa to dần, nhỏ dần
        if ((frameStart / 10000) % 2 == 0) {
            gRainTexture.setAlpha((frameStart / 100) % 100);  // 20 đến 30 mưa to dần
        } else {
            gRainTexture.setAlpha(100 - (frameStart / 100) % 100);  // Mưa nhỏ dần
            // Dừng nhạc mưa khi mưa nhỏ dần (dưới 40)
            if ((100 - (frameStart / 100) % 100) < 40) {
                Mix_HaltChannel(1);  // Dừng nhạc mưa
                isRaining = false;  // Đánh dấu là mưa đã kết thúc
            }
        }

        // Tạo hiệu ứng mưa
        frameRain = fmod((frameRain + 0.1), 4);
        SDL_Rect clip = {0, (int)frameRain * 100, 800, 500};
        gRainTexture.render(0, 0, &clip);
    }
}

void AiHandle(SDL_Event& e, Tile* tileSet[],std:: pair<int,int>* position = nullptr) {

    for (int i = 0; i < numWolve; i++) {
        dotWolve[i]->AiHandleEvent(e, tileSet, position); // Xử lý sự kiện cho từng Wolve
    }

    // Xử lý sự kiện cho tất cả các Slime
    for (int i = 0; i < numSlime; i++) {
        dotSlime[i]->AiHandleEvent(e, tileSet, position); // Xử lý sự kiện cho từng Slime
    }
}







int main( int argc, char* args[] )
{
    srand(static_cast<unsigned int>(time(0)));

    // Start up SDL and create window
    if( !init() )
    {
        printf( "Failed to initialize!\n" );
    }
    else
    {
        // The level tiles
        Tile* tileSet[ TOTAL_TILES ];

        // Load media
        if( !loadMedia( tileSet ) )
        {
            printf( "Failed to load media!\n" );
        }
        else
        {
            // Main loop flag
            bool quit = false;

            // Event handler
            SDL_Event e;

            // Keeps track of time between steps
            LTimer stepTimer;

            // Level camera
            SDL_Rect camera = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };

            wolve** Wolve=createWolvesFromFile("save_game/Slime1.txt",numWolve,dotWolve);
            slime** Slime=createSlimesFromFile("save_game/Slime1.txt",numSlime,dotSlime);
            std::pair<int,int>* position = createPositionArray(20);

            // Calculate the start time
            Uint32 frameStart;
            int frameTime;

            /////////


            // While application is running
            while( !quit )
            {
                frameStart = SDL_GetTicks();  // Get the start time for the frame

                static bool wait_for_quit = 0;

                // Handle events on queue
                static int renderP = 0; // chưa in pass lần nào;
                while( SDL_PollEvent( &e ) != 0 )
                {
                    wait_for_quit = handleEvent(e, gCharacter, quit);
                }

                // Calculate time step
                float timeStep = stepTimer.getTicks() / 1000.f;

                // Move the character
                if(wait_for_quit == 0){
                     gCharacter.move(tileSet, timeStep);
                    if((frameStart/100)%2)  // cứ 1/10 giây mới lưu bóng 1 lần;
                     {dotCharacter->updatePosition(position);}
                     AiHandle(e,tileSet,position);   // enemy xử lý sự kiện

                }
                gCharacter.attackEnemy(dotWolve, numWolve, 1);
                gCharacter.attackEnemy(dotSlime, numSlime, 1);

                for(int i=0;i<numWolve;i++){
                    if(!Wolve[i]->isDead())
                    Wolve[i]->attackEnemy();
                }

                // Restart step timer
                stepTimer.start();

                gCharacter.setCamera(camera);


                gCharacter.setBlendMode(SDL_BLENDMODE_BLEND);
                SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
                SDL_RenderClear(gRenderer);

                // Render level
                for(int i = 0; i < TOTAL_TILES; ++i)
                {
                    tileSet[i]->render(camera);
                }

                for(int i = 0; i < numWolve; i++){
                    Wolve[i]->render(camera);
                }
                for(int i = 0; i < numSlime; i++){
                    Slime[i]->render(camera);
                }

                gCharacter.render(camera);
                handleRain(frameStart);


                if(renderPass(dotWolve, numWolve)&&renderPass(dotSlime,numSlime)){
                    renderP++;
                    gPassTexture.setAlpha((renderP < 127) ? renderP * 2 : 255);
                }
                if(renderP >= 300){ quit = true; }
                if(wait_for_quit){ gPauseTexture.render(0, 0); }

                // Update screen
                SDL_RenderPresent(gRenderer);

                // FPS
                frameTime = SDL_GetTicks() - frameStart;

                if(frameDelay > frameTime)
                {
                    SDL_Delay(frameDelay - frameTime); // Delay to maintain the FPS target
                }
                if(gCharacter.isDie())quit=true;

            }

            // Free resources and close SDL
            freeSlimes(Slime, dotSlime, numSlime);

            freeWolves(Wolve,dotWolve,numWolve);

            close(tileSet);
        }
    }

    return 0;
}

