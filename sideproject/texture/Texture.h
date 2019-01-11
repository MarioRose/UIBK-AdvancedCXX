#ifndef TEXTURE_H
#define TEXTURE_H

#include <SDL.h>
#include <string>

enum class TextureType {
	IDLE,
	RUNNING,
	DYING,
	ATTACK
};

//Texture wrapper class
class Texture {
public:
	//Initializes variables
	Texture();

	//Deallocates memory
	~Texture();

	//Loads image at specified path
	bool loadFromFile( std::string path, SDL_Renderer* gRenderer );

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
	void render( int x, int y, SDL_Renderer* gRenderer,  SDL_Rect* clip = NULL,
		double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE );

	//Gets image dimensions
	int getWidth();
	int getHeight();

    void scaleToWidth(int width);
    void scaleToHeight(int height);

	//TODO Debug only
	std::string filePath;

private:
	//Image dimensions
	int mWidth;
	int mHeight;
	//The actual hardware texture
	SDL_Texture* mTexture;
};

#endif
