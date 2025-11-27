#include "../headers/SDLTextureManager.hpp"

#include <filesystem>

SDLTextureManager::SDLTextureManager(SDL_Renderer *renderer, const char* appPath)
{
	const std::string path = "resources/textures";

	for (const auto &entry : std::filesystem::directory_iterator(path))
	{
		if (!entry.is_regular_file())
			continue;

		std::string path = entry.path().string();
		std::string name = entry.path().stem().string();

		SDL_Texture *tex = IMG_LoadTexture(renderer, path.c_str());
		if (!tex)
		{
			SDL_Log("Failed to load %s: %s", path.c_str(), SDL_GetError());
			continue;
		}

		textures[name] = tex;
	}
}

SDLTextureManager::~SDLTextureManager()
{
	for (auto &[_, tex] : textures)
	{
		SDL_DestroyTexture(tex);
	}
}

SDL_Texture *SDLTextureManager::getTexture(const std::string name) const
{
	auto it = textures.find(name);

	if (it != textures.end())
		return it->second;
	else
		return nullptr;
}
