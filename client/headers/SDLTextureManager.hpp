#pragma once

#include <unordered_map>
#include <string>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

class SDLTextureManager
{
private:
	std::unordered_map<std::string, SDL_Texture *> textures;

public:
	SDLTextureManager(SDL_Renderer *renderer, const char* appPath);
	~SDLTextureManager();

	SDL_Texture *getTexture(const std::string name) const;
};
