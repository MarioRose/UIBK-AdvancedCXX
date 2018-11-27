/*This source code copyrighted by Lazy Foo' Productions (2004-2015)
and may not be redistributed without written permission.*/

// Using SDL, SDL_image, standard IO, and strings

#include <CharacterStatus.h>
#include <Character.h>
#include <iostream>
#include <Room.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <Settings.h>
#include <stdio.h>
#include <string>
#include <Texture.h>
#include <Timer.h>


// Paths to files
const std::string backgroundPath = "images/background/plx-1.png";
const std::string backgroundPath2 = "images/background/plx-2.png";
const std::string backgroundPath3 = "images/background/plx-3.png";
const std::string backgroundPath4 = "images/background/plx-4.png";
const std::string backgroundPath5 = "images/background/plx-5.png";

// Starts up SDL and creates window
bool init();

// Loads media
bool loadMedia();

// Frees media and shuts down SDL
void close();

// The window we'll be rendering to
SDL_Window *gWindow = NULL;

// The window renderer
SDL_Renderer *gRenderer = NULL;

SDL_Surface *background_surface = NULL;
SDL_Texture *background_texture = NULL;

SDL_Surface *background_surface2 = NULL;
SDL_Texture *background_texture2 = NULL;

SDL_Surface *background_surface3 = NULL;
SDL_Texture *background_texture3 = NULL;

SDL_Surface *background_surface4 = NULL;
SDL_Texture *background_texture4 = NULL;

SDL_Surface *background_surface5 = NULL;
SDL_Texture *background_texture5 = NULL;

bool init()
{
	// Initialization flag
	bool success = true;

	// Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
		printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		success = false;
	} else {

		//Initialize SDL_mixer
		int flags = MIX_INIT_OGG|MIX_INIT_MP3;
		int initted = Mix_Init(flags);
		if(initted&flags != flags) {
				printf("Mix_Init: %s\n", Mix_GetError());
				exit(1);
		}
		Mix_Volume(-1, MIX_MAX_VOLUME);

		if(Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0){
			printf("Mix_OpenAudio: %s\n", Mix_GetError());
		}

		// Set texture filtering to linear
		if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
			printf("Warning: Linear texture filtering not enabled!");
		}

		// Create window
		gWindow = SDL_CreateWindow("Uncharted - PC Version", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		                           SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

		if (gWindow == NULL) {
			printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
			success = false;
		} else {
			// Create vsynced renderer for window
			gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
			if (gRenderer == NULL) {
				printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
				success = false;
			} else {
				// Initialize renderer color
				// SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );

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

				// Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if (!(IMG_Init(imgFlags) & imgFlags)) {
					printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
					success = false;
				}
			}
		}
	}

	return success;
}

bool loadMedia(Character *character)
{
	// Loading success flag
	bool success = true;
	character->loadTexture("images/character/idle/1.png", gRenderer);
	std::vector<std::string> pathsIdleTextures;
	for (int i = 1; i <= 12; i++) {
		pathsIdleTextures.push_back("images/character/idle/" + std::to_string(i) + ".png");
	}
	character->loadIdleTextures(pathsIdleTextures, gRenderer);
	//std::cout << "idle Textures" << character->idleTextures.size() << '\n';

	std::vector<std::string> pathsRunningTextures;
	for (int i = 0; i < 8; i++) {
		pathsRunningTextures.push_back("images/character/running/" + std::to_string(i) + ".png");
	}
	character->loadRunningTextures(pathsRunningTextures, gRenderer);
	//std::cout << "idle Textures" << character->runningTextures.size() << '\n';

	return success;
}

void close(Character *character, Room *room)
{
	// Free loaded images
	character->free();

	//Free Room
	room->free();

	// Destroy window
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;
	gRenderer = NULL;

	// Quit SDL subsystems
	IMG_Quit();
	Mix_Quit();
	SDL_Quit();
}

int main(int argc, char *args[])
{

	// The Character that will be moving around on the screen
	Character character;

	Room room;

	// Start up SDL and create window
	if (!init()) {
		printf("Failed to initialize!\n");
	} else {
		// Load media
		if (!loadMedia(&character)) {
			printf("Failed to load media!\n");
		} else {
			// Main loop flag
			bool quit = false;

			// Music
			room.playMusic();

			// Event handler
			SDL_Event e;

			// Keep track of the current frame
			int frame = 0;

			// Whether or not to cap the frame rate
			bool cap = true;

			// The frame rate regulator
			Timer fps;

			int spriteNumber = 1;
			CharacterStatus oldStatus = character.getStatus();
			int numberOfSprites = 11;

			// While application is running
			while (!quit) {
				// Start the frame timer
				fps.start();

				// Handle events on queue
				while (SDL_PollEvent(&e) != 0) {
					// User requests quit
					if (e.type == SDL_QUIT) {
						quit = true;
					}

					// Handle input for the character
					character.control(e);
				}

				// Move the character
				character.move();

				// Clear screen
				SDL_RenderClear(gRenderer);

				// SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
				SDL_RenderCopy(gRenderer, background_texture, NULL, NULL);
				SDL_RenderCopy(gRenderer, background_texture2, NULL, NULL);
				SDL_RenderCopy(gRenderer, background_texture3, NULL, NULL);
				SDL_RenderCopy(gRenderer, background_texture4, NULL, NULL);
				SDL_RenderCopy(gRenderer, background_texture5, NULL, NULL);

				// Render objects
				/*TODO: this Solution is only for testing, i'll find a better one */
				if (frame % 2 == 0) {
					if (oldStatus != character.getStatus()) {
						spriteNumber = 0;
						numberOfSprites == 11 ? numberOfSprites = 7 : numberOfSprites = 11;
					} else {
						if (spriteNumber == numberOfSprites)
							spriteNumber = 0;
						else {
							spriteNumber++;
						}
					}
					oldStatus = character.getStatus();
				}
				//std::cout << "spriteNumber: " << spriteNumber << "\n";
				character.render(spriteNumber, gRenderer);
				//character.render(gRenderer);

				// Update screen
				SDL_RenderPresent(gRenderer);

				// Increment the frame counter
				frame++;

				// If we want to cap the frame rate
				if (cap && (fps.getTicks() < 1000 / FRAMES_PER_SECOND)) {
					// Sleep the remaining frame time
					SDL_Delay((1000 / FRAMES_PER_SECOND) - fps.getTicks());
				}
			}
		}
	}

	// Free resources and close SDL
	close(&character, &room);

	return 0;
}
