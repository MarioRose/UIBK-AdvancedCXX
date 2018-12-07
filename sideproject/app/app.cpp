/*This source code copyrighted by Lazy Foo' Productions (2004-2015)
and may not be redistributed without written permission.*/

// Using SDL, SDL_image, standard IO, and strings

#include <Character.h>
#include <Player.h>
#include <Room.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <Settings.h>
#include <Texture.h>
#include <Timer.h>
#include <iostream>
#include <stdio.h>
#include <string>

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

bool init()
{
	// Initialization flag
	bool success = true;

	// Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
		printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		success = false;
	} else {

		// Initialize SDL_mixer
		if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
			printf("Mix_OpenAudio: %s\n", Mix_GetError());
		}
		Mix_Volume(-1, MIX_MAX_VOLUME);

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

void close(Player *player, Room *room)
{
	// Free loaded images
	player->free();

	// Free Room
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
	Player player;
	Enemy enemy;
	Room room;

	// Start up SDL and create window
	if (!init()) {
		printf("Failed to initialize!\n");
	} else {

		// Main loop flag
		bool quit = false;

		player.loadFromFile("../../assets/profiles/main.txt", gRenderer);
		enemy.loadFromFile("../../assets/profiles/evil.txt", gRenderer);
		room.loadFromFile("../../assets/maps/room01.txt", gRenderer);

		// Event handler
		SDL_Event e;

		// Keep track of the current frame
		int frame = 0;

		// Whether or not to cap the frame rate
		bool cap = true;

		// The frame rate regulator
		Timer fps;

		int spriteNumber = 1;
		CharacterStatus oldStatus = player.getStatus();
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
				player.control(e);
			}

			// Move the character
			player.move();

			// Clear screen
			SDL_RenderClear(gRenderer);

			// SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
			SDL_RenderCopy(gRenderer, room.background_texture, NULL, NULL);

			// Render objects
			/*TODO: this Solution is only for testing, i'll find a better one */
			if (oldStatus != player.getStatus()) {
				spriteNumber = 0;
				if (player.getStatus() == CharacterStatus::IDLE) {
					numberOfSprites = 11;
				} else {
					numberOfSprites = 7;
				}
			} else {
				if (spriteNumber == numberOfSprites)
					spriteNumber = 0;
				else {
					spriteNumber++;
				}
			}
			oldStatus = player.getStatus();

			// std::cout << "spriteNumber: " << spriteNumber << "\n";
			player.render(spriteNumber, gRenderer);
			room.enemy.render(0, gRenderer);
			// character.render(gRenderer);

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

	// Free resources and close SDL
	close(&player, &room);

	return 0;
}
