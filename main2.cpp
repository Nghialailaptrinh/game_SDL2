
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
 #include <utility>
 #include <iostream>

 //Screen dimension constants
 const int SCREEN_WIDTH = 800;
 const int SCREEN_HEIGHT = 500;

 //The dimensions of the level
 const int LEVEL_WIDTH = 1280;
 const int LEVEL_HEIGHT = 960;

 //Tile constants
 const int TILE_WIDTH = 80;
 const int TILE_HEIGHT = 80;
 const int TOTAL_TILES = 192;
 const int TOTAL_TILE_SPRITES = 12;

  // Store the frame rate target
 const int FPS = 60;
 const int frameDelay = 1000 / FPS;  // Delay per frame to reach 40 FPS (in milliseconds)

 //The different tile sprites
 const int TILE_RED = 0;
 const int TILE_GREEN = 1;
 const int TILE_BLUE = 2;
 const int TILE_CENTER = 3;
 const int TILE_TOP = 4;
 const int TILE_TOPRIGHT = 5;
 const int TILE_RIGHT = 6;
 const int TILE_BOTTOMRIGHT = 7;
 const int TILE_BOTTOM = 8;
 const int TILE_BOTTOMLEFT = 9;
 const int TILE_LEFT = 10;
 const int TILE_TOPLEFT = 11;


////////// hàm hỗ trợ mạnh

 void SetColor(SDL_Surface* &surface, Uint8 r, Uint8 g, Uint8 b, Uint8 a, Uint8 r2=0, Uint8 g2=0, Uint8 b2=0, Uint8 a2=0)
{
 if (surface == NULL) {
     printf("Surface is NULL! Cannot set color key.\n");
     return;
 }

 // Kiểm tra định dạng pixel
 if (surface->format->format != SDL_PIXELFORMAT_RGBA8888) {
     printf("Surface format is not RGBA8888! Cannot set color key.\n");
     return;
 }

 // Lấy định dạng pixel của surface
 SDL_PixelFormat* format = surface->format;

 // Tạo màu từ các giá trị RGBA
 Uint32 color1 = SDL_MapRGBA(format, r, g, b, a);
 Uint32 color2;
 if(r2==0 &&g2==0&b2==0&&a2==0)
     {color2 = SDL_MapRGBA(format, r, g, b, 0) ;} // Nếu không có màu để đôit thì ta làm tàng hình màu nền cũ;
 else{color2 = SDL_MapRGBA(format, r2, g2, b2, a2);} // chuyển thành màu mong muốn
 // Lặp qua tất cả các pixel trong surface
 Uint32* pixels = static_cast<Uint32*>(surface->pixels);
 int pixelCount = surface->w * surface->h;

 for (int i = 0; i < pixelCount; ++i) {
     // Nếu pixel trùng với màu cần khóa, đặt alpha về 0 (trong suốt)
     if (pixels[i] == color1) {
         pixels[i] = color2; // Đặt alpha = 0
     }
 }
}

// Hàm thay đổi kích thước của SDL_Surface
 SDL_Surface* resizeSurface(SDL_Surface* original, int newWidth, int newHeight)
{
    // Tạo một surface mới với kích thước mong muốn
    SDL_Surface* resizedSurface = SDL_CreateRGBSurface(
        0, newWidth, newHeight, original->format->BitsPerPixel,
        original->format->Rmask, original->format->Gmask,
        original->format->Bmask, original->format->Amask
    );

    if (resizedSurface == nullptr) {
        std::printf( "Unable to create resized surface! SDL_Error: %s\n " , SDL_GetError() );
        return nullptr;
    }

    // Sao chép dữ liệu từ surface ban đầu vào surface mới với kích thước thay đổi
    if (SDL_BlitScaled(original, NULL, resizedSurface, NULL) != 0) {
        std::printf( "Unable to resize surface! SDL_Error: %s\n ", SDL_GetError());
        SDL_FreeSurface(resizedSurface); // Giải phóng surface nếu gặp lỗi
        return nullptr;
    }

    return resizedSurface;
}

// Hàm thay đổi kích thước texture
bool resizeTexture(SDL_Renderer* renderer, SDL_Texture* texture, int newWidth, int newHeight) {
    if (texture == NULL) {
        printf("Texture is NULL!\n");
        return false;
    }

    // Lấy kích thước hiện tại của texture
    int width, height;
    if (SDL_QueryTexture(texture, NULL, NULL, &width, &height) != 0) {
        printf("Failed to query texture: %s\n", SDL_GetError());
        return false;
    }

    // Tạo một rect để thay đổi kích thước mới
    SDL_Rect destRect = { 0, 0, newWidth, newHeight };

    // Vẽ lại texture với kích thước mới
    if (SDL_RenderCopy(renderer, texture, NULL, &destRect) != 0) {
        printf("Failed to render texture with new size: %s\n", SDL_GetError());
        return false;
    }

    return true;
}

// Hàm kiểm tra khoảng cách giữa hai điểm (x1, y1) và (x2, y2) có nhỏ hơn d không
int checkDistance(float x1, float y1, float x2, float y2, float d)
{
    // Tính khoảng cách giữa hai điểm
    float distance = sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));

    // Kiểm tra nếu khoảng cách nhỏ hơn d
    if (distance < d) {
        return 1;  // Trả về 1 nếu khoảng cách nhỏ hơn d
    } else {
        return 0;  // Trả về 0 nếu khoảng cách không nhỏ hơn d
    }
}

int checkDiRect(float x1, float y1, float x2, float y2) {
    float diffX = x2 - x1;
    float diffY = y2 - y1;

    // So sánh hiệu x và y, chọn hướng dựa trên giá trị tuyệt đối lớn hơn
    if (fabs(diffX) > fabs(diffY)) {
        if (diffX > 0) {
            return 0; // Điểm 2 ở bên phải điểm 1
        } else {
            return 2; // Điểm 2 ở bên trái điểm 1
        }
    } else {
        if (diffY > 0) {
            return 3; // Điểm 2 ở trên điểm 1
        } else {
            return 1; // Điểm 2 ở dưới điểm 1
        }
    }
}

float getDistance(float x1, float y1, float x2, float y2) {
    // Tính khoảng cách theo công thức Euclidean
    return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
}

std::pair<int, int>* createPositionArray(int size = 20) {
   std::pair<int, int>* position = new std::pair<int, int>[size];
    // Khởi tạo các cặp pair với giá trị {50, 50}
    for (int i = 0; i < size; ++i) {
        position[i] = std::make_pair(50, 50);
    }

    return position;  // Trả về con trỏ chứa mảng các pair
}


 //Texture wrapper class
 class LTexture
 {
 public:
 	//Initializes variables
 	LTexture();

 	//Deallocates memory
 	~LTexture();

 	//Loads image at specified path
 	bool loadFromFile( std::string path, Uint8 red=0, Uint8 green=255, Uint8 blue=0 );

 	//Loads image into pixel buffer
 	bool loadPixelsFromFile( std::string path );

 	//Creates image from preloaded pixels
 	bool loadFromPixels(  Uint8 red, Uint8 green, Uint8 blue);

 #if defined(SDL_TTF_MAJOR_VERSION)
 	//Creates image from font string
 	bool loadFromRenderedText( std::string textureText, SDL_Color textColor );
 #endif

 	//Creates blank texture
 	bool createBlank( int width, int height );

 	//Deallocates texture
 	void free();

 	//Set color modulation
 	void setColor( Uint8 red, Uint8 green, Uint8 blue );

 	//Set blending
 	void setBlendMode( SDL_BlendMode blending );

 	//Set alpha modulation
 	void setAlpha( Uint8 alpha );

 	//Renders texture at given point
 	void render( int x, int y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE );

 	//Gets image dimensions
 	int getWidth();
 	int getHeight();

 	//Pixel accessors
 	Uint32* getPixels32();
 	Uint32 getPixel32( Uint32 x, Uint32 y );
 	Uint32 getPitch32();
 	Uint32 mapRGBA( Uint8 r, Uint8 g, Uint8 b, Uint8 a );
 	void copyRawPixels32( void* pixels );
 	bool lockTexture();
 	bool unlockTexture();

 private:
 	//The actual hardware texture
 	SDL_Texture* mTexture;

 	//Surface pixels
 	SDL_Surface* mSurfacePixels;

 	//Raw pixels
 	void* mRawPixels;
 	int mRawPitch;

 	//Image dimensions
 	int mWidth;
 	int mHeight;


 };

 //The tile
 class Tile
 {
     public:
 		//Initializes position and type
 		Tile( int x, int y, int tileType );
 		~Tile(){}

 		//Shows the tile
 		void render( SDL_Rect& camera );

 		//Get the tile type
 		int getType();

 		//Get the collision box
 		SDL_Rect getBox();

     private:
 		//The attributes of the tile
 		SDL_Rect mBox;

 		//The tile type
 		int mType;
 };

 class DataStream
 {
 	public:
 		//Initializes internals
 		DataStream(int Frame);
        ~DataStream(){free();};

 		//Loads initial data
 		bool loadMedia(std::string path1);

 		//Deallocator
 		void free();

 		//Gets current frame data
 		void* getBuffer(int n=0);

 		int getFrame(){return mFrame;}
 		int getCurrentImage(){return mCurrentImage;}
 		int getCurrentFrame(){return (int)mCurrentFrame;}
 		float FgetCurrentFrame(){return mCurrentFrame;}
 		void upCurrentFrame(int n ) {
            // Tăng giá trị mCurrentFrame lên 0.1 và đảm bảo nó quay lại trong phạm vi [0, n)
            mCurrentFrame += 0.1;

            // Nếu mCurrentFrame vượt quá phạm vi, reset lại với phép toán modulo
            mCurrentFrame = fmod(mCurrentFrame, n);
        }

 		void upCurrentImage (){mCurrentImage=(mCurrentImage+1)%mFrame;}

 	private:
 		//Internal data
 		SDL_Surface* mImages[24];
 		int mCurrentImage;
 		float mCurrentFrame;
 		int mDelayFrames;
 		int mFrame;
 };

 //The application time based timer
 class LTimer
 {
     public:
 		//Initializes variables
 		LTimer();
 		~LTimer(){};

 		//The various clock actions
 		void start();
 		void stop();
 		void pause();
 		void unpause();

 		//Gets the timer's time
 		Uint32 getTicks();

 		//Checks the status of the timer
 		bool isStarted();
 		bool isPaused();

     private:
 		//The clock time when the timer started
 		Uint32 mStartTicks;

 		//The ticks stored when the timer was paused
 		Uint32 mPausedTicks;

 		//The timer status
 		bool mPaused;
 		bool mStarted;
 };

 //The dot that will move around on the screen
 class Dot
 {
     public:
 		//The dimensions of the dot
 		static const int DOT_WIDTH = 20;
 		static const int DOT_HEIGHT = 20;

 		//Maximum axis velocity of the dot
        int DOT_VEL = 100;

 		//Initializes the variables
 		Dot(int x=50,int y=50,int typeE=0);
 		~Dot(){};

 		//Takes key presses and adjusts the dot's velocity
 		void handleEvent( SDL_Event& e );
 		void AiHandleEvent( SDL_Event& e,Tile *tiles[],std::pair<int,int>* position=nullptr );

 		//Moves the dot and check collision against tiles
 		void move( Tile *tiles[], float timeStep );

 		bool attackEnemy(Dot* dotEnemy[]=nullptr, int numEnemies=0, int attackRange=0, bool inThisFrame=0);

 		void setVel(int Vel){DOT_VEL = Vel;}

 		//Centers the camera over the dot
 		void setCamera( SDL_Rect& camera );

 		//Shows the dot on the screen
 		void render( SDL_Rect& camera );


 		void SetDie(bool Die){die=Die;}
 		void SetAttack(bool Attack){attack=Attack;} void SetAttacking(bool Attacking){attacking=Attacking;}

 		void updatePosition(std::pair<int, int> position[20]);    // để lại bóng
 		//get HP
 		int getHP (){return mHP;}

 		// Get x,y
 		int GetX(){return mBox.x;}
 		int GetY(){return mBox.y;}
 		int GetDameSword(){return dameSword;}

 		int GetHP(){return mHP;}
 		int GetMaxHP(){return maxHP;}
 		void SetHP(int HP){mHP=HP;}
 		void SetMaxHP(int MaxHP){maxHP=MaxHP;}
 		void SetDameSword (int dame){dameSword=dame;}
 		void SetHurt(bool Hurt){hurt=Hurt;}
 		void SetTimeHurt(int TimeHurt){timeHurt=TimeHurt;}

        // check direction
        bool isRight(){return goRight;}
        bool isLeft(){return goLeft;}
        bool isUp(){return goUp;}
        bool isDown(){return goDown;}

        bool isWalk (){return walk;}
        bool isRun (){return run;}
        bool isDead(){return dead;}
        bool isDie (){return die;}
        bool isAttack(){return attack;} bool isAttacking(){return attacking;}


     private:
 		//Collision box of the dot
 		SDL_Rect mBox;

 		//The velocity of the dot
 		int mVelX, mVelY;
 		int lastDirection ; // Biến lưu hướng di chuyển trước đó (0: right, 1: up, 2: left, 3: down)
 		bool die;
 		bool dead;
        bool run;
 		bool walk;
 		bool goRight;
 		bool goLeft;
 	    bool goUp;
 	    bool goDown;
 	    bool touch;
 	    bool attack; bool attacking;
 	    bool hurt; int timeHurt;
        int mHP;
        int maxHP;
        int dameSword;
        int typeEnemy;



 };

class Character  // nhân vật là 1 chấm  ; hoạt ảnh là các textrure có luồng datastream
{ public:
    Character();
    ~Character() {
        free();
    }
    bool loadMedia();
    void handleEvent(SDL_Event& e);
    bool attackEnemy(Dot* dotEnemy[]=nullptr, int numEnemies=0, int weapon=1);
    void move(Tile* tiles[], float timeStep);
    void setCamera(SDL_Rect& camera);
    void render(SDL_Rect& camera);


    int GetX() { return dotCharacter.GetX(); }
    int GetY() { return dotCharacter.GetY(); }
    bool isRight() { return dotCharacter.isRight(); }
    bool isLeft() { return dotCharacter.isLeft(); }
    bool isUp(){ return dotCharacter.isUp();}
    bool isDown() { return dotCharacter.isDown(); }

    bool isWalk() { return dotCharacter.isWalk(); }
    bool isRun() { return dotCharacter.isRun(); }
    void setBlendMode(SDL_BlendMode blending );
    void free();
    int GetHP(){return dotCharacter.GetHP();};
    bool isDead(){return dotCharacter.isDead();}
    bool isDie(){return dotCharacter.isDie();}
    bool isAttack(){return dotCharacter.isAttack();}
    bool isAttacking(){return dotCharacter.isAttacking();}
    Dot* GetDot(){return &dotCharacter;}
private:

     Dot dotCharacter;

     //Scene textures
     LTexture mName;
     LTexture HP;
     LTexture HPTexture;
     LTexture gStreamingGo;
     LTexture gStreamingStand;
     LTexture gStreamingRun;
     LTexture gStreamingDie;
     LTexture gStreamingAttack;
     //Animation stream
     DataStream gDataStreamGo;
     DataStream gDataStreamStand;
     DataStream gDataStreamRun;
     DataStream gDataStreamDie;
     DataStream gDataStreamAttack;


};

 class slime  // nhân vật là 1 chấm  ; hoạt ảnh là các textrure có luồng datastream
{ public:
    slime(int x=50,int y=50);
    ~slime(){free();}
    bool loadMedia();
    void AiHandleEvent(SDL_Event &e, Tile* tiles[],std::pair<int,int>* position=nullptr );
    void render(SDL_Rect& camera);

    int GetX() { return dotSlime.GetX(); }
    int GetY() { return dotSlime.GetY(); }
    bool isRight() { return dotSlime.isRight(); }
    bool isLeft() { return dotSlime.isLeft(); }
    bool isUp(){ return dotSlime.isUp();}
    bool isDown() { return dotSlime.isDown(); }

    bool isWalk() { return dotSlime.isWalk(); }
    bool isRun() { return dotSlime.isRun(); }
    void setBlendMode(SDL_BlendMode blending );
    void free();
    int GetHP(){return dotSlime.GetHP();};
    bool isDead(){return dotSlime.isDead();}
    bool isDie(){return dotSlime.isDie();}
    Dot* GetDot(){return &dotSlime;}
private:

     Dot dotSlime;
     float mFrame=0;

     //Scene textures
     static LTexture mName;
     LTexture HP;
     static LTexture gStreamingGo;
     static LTexture gStreamingAttack;      // tạm thời chưa có nhưng tiềm năng slime có thể tấn công hoặc tương tự
     static LTexture gStreamingDie;

};

 class wolve  // nhân vật là 1 chấm  ; hoạt ảnh là các textrure có luồng datastream
{ public:
    wolve(int x=50,int y=50);
    ~wolve(){free();}
    bool loadMedia();
    void AiHandleEvent(SDL_Event &e, Tile* tiles[],std::pair<int,int>* position=nullptr );
    void render(SDL_Rect& camera);

    int GetX() { return dotWolve.GetX(); }
    int GetY() { return dotWolve.GetY(); }
    bool isRight() { return dotWolve.isRight(); }
    bool isLeft() { return dotWolve.isLeft(); }
    bool isUp(){ return dotWolve.isUp();}
    bool isDown() { return dotWolve.isDown(); }

    bool isWalk() { return dotWolve.isWalk(); }
    bool isRun() { return dotWolve.isRun(); }
    void setBlendMode(SDL_BlendMode blending );
    void free();
    int GetHP(){return dotWolve.GetHP();};
    bool isDead(){return dotWolve.isDead();}
    bool isDie(){return dotWolve.isDie();}
    bool isAttack(){return dotWolve.isAttack();}
    bool isAttacking(){return dotWolve.isAttacking();}
    Dot* GetDot(){return &dotWolve;}
private:

     Dot dotWolve;
     float mFrame=0;

     //Scene textures
     static LTexture mName;
     LTexture HP;
     static LTexture gStreamingGo;
     static LTexture gStreamingAttack;
     static LTexture gStreamingDie;

};


 //Starts up SDL and creates window
 bool init();

 //Loads media
 bool loadMedia( Tile* tiles[] );

 //Frees media and shuts down SDL
 void close( Tile* tiles[] );

 //Box collision detector
 bool checkCollision( SDL_Rect a, SDL_Rect b );

 //Checks collision box against set of tiles
 bool touchesWall( SDL_Rect box, Tile* tiles[] );

 //Sets tiles from tile map
 bool setTiles( Tile *tiles[] );

 //The window we'll be rendering to
 SDL_Window* gWindow = NULL;

 //The window renderer
 SDL_Renderer* gRenderer = NULL;

 //Globally used font
 TTF_Font* gFont = NULL;

//slime[10]
SDL_Color nameColor {0,0,255};
SDL_Color HPColor {0,100,255};
SDL_Color eHPColor {255,0,0};

 //Scene textures
 LTexture gDotTexture; //thuận tiện test thay vì dùng hoạt ảnh
 LTexture gTileTexture;
 LTexture gPauseTexture; // kết thúc;
 LTexture gPassTexture;

 LTexture gRainTexture; // thời tiết;

 SDL_Rect gTileClips[ TOTAL_TILE_SPRITES ];

 //audio
 Mix_Music *gGo= NULL;
 Mix_Music *gRun = NULL;
 Mix_Chunk *gRain = NULL;
 Mix_Chunk *gSword = NULL;

 //////////////
 Character gCharacter;
 int numWolve;
 Dot** dotWolve;
 int numSlime;
 Dot** dotSlime;

 //////////////
 LTexture::LTexture()
 {
 	//Initialize
 	mTexture = NULL;
 	mWidth = 0;
 	mHeight = 0;

 	mSurfacePixels = NULL;
 	mRawPixels = NULL;
 	mRawPitch = 0;
 }

 LTexture::~LTexture()
 {
 	//Deallocate
 	free();
 }

 bool LTexture::loadFromFile( std::string path, Uint8 red, Uint8 green, Uint8 blue )
 {
 	//Load pixels
 	if( !loadPixelsFromFile( path ) )
 	{
 		printf( "Failed to load pixels for %s!\n", path.c_str() );
 	}
 	else
 	{
 		//Load texture from pixels
 		if( !loadFromPixels(  red, green, blue) )
 		{
 			printf( "Failed to texture from pixels from %s!\n", path.c_str() );
 		}
 	}

 	//Return success
 	return mTexture != NULL;
 }

 bool LTexture::loadPixelsFromFile( std::string path )
 {
 	//Free preexisting assets
 	free();

 	//Load image at specified path
 	SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
 	if( loadedSurface == NULL )
 	{
 		printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
 	}
 	else
 	{
 		//Convert surface to display format
 		mSurfacePixels = SDL_ConvertSurfaceFormat( loadedSurface, SDL_GetWindowPixelFormat( gWindow ), 0 );
 		if( mSurfacePixels == NULL )
 		{
 			printf( "Unable to convert loaded surface to display format! SDL Error: %s\n", SDL_GetError() );
 		}
 		else
 		{
 			//Get image dimensions
 			mWidth = mSurfacePixels->w;
 			mHeight = mSurfacePixels->h;
 		}

 		//Get rid of old loaded surface
 		SDL_FreeSurface( loadedSurface );
 	}

 	return mSurfacePixels != NULL;
 }

 bool LTexture::loadFromPixels( Uint8 red, Uint8 green, Uint8 blue)
 {
 	//Only load if pixels exist
 	if( mSurfacePixels == NULL )
 	{
 		printf( "No pixels loaded!" );
 	}
 	else
 	{
 		//Color key image
 		SDL_SetColorKey( mSurfacePixels, SDL_TRUE, SDL_MapRGB( mSurfacePixels->format, red, green, blue) );

 		//Create texture from surface pixels
 		mTexture = SDL_CreateTextureFromSurface( gRenderer, mSurfacePixels );
 		if( mTexture == NULL )
 		{
 			printf( "Unable to create texture from loaded pixels! SDL Error: %s\n", SDL_GetError() );
 		}
 		else
 		{
 			//Get image dimensions
 			mWidth = mSurfacePixels->w;
 			mHeight = mSurfacePixels->h;
 		}

 		//Get rid of old loaded surface
 		SDL_FreeSurface( mSurfacePixels );
 		mSurfacePixels = NULL;
 	}

 	//Return success
 	return mTexture != NULL;
 }

 #if defined(SDL_TTF_MAJOR_VERSION)
 bool LTexture::loadFromRenderedText( std::string textureText, SDL_Color textColor )
 {
 	//Get rid of preexisting texture
 	free();

 	//Render text surface
 	SDL_Surface* textSurface = TTF_RenderText_Solid( gFont, textureText.c_str(), textColor );
 	if( textSurface != NULL )
 	{
 		//Create texture from surface pixels
 		mTexture = SDL_CreateTextureFromSurface( gRenderer, textSurface );
 		if( mTexture == NULL )
 		{
 			printf( "Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError() );
 		}
 		else
 		{
 			//Get image dimensions
 			mWidth = textSurface->w;
 			mHeight = textSurface->h;
 		}

 		//Get rid of old surface
 		SDL_FreeSurface( textSurface );
 	}
 	else
 	{
 		printf( "Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError() );
 	}


 	//Return success
 	return mTexture != NULL;
 	}
 #endif

 bool LTexture::createBlank( int width, int height )
 {
 	//Get rid of preexisting texture
 	free();

 	//Create uninitialized texture
 	mTexture = SDL_CreateTexture( gRenderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, width, height );
 	if( mTexture == NULL )
 	{
 		printf( "Unable to create streamable blank texture! SDL Error: %s\n", SDL_GetError() );
 	}
 	else
 	{
 		mWidth = width;
 		mHeight = height;
 	}

 	return mTexture != NULL;
 }

 void LTexture::free()
 {
 	//Free texture if it exists
 	if( mTexture != NULL )
 	{
 		SDL_DestroyTexture( mTexture );
 		mTexture = NULL;
 		mWidth = 0;
 		mHeight = 0;
 	}

 	//Free surface if it exists
 	if( mSurfacePixels != NULL )
 	{
 		SDL_FreeSurface( mSurfacePixels );
 		mSurfacePixels = NULL;
 	}
 }

 void LTexture::setColor( Uint8 red, Uint8 green, Uint8 blue )
 {
 	//Modulate texture rgb
 	SDL_SetTextureColorMod( mTexture, red, green, blue );
 }

 void LTexture::setBlendMode( SDL_BlendMode blending )
 {
 	//Set blending function
 	SDL_SetTextureBlendMode( mTexture, blending );
 }

 void LTexture::setAlpha( Uint8 alpha )
 {
 	//Modulate texture alpha
 	SDL_SetTextureAlphaMod( mTexture, alpha );
 }

 void LTexture::render( int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip )
 {
 	//Set rendering space and render to screen
 	SDL_Rect renderQuad = { x, y, mWidth, mHeight };

 	//Set clip rendering dimensions
 	if( clip != NULL )
 	{
 		renderQuad.w = clip->w;
 		renderQuad.h = clip->h;
 	}

 	//Render to screen
 	SDL_RenderCopyEx( gRenderer, mTexture, clip, &renderQuad, angle, center, flip );
 }

 int LTexture::getWidth()
 {
 	return mWidth;
 }

 int LTexture::getHeight()
 {
 	return mHeight;
 }

 Uint32* LTexture::getPixels32()
 {
 	Uint32* pixels = NULL;

 	if( mSurfacePixels != NULL )
 	{
 		pixels =  static_cast<Uint32*>( mSurfacePixels->pixels );
 	}

 	return pixels;
 }

 Uint32 LTexture::getPixel32( Uint32 x, Uint32 y )
 {
 	//Convert the pixels to 32 bit
 	Uint32* pixels = static_cast<Uint32*>( mSurfacePixels->pixels );

 	//Get the pixel requested
 	return pixels[ ( y * getPitch32() ) + x ];
 }

 Uint32 LTexture::getPitch32()
 {
 	Uint32 pitch = 0;

 	if( mSurfacePixels != NULL )
 	{
 		pitch = mSurfacePixels->pitch / 4;
 	}

 	return pitch;
 }

 Uint32 LTexture::mapRGBA( Uint8 r, Uint8 g, Uint8 b, Uint8 a )
 {
 	Uint32 pixel = 0;

 	if( mSurfacePixels != NULL )
 	{
 		pixel = SDL_MapRGBA( mSurfacePixels->format, r, g, b, a );
 	}

 	return pixel;
 }

 bool LTexture::lockTexture()
 {
 	bool success = true;

 	//Texture is already locked
 	if( mRawPixels != NULL )
 	{
 		printf( "Texture is already locked!\n" );
 		success = false;
 	}
 	//Lock texture
 	else
 	{
 		if( SDL_LockTexture( mTexture, NULL, &mRawPixels, &mRawPitch ) != 0 )
 		{
 			printf( "Unable to lock texture! %s\n", SDL_GetError() );
 			success = false;
 		}
 	}

 	return success;
 }

 bool LTexture::unlockTexture()
 {
 	bool success = true;

 	//Texture is not locked
 	if( mRawPixels == NULL )
 	{
 		printf( "Texture is not locked!\n" );
 		success = false;
 	}
 	//Unlock texture
 	else
 	{
 		SDL_UnlockTexture( mTexture );
 		mRawPixels = NULL;
 		mRawPitch = 0;
 	}

 	return success;
 }

 void LTexture::copyRawPixels32( void* pixels )
 {
 	//Texture is locked
 	if( mRawPixels != NULL )
 	{
 		//Copy to locked pixels
 		memcpy( mRawPixels, pixels, mRawPitch * mHeight );
 	}
 }


 //////////////////////////
 Tile::Tile( int x, int y, int tileType )
 {
     //Get the offsets
     mBox.x = x;
     mBox.y = y;

     //Set the collision box
     mBox.w = TILE_WIDTH;
     mBox.h = TILE_HEIGHT;

     //Get the tile type
     mType = tileType;
 }

 void Tile::render( SDL_Rect& camera )
 {
     //If the tile is on screen
     if( checkCollision( camera, mBox ) )
     {
         //Show the tile
         gTileTexture.render( mBox.x - camera.x, mBox.y - camera.y, &gTileClips[ mType ] );
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
 /////////////
 DataStream::DataStream( int Frame)     // ta cài tối đa 24 frame vào 1 luồng
 {  mFrame=Frame;
 	mImages[ 0 ] = NULL;
 	mImages[ 1 ] = NULL;
 	mImages[ 2 ] = NULL;
 	mImages[ 3 ] = NULL;
 	mImages[ 4 ] = NULL;
 	mImages[ 5 ] = NULL;
 	mImages[ 6 ] = NULL;
 	mImages[ 7 ] = NULL;
 	mImages[ 8 ] = NULL;
 	mImages[ 9 ] = NULL;
 	mImages[ 10 ] = NULL;
 	mImages[ 11] = NULL;
 	mImages[ 12 ] = NULL;
 	mImages[ 13 ] = NULL;
 	mImages[ 14 ] = NULL;
 	mImages[ 15 ] = NULL;
 	mImages[ 16 ] = NULL;
 	mImages[ 17 ] = NULL;
 	mImages[ 18 ] = NULL;
 	mImages[ 19 ] = NULL;
 	mImages[ 20 ] = NULL;
 	mImages[ 21 ] = NULL;
 	mImages[ 22 ] = NULL;
 	mImages[ 23 ] = NULL;

    mCurrentFrame = 0;
 	mCurrentImage = 0;
 	mDelayFrames = 4;
 }


 bool DataStream::loadMedia( std:: string path1)
 {
 	bool success = true;

 	for( int i = 0; i < mFrame; ++i )
 	{
 		std::stringstream path;
        path << path1 << std::setw(2) << std::setfill('0') << i << ".png";

 		SDL_Surface* loadedSurface = IMG_Load( path.str().c_str() );
 		if( loadedSurface == NULL )
 		{
 			printf( "Unable to load %s! SDL_image error: %s\n", path.str().c_str(), IMG_GetError() );
 			success = false;
 		}
 		else
 		{
             if(mFrame!=1){loadedSurface=resizeSurface(loadedSurface,80,80);}
             mImages[i] = SDL_ConvertSurfaceFormat(loadedSurface, SDL_PIXELFORMAT_RGBA8888, 0);
             SetColor(mImages[i],255,255,255,0); //khóa màu nền
             // ta cần bật chế độ blen mode của texture sau đó


             //mImages[i] = SDL_ConvertSurface(loadedSurface, loadedSurface->format, 0);
             //SDL_SetColorKey(mImages[i],SDL_TRUE,SDL_MapRGB(mImages[i]->format,255,0,255));
             /////////////// khóa màu không thể thực hiện với surface RGBA. ta tạo khóa màu thủ công
 		}

 		SDL_FreeSurface( loadedSurface );
 	}

 	return success;
 }

 void DataStream::free()
 {
 	for( int i = 0; i < mFrame; ++i )
 	{
 		SDL_FreeSurface( mImages[ i ] );
 		mImages[ i ] = NULL;
 	}
 }

 void* DataStream::getBuffer(int n)
 {
     if(n==0){
 	--mDelayFrames;
 	if( mDelayFrames == 0 )
 	{
 		upCurrentImage();
 		mDelayFrames = 2;
 	}

 	return mImages[ mCurrentImage ]->pixels;}
 	else return mImages[ n-1 ]->pixels;
 }


 ///////////////////////////////////////
 LTimer::LTimer()
 {
     //Initialize the variables
     mStartTicks = 0;
     mPausedTicks = 0;

     mPaused = false;
     mStarted = false;
 }

 void LTimer::start()        // startstick = thời gian đã bỏ; // pausestick = thời gian đã được đếm
 {
     //Start the timer
     mStarted = true;

     //Unpause the timer
     mPaused = false;

     //Get the current clock time
     mStartTicks = SDL_GetTicks();
 	mPausedTicks = 0;
 }

 void LTimer::stop()
 {
     //Stop the timer
     mStarted = false;

     //Unpause the timer
     mPaused = false;

 	//Clear tick variables
 	mStartTicks = 0;
 	mPausedTicks = 0;
 }

 void LTimer::pause()
 {
     //If the timer is running and isn't already paused
     if( mStarted && !mPaused )
     {
         //Pause the timer
         mPaused = true;

         //Calculate the paused ticks
         mPausedTicks = SDL_GetTicks() - mStartTicks;
 		mStartTicks = 0;
     }
 }

 void LTimer::unpause()
 {
     //If the timer is running and paused
     if( mStarted && mPaused )
     {
         //Unpause the timer
         mPaused = false;

         //Reset the starting ticks
         mStartTicks = SDL_GetTicks() - mPausedTicks;

         //Reset the paused ticks
         mPausedTicks = 0;
     }
 }

 Uint32 LTimer::getTicks()
 {
 	//The actual timer time
 	Uint32 time = 0;

     //If the timer is running
     if( mStarted )
     {
         //If the timer is paused
         if( mPaused )
         {
             //Return the number of ticks when the timer was paused
             time = mPausedTicks;
         }
         else
         {
             //Return the current time minus the start time
             time = SDL_GetTicks() - mStartTicks;
         }
     }

     return time;
 }

 bool LTimer::isStarted()
 {
 	//Timer is running and paused or unpaused
     return mStarted;
 }

 bool LTimer::isPaused()
 {
 	//Timer is running and paused
     return mPaused && mStarted;
 }



 //////////////
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
    if(mHP<=0){dead=1;}

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

    if (hurt && (typeEnemy > 1) && getDistance((float)mBox.x, (float)mBox.y, (float)gCharacter.GetX(), (float)gCharacter.GetY()) < 25) {
        attack = 1;
        goUp = goDown = goRight = goLeft = 0;
        lastDirection= checkDiRect((float)mBox.x, (float)mBox.y, (float)gCharacter.GetX(), (float)gCharacter.GetY());
            if (lastDirection == 3) { goDown = 1;  }
            else if (lastDirection == 1) { goUp = 1;  }
            else if (lastDirection == 0) { goRight = 1; }
            else if (lastDirection == 2) { goLeft = 1;  }
    } else {
        attack = 0;
        if (hurt && (typeEnemy > 1)) {
            isAngry = true;
        }

        if (isAngry && (position != nullptr)){
            for (int i = 19; i >=0 ; --i) {
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
            goUp = goDown = goRight = goLeft = 0;
            lastDirection= checkDiRect((float)mBox.x, (float)mBox.y, (float)gCharacter.GetX(), (float)gCharacter.GetY());
            if (lastDirection == 3) { goDown = 1;  }
            else if (lastDirection == 1) { goUp = 1;  }
            else if (lastDirection == 0) { goRight = 1; }
            else if (lastDirection == 2) { goLeft = 1;  }
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
            goUp = goDown = goRight = goLeft = 0;
            if (mVelX > 0) { goRight = 1; lastDirection = 0; }
            else if (mVelX < 0) { goLeft = 1; lastDirection = 2; }

            if (mVelY > 0) { goDown = 1; lastDirection = 3; }
            else if (mVelY < 0) { goUp = 1; lastDirection = 1; }
        }
    }


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

 void Dot::render( SDL_Rect& camera )
 {
     //Show the dot
 	gDotTexture.render( mBox.x - camera.x, mBox.y - camera.y );
 	touch=0;
 }
 //////////////////
 // Character methods
Character::Character()
    : gDataStreamGo(1), gDataStreamStand(1), gDataStreamRun(1), gDataStreamDie(1), gDataStreamAttack(1){
    dotCharacter.SetMaxHP(200);
    dotCharacter.SetHP(dotCharacter.GetMaxHP());
    dotCharacter.SetDameSword(20);
    dotCharacter.setVel(150);
    }

bool Character::loadMedia() {
    //Loading success flag
 	bool success = true;
 	if( !mName.loadFromRenderedText("Nghia sad thu",nameColor) )
 	{
 		printf( "Failed to create Name texture!\n" );
 		success = false;
 	}
 	std::stringstream path;
        path << (GetHP()) << "/" <<dotCharacter.GetMaxHP();
        if( !HP.loadFromRenderedText(path.str(),HPColor) )
 	{
 		printf( "Failed to create HP texture!\n" );
 		success = false;
 	}
 	    if( !HPTexture.loadFromFile("image/character/HP.png") )
 	{
 		printf( "Failed to create HPTexture texture!\n" );
 		success = false;
 	}


    if( !gStreamingGo.createBlank( 480, 320 ) )
        {
            printf( "Failed to create streamingGo texture!\n" );
            success = false;
        }
 	if( !gStreamingStand.createBlank( 960, 320 ) )
 	{
 		printf( "Failed to create streamingStand texture!\n" );
 		success = false;
 	}
 	if( !gStreamingRun.createBlank( 640, 320 ) )
        {
            printf( "Failed to create streamingRun texture!\n" );
            success = false;
        }
    if( !gStreamingDie.createBlank( 560, 320 ) )
    {
        printf( "Failed to create streamingDie texture!\n" );
        success = false;
    }
    if( !gStreamingAttack.createBlank( 640, 320 ) )
    {
        printf( "Failed to create streamingAttack texture!\n" );
        success = false;
    }


 	//Load data stream
 	if( !gDataStreamGo.loadMedia("image/character/Walk_full_") )
 	{
 		printf( "Unable to load data streamGo!\n" );
 		success = false;
 	}
 	else{
       gStreamingGo.lockTexture();
        gStreamingGo.copyRawPixels32(gDataStreamGo.getBuffer());
        gStreamingGo.unlockTexture();
    }

 	if( !gDataStreamStand.loadMedia("image/character/Sword_Idle_full_") )
 	{
 		printf( "Unable to load data streamStand!\n" );
 		success = false;

 	}
 	else{
        gStreamingStand.lockTexture();
        gStreamingStand.copyRawPixels32(gDataStreamStand.getBuffer());
        gStreamingStand.unlockTexture();
 	}

    if( !gDataStreamRun.loadMedia("image/character/Sword_Run_full_") )
    {
        printf( "Unable to load data streamRun!\n" );
        success = false;

    }
    else{
        gStreamingRun.lockTexture();
        gStreamingRun.copyRawPixels32(gDataStreamRun.getBuffer());
        gStreamingRun.unlockTexture();
    }
    if( !gDataStreamDie.loadMedia("image/character/Sword_Death_full_") )
    {
        printf( "Unable to load data streamDie!\n" );
        success = false;

    }
    else{
        gStreamingDie.lockTexture();
        gStreamingDie.copyRawPixels32(gDataStreamDie.getBuffer());
        gStreamingDie.unlockTexture();
    }
    if( !gDataStreamAttack.loadMedia("image/character/Sword_Attack_full_") )
    {
        printf( "Unable to load data streamAttack!\n" );
        success = false;

    }
    else{
        gStreamingAttack.lockTexture();
        gStreamingAttack.copyRawPixels32(gDataStreamAttack.getBuffer());
        gStreamingAttack.unlockTexture();
    }



 	return success;
}

void Character::handleEvent(SDL_Event& e) {
    dotCharacter.handleEvent(e);
}

bool Character::attackEnemy(Dot* dotEnemy[], int numEnemies,  int weapon){
   int attackRange=50;
if(gDataStreamAttack.FgetCurrentFrame()>=2.9&&gDataStreamAttack.FgetCurrentFrame()<=3)Mix_PlayChannel(-1,gSword,0);
return dotCharacter.attackEnemy(dotEnemy,numEnemies,attackRange,gDataStreamAttack.FgetCurrentFrame()>=4.85&&gDataStreamAttack.FgetCurrentFrame()<=5.05);
}


void Character::move(Tile* tiles[], float timeStep) {
    dotCharacter.move(tiles, timeStep);
    std::stringstream path;
        path << (GetHP()) << "/"<<dotCharacter.GetMaxHP();
        HP.free();
        if( !HP.loadFromRenderedText(path.str(),HPColor) )
 	{
 		printf( "Failed to create HP texture!\n" );
 	}

 	 static bool isRunMusicPlaying =false,isWalkMusicPlaying = false;
        if (dotCharacter.isRun() && dotCharacter.isWalk()) {
            // Nếu nhạc không đang phát, hoặc đang phát nhạc đi bộ, ta chuyển sang nhạc chạy
            if (Mix_PlayingMusic() == 0 || (Mix_PlayingMusic() && !isRunMusicPlaying)) {
                Mix_PlayMusic(gRun, -1);  // Phát nhạc chạy liên tục (-1 cho số lần lặp lại)
                isRunMusicPlaying = true; // Đánh dấu nhạc chạy đang phát
                isWalkMusicPlaying = false; // Đánh dấu nhạc đi bộ không còn phát
            }
        }
        // Nếu chỉ đi bộ, phát nhạc đi bộ
        else if (dotCharacter.isWalk()) {
            // Nếu nhạc không đang phát, hoặc đang phát nhạc chạy thì chuyển sang nhạc đi bộ
            if (Mix_PlayingMusic() == 0 || (Mix_PlayingMusic() && isRunMusicPlaying)) {
                Mix_PlayMusic(gGo, -1);  // Phát nhạc đi bộ
                isRunMusicPlaying = false; // Đánh dấu nhạc chạy không còn phát
                isWalkMusicPlaying = true; // Đánh dấu nhạc đi bộ đang phát
            }
        }
        // Nếu không di chuyển, dừng nhạc
        else {
            if (Mix_PlayingMusic() != 0) {
                Mix_HaltMusic();  // Dừng nhạc nếu không di chuyển
                isRunMusicPlaying = false; // Đánh dấu nhạc chạy không còn phát
                isWalkMusicPlaying = false; // Đánh dấu nhạc đi bộ không còn phát
            }
}


}

void Character::setCamera(SDL_Rect& camera) {
    dotCharacter.setCamera(camera);
}

void Character::render(SDL_Rect& camera) {

    if(isAttack()) {dotCharacter.SetAttacking(1);}

    int X=dotCharacter.GetX() + dotCharacter.DOT_WIDTH / 2 - 40 - camera.x;
    int Y=dotCharacter.GetY() + dotCharacter.DOT_HEIGHT - 80 + 10 - camera.y;
    int Frame;

    mName.render(X + 40 - mName.getWidth() / 2  ,
                             Y);
    HP.render(X + 40 - HP.getWidth() / 2 -HPTexture.getWidth()+10,Y+15);


    int hp;
    if(GetHP()<=dotCharacter.GetMaxHP()*1.0/8)hp=0;
    else if(GetHP()<=dotCharacter.GetMaxHP()*2*1.0/8)hp=1;
    else if(GetHP()<=dotCharacter.GetMaxHP()*3*1.0/8) hp=2;
    else if(GetHP()<=dotCharacter.GetMaxHP()*4*1.0/8) hp=3;
    else if(GetHP()<=dotCharacter.GetMaxHP()*5*1.0/8)hp=4;
    else if(GetHP()<=dotCharacter.GetMaxHP()*6*1.0/8)hp=5;
    else if(GetHP()<=dotCharacter.GetMaxHP()*7*1.0/8)hp=6;
    else if(GetHP()<=dotCharacter.GetMaxHP()*8*1.0/8)hp=7;
    SDL_Rect Clip = { 0, hp * 10, 80, 10 };

    HPTexture.render(X + 40 - HPTexture.getWidth() / 2,Y+15,&Clip);


                int i;
                if(dotCharacter.isRight()) i=2;
                else if(dotCharacter.isLeft()) i=1;
                else if(dotCharacter.isUp()) i=3;
                else i=0;


    if(isDead()){

        Frame=16;
        gDataStreamDie.upCurrentFrame(Frame);
        if(gDataStreamDie.FgetCurrentFrame()>=Frame-0.2){dotCharacter.SetDie(1);printf("died \n");}
        // Tính toán vị trí clip cho sprite
        int x = (gDataStreamDie.getCurrentFrame() % Frame)/2;  // Tính x từ mCurrentFrame (số cột)
        int y = gDataStreamDie.getCurrentFrame() / Frame +i;  // Tính y từ mCurrentFrame (số dòng)

        // Tạo vùng clip cho sprite, mỗi sprite có kích thước 80x80
        SDL_Rect clip = { x * 80, y * 80, 80, 80 };


        // Vẽ sprite với camera offset
        gStreamingDie.render(
            X,
            Y+5,
            &clip,        // Vùng cắt cho sprite
            0,            // Góc xoay (không xoay)
            NULL,         // Không có điểm xoay
            SDL_FLIP_NONE // Flip nếu nhân vật đi sang trái
        );

    }
    else if(isAttacking()){


        Frame=8;
        gDataStreamAttack.upCurrentFrame(Frame);
        gDataStreamAttack.upCurrentFrame(Frame);
         if(gDataStreamAttack.FgetCurrentFrame()>=Frame-0.2) {dotCharacter.SetAttacking(isAttack());}
        int x = gDataStreamAttack.getCurrentFrame()%Frame;
        int y = gDataStreamAttack.getCurrentFrame() / Frame +i;

        SDL_Rect clip = { x * 80, y * 80, 80, 80 };

        gStreamingAttack.render(
            X,
            Y+5,
            &clip,
            0,
            NULL,
            SDL_FLIP_NONE
        );
    }

     else if(dotCharacter.isRun() && dotCharacter.isWalk()){




        Frame=8;
        gDataStreamRun.upCurrentFrame(Frame);
        gDataStreamRun.upCurrentFrame(Frame);


        int x = gDataStreamRun.getCurrentFrame()%Frame;
        int y = gDataStreamRun.getCurrentFrame() / Frame +i;
        // Tạo vùng clip cho sprite, mỗi sprite có kích thước 80x80
        SDL_Rect clip = { x * 80, y * 80, 80, 80 };

        // Vẽ sprite với camera offset
        gStreamingRun.render(
            X,
            Y+5,
            &clip,        // Vùng cắt cho sprite
            0,            // Góc xoay (không xoay)
            NULL,         // Không có điểm xoay
            SDL_FLIP_NONE // Flip nếu nhân vật đi sang trái
        );
    }
    else if (dotCharacter.isWalk()) {


        Frame=6;
        gDataStreamGo.upCurrentFrame(Frame);

        int x = gDataStreamGo.getCurrentFrame() %Frame;
        int y = gDataStreamGo.getCurrentFrame() / Frame +i;

        // Tạo vùng clip cho sprite, mỗi sprite có kích thước 80x80
        SDL_Rect clip = { x * 80, y * 80, 80, 80 };


        // Vẽ sprite với camera offset
        gStreamingGo.render(
            X,
            Y+5,
            &clip,        // Vùng cắt cho sprite
            0,            // Góc xoay (không xoay)
            NULL,         // Không có điểm xoay
            SDL_FLIP_NONE // Flip nếu nhân vật đi sang trái
        );
    }

     else {
        // Nếu không đi, render đứng yên

        Frame=((dotCharacter.isUp())? 4:12);
        gDataStreamStand.upCurrentFrame(Frame);

        // Tính toán vị trí clip cho sprite
        int x = gDataStreamStand.getCurrentFrame() %Frame ;  // Tính x từ mCurrentFrame (số cột)
        int y = gDataStreamStand.getCurrentFrame() / Frame +i;  // Tính y từ mCurrentFrame (số dòng)

        // Tạo vùng clip cho sprite, mỗi sprite có kích thước 80x80
        SDL_Rect clip = { x * 80, y * 80, 80, 80 };

        // Vẽ sprite với camera offset
        gStreamingStand.render(
            X,
            Y+5,
            &clip,        // Vùng cắt cho sprite
            0,            // Góc xoay (không xoay)
            NULL,         // Không có điểm xoay
            SDL_FLIP_NONE // Flip nếu nhân vật đi sang trái
        );
     }

}

void Character::setBlendMode(SDL_BlendMode blending){
    HPTexture.setBlendMode(blending);
    gStreamingGo.setBlendMode(blending);
    gStreamingStand.setBlendMode(blending);
    gStreamingRun.setBlendMode(blending);
    gStreamingDie.setBlendMode(blending);
    gStreamingAttack.setBlendMode(blending);
}

void Character::free(){

     //Scene textures
     mName.free();
     HP.free();
     HPTexture.free();
     gStreamingGo.free();
     gStreamingStand.free();
     gStreamingRun.free();
     gStreamingDie.free();
     gStreamingAttack.free();
     //Animation stream
     gDataStreamGo.free();
     gDataStreamStand.free();
     gDataStreamRun.free();
     gDataStreamDie.free();
     gDataStreamAttack.free();

}


/////////////////

slime::slime(int x,int y): dotSlime(x,y,1){dotSlime.setVel(100); }

LTexture slime::mName;
LTexture slime::gStreamingGo;
LTexture slime::gStreamingAttack;
LTexture slime::gStreamingDie;

bool slime::loadMedia(){
    bool success = true;
    if( !mName.loadFromRenderedText("Slime mini",eHPColor) )
 	{
 		printf( "Failed to create Name texture!\n" );
 		success = false;
 	}
 	std::stringstream path;
        path << (GetHP()) << "/100";
        if( !HP.loadFromRenderedText(path.str(),eHPColor) )
 	{
 		printf( "Failed to create HP texture!\n" );
 		success = false;
 	}
 	if( !gStreamingGo.loadFromFile("image/enemy/1/Walk_00.png") )
 	{
 		printf( "Failed to create Name texture!\n" );
 		success = false;
 	}
 	if( !gStreamingAttack.loadFromFile("image/enemy/1/Walk_00.png") )
 	{
 		printf( "Failed to create Name texture!\n" );
 		success = false;
 	}
 	if( !gStreamingDie.loadFromFile("image/enemy/1/Death_00.png") )
 	{
 		printf( "Failed to create Name texture!\n" );
 		success = false;
 	}
 	return success;


}

void slime::AiHandleEvent(SDL_Event &e, Tile* tiles[],std::pair<int,int>* position){
dotSlime.AiHandleEvent(e,tiles,position);
}

void slime::render(SDL_Rect& camera){
    int width=50;
    int X=dotSlime.GetX() + dotSlime.DOT_WIDTH / 2 - width/2 - camera.x;
    int Y=dotSlime.GetY() + dotSlime.DOT_HEIGHT - width  - camera.y +5;
    int Frame;

    if((!isDead())&&checkDistance(dotSlime.GetX(),dotSlime.GetY(),gCharacter.GetDot()->GetX(),gCharacter.GetDot()->GetY(),150)){
        mName.render(X + width/2 - mName.getWidth() / 2 ,
                                 Y-25);

        std::stringstream path;                                         // load lại HP cho Sllime
            path << (GetHP()) << "/"<< dotSlime.GetMaxHP();
            if( !HP.loadFromRenderedText(path.str(),eHPColor) )
        {
            printf( "Failed to create HP texture!\n" );
        }
        HP.render(X + width/2 - HP.getWidth() / 2 ,
                                 Y-15);

    }
//                int i;             // đoạn này tạm không cần thiết do slime ko có hường quay
//                if(dotSlime.isRight()) i=2;
//                else if(dotSlime.isLeft()) i=1;
//                else if(dotSlime.isUp()) i=3;
//                else i=0;



      if(isDie()){}
      else if(isDead()){
        Frame=6;

        if(mFrame>=5.8){dotSlime.SetDie(1);printf("Slime died; %f \n",mFrame);}
        // Tính toán vị trí clip cho sprite
        int x = (int)mFrame;  // Tính x từ mCurrentFrame (số cột)
        int y = 0;  // Tính y từ mCurrentFrame (số dòng)
        mFrame = fmod(double(mFrame + 0.15), double(Frame));

        // Tạo vùng clip cho sprite, mỗi sprite có kích thước 80x80
        SDL_Rect clip = { x * width, y * width, width, width };

        // Vẽ sprite với camera offset
        gStreamingDie.render(
            X,
            Y,
            &clip,        // Vùng cắt cho sprite
            0,            // Góc xoay (không xoay)
            NULL,         // Không có điểm xoay
            SDL_FLIP_NONE // Flip nếu nhân vật đi sang trái
        );

    }

     else {


        Frame=6;
        int x = mFrame;
        int y = 0;
            mFrame =(mFrame+0.1);
            if(mFrame>=Frame)mFrame=mFrame-Frame;
        SDL_Rect clip = { x * width, y * width, width, width };

        gStreamingGo.render(
            X,
            Y,
            &clip,
            0,
            NULL,
            SDL_FLIP_NONE
        );

    }

}

void slime::setBlendMode(SDL_BlendMode blending){
    gStreamingGo.setBlendMode(blending);
    gStreamingAttack.setBlendMode(blending);
    gStreamingDie.setBlendMode(blending);
}

void slime::free(){

     //Scene textures
     mName.free();
     HP.free();
     gStreamingGo.free();
     gStreamingAttack.free();
     gStreamingDie.free();

}

//////////// Wolve class is same Slime

wolve::wolve(int x,int y): dotWolve(x,y,2){dotWolve.setVel(100); dotWolve.SetMaxHP(100);dotWolve.SetDameSword(20); } // tuy sói ko nhiều máu bằng slime nhưng chúng rất nhanh nhẹn

LTexture wolve::mName;
LTexture wolve::gStreamingGo;
LTexture wolve::gStreamingAttack;
LTexture wolve::gStreamingDie;

bool wolve::loadMedia(){
    bool success = true;
 	if( !mName.loadFromRenderedText("wild Wolve",eHPColor) )
 	{
 		printf( "Failed to create Name texture!\n" );
 		success = false;
 	}
 	std::stringstream path;
        path << (GetHP()) << "/"<<dotWolve.GetMaxHP();
        if( !HP.loadFromRenderedText(path.str(),HPColor) )
 	{
 		printf( "Failed to create HP texture!\n" );
 		success = false;
 	}

    if(!gStreamingGo.loadFromFile("image/enemy/2/Walk_00.png"))
    {
 		printf( "Failed to create W walk texture!\n" );
 		success = false;
 	}
    if(!gStreamingDie.loadFromFile("image/enemy/2/Death_00.png"))
    {
 		printf( "Failed to create W death texture!\n" );
 		success = false;
 	}
    if(!gStreamingAttack.loadFromFile("image/enemy/2/Attack_00.png"))
    {
 		printf( "Failed to create W attack texture!\n" );
 		success = false;
 	}

 	return success;
}

void wolve::AiHandleEvent(SDL_Event &e, Tile* tiles[],std::pair<int,int>* position){
dotWolve.AiHandleEvent(e,tiles,position);
}

void wolve::render(SDL_Rect& camera){
    if(isAttack()) {dotWolve.SetAttacking(1);} // kiểm tra xem có vẻ hoạt ảnh tấn công ko; chỉ cần có lệnh attack thì hoath ảnh sẽ đc vẽ đủ
    int width=50;
    int X=dotWolve.GetX() + dotWolve.DOT_WIDTH / 2 - width/2 - camera.x;
    int Y=dotWolve.GetY() + dotWolve.DOT_HEIGHT - width  - camera.y +5;
    int Frame;

    if((!isDead())&&checkDistance(dotWolve.GetX(),dotWolve.GetY(),gCharacter.GetDot()->GetX(),gCharacter.GetDot()->GetY(),150)){
        mName.render(X + width/2 - mName.getWidth() / 2 ,
                                 Y-25);

        std::stringstream path;                                         // load lại HP cho Sllime
            path << (GetHP()) << "/"<<dotWolve.GetMaxHP();
            if( !HP.loadFromRenderedText(path.str(),eHPColor) )
        {
            printf( "Failed to create HP texture!\n" );
        }
        HP.render(X + width/2 - HP.getWidth() / 2 ,
                                 Y-15);

    }
                int i;
                if(dotWolve.isRight()) i=2;
                else if(dotWolve.isLeft()) i=1;
                else if(dotWolve.isUp()) i=3;
                else i=0;



      if(isDie()){}
      else if(isDead()){
        Frame=6;

        if(mFrame>=5.9){dotWolve.SetDie(1);printf("Wolve died; %f \n",mFrame);}

        int x = (int)mFrame;
        int y = i;
        mFrame = fmod(double(mFrame + 0.05), double(Frame));

        SDL_Rect clip = { x * width, y * width, width, width };

        gStreamingDie.render(
            X,
            Y,
            &clip,
            0,
            NULL,
            SDL_FLIP_NONE
        );

    }
    else if(isAttacking()){
        Frame=6;

        if(mFrame>=5.9) {dotWolve.SetAttacking(isAttack());}
        int x = (int)mFrame;
        int y = i;
        mFrame = fmod(double(mFrame + 0.1), double(Frame));

        SDL_Rect clip = { x * width, y * width, width, width };

        gStreamingAttack.render(
            X,
            Y,
            &clip,
            0,
            NULL,
            SDL_FLIP_NONE
        );
    }

    else {


        Frame=6;
        int x = mFrame;
        int y = i;
            mFrame = fmod(double(mFrame + 0.2), double(Frame));
        SDL_Rect clip = { x * width, y * width, width, width };

        gStreamingGo.render(
            X,
            Y,
            &clip,
            0,
            NULL,
            SDL_FLIP_NONE
        );

    }

}

void wolve::setBlendMode(SDL_BlendMode blending){
    gStreamingGo.setBlendMode(blending);
    gStreamingAttack.setBlendMode(blending);
    gStreamingDie.setBlendMode(blending);
}

void wolve::free(){

     //Scene textures
     mName.free();
     HP.free();
     gStreamingGo.free();
     gStreamingAttack.free();
     gStreamingDie.free();

}






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

 	//Load dot texture
 	if( !gDotTexture.loadFromFile( "image/dot.bmp" ) ) // nhiều trường hợp dùng dot để test
 	{
 		printf( "Failed to load dot texture!\n" );
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
 	if( !gTileTexture.loadFromFile( "image/tiles.png" ) )
 	{
 		printf( "Failed to load tile set texture!\n" );
 		success = false;
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
	if( gRun == NULL )
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

 void close( Tile* tiles[] )
 {
 	//Deallocate tiles
 	for( int i = 0; i < TOTAL_TILES; ++i )
 	{
 		 if( tiles[ i ] != NULL )
 		 {
 			delete tiles[ i ];
 			tiles[ i ] = NULL;
 		 }
 	}

 	//Free loaded images
 	gDotTexture.free();
 	gTileTexture.free();
    gPauseTexture.free();
    gPassTexture.free();
    gRainTexture.free();
 	gCharacter.free();

 	//Destroy window
 	SDL_DestroyRenderer( gRenderer );
 	SDL_DestroyWindow( gWindow );
 	gWindow = NULL;
 	gRenderer = NULL;

 	//Free global font
	TTF_CloseFont( gFont );
	gFont = NULL;

	Mix_FreeChunk(gSword);
	gSword=NULL;
    Mix_FreeMusic(gRun);
    Mix_FreeMusic(gGo);
    Mix_FreeChunk(gRain);

    gRun=NULL;
    gGo=NULL;
    gRain=NULL;

 	//Quit SDL subsystems
 	Mix_Quit();
 	TTF_Quit();
 	IMG_Quit();
 	SDL_Quit();
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

 bool setTiles( Tile* tiles[] )
 {
 	//Success flag
 	bool tilesLoaded = true;

     //The tile offsets
     int x = 0, y = 0;

     //Open the map
     std::ifstream map( "map/map2.map" );

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
    return waitingForQuit;
}

 bool renderPass(Dot* dotEnemy[], int numEnemies) {
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

slime** createSlimesFromFile(const std::string& filename, int& numSlime, Dot**& dotSlime) { // tạo mảng slime và mảng dot cảu nó
    std::ifstream file(filename); // Mở file
    if (!file) {
       printf("Failed to open the file!\n");
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
        if(i==0) Slime[i]->loadMedia();
        dotSlime[i] = Slime[i]->GetDot();  // Cập nhật Dot* của slime vào mảng dotSlime
    }

    // Đóng file
    file.close();

    return Slime; // Trả về mảng các con trỏ slime*
}

void freeSlimes(slime** Slime, Dot** dotSlime, int numSlime)  // hàm xóa
{
    for (int i = 0; i < numSlime; ++i) {
         Slime[i]->free();  // Gọi free() để giải phóng tài nguyên của slime
        delete Slime[i];  // Giải phóng bộ nhớ cho đối tượng slime
    }
    delete[] Slime;  // Giải phóng mảng chứa các con trỏ slime

    delete[] dotSlime;  // Giải phóng mảng Dot* sau khi không còn sử dụng
}



wolve** createVolvesFromFile(const std::string& filename, int& numWolve, Dot**& dotWolve) {
    std::ifstream file(filename);
    if (!file) {
       printf("Failed to open the file!\n");
        return nullptr;
    }


    file >> numWolve;

    wolve** Wolve = new wolve*[numWolve];
    dotWolve = new Dot*[numWolve];

    int x, y;
    for (int i = 0; i < numWolve; ++i) {
        file >> x >> y;
        Wolve[i] = new wolve(x, y);
        if(i==0) Wolve[i]->loadMedia(); // ta chỉ cần load ảnh của 1 kẻ địch vì static
        dotWolve[i] = Wolve[i]->GetDot();
    }


    file.close();

    return Wolve;
}

void freeWolves(wolve** Wolve, Dot** dotWolve, int numWolve)
{
    for (int i = 0; i < numWolve; ++i) {
         Wolve[i]->free(); // ta cũng chỉ cần free 1 kẻ địch
        delete Wolve[i];
    }
    delete[] Wolve;

    delete[] dotWolve;
}

// Hàm xử lý mưa
void handleRain(Uint32 frameStart) {
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

            wolve** Wolve=createVolvesFromFile("save_game/Slime1.txt",numWolve,dotWolve);
            slime** Slime=createSlimesFromFile("save_game/Slime1.txt",numSlime,dotSlime);
            std::pair<int,int>* position = createPositionArray(20);

            // Calculate the start time
            Uint32 frameStart;
            int frameTime;

            /////////
             Dot* a= gCharacter.GetDot();

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

                     a->updatePosition(position);
                     AiHandle(e,tileSet,position);   // enemy xử lý sự kiện

                }
                gCharacter.attackEnemy(dotWolve, numWolve, 1);
                gCharacter.attackEnemy(dotSlime, numSlime, 1);
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

            }

            // Free resources and close SDL
            freeSlimes(Slime, dotSlime, numSlime);
            freeWolves(Wolve,dotWolve,numWolve);
            close(tileSet);
        }
    }

    return 0;
}

