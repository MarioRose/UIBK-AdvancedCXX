/*This source code copyrighted by Lazy Foo' Productions (2004-2015)
and may not be redistributed without written permission.*/

// Using SDL, SDL_image, standard IO, and strings

#include <Arrows.h>
#include <Character.h>
#include <Fireball.h>
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
#include <Util.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <string>

#include <algorithm>

enum class GameStatus { NEW, PAUSE, GAME_OVER };

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

SDL_Texture *tileTexture[3];

// pause
bool pause = false;

// game over
bool gameOver = false;

std::string mapName;

// RECURSIVELY BUILD MAP
void renderMapRoom(std::vector<Room *> rooms, int i, int current, int x, int y, int w, int h)
{

	Room *room = rooms.at(i);

	if (room->renderedInMap) {
		return;
	}

	room->renderedInMap = true;

	if (room->isVisited()) {
		SDL_SetRenderDrawColor(gRenderer, room->mapColor.red, room->mapColor.green, room->mapColor.blue, 255);
	} else {
		SDL_SetRenderDrawColor(gRenderer, 180, 180, 180, 255);
	}

	SDL_Rect r = {x, SCREEN_HEIGHT - y, w, h};
	SDL_RenderFillRect(gRenderer, &r);

	if (i == current) {
		SDL_SetRenderDrawColor(gRenderer, 0, 20, 250, 255);
		SDL_Rect r = {x + w / 2, SCREEN_HEIGHT - y + h / 2, 8, 8};
		SDL_RenderFillRect(gRenderer, &r);
	}

	if (room->hasSavePoint && room->isVisited()) {
		SDL_SetRenderDrawColor(gRenderer, 250, 20, 0, 255);
		SDL_Rect r = {x + 3 * w / 4, SCREEN_HEIGHT - y + h / 4, 8, 8};
		SDL_RenderFillRect(gRenderer, &r);
	}

	if (room->roomIndexRight >= 0) {
		SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
		SDL_Rect r = {x + w, SCREEN_HEIGHT - y + h / 2, 10, 6};
		SDL_RenderFillRect(gRenderer, &r);
		renderMapRoom(rooms, room->roomIndexRight, current, x + w + 10, y, w, h);
	}

	if (room->roomIndexLeft >= 0) {
		renderMapRoom(rooms, room->roomIndexLeft, current, x - w - 10, y, w, h);
	}

	if (room->roomIndexAbove >= 0) {
		SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
		SDL_Rect r = {x + w / 2, SCREEN_HEIGHT - y - 10, 6, 10};
		SDL_RenderFillRect(gRenderer, &r);
		renderMapRoom(rooms, room->roomIndexAbove, current, x, y + h + 10, w, h);
	}

	if (room->roomIndexBelow >= 0) {
		renderMapRoom(rooms, room->roomIndexBelow, current, x, y - h - 10, w, h);
	}
}

void clearMapRoom(Room *room)
{
	room->renderedInMap = false;
}

int showmap(std::vector<Room *> rooms, int currentRoomIndex)
{
	// Clear winow
	SDL_RenderClear(gRenderer);

	SDL_Surface *background_surface = IMG_Load("assets/images/menu.jpg");
	SDL_Texture *background_texture = SDL_CreateTextureFromSurface(gRenderer, background_surface);
	SDL_FreeSurface(background_surface);

	SDL_RenderCopy(gRenderer, background_texture, nullptr, nullptr);
	std::for_each(rooms.begin(), rooms.end(), &clearMapRoom);
	renderMapRoom(rooms, 0, currentRoomIndex, 50, 100, 50, 50);

	// Render the rect to the screen
	SDL_RenderPresent(gRenderer);

	SDL_Event event;
	while (1) {
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_KEYDOWN:
				SDL_DestroyTexture(background_texture);
				return 1;
				break;
			}
		}
	}

	SDL_DestroyTexture(background_texture);
	return 0;
}

struct inventoryBox {
	int x;
	int y;
	int w = 100;
	int h = 100;
	Texture *texture;
	Texture *itemTexture = NULL;
	bool selected = false;
} inventoryBoxes[5];

void showInventory(TTF_Font *font, Player *player)
{

	// Clear winow
	SDL_RenderClear(gRenderer);

	SDL_Surface *screen = SDL_GetWindowSurface(gWindow);
	Uint32 time;
	int x, y;

	// TODO: Outsource texture loading stuff
	SDL_Surface *background_surface = IMG_Load("assets/images/menu.jpg");
	SDL_Texture *background_texture = SDL_CreateTextureFromSurface(gRenderer, background_surface);
	SDL_FreeSurface(background_surface);

	Texture box_texture;
	box_texture.loadFromFile("assets/images/inventory_box.png", gRenderer);
	box_texture.scaleToHeight(SCREEN_HEIGHT * 0.2);

	Texture box_white_texture;
	box_white_texture.loadFromFile("assets/images/inventory_box_white.png", gRenderer);
	box_white_texture.scaleToHeight(SCREEN_HEIGHT * 0.2);

	Texture bow_texture;
	bow_texture.loadFromFile("assets/images/sprites/bow.png", gRenderer);
	bow_texture.scaleToHeight(SCREEN_HEIGHT * 0.2);

	Texture silver_bow_texture;
	silver_bow_texture.loadFromFile("assets/images/sprites/silverBow.png", gRenderer);
	silver_bow_texture.scaleToHeight(SCREEN_HEIGHT * 0.2);

	Texture golden_bow_texture;
	golden_bow_texture.loadFromFile("assets/images/sprites/goldenBow.png", gRenderer);
	golden_bow_texture.scaleToHeight(SCREEN_HEIGHT * 0.2);

	Texture doubleJump_texture;
	doubleJump_texture.loadFromFile("assets/images/sprites/chest.png", gRenderer);
	doubleJump_texture.scaleToHeight(SCREEN_HEIGHT * 0.2);

	Texture fireItem_texture;
	fireItem_texture.loadFromFile("assets/images/sprites/chest.png", gRenderer);
	fireItem_texture.scaleToHeight(SCREEN_HEIGHT * 0.1);

	// Weapons
	switch (player->getEquippedItem()) {
	case EquippedItem::BOW:
		inventoryBoxes[0].selected = true;
		break;
	case EquippedItem::SILVERBOW:
		inventoryBoxes[1].selected = true;
		break;
	case EquippedItem::GOLDENBOW:
		inventoryBoxes[2].selected = true;
		break;
	default:
		break;
	}

	// Abilities
	switch (player->getEquippedAbility()) {
	case EquippedAbility::JUMP:
		inventoryBoxes[3].selected = true;
		break;
	case EquippedAbility::FIRE:
		inventoryBoxes[4].selected = true;
		break;

	default:
		break;
	}

	// first row of inventory boxes
	for (int i = 0; i < 3; i++) {
		inventoryBoxes[i].x = SCREEN_WIDTH * (i / 3.0) + 50;
		inventoryBoxes[i].y = SCREEN_HEIGHT * 0.27;
		if (inventoryBoxes[i].selected) {
			inventoryBoxes[i].texture = &box_texture;
		} else {
			inventoryBoxes[i].texture = &box_white_texture;
		}
	}

	// second row of inventory boxes
	for (int i = 3; i < 5; i++) {
		inventoryBoxes[i].x = SCREEN_WIDTH * ((i - 3) / 2.0) + 100;
		inventoryBoxes[i].y = SCREEN_HEIGHT * 0.57;
		inventoryBoxes[i].texture = &box_white_texture;
		if (inventoryBoxes[i].selected) {
			inventoryBoxes[i].texture = &box_texture;
		} else {
			inventoryBoxes[i].texture = &box_white_texture;
		}
	}

	// Weapons
	if (player->getHasBow()) {
		inventoryBoxes[0].itemTexture = &bow_texture;
	}
	if (player->getHasSilverBow()) {
		inventoryBoxes[1].itemTexture = &silver_bow_texture;
	}
	if (player->getHasGoldenBow()) {
		inventoryBoxes[2].itemTexture = &golden_bow_texture;
	}

	// Abilities
	if (player->hasDoubleJump()) {
		inventoryBoxes[3].itemTexture = &doubleJump_texture;
	}
	if (player->hasFireItem()) {
		inventoryBoxes[4].itemTexture = &fireItem_texture;
	}

	const int numLabels = 3;

	const char *labels[numLabels] = {"Weapons", "Power Ups", "Return to Game"};

	SDL_Surface *menus[numLabels];
	SDL_Texture *textureMenus[numLabels];

	SDL_Color color[2] = {{240, 0, 0, 100}, {255, 255, 255, 100}};

	for (int i = 0; i < numLabels; i++) {
		menus[i] = TTF_RenderText_Solid(font, labels[i], color[0]);
		textureMenus[i] = SDL_CreateTextureFromSurface(gRenderer, menus[i]);
	}

	SDL_Rect pos[numLabels];
	for (int i = 0; i < numLabels; i++) {
		pos[i].x = SCREEN_WIDTH * 0.4;
	}

	pos[0].y = SCREEN_HEIGHT * 0.2;
	pos[1].y = SCREEN_HEIGHT * 0.5;
	pos[2].y = SCREEN_HEIGHT * 0.9;

	SDL_Event event;
	while (true) {
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_KEYDOWN:
				SDL_DestroyTexture(background_texture);
				return;
			case SDL_MOUSEMOTION:
				x = event.motion.x;
				y = event.motion.y;
				for (int i = 0; i < 5; i++) {
					if (inventoryBoxes[i].selected) {
						continue;
					}
					if (x >= inventoryBoxes[i].x && x <= inventoryBoxes[i].x + inventoryBoxes[i].w &&
					    y >= inventoryBoxes[i].y && y <= inventoryBoxes[i].y + inventoryBoxes[i].h) {
						inventoryBoxes[i].texture = &box_texture;
					} else {
						inventoryBoxes[i].texture = &box_white_texture;
					}
				}
				if (x >= pos[2].x && x <= pos[2].x + pos[2].w && y >= pos[2].y && y <= pos[2].y + pos[2].h) {
					SDL_FreeSurface(menus[2]);
					menus[2] = TTF_RenderText_Solid(font, labels[2], color[1]);
					textureMenus[2] = SDL_CreateTextureFromSurface(gRenderer, menus[2]);
				} else {
					SDL_FreeSurface(menus[2]);
					menus[2] = TTF_RenderText_Solid(font, labels[2], color[0]);
					textureMenus[2] = SDL_CreateTextureFromSurface(gRenderer, menus[2]);
				}

				break;
			default:
				break;
			case SDL_MOUSEBUTTONDOWN:
				x = event.button.x;
				y = event.button.y;
				if (x >= pos[2].x && x <= pos[2].x + pos[2].w && y >= pos[2].y && y <= pos[2].y + pos[2].h) {
					// Equipped Weapon
					for (int i = 0; i < 3; i++) {
						if (inventoryBoxes[i].selected == true) {
							switch (i) {
							case 0:
								player->setEquippedItem(EquippedItem::BOW);
								break;
							case 1:
								player->setEquippedItem(EquippedItem::SILVERBOW);
								break;
							case 2:
								player->setEquippedItem(EquippedItem::GOLDENBOW);
								break;
							}
							break;
						}
					}
					// Equipped Ability
					for (int i = 3; i < 5; i++) {
						if (inventoryBoxes[i].selected == true) {
							switch (i) {
							case 3:
								player->setEquippedAbility(EquippedAbility::JUMP);
								break;
							case 4:
								player->setEquippedAbility(EquippedAbility::FIRE);
								break;
							case 5:
								player->setEquippedAbility(EquippedAbility::NONE);
								break;
							}
							break;
						}
					}
					for (int i = 0; i < numLabels; i++) {
						SDL_FreeSurface(menus[i]);
						// TODO: free rest
					}
					return;
				}
				for (int i = 0; i < 5; i++) {
					if (x >= inventoryBoxes[i].x && x <= inventoryBoxes[i].x + inventoryBoxes[i].w &&
					    y >= inventoryBoxes[i].y && y <= inventoryBoxes[i].y + inventoryBoxes[i].h) {
						if (i < 3) {
							for (int j = 0; j < 3; j++) {
								inventoryBoxes[j].selected = false;
							}
						} else {
							for (int j = 3; j < 5; j++) {
								inventoryBoxes[j].selected = false;
							}
						}
						inventoryBoxes[i].selected = true;
						break;
					}
				}
				break;
			}
		}

		for (int i = 0; i < numLabels; i++) {
			SDL_BlitSurface(menus[i], NULL, screen, &pos[i]);
		}

		SDL_RenderCopy(gRenderer, background_texture, nullptr, nullptr);

		for (int i = 0; i < 5; i++) {
			inventoryBoxes[i].texture->render(inventoryBoxes[i].x, inventoryBoxes[i].y, gRenderer, NULL, 0, NULL,
			                                  SDL_FLIP_NONE);
			if (inventoryBoxes[i].itemTexture != NULL) {
				inventoryBoxes[i].itemTexture->render(inventoryBoxes[i].x + 20, inventoryBoxes[i].y, gRenderer, NULL, 0,
				                                      NULL, SDL_FLIP_NONE);
			}
		}

		for (int i = 0; i < numLabels; i++) {
			SDL_RenderCopy(gRenderer, textureMenus[i], NULL, &pos[i]);
		}
		SDL_RenderPresent(gRenderer);

		if (1000 / 30 > (SDL_GetTicks() - time))
			SDL_Delay(1000 / 30 - (SDL_GetTicks() - time));
	}
}

int showmenu(TTF_Font *font, std::string title, GameStatus status)
{
	SDL_Surface *screen = SDL_GetWindowSurface(gWindow);
	Uint32 time;
	int x, y;
	int NUMMENU = 0;

	switch (status) {
	case GameStatus::GAME_OVER:
		NUMMENU = 4;
		break;
	case GameStatus::NEW:
		NUMMENU = 4;
		break;
	case GameStatus::PAUSE:
		NUMMENU = 6;
		break;
	default:
		NUMMENU = 0;
		break;
	}

	const char *labels[4] = {title.c_str(), "Start Game", "Load Game", "Exit Game"};
	const char *labels_gameover[4] = {title.c_str(), "New Game", "Load Game", "Exit Game"};
	const char *labels_pause[6] = {title.c_str(), "Continue", "Inventory", "Show Map", "Save Game", "Exit Game"};

	SDL_Surface *menus[NUMMENU];
	SDL_Surface *background_surface;
	SDL_Texture *textureMenus[NUMMENU];
	SDL_Texture *background_texture;

	background_surface = IMG_Load("assets/images/menu.jpg");
	background_texture = SDL_CreateTextureFromSurface(gRenderer, background_surface);

	bool selected[NUMMENU] = {0, 0};
	SDL_Color color[2] = {{240, 0, 0, 100}, {255, 255, 255, 100}};

	for (int i = 0; i < NUMMENU; i++) {
		switch (status) {
		case GameStatus::NEW:
			menus[i] = TTF_RenderText_Solid(font, labels[i], color[0]);
			break;
		case GameStatus::PAUSE:
			menus[i] = TTF_RenderText_Solid(font, labels_pause[i], color[0]);
			break;
		case GameStatus::GAME_OVER:
			menus[i] = TTF_RenderText_Solid(font, labels_gameover[i], color[0]);
			break;
		}

		textureMenus[i] = SDL_CreateTextureFromSurface(gRenderer, menus[i]);
	}

	SDL_Rect pos[NUMMENU];
	for (int i = 0; i < NUMMENU; i++) {
		pos[i].x = SCREEN_WIDTH * 0.1;
		if (i < 1) {
			pos[i].y = SCREEN_HEIGHT * 0.1;
		} else {
			pos[i].y = SCREEN_HEIGHT * (0.3 + (i - 1) * 0.1);
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
						if (i == 0)
							continue;
						if (!selected[i]) {
							selected[i] = 1;
							SDL_FreeSurface(menus[i]);
							switch (status) {
							case GameStatus::NEW:
								menus[i] = TTF_RenderText_Solid(font, labels[i], color[1]);
								break;
							case GameStatus::PAUSE:
								menus[i] = TTF_RenderText_Solid(font, labels_pause[i], color[1]);
								break;
							case GameStatus::GAME_OVER:
								menus[i] = TTF_RenderText_Solid(font, labels_gameover[i], color[1]);
								break;
							}
							textureMenus[i] = SDL_CreateTextureFromSurface(gRenderer, menus[i]);
						}
					} else {
						if (selected[i]) {
							selected[i] = 0;
							SDL_FreeSurface(menus[i]);
							switch (status) {
							case GameStatus::NEW:
								menus[i] = TTF_RenderText_Solid(font, labels[i], color[0]);
								break;
							case GameStatus::PAUSE:
								menus[i] = TTF_RenderText_Solid(font, labels_pause[i], color[0]);
								break;
							case GameStatus::GAME_OVER:
								menus[i] = TTF_RenderText_Solid(font, labels_gameover[i], color[0]);
								break;
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

void loadTileTexture(int index, std::string path)
{
	// Load image at specified path
	SDL_Surface *loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == NULL) {
		printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
	} else {
		// Color key image
		SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, 0xFF, 0xFF));

		// Create texture from surface pixels
		tileTexture[index] = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
		if (tileTexture[index] == NULL) {
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
	gWindow = SDL_CreateWindow("Uncharted - PC Version", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH,
	                           SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

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
	loadTileTexture(0, "assets/images/tiles/jungle_tileset.png");
	loadTileTexture(1, "assets/images/tiles/mountainTile.png");
	loadTileTexture(2, "assets/images/tiles/hell_tileset.png");

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

void initRooms(std::string path, std::vector<Room *> &rooms)
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

			if (key == "MAP") {
				mapName = value;
				continue;
			}

			std::vector<std::string> result;
			result = util::getValues(value);

			Room *room = new Room(std::stoi(key), std::stoi(result.at(1)), std::stoi(result.at(2)),
			                      std::stoi(result.at(3)), std::stoi(result.at(4)), std::stoi(result.at(5)));
			room->loadFromFile(result.at(0), gRenderer);
			rooms.push_back(room);
		}

		map.close();

	} else {
		std::cout << "Error loading map " << path << std::endl;
	}
}

void saveGame(Player &player, std::vector<Room *> &rooms, int currentRoomIndex, std::string mapPath)
{
	std::cout << "Saving ..." << '\n';
	std::ofstream file("assets/games/01.txt", std::ios::out);
	// TODO Generate unique filename
	file << "MAP " << mapPath << '\n';
	file << "POINTS " << player.getPoints() << '\n';
	file << "HEALTH " << player.getHealth() << '\n';
	file << "POS " << player.getPosX() << "," << player.getPosY() << '\n';
	file << "CURRENT_ROOM " << currentRoomIndex << '\n';
	file << "HAS_BOW " << player.getHasBow() << '\n';
	file << "HAS_SILVERBOW " << player.getHasSilverBow() << '\n';
	file << "HAS_GOLDENBOW " << player.getHasGoldenBow() << '\n';
	file << "HAS_DOUBLEJUMP " << player.hasDoubleJump() << '\n';
	file << "HAS_FIREITEM " << player.hasFireItem() << '\n';
	file << "SAVE_POINT " << player.lastSavePoint.x << "," << player.lastSavePoint.y << ","
	     << player.lastSavePoint.roomIndex << '\n';

	int i = 0;
	for (auto &room : rooms) {
		if (room->isVisited()) {
			file << "ROOM " << i++ << '\n';
			for (auto &enemy : room->enemies) {
				file << "ENEMY " << enemy->isAlive() << '\n';
			}

			for (auto &sprite : room->sprites) {
				file << "SPRITE " << sprite->visible << '\n';
			}
		}
	}
	file.flush();
	file.close();
}

int loadGame(Player &player, std::vector<Room *> &rooms, std::string &mapPath)
{
	std::cout << "Loading ..." << '\n';
	std::ifstream map("assets/games/01.txt");

	int currentRoomIndex = -1;

	if (map.is_open()) {

		std::string line;

		int enemyCounter = 0;
		int spriteCounter = 0;
		Room *tmpRoom;

		while (std::getline(map, line)) {
			std::istringstream iss(line);
			std::string key, value;
			if (!(iss >> key >> value)) {
				continue;
			}

			if (key == "#") {
				continue;
			} else if (key == "MAP") {
				mapPath = value;
				initRooms(value, rooms);
			} else if (key == "POS") {
				std::vector<std::string> coords;
				coords = util::getValues(value);
				player.setPosX(std::stoi(coords.at(0)));
				player.setPosY(std::stoi(coords.at(1)) - 10);
			} else if (key == "POINTS") {
				player.setPoints(std::stoi(value));
			} else if (key == "HEALTH") {
				player.setHealth(std::stoi(value));
			} else if (key == "HAS_BOW") {
				player.setHasBow(std::stoi(value));
			} else if (key == "HAS_SILVER_BOW") {
				player.setHasSilverBow(std::stoi(value));
			} else if (key == "HAS_GOLDEN_BOW") {
				player.setHasGoldenBow(std::stoi(value));
			} else if (key == "HAS_DOUBLEJUMP") {
				player.setDoubleJumpItem(std::stoi(value));
			} else if (key == "HAS_FIREITEM") {
				player.setFireItem(std::stoi(value));
			} else if (key == "CURRENT_ROOM") {
				currentRoomIndex = std::stoi(value);
			} else if (key == "SAVE_POINT") {
				std::vector<std::string> res;
				res = util::getValues(value);
				player.setLastSavePoint(std::stoi(res.at(0)), std::stoi(res.at(1)), std::stoi(res.at(2)));
			} else if (key == "ROOM") {
				tmpRoom = rooms.at(std::stoi(value));
				spriteCounter = 0;
				enemyCounter = 0;
			} else if (key == "ENEMY") {
				if (std::stoi(value))
					tmpRoom->enemies.at(enemyCounter)->setDeath();
				enemyCounter++;
			} else if (key == "SPRITE") {
				tmpRoom->sprites.at(spriteCounter)->visible = std::stoi(value);
				spriteCounter++;
			}
		}

		map.close();
	}

	return currentRoomIndex;
}

int main(int argc, char *args[])
{

	// The Character that will be moving around on the screen
	Player player;
	std::vector<Room *> rooms;
	Room *currentRoom;
	std::string mapPath = "assets/maps/map03.txt";

	// Start up SDL and create window
	if (!init()) {
		printf("Failed to initialize!\n");
	} else {

		// Head-up display
		HUD hud{gRenderer};

		// Main loop flag
		bool quit = false;

		bool newGame = false;

		player.loadFromFile("assets/profiles/main.txt", gRenderer);

		// Event handler
		SDL_Event e;

		// Keep track of the current frame
		int frame = 0;

		// Whether or not to cap the frame rate
		bool cap = true;

		// The frame rate regulator
		Timer fps;

		bool collision;

		TTF_Font *font;
		font = TTF_OpenFont("assets/fonts/menuFont.ttf", 30);
		int index = showmenu(font, "Best Game Ever", GameStatus::NEW);
		if (index > 3) {
			quit = true;
		} else if (index == 1) {
			initRooms(mapPath, rooms);
			currentRoom = rooms.at(0);
			currentRoom->enter();
		} else if (index == 2) {
			int currentRoomIndex = loadGame(player, rooms, mapPath);
			currentRoom = rooms.at(currentRoomIndex);
			currentRoom->enter();
		}

		// quit = true;
		// While application is running

		while (!quit) {

			if (pause) {
				int index = showmenu(font, "Pause", GameStatus::PAUSE);
				if (index > 4) {
					break;
				} else if (index == 2) {
					showInventory(font, &player);
				} else if (index == 3) {
					showmap(rooms, currentRoom->getIndex());
				} else if (index == 4) {
					saveGame(player, rooms, currentRoom->getIndex(), mapPath);
				}
				pause = false;
			}
			if (gameOver) {
				int index = showmenu(font, "Game Over", GameStatus::GAME_OVER);
				if (index > 3) {
					break;
				} else if (index == 1) {
					for (auto room : rooms) {
						room->resetRoom();
					}
					player.resetPlayer();
					currentRoom = rooms.at(0);
					currentRoom->enter();
					// Um Hud zu updaten
					newGame = true;
				} else if (index == 2) {
					int currentRoomIndex = loadGame(player, rooms, mapPath);
					currentRoom = rooms.at(currentRoomIndex);
					currentRoom->enter();
				} else if (index == 3) {
					break;
				}
				gameOver = false;
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

				if (e.type == SDL_KEYDOWN && e.key.repeat == 0 && e.key.keysym.sym == SDLK_SPACE) {
					if (currentRoom->fireball.getState() != FireballState::CHANNELING) {
						switch (player.getEquippedItem()) {
						case EquippedItem::BOW:
							currentRoom->arrows.shootArrow(player.getPosX(), player.getPosY(), player.getFlipType(), 3,
							                               6, 1);
							break;
						case EquippedItem::SILVERBOW:
							currentRoom->arrows.shootArrow(player.getPosX(), player.getPosY(), player.getFlipType(), 1,
							                               10, 3);
							break;
						case EquippedItem::GOLDENBOW:
							currentRoom->arrows.shootArrow(player.getPosX(), player.getPosY(), player.getFlipType(), 3,
							                               8, 3);
							break;
						case EquippedItem::NONE:
							break;
						}
					}
				}

				if (e.type == SDL_KEYDOWN && e.key.repeat == 0 && e.key.keysym.sym == SDLK_v) {
					if (currentRoom->fireball.getState() == FireballState::INACTIVE &&
					    player.getEquippedAbility() == EquippedAbility::FIRE) {
						currentRoom->fireball.shoot(player.getPosX(), player.getPosY(), player.getFlipType());
					}
				}

				// Handle input for the character
				player.control(e);
			}

			// Move the character
			player.move();
			currentRoom->collisionTiles(&player);
			currentRoom->collisionWall(&player);
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
					player.setPosY(SCREEN_HEIGHT - 50);
				}
			}

			if (player.onBottomBorder()) {
				if (currentRoom->roomIndexBelow != -1) {
					currentRoom = rooms.at(currentRoom->roomIndexBelow);
					currentRoom->enter();
					player.setPosY(5);
				}
			}

			collision = player.collisionDetectionEnemies(currentRoom->enemies);
			if (collision) {
				currentRoom = rooms.at(player.lastSavePoint.roomIndex);
				currentRoom->enter();
				player.setPosX(player.lastSavePoint.x);
				player.setPosY(player.lastSavePoint.y);
			}

			// To enter the correct room when character dies in flames
			bool flameCollision = false;
			collision |= player.collisionDetectionSprites(currentRoom->sprites, flameCollision);
			if (flameCollision) {
				currentRoom = rooms.at(player.lastSavePoint.roomIndex);
				currentRoom->enter();
			}

			if (player.getFallingDown()) {
				currentRoom = rooms.at(player.lastSavePoint.roomIndex);
				currentRoom->enter();
				player.setFallingDown(false);
				collision = true;
			}

			if (player.getHealth() == 0) {
				gameOver = true;
				continue;
			}

			// Increase Player health when at 4 points
			if (player.getPoints() == 4) {
				player.setPoints(0);
				player.increaseMaxHealth();
			}
			currentRoom->arrows.collisionDetectionEnemies(currentRoom->enemies);
			currentRoom->fireball.collisionDetectionEnemies(currentRoom->enemies);

			// Clear screen
			SDL_RenderClear(gRenderer);

			// SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
			SDL_RenderCopy(gRenderer, currentRoom->background_texture, NULL, NULL);

			// Render objects
			currentRoom->moveEnemies(&player);
			player.render(gRenderer);
			currentRoom->renderEnemies(gRenderer);
			currentRoom->renderSprites(gRenderer);
			currentRoom->renderTiles(gRenderer, tileTexture[currentRoom->level]);
			currentRoom->arrows.render(gRenderer, player.getPosX(), player.getPosY(), player.getFlipType());
			currentRoom->fireball.render(gRenderer, player.getPosX(), player.getPosY(), player.getFlipType());
			hud.render(&player, collision || newGame);

			// Update screen
			SDL_RenderPresent(gRenderer);

			// Increment the frame counter
			frame++;

			newGame = false;

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
		for (auto &room : rooms) {
			room->free();
		}
	}

	close();
	return 0;
}
