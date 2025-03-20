
 // Code được tham khảo từ lazyfoo.net

 #include <SDL.h>
 #include <SDL_image.h>
 #include <SDL_ttf.h>
 #include <stdio.h>
 #include <string>
 #include <fstream>
 #include <sstream>
 #include <iomanip>

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


////////////
 //Texture wrapper class
 class LTexture
 {
 public:
 	//Initializes variables
 	LTexture();

 	//Deallocates memory
 	~LTexture();

 	//Loads image at specified path
 	bool loadFromFile( std::string path );

 	//Loads image into pixel buffer
 	bool loadPixelsFromFile( std::string path );

 	//Creates image from preloaded pixels
 	bool loadFromPixels();

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

 		//Loads initial data
 		bool loadMedia(std::string path1);

 		//Deallocator
 		void free();

 		//Gets current frame data
 		void* getBuffer();

 		int getFrame(){return mFrame;}

 	private:
 		//Internal data
 		SDL_Surface* mImages[24];
 		int mCurrentImage;
 		int mDelayFrames;
 		int mFrame;
 };

 //The application time based timer
 class LTimer
 {
     public:
 		//Initializes variables
 		LTimer();

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
 		static const int DOT_VEL = 5;

 		//Initializes the variables
 		Dot();

 		//Takes key presses and adjusts the dot's velocity
 		void handleEvent( SDL_Event& e );

 		//Moves the dot and check collision against tiles
 		void move( Tile *tiles[], float timeStep );

 		//Centers the camera over the dot
 		void setCamera( SDL_Rect& camera );

 		//Shows the dot on the screen
 		void render( SDL_Rect& camera );

 		// Get x,y
 		int GetX(){return mBox.x;}
 		int GetY(){return mBox.y;}

        // check direction
        bool isRight(){return goRight;}

        bool isWalk (){return walk;}
     private:
 		//Collision box of the dot
 		SDL_Rect mBox;

 		//The velocity of the dot
 		int mVelX, mVelY;

 		bool walk;
 		bool goRight;
 	    bool goLeft;
 };


class Character  // nhân vật là 1 chấm  ; hoạt ảnh là các textrure có luồng datastream
{ public:
    Character();
    bool loadMedia();
    void handleEvent(SDL_Event& e);
    void move(Tile* tiles[], float timeStep);
    void setCamera(SDL_Rect& camera);
    void render(SDL_Rect& camera);


    int GetX() { return dotCharacter.GetX(); }
    int GetY() { return dotCharacter.GetY(); }
    bool isRight() { return dotCharacter.isRight(); }
    bool isWalk() { return dotCharacter.isWalk(); }
    void setBlendMode(SDL_BlendMode blending );
    void free();
    int getFrame(){return mCurrentFrame;}

private:

     Dot dotCharacter;

     //Scene textures
     LTexture mName;
     LTexture gStreamingGo;
     LTexture gStreamingStand;
     //Animation stream
     DataStream gDataStreamGo;
     DataStream gDataStreamStand;
       int mCurrentFrame;
       bool dead;
       int HP;

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

Character gCharacter;
SDL_Color nameColor {0,0,255};

 //Scene textures
 LTexture gDotTexture; //thuận tiện test thay vì dùng hoạt ảnh
 LTexture gTileTexture;
 SDL_Rect gTileClips[ TOTAL_TILE_SPRITES ];


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

 bool LTexture::loadFromFile( std::string path )
 {
 	//Load pixels
 	if( !loadPixelsFromFile( path ) )
 	{
 		printf( "Failed to load pixels for %s!\n", path.c_str() );
 	}
 	else
 	{
 		//Load texture from pixels
 		if( !loadFromPixels() )
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

 bool LTexture::loadFromPixels()
 {
 	//Only load if pixels exist
 	if( mSurfacePixels == NULL )
 	{
 		printf( "No pixels loaded!" );
 	}
 	else
 	{
 		//Color key image
 		SDL_SetColorKey( mSurfacePixels, SDL_TRUE, SDL_MapRGB( mSurfacePixels->format, 0, 0xFF, 0xFF ) );

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
 DataStream::DataStream( int Frame)
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

 	mCurrentImage = 0;
 	mDelayFrames = 1;
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

 void* DataStream::getBuffer()
 {
 	--mDelayFrames;
 	if( mDelayFrames == 0 )
 	{
 		++mCurrentImage;
 		mDelayFrames = 1;
 	}

 	if( mCurrentImage == mFrame )
 	{
 		mCurrentImage = 0;
 	}

 	return mImages[ mCurrentImage ]->pixels;
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
 Dot::Dot()
 {
     //Initialize the collision box
     mBox.x = 50;
     mBox.y = 50;
 	mBox.w = DOT_WIDTH;
 	mBox.h = DOT_HEIGHT;

     //Initialize the velocity
     mVelX = 0;
     mVelY = 0;
    walk=0;
    goRight = 1;
 	goLeft = 0;
 }

 void Dot::handleEvent( SDL_Event& e )
 {
     //If a key was pressed
 	if( e.type == SDL_KEYDOWN && e.key.repeat == 0 )
     {
         //Adjust the velocity
         switch( e.key.keysym.sym )
         {
             case SDLK_UP: mVelY -= DOT_VEL; break;
             case SDLK_DOWN: mVelY += DOT_VEL; break;
             case SDLK_LEFT: mVelX -= DOT_VEL; goRight = 0; goLeft = 1; break;
             case SDLK_RIGHT: mVelX += DOT_VEL; goRight = 1;goLeft = 0; break;
         }
     }
     //If a key was released
     else if( e.type == SDL_KEYUP && e.key.repeat == 0 )
     {
         //Adjust the velocity
         switch( e.key.keysym.sym )
         {
             case SDLK_UP: mVelY += DOT_VEL; break;
             case SDLK_DOWN: mVelY -= DOT_VEL; break;
             case SDLK_LEFT: mVelX += DOT_VEL; break;
             case SDLK_RIGHT: mVelX -= DOT_VEL; break;
         }
     }

     if(mVelX!=0 || mVelY!=0){walk=1;}
     else walk =0;
 }


 void Dot::move( Tile *tiles[], float timeStep )
 {
     //Move the dot left or right
     mBox.x += mVelX * timeStep;

     //If the dot went too far to the left or right or touched a wall
     if( ( mBox.x < 0 ) || ( mBox.x + DOT_WIDTH > LEVEL_WIDTH ) || touchesWall( mBox, tiles ) )
     {
         //move back
         mBox.x -= mVelX* timeStep;
     }

     //Move the dot up or down

     mBox.y += mVelY * timeStep;

     //If the dot went too far up or down or touched a wall
     if( ( mBox.y < 0 ) || ( mBox.y + DOT_HEIGHT > LEVEL_HEIGHT ) || touchesWall( mBox, tiles ) )
     {
         //move back
         mBox.y -= mVelY* timeStep;
     }

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
 }
 //////////////////
 // Character methods
Character::Character()
    : gDataStreamGo(1), gDataStreamStand(18), mCurrentFrame(0), dead(false), HP(100) {}

bool Character::loadMedia() {
    //Loading success flag
 	bool success = true;
 	if( !mName.loadFromRenderedText("Nghia sad thu",nameColor) )
 	{
 		printf( "Failed to create Name texture!\n" );
 		success = false;
 	}

// 	if( !gStreamingGo.createBlank( 80, 80 ) )
// 	{
// 		printf( "Failed to create streamingGo texture!\n" );
// 		success = false;
// 	}
    if( !gStreamingGo.createBlank( 480, 320 ) )
        {
            printf( "Failed to create streamingGo texture!\n" );
            success = false;
        }
 	if( !gStreamingStand.createBlank( 80, 80 ) )
 	{
 		printf( "Failed to create streamingStand texture!\n" );
 		success = false;
 	}

 	//Load data stream
 	if( !gDataStreamGo.loadMedia("image/character/Walk_full_") )
 	{
 		printf( "Unable to load data stream!\n" );
 		success = false;
 	}
 	if( !gDataStreamStand.loadMedia("image/character/Idle/0_Valkyrie_Idle_0") )
 	{
 		printf( "Unable to load data stream!\n" );
 		success = false;
 	}

 	return success;
}

void Character::handleEvent(SDL_Event& e) {
    dotCharacter.handleEvent(e);
}

void Character::move(Tile* tiles[], float timeStep) {
    dotCharacter.move(tiles, timeStep);
}

void Character::setCamera(SDL_Rect& camera) {
    dotCharacter.setCamera(camera);
}

void Character::render(SDL_Rect& camera) {
    mName.render(dotCharacter.GetX() + dotCharacter.DOT_WIDTH / 2 - mName.getWidth() / 2 - camera.x,
                             dotCharacter.GetY() + dotCharacter.DOT_HEIGHT - gStreamingGo.getHeight() + 10 - camera.y);


    // Kiểm tra xem nhân vật có đang di chuyển không
    if (dotCharacter.isWalk()) {
        gStreamingGo.lockTexture();
        gStreamingGo.copyRawPixels32(gDataStreamGo.getBuffer());
        gStreamingGo.unlockTexture();

        // Cập nhật frame hoạt ảnh "go" (di chuyển)
        mCurrentFrame++;
        if (mCurrentFrame >= 6) {  // Giới hạn số frame
            mCurrentFrame = 0;  // Quay lại đầu nếu hết các frame
        }

        // Tính toán vị trí clip cho sprite
        int x = mCurrentFrame % 6;  // Tính x từ mCurrentFrame (số cột)
        int y = mCurrentFrame / 6;  // Tính y từ mCurrentFrame (số dòng)

        // Tạo vùng clip cho sprite, mỗi sprite có kích thước 80x80
        SDL_Rect clip = { x * 80, y * 80, 80, 80 };

        // Vẽ sprite với camera offset
        gStreamingGo.render(
            dotCharacter.GetX() + dotCharacter.DOT_WIDTH / 2 - 40 - camera.x,
            dotCharacter.GetY() + dotCharacter.DOT_HEIGHT - 80 + 10 - camera.y,
            &clip,        // Vùng cắt cho sprite
            0,            // Góc xoay (không xoay)
            NULL,         // Không có điểm xoay
            (!dotCharacter.isRight()) ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE // Flip nếu nhân vật đi sang trái
        );
    }
     else {
        // Nếu không đi, render đứng yên
        gStreamingStand.lockTexture();
        gStreamingStand.copyRawPixels32(gDataStreamStand.getBuffer());
        gStreamingStand.unlockTexture();
        gStreamingStand.render(dotCharacter.GetX() + dotCharacter.DOT_WIDTH / 2 - gStreamingStand.getWidth() / 2 - camera.x,
                               dotCharacter.GetY() + dotCharacter.DOT_HEIGHT - gStreamingStand.getHeight() + 10 - camera.y,
                               nullptr, 0, NULL, (!dotCharacter.isRight()) ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE);
    }

}

void Character::setBlendMode(SDL_BlendMode blending){
    gStreamingGo.setBlendMode(blending);
    gStreamingStand.setBlendMode(blending);
}
void Character::free(){

     //Scene textures
     gStreamingGo.free();
     gStreamingStand.free();
     //Animation stream
     gDataStreamGo.free();
     gDataStreamStand.free();
     dead=0;
     HP=100;
}
 bool init()
 {
 	//Initialization flag
 	bool success = true;

 	//Initialize SDL
 	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
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
	gFont = TTF_OpenFont( "font/lazy.ttf", 12 );
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

 	//Load dot texture
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

 	gCharacter.free();

 	//Destroy window
 	SDL_DestroyRenderer( gRenderer );
 	SDL_DestroyWindow( gWindow );
 	gWindow = NULL;
 	gRenderer = NULL;

 	//Free global font
	TTF_CloseFont( gFont );
	gFont = NULL;

 	//Quit SDL subsystems
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
     std::ifstream map( "map/lazy.map" );

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

 int main( int argc, char* args[] )
 {
 	//Start up SDL and create window
 	if( !init() )
 	{
 		printf( "Failed to initialize!\n" );
 	}
 	else
 	{
 		//The level tiles
 		Tile* tileSet[ TOTAL_TILES ];

 		//Load media
 		if( !loadMedia( tileSet ) )
 		{
 			printf( "Failed to load media!\n" );
 		}
 		else
 		{
 			//Main loop flag
 			bool quit = false;

 			//Event handler
 			SDL_Event e;

 			//Level camera
 			SDL_Rect camera = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };

 			//While application is running
 			while( !quit )
 			{
 				//Handle events on queue
 				while( SDL_PollEvent( &e ) != 0 )
 				{
 					//User requests quit
 					if( e.type == SDL_QUIT )
 					{
 						quit = true;
 					}

 					//Handle input for the dot
 					gCharacter.handleEvent( e );
 				}

 				//Move the character
 				gCharacter.move(tileSet,1);
 				gCharacter.setCamera( camera );

                gCharacter.setBlendMode(SDL_BLENDMODE_BLEND);
 				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
 				SDL_RenderClear( gRenderer );

 				//Render level
 				for( int i = 0; i < TOTAL_TILES; ++i )
 				{
 					tileSet[ i ]->render( camera );
 				}


                gCharacter.render(camera);

 				//Update scree
 				SDL_RenderPresent( gRenderer );
            }

 		//Free resources and close SDL
 		close( tileSet );
        }
    }
 	return 0;
 }
