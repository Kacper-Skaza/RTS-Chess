#include "../headers/SDLTextureManager.hpp"

#include <filesystem>
#include <stdexcept>

SDLTextureManager::SDLTextureManager(SDL_Renderer *renderer) : textures()
{
	const std::string path = "resources/textures";

	if (!std::filesystem::exists(path))
		throw std::runtime_error("Resources folder does not exist!");

	for (const auto &entry : std::filesystem::directory_iterator(path))
	{
		if (!entry.is_regular_file())
			continue;

		std::string filePath = entry.path().string();
		std::string name = entry.path().stem().string();

		SDL_Texture *tex = IMG_LoadTexture(renderer, filePath.c_str());
		if (!tex)
		{
			SDL_Log("Failed to load %s: %s", filePath.c_str(), SDL_GetError());
			continue;
		}

		textures.emplace(name, std::unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)>(tex, SDL_DestroyTexture));
	}

	if (getTexture("Coconut") == nullptr)
		throw std::runtime_error("Coconut.jpg file not found!");
}

SDL_Texture *SDLTextureManager::getTexture(const std::string name) const
{
	auto it = textures.find(name);

	if (it != textures.end())
		return it->second.get();
	else
		return nullptr;
}
