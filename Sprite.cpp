#include "Sprite.h"
#include "GameEngine.h"

Sprite::Sprite(const char* spritePath, SDL_Rect srcRect, SDL_Rect destRect, SDL_Renderer* renderer) {
	sprite = IMG_Load(spritePath); // Step 1: Load sprite via IMG_Load function (part of SDL2_image package)
	spriteSrcRect = srcRect;
	spriteDestRect = destRect;
	texture = SDL_CreateTextureFromSurface(renderer, sprite); // Step 2: create a texture from the sprite surface
}

Sprite::~Sprite() {
	SDL_FreeSurface(sprite);
	SDL_DestroyTexture(texture);
}

void Sprite::Render(SDL_Renderer* renderer) { // Render function 
	SDL_RenderCopy(renderer, texture, &spriteSrcRect, &spriteDestRect);
}