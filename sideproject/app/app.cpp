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
#include <fstream>
#include <iostream>
#include <sstream>

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

// RECURSIVELY BUILD MAP
void renderRoom(std::vector<Room*> rooms, int i, int x, int y, int w, int h){

    Room *room = rooms.at(i);
    if(room->isVisited()) {
        SDL_SetRenderDrawColor( gRenderer, room->red, room->green, room->blue, 255 );
    }
    else {
        SDL_SetRenderDrawColor( gRenderer, 180, 180, 180, 255 );
    }
    SDL_Rect r = {x, y, w, h};
    SDL_RenderFillRect( gRenderer, &r );

    if(room->roomIndexRight > 0)
    {
        SDL_SetRenderDrawColor( gRenderer, 255, 255, 255, 255 );
        SDL_Rect r = {x + w, y + h/2, 10, 6};
        SDL_RenderFillRect( gRenderer, &r );
        renderRoom(rooms, room->roomIndexRight, x + w + 10, y, w, h);
    }
    if(room->roomIndexAbove > 0)
    {
        SDL_SetRenderDrawColor( gRenderer, 255, 255, 255, 255 );
        SDL_Rect r = {x + w/2, y + h, 6, 10};
        SDL_RenderFillRect( gRenderer, &r );
        renderRoom(rooms, room->roomIndexAbove, x, y + h + 10, w, h);
    }

}

int showmap(std::vector<Room*> rooms){



    // Clear winow
    SDL_RenderClear( gRenderer );

    renderRoom(rooms, 0, 50, 50, 50, 50);

    // Render the rect to the screen
    SDL_RenderPresent(gRenderer);

    SDL_Event event;
    while (1) {
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_KEYDOWN:
                    return 1;
                    break;
            }

        }
    }

    return 0;

}

int showmenu(TTF_Font *font, std::string title, GameStatus status)
{
	SDL_Surface *screen = SDL_GetWindowSurface(gWindow);
	Uint32 time;
	int x, y;

    int NUMMENU = 4;

	const char *labels[4] = {title.c_str(), "Start Game", "Exit Game"};
    const char *labels_gameover[3] = {title.c_str(), "Show Map", "Exit Game"};
    const char *labels_pause[4] = {title.c_str(), "Continue", "Show Map", "Exit Game"};

    if(status == GameStatus::GAME_OVER || status == GameStatus::NEW){
        NUMMENU = 3;
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
  		  case 1: pos[i].y = SCREEN_HEIGHT * 0.3; break;
  		  case 2: pos[i].y = SCREEN_HEIGHT * 0.4; break;
  		  case 3: pos[i].y = SCREEN_HEIGHT * 0.5; break;
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
	// Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
		printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		return false;
	}

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
		return false;
	}

    // Create vsynced renderer for window
	gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
	if (gRenderer == NULL) {
		printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
		return false;
	}

    // Initialize renderer color
	// SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
	// Initialize PNG loading
	int imgFlags = IMG_INIT_PNG;
	if (!(IMG_Init(imgFlags) & imgFlags)) {
		printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
		return false;
	}

    TTF_Init();

    Tile::initCroppedTiles();
	loadTileTexture("../../assets/images/tiles/jungle_tileset.png");

	return true;
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
    TTF_Quit();
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

void initRooms(std::string path, std::vector<Room*> *rooms)
{

    std::ifstream map(path);

	if (map.is_open()) {

		std::string line;

		while (std::getline(map, line)) {
			std::istringstream iss(line);
			std::string key, value;
			if (!(iss >> key >> value)) {
				continue;
			} // error

            if(key == "MAP")
                continue;

            std::stringstream ss(value);
            std::vector<std::string> result;

            while (ss.good()) {
                std::string substr;
                getline(ss, substr, ',');
                result.push_back(substr);
            }

        	Room *room = new Room(std::stoi(key), std::stoi(result.at(1)), std::stoi(result.at(2)),
                    std::stoi(result.at(3)), std::stoi(result.at(4)));
            room->loadFromFile(result.at(0).c_str(), gRenderer);
        	rooms->push_back(room);
		}

		map.close();

	} else {
		std::cout << "Error loading map " << path << std::endl;
	}
}

int main(int argc, char *args[])
{

	// The Character that will be moving around on the screen
	Player player;
	std::vector<Room*> rooms;
	Room *currentRoom;

	// Start up SDL and create window
	if (!init()) {
		printf("Failed to initialize!\n");
	} else {

		initRooms("../../assets/maps/map01.txt", &rooms);

		// Head-up display
		HUD hud{gRenderer};

		// Main loop flag
		bool quit = false;

		player.loadFromFile("../../assets/profiles/main.txt", gRenderer);

		currentRoom = rooms.at(0);
        currentRoom->enter();

		// Event handler
		SDL_Event e;

		// Keep track of the current frame
		int frame = 0;

		// Whether or not to cap the frame rate
		bool cap = true;

		// The frame rate regulator
		Timer fps;

		TTF_Font *font;
		font = TTF_OpenFont("../../assets/fonts/menuFont.ttf", 30);
		if (showmenu(font, "Best Game Ever", GameStatus::NEW) > 1) {
		  quit = true;
		}
        //quit = true;
		// While application is running
		while (!quit) {
            if (pause) {
                int index = showmenu(font, "Pause", GameStatus::PAUSE);
				if (index > 2) {
					break;
				} else if (index == 2){
                    if(showmap(rooms)){
                        continue;
                    }
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
                    currentRoom->enter();
					player.setPosX(SCREEN_WIDTH - player.getWidth() - 7);
				}
			}

			if (player.onRightBorder()) {
				if (currentRoom->roomIndexRight != -1) {
					currentRoom = rooms.at(currentRoom->roomIndexRight);
                    currentRoom->enter();
					player.setPosX(7);
				}
			}

			if (player.onTopBorder()) {
				if (currentRoom->roomIndexAbove != -1) {
					currentRoom = rooms.at(currentRoom->roomIndexAbove);
                    currentRoom->enter();
					player.setPosY(SCREEN_WIDTH - 32);
				}
			}

			if (player.onBottomBorder()) {
				if (currentRoom->roomIndexBelow != -1) {
					currentRoom = rooms.at(currentRoom->roomIndexBelow);
                    currentRoom->enter();
					player.setPosY(5);
				}
			}

			player.collisionDetectionEnemies(currentRoom->enemies);
			if (player.getLifeCount() == 0) {
				std::cout << "You Lost!!!!" << std::endl;
				quit = true;
    			showmenu(font, "Game Over", GameStatus::GAME_OVER);
			}

            bool collision = player.collisionDetectionSprites(currentRoom->sprites);
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
			hud.render(&player, collision);
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
