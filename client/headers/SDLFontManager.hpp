#pragma once

#include <unordered_map>
#include <string>
#include <memory>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

class SDLFontManager
{
private:
    SDL_Renderer* renderer;
    std::unordered_map<std::pair<std::string, int>, std::unique_ptr<TTF_Font, decltype(&TTF_CloseFont)>> fonts;
    std::unordered_map<std::string, std::unique_ptr<SDL_Texture,decltype(&SDL_DestroyTexture)>> fontTextures;

    std::string createTextureString(std::string str, std::string font, int size, SDL_Color color);
    SDL_Texture *createFontTexture(std::string str, std::string font, int size, SDL_Color color);
public:
    explicit SDLFontManager(SDL_Renderer* renderer);
    ~SDLFontManager();

    SDL_Texture* getFontTexture(std::string str, std::string font, int size, SDL_Color color = {255,255,255,255});
    int preGenFontTextures(std::vector<std::tuple<std::string, std::string, int, SDL_Color>> fontTextureVector);
};