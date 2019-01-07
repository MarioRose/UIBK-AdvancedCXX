/*This source code copyrighted by Lazy Foo' Productions (2004-2015)
and may not be redistributed without written permission.*/

// Using SDL, SDL_image, standard IO, and strings

#include <Arrows.h>
#include <Character.h>
#include <HUD.h>
#include <Player.h>
#include <Room.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <Settings.h>
#include <Texture.h>
#include <Tile.h>
#include <Timer.h>
#include <iostream>
#include <stdio.h>
#include <string>

enum class GameStatus {
    NEW,
    PAUSE,
    GAME_OVER
};

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

SDL_Texture *tileTexture;

// pause
bool pause = false;

int showmenu(TTF_Font *font, std::string title, GameStatus status)
{
	SDL_Surface *screen = SDL_GetWindowSurface(gWindow);
	Uint32 time;
	int x, y;

    int NUMMENU = 3;
	const char *labels[NUMMENU] = {title.c_str(), "Start Game", "Exit Game"};
    const char *labels_gameover[NUMMENU] = {title.c_str(), "Exit Game"};
    const char *labels_pause[NUMMENU] = {title.c_str(), "Continue", "Exit Game"};

    if(status == GameStatus::GAME_OVER){
        NUMMENU = 2;
    }

    SDL_Surface *menus[NUMMENU];
	SDL_Surface *background_surface;
	SDL_Texture *textureMenus[NUMMENU];
	SDL_Texture *background_texture;

	background_surface = IMG_Load("../../assets/images/menu.jpg");
	background_texture = SDL_CreateTextureFromSurface(gRenderer, background_surface);

	bool selected[NUMMENU] = {0, 0};
	SDL_Color color[2] = {{240, 0, 0, 100}, {255, 255, 255, 100}};

	for (int i = 0; i < NUMMENU; i++) {
        switch(status){
            case GameStatus::NEW:
                menus[i] = TTF_RenderText_Solid(font, labels[i], color[0]); break;
            case GameStatus::PAUSE:
                menus[i] = TTF_RenderText_Solid(font, labels_pause[i], color[0]); break;
            case GameStatus::GAME_OVER:
                menus[i] = TTF_RenderText_Solid(font, labels_gameover[i], color[0]); break;
        }

		textureMenus[i] = SDL_CreateTextureFromSurface(gRenderer, menus[i]);
	}

	SDL_Rect pos[NUMMENU];
    for (int i = 0; i < NUMMENU; i++) {
		pos[i].x = SCREEN_WIDTH * 0.1;
        switch(i){
		  case 0: pos[i].y = SCREEN_HEIGHT / 5 - menus[0]->clip_rect.h; break;
  		  case 1: pos[i].y = SCREEN_HEIGHT / 2 - menus[0]->clip_rect.h; break;
  		  case 2: pos[i].y = SCREEN_HEIGHT / 2 + menus[0]->clip_rect.h; break;
        }
	}

	SDL_Event event;
	while (1) {
		time = SDL_GetTicks();
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_QUIT:
                for (int i = 0; i < NUMMENU; i++) {
    				SDL_FreeSurface(menus[i]);
                }
				return 1;
			case SDL_MOUSEMOTION:
				x = event.motion.x;
				y = event.motion.y;
				for (int i = 0; i < NUMMENU; i += 1) {
					if (x >= pos[i].x && x <= pos[i].x + pos[i].w && y >= pos[i].y && y <= pos[i].y + pos[i].h) {
						if (!selected[i]) {
							selected[i] = 1;
							SDL_FreeSurface(menus[i]);
                            switch(status){
                                case GameStatus::NEW:
                                    menus[i] = TTF_RenderText_Solid(font, labels[i], color[1]); break;
                                case GameStatus::PAUSE:
                                    menus[i] = TTF_RenderText_Solid(font, labels_pause[i], color[1]); break;
                                case GameStatus::GAME_OVER:
                                    menus[i] = TTF_RenderText_Solid(font, labels_gameover[i], color[1]); break;
                            }
							textureMenus[i] = SDL_CreateTextureFromSurface(gRenderer, menus[i]);
						}
					} else {
						if (selected[i]) {
							selected[i] = 0;
							SDL_FreeSurface(menus[i]);
                            switch(status){
                                case GameStatus::NEW:
                                    menus[i] = TTF_RenderText_Solid(font, labels[i], color[0]); break;
                                case GameStatus::PAUSE:
                                    menus[i] = TTF_RenderText_Solid(font, labels_pause[i], color[0]); break;
                                case GameStatus::GAME_OVER:
                                    menus[i] = TTF_RenderText_Solid(font, labels_gameover[i], color[0]); break;
                            }
							textureMenus[i] = SDL_CreateTextureFromSurface(gRenderer, menus[i]);
						}
					}
				}
				break;
			case SDL_MOUSEBUTTONDOWN:
				x = event.button.x;
				y = event.button.y;
				for (int i = 0; i < NUMMENU; i += 1) {
					if (x >= pos[i].x && x <= pos[i].x + pos[i].w && y >= pos[i].y && y <= pos[i].y + pos[i].h) {
						for (int i = 0; i < NUMMENU; i++) {
						  SDL_FreeSurface(menus[i]);
						}

						return i;
					}
				}
				break;
			}
		}

		for (int i = 0; i < NUMMENU; i++) {
			SDL_BlitSurface(menus[i], NULL, screen, &pos[i]);
		}

		SDL_RenderCopy(gRenderer, background_texture, nullptr, nullptr);

		for (int i = 0; i < NUMMENU; i++) {
			SDL_RenderCopy(gRenderer, textureMenus[i], NULL, &pos[i]);
		}
		SDL_RenderPresent(gRenderer);

		if (1000 / 30 > (SDL_GetTicks() - time))
			SDL_Delay(1000 / 30 - (SDL_GetTicks() - time));
	}
}

bool loadTileTexture(std::string path)
{
	// Load image at specified path
	SDL_Surface *loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == NULL) {
		printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
	} else {
		// Color key image
		SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, 0xFF, 0xFF));

		// Create texture from surface pixels
		tileTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
		if (tileTexture == NULL) {
			printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
		}

		// Get rid of old loaded surface
		SDL_FreeSurface(loadedSurface);
	}
}

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

	if (success) {
		Tile::initCroppedTiles();
		loadTileTexture("../../assets/images/tiles/jungle_tileset.png");
	}

	return success;
}

void close()
{
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

void input(SDL_Event &event)
{

	if (event.type == SDL_KEYDOWN) {
		switch (event.key.keysym.sym) {
		case SDLK_ESCAPE:
			pause = !pause;
			break;
		default:
			break;
		}
	}
}

void initRooms(std::vector<Room *> *rooms)
{
	Room *room = new Room(0, -1, 1, 2, -1);
    room->loadFromFile("../../assets/maps/room01.txt", gRenderer);
	rooms->push_back(room);
	room = new Room(1, 0, -1, -1, -1);
    room->loadFromFile("../../assets/maps/room02.txt", gRenderer);
	rooms->push_back(room);
	room = new Room(2, -1, -1, -1, 0);
    room->loadFromFile("../../assets/maps/room01.txt", gRenderer);
	rooms->push_back(room);
}

int main(int argc, char *args[])
{

	// The Character that will be moving around on the screen
	Player player;
	std::vector<Room *> rooms;
	Room room;
	Room *currentRoom;

	// Start up SDL and create window
	if (!init()) {
		printf("Failed to initialize!\n");
	} else {

		initRooms(&rooms);

		// Head-up display
		HUD hud{gRenderer};

		// Main loop flag
		bool quit = false;

		player.loadFromFile("../../assets/profiles/main.txt", gRenderer);

		currentRoom = rooms.at(0);

		// Event handler
		SDL_Event e;

		// Keep track of the current frame
		int frame = 0;

		// Whether or not to cap the frame rate
		bool cap = true;

		// The frame rate regulator
		Timer fps;

		TTF_Init();
		TTF_Font *font;
		font = TTF_OpenFont("../../assets/fonts/menuFont.ttf", 30);
		if (showmenu(font, "Best Game Ever", GameStatus::NEW) > 1) {
			quit = true;
		}

		// While application is running
		while (!quit) {

			if (pause) {
				if (showmenu(font, "Pause", GameStatus::PAUSE) > 1) {
					break;
				}
				pause = false;
			}

			// Start the frame timer
			fps.start();

			// Handle events on queue
			while (SDL_PollEvent(&e) != 0) {
				// User requests quit
				input(e);

				if (e.type == SDL_QUIT) {
					quit = true;
				}

				if(e.type == SDL_KEYDOWN && e.key.repeat == 0 && e.key.keysym.sym == SDLK_SPACE) {
					currentRoom->arrows.shootArrow(player.getPosX(), player.getPosY(), player.getFlipType(), gRenderer);
				}

				// Handle input for the character
				player.control(e);
			}

			// Move the character
			player.move();

			currentRoom->collisionTiles(&player);
			currentRoom->collisionTilesEnemies();

			if (player.onLeftBorder()) {
				if (currentRoom->roomIndexLeft != -1) {
					currentRoom = rooms.at(currentRoom->roomIndexLeft);
					player.setPosX(SCREEN_WIDTH - player.getWidth() - 7);
				}
			}

			if (player.onRightBorder()) {
				if (currentRoom->roomIndexRight != -1) {
					currentRoom = rooms.at(currentRoom->roomIndexRight);
					player.setPosX(7);
				}
			}

			if (player.onTopBorder()) {
				if (currentRoom->roomIndexAbove != -1) {
					currentRoom = rooms.at(currentRoom->roomIndexAbove);
					player.setPosY(SCREEN_WIDTH - 32);
				}
			}

			if (player.onBottomBorder()) {
				if (currentRoom->roomIndexBelow != -1) {
					currentRoom = rooms.at(currentRoom->roomIndexBelow);
					player.setPosY(5);
				}
			}

			player.collisionDetectionEnemies(currentRoom->enemies);
			if (player.getLifeCount() == 0) {
				std::cout << "You Lost!!!!" << std::endl;
				quit = true;
    			showmenu(font, "Game Over", GameStatus::GAME_OVER);
			}

            player.collisionDetectionSprites(currentRoom->sprites);
			currentRoom->arrows.collisionDetectionEnemies(currentRoom->enemies);

			// Clear screen
			SDL_RenderClear(gRenderer);

			// SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
			SDL_RenderCopy(gRenderer, currentRoom->background_texture, NULL, NULL);

			// Render objects
			currentRoom->moveEnemies(&player);

			player.render(gRenderer);
			currentRoom->renderEnemies(gRenderer);
			currentRoom->renderSprites(gRenderer);
			currentRoom->renderTiles(gRenderer, tileTexture);
			hud.render(gRenderer, &player);
			currentRoom->arrows.render(gRenderer);

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

		// Free resources
		hud.free();
		player.free();
		currentRoom->free();
	}

	close();
	return 0;
}
