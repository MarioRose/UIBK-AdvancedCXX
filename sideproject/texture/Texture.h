#ifndef TEXTURE_H
#define TEXTURE_H

#include <SDL.h>
#include <string>

enum class TextureType { IDLE, RUNNING, DYING, ATTACK, FALLING, JUMPING };

// Texture wrapper class
class Texture {
  public:
	// Initializes variables
	Texture();

	// Deallocates memory
	~Texture();

	// Loads image at specified path
	bool loadFromFile(std::string path, SDL_Renderer *gRenderer);

	// Deallocates texture
	void free();

	// Renders texture at given point
	void render(int x, int y, SDL_Renderer *gRenderer, SDL_Rect *clip = NULL, double angle = 0.0,
	            SDL_Point *center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);

	// Gets image dimensions
	int getWidth();

	void scaleToWidth(int width);
	void scaleToHeight(int height);

  private:
	// Image dimensions
	int mWidth;
	int mHeight;
	// The actual hardware texture
	SDL_Texture *mTexture;
};

#endif
