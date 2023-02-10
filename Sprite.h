#pragma once
#include <SDL.h>
#include <SDL_image.h>

class Sprite
{
private:
	SDL_Rect spriteSrcRect; // defines part of spreadsheet we'd like to render
	SDL_Surface* sprite; //the handle to the image
	SDL_Texture* texture; // texture holds sprite as a texture after loading the above

public:
	SDL_Rect spriteDestRect; // position we'd like to render the above
	Sprite(const char* spritePath, SDL_Rect srcRect, SDL_Rect destRect, SDL_Renderer* renderer);
	~Sprite();
	void Render(SDL_Renderer* renderer);
};