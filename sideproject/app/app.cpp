/*This source code copyrighted by Lazy Foo' Productions (2004-2015)
and may not be redistributed without written permission.*/

// Using SDL, SDL_image, standard IO, and strings

#include <Character.h>
#include <Player.h>
#include <Room.h>
#include <HUD.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
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

//pause
bool pause = false;


int showmenu(TTF_Font* font) {

    SDL_Surface* screen = SDL_GetWindowSurface(gWindow);
    Uint32 time;
    int x, y;
    const int NUMMENU = 3;
    const char* labels[NUMMENU] = {"Best Game Ever","Start Game","Exit Game"};
    SDL_Surface* menus[NUMMENU];
    SDL_Surface* background_surface;
    SDL_Texture* textureMenus[NUMMENU];
    SDL_Texture* background_texture;

    background_surface = IMG_Load("../../assets/images/rooms/plx.png");
    background_texture = SDL_CreateTextureFromSurface(gRenderer, background_surface);

    bool selected[NUMMENU] = {0,0};
    SDL_Color color[2] = {{255,255,255,100},{0,0,0,0}};

    for(int i = 0; i < NUMMENU; i++){
        menus[i] = TTF_RenderText_Solid(font,labels[i],color[0]);
        textureMenus[i] = SDL_CreateTextureFromSurface(gRenderer, menus[i]);
    }

    SDL_Rect pos[NUMMENU];
    pos[0].x = SCREEN_WIDTH/2 - menus[0]->clip_rect.w/2;
    pos[0].y = SCREEN_HEIGHT/5 - menus[0]->clip_rect.h;
    pos[1].x = SCREEN_WIDTH/2 - menus[0]->clip_rect.w/2;
    pos[1].y = SCREEN_HEIGHT/2 - menus[0]->clip_rect.h;
    pos[2].x = SCREEN_WIDTH/2 - menus[0]->clip_rect.w/2;
    pos[2].y = SCREEN_HEIGHT/2 + menus[0]->clip_rect.h;

    SDL_Event event;
    while(1) {
        time = SDL_GetTicks();
        while(SDL_PollEvent(&event)) {
            switch(event.type) {
                case SDL_QUIT:
                    SDL_FreeSurface(menus[0]);
                    SDL_FreeSurface(menus[1]);
                    return 1;
                case SDL_MOUSEMOTION:
                    x = event.motion.x;
                    y = event.motion.y;
                    for(int i = 0; i < NUMMENU; i += 1) {
                        if(x>=pos[i].x && x<=pos[i].x+pos[i].w && y>=pos[i].y && y<=pos[i].y+pos[i].h){
                            if(!selected[i]){
                                selected[i] = 1;
                                SDL_FreeSurface(menus[i]);
                                menus[i] = TTF_RenderText_Solid(font,labels[i],color[1]);
                                textureMenus[i] = SDL_CreateTextureFromSurface(gRenderer, menus[i]);
                            }
                        }
                        else {
                            if(selected[i]) {
                                selected[i] = 0;
                                SDL_FreeSurface(menus[i]);
                                menus[i] = TTF_RenderText_Solid(font,labels[i],color[0]);
                                textureMenus[i] = SDL_CreateTextureFromSurface(gRenderer, menus[i]);
                            }
                        }
                    }
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    x = event.button.x;
                    y = event.button.y;
                    for(int i = 0; i < NUMMENU; i += 1) {
                        if(x>=pos[i].x && x<=pos[i].x+pos[i].w && y>=pos[i].y && y<=pos[i].y+pos[i].h)
                        {
                            SDL_FreeSurface(menus[0]);
                            SDL_FreeSurface(menus[1]);
                            SDL_FreeSurface(menus[2]);

                            return i;
                        }
                    }
                    break;
            }
        }

        for(int i = 0; i < NUMMENU; i++) {
            SDL_BlitSurface(menus[i],NULL,screen,&pos[i]);
        }

        SDL_RenderCopy(gRenderer, background_texture, nullptr, nullptr);

        for(int i = 0; i < NUMMENU; i++){
            SDL_RenderCopy(gRenderer, textureMenus[i], NULL, &pos[i]);
        }
        SDL_RenderPresent(gRenderer);

        if(1000/30 > (SDL_GetTicks()-time))
            SDL_Delay(1000/30 - (SDL_GetTicks()-time));
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

void input(SDL_Event &event) {

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

int main(int argc, char *args[])
{

	// The Character that will be moving around on the screen
	Player player;
	Room room;

	// Start up SDL and create window
	if (!init()) {
		printf("Failed to initialize!\n");
	} else {


        //Head-up display
        HUD hud{gRenderer};

		// Main loop flag
		bool quit = false;

		player.loadFromFile("../../assets/profiles/main.txt", gRenderer);
		room.loadFromFile("../../assets/maps/room01.txt", gRenderer);

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
        font = TTF_OpenFont("../../assets/fonts/menuFont.ttf",30);
        if(showmenu(font) > 1){
            quit = true;
        }

		// While application is running
		while (!quit) {

		    while(pause) {
		        //show pause screen
                while (SDL_PollEvent(&e) != 0) {
                    // User requests quit
                    input(e);
                }
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

				// Handle input for the character
				player.control(e);
			}

			// Move the character
			player.move();

			player.collisionDetection(room.enemy);
			if(player.getLifeCount() == 0){
			    std::cout << "You Lost!!!!" << std::endl;
			    quit = true;
			}

			// Clear screen
			SDL_RenderClear(gRenderer);

			// SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
			SDL_RenderCopy(gRenderer, room.background_texture, NULL, NULL);

			// Render objects
            room.enemy.moveAI(&player);
			// std::cout << "spriteNumber: " << spriteNumber << "\n";
			player.render(gRenderer);
			room.renderEnemies(gRenderer);
            hud.render(gRenderer, &player);
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
        room.free();
	}

    close();
	return 0;
}
