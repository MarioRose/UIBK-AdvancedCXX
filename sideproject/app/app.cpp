/*This source code copyrighted by Lazy Foo' Productions (2004-2015)
and may not be redistributed without written permission.*/

//Using SDL, SDL_image, standard IO, and strings
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>

//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

//Paths to files
const std::string backgroundPath = "../../images/background/plx-1.png";
const std::string backgroundPath2 = "../../images/background/plx-2.png";
const std::string backgroundPath3 = "../../images/background/plx-3.png";
const std::string backgroundPath4 = "../../images/background/plx-4.png";
const std::string backgroundPath5 = "../../images/background/plx-5.png";

//The frames per second
const int FRAMES_PER_SECOND = 30;

//External Force (Gravitation)
const float GRAVITATION = 0.3;

//Max Velocity downwards
const int MAX_VELOCITY_DOWN = 5;

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

#ifdef _SDL_TTF_H
	//Creates image from font string
		bool loadFromRenderedText( std::string textureText, SDL_Color textColor );
#endif

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

private:
	//The actual hardware texture
	SDL_Texture* mTexture;

	//Image dimensions
	int mWidth;
	int mHeight;
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

LTimer::LTimer()
{
	//Initialize the variables
	mStartTicks = 0;
	mPausedTicks = 0;

	mPaused = false;
	mStarted = false;
}

void LTimer::start()
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

void LTimer::unpause() {
	//If the timer is running and paused
	if (mStarted && mPaused) {
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

//The character that will move around on the screen
class Character
{
public:
	//The dimensions of the Character
	static const int CHAR_WIDTH = 20;
	static const int CHAR_HEIGHT = 32;

	//Maximum axis velocity of the Character
	static const int CHAR_VEL = 5;

    //Initializes the variables
    Character();

	//Takes key presses and adjusts the Character's velocity
	void handleEvent( SDL_Event& e );

	//Moves the Character
	void move();

	//Lets the Character jump
	void jump();

	//Shows the Character on the screen
	void render();

    //Shows the Character on the screen
    void render(int spriteNumber);

    int getStatus();


private:
	//The X and Y offsets of the Character
	double mPosX, mPosY;

	//The velocity of the Character
	double mVelX, mVelY;

	//The force of the Character
	double mForceY;

    /*TODO: add enum of stati*/
    //Status (e.g. idle or running) the Character
    int status;
};

//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia();

//Frees media and shuts down SDL
void close();

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

SDL_Surface* background_surface = NULL;
SDL_Texture* background_texture = NULL;

SDL_Surface* background_surface2 = NULL;
SDL_Texture* background_texture2 = NULL;

SDL_Surface* background_surface3 = NULL;
SDL_Texture* background_texture3 = NULL;

SDL_Surface* background_surface4 = NULL;
SDL_Texture* background_texture4 = NULL;

SDL_Surface* background_surface5 = NULL;
SDL_Texture* background_texture5 = NULL;


//The window renderer
SDL_Renderer* gRenderer = NULL;

//Scene textures
LTexture gCharacterTexture;

LTexture gIdleCharacterTextures[11];
LTexture gRunningCharacterTextures[8];


LTexture::LTexture()
{
	//Initialize
	mTexture = NULL;
	mWidth = 0;
	mHeight = 0;
}

LTexture::~LTexture()
{
	//Deallocate
	free();
}

bool LTexture::loadFromFile( std::string path )
{
	//Get rid of preexisting texture
	free();

	//The final texture
	SDL_Texture* newTexture = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
	if( loadedSurface == NULL )
	{
		printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
	}
	else
	{
		//Color key image
		SDL_SetColorKey( loadedSurface, SDL_TRUE, SDL_MapRGB( loadedSurface->format, 0, 0xFF, 0xFF ) );

		//Create texture from surface pixels
		newTexture = SDL_CreateTextureFromSurface( gRenderer, loadedSurface );
		if( newTexture == NULL )
		{
			printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
		}
		else
		{
			//Get image dimensions
			mWidth = loadedSurface->w;
			mHeight = loadedSurface->h;
		}

		//Get rid of old loaded surface
		SDL_FreeSurface( loadedSurface );
	}

	//Return success
	mTexture = newTexture;
	return mTexture != NULL;
}

#ifdef _SDL_TTF_H
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


Character::Character()
{
	//Initialize the offsets
	mPosX = 0;
	mPosY = 0;

	//Initialize the velocity
	mVelX = 0;
	mVelY = 0;

	//Initialize the force
	mForceY = 0;

	//Initialize status of character
	status = 0;
}

void Character::handleEvent( SDL_Event& e )
{
	//If a key was pressed
	if( e.type == SDL_KEYDOWN && e.key.repeat == 0 )
	{
		//Adjust the velocity
		switch( e.key.keysym.sym )
		{
			//case SDLK_UP: mVelY -= CHAR_VEL; break;
			case SDLK_UP: jump(); break;
			case SDLK_DOWN: mVelY += CHAR_VEL; break;
			case SDLK_LEFT: mVelX -= CHAR_VEL; break;
			case SDLK_RIGHT: mVelX += CHAR_VEL; break;
			default: break;
		}
		status = 1;
	}
		//If a key was released
	else if( e.type == SDL_KEYUP && e.key.repeat == 0 )
	{
		//Adjust the velocity
		switch( e.key.keysym.sym )
		{
			//case SDLK_UP: mVelY += DOT_VEL; break;
			case SDLK_DOWN: mVelY -= CHAR_VEL; break;
			case SDLK_LEFT: mVelX += CHAR_VEL; break;
			case SDLK_RIGHT: mVelX -= CHAR_VEL; break;
			default: break;
		}
		status = 0;
	}
}

void Character::jump(){
	mForceY = -1;
}

void Character::move()
{
	//Move the character left or right
	mPosX += mVelX;

	//If the character went too far to the left or right
	if( ( mPosX < 0 ) || ( mPosX + CHAR_WIDTH > SCREEN_WIDTH ) )
	{
		//Move back
		mPosX -= mVelX;
	}


	//Jumping of Falling
	if(mForceY != 0 || mPosY != SCREEN_HEIGHT - CHAR_HEIGHT){


		//Calculate Force
		if( mPosY + CHAR_HEIGHT < SCREEN_HEIGHT ){
			mForceY = mForceY + GRAVITATION + ((SCREEN_HEIGHT - CHAR_HEIGHT) - mPosY)/4.5;
		}

		//Calculate acceleration
		float a = mForceY + mVelY;

		//Calculate velocity
		mVelY = mVelY + a;

		//Move the character up or down
		mPosY += mVelY;


		//Maximum Velocity downwards
		if(mVelY >= MAX_VELOCITY_DOWN){
			mVelY = MAX_VELOCITY_DOWN;
		}

		//If the character went too far up or down
		if( ( mPosY < 0 ) || ( mPosY + CHAR_HEIGHT >= SCREEN_HEIGHT ) )
		{
			//Move back
			//mPosY -= mVelY;
			mVelY = 0;
			mPosY = SCREEN_HEIGHT - CHAR_HEIGHT;
		}

		mForceY = 0;
	}
}

int Character::getStatus()
{
    return status;
}


void Character::render()
{
	//Show the Character
    gCharacterTexture.render( mPosX, mPosY );
}

void Character::render(int spriteNumber)
{
    //Show the Character
    switch (status){
        case 1: gRunningCharacterTextures[spriteNumber].render( mPosX, mPosY );
                break;
        default: gIdleCharacterTextures[spriteNumber].render( mPosX, mPosY );
                break;
    }
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
		gWindow = SDL_CreateWindow( "Uncharted - PC Version", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( gWindow == NULL )
		{
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}
		else
		{
			//Create vsynced renderer for window
			gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED );
			if( gRenderer == NULL )
			{
				printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
				success = false;
			}
			else
			{
				//Initialize renderer color
				//SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );

                background_surface = IMG_Load(backgroundPath.c_str());
                background_texture = SDL_CreateTextureFromSurface(gRenderer, background_surface);
				background_surface2 = IMG_Load(backgroundPath2.c_str());
				background_texture2 = SDL_CreateTextureFromSurface(gRenderer, background_surface2);
                background_surface3 = IMG_Load(backgroundPath3.c_str());
                background_texture3 = SDL_CreateTextureFromSurface(gRenderer, background_surface3);
                background_surface4 = IMG_Load(backgroundPath4.c_str());
                background_texture4 = SDL_CreateTextureFromSurface(gRenderer, background_surface4);
                background_surface5 = IMG_Load(backgroundPath5.c_str());
                background_texture5 = SDL_CreateTextureFromSurface(gRenderer, background_surface5);


                //Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if( !( IMG_Init( imgFlags ) & imgFlags ) )
				{
					printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
					success = false;
				}
			}
		}
	}

	return success;
}

bool loadMedia()
{
	//Loading success flag
	bool success = true;

	//Load Character texture
	if( !gCharacterTexture.loadFromFile("../../images/character/idle/1.png" ))
	{
		printf( "Failed to load character texture!\n" );
		success = false;
	}

	for(int i = 1; i <= 12; i++){
        if( !gIdleCharacterTextures[i-1].loadFromFile("../../images/character/idle/" + std::to_string(i) +".png"))
        {
            printf( "Failed to load idle character texture nr: %d!\n", i );
            success = false;
        }
	}

    for(int i = 0; i < 8; i++){
        if( !gRunningCharacterTextures[i].loadFromFile("../../images/character/running/" + std::to_string(i) +".png"))
        {
            printf( "Failed to load running character texture nr: %d!\n", i );
            success = false;
        }
    }

	return success;
}

void close()
{
	//Free loaded images
	gCharacterTexture.free();

	for(int i = 0; i < 11; i++){
        gIdleCharacterTextures[i].free();
    }

    for(int i = 0; i < 8; i++){
        gRunningCharacterTextures[i].free();
    }

	//Destroy window
	SDL_DestroyRenderer( gRenderer );
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
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
		//Load media
		if( !loadMedia() )
		{
			printf( "Failed to load media!\n" );
		}
		else
		{
			//Main loop flag
			bool quit = false;

			//Event handler
			SDL_Event e;

			//The Character that will be moving around on the screen
            Character character;

			//Keep track of the current frame
			int frame = 0;

			//Whether or not to cap the frame rate
			bool cap = true;

			//The frame rate regulator
			LTimer fps;

            int spriteNumber = 1;
            int oldStatus = character.getStatus();
            int numberOfSprites = 11;

			//While application is running
			while( !quit )
			{
				//Start the frame timer
				fps.start();

				//Handle events on queue
				while( SDL_PollEvent( &e ) != 0 )
				{
					//User requests quit
					if( e.type == SDL_QUIT )
					{
						quit = true;
					}

					//Handle input for the character
                    character.handleEvent( e );
				}

				//Move the character
                character.move();

				//Clear screen
				SDL_RenderClear(gRenderer);

				//SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
				SDL_RenderCopy(gRenderer, background_texture, NULL, NULL);
				SDL_RenderCopy(gRenderer, background_texture2, NULL, NULL);
                SDL_RenderCopy(gRenderer, background_texture3, NULL, NULL);
                SDL_RenderCopy(gRenderer, background_texture4, NULL, NULL);
                SDL_RenderCopy(gRenderer, background_texture5, NULL, NULL);


				//Render objects
                /*TODO: this Solution is only for testing, i'll find a better one */
                if(frame % 2 == 0){
                    if(oldStatus != character.getStatus()){
                        spriteNumber = 0;
                        numberOfSprites == 11 ? numberOfSprites = 7 : numberOfSprites = 11;
                    }
                    else{
                        if(spriteNumber == numberOfSprites)
                            spriteNumber = 0;
                        else{
                            spriteNumber++;
                        }
                    }
                    oldStatus = character.getStatus();

                }

                character.render(spriteNumber);

				//Update screen
				SDL_RenderPresent( gRenderer );

				//Increment the frame counter
				frame++;

				//If we want to cap the frame rate
				if( ( cap == true ) && ( fps.getTicks() < 1000 / FRAMES_PER_SECOND ) )
				{
					//Sleep the remaining frame time
					SDL_Delay( ( 1000 / FRAMES_PER_SECOND ) - fps.getTicks() );
				}
			}
		}
	}

	//Free resources and close SDL
	close();

	return 0;
}