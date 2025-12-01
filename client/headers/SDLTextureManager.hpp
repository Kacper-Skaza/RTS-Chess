#pragma once

#include <unordered_map>
#include <string>
#include <memory>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

class SDLTextureManager
{
private:
	std::unordered_map<std::string, std::unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)>> textures;

public:
	explicit SDLTextureManager(SDL_Renderer *renderer);
	~SDLTextureManager() = default;

	SDL_Texture *getTexture(const std::string &name) const;
};
