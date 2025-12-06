#pragma once

#include <unordered_map>
#include <string>
#include <memory>
#include <vector>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

class SDLFontManager
{
private:
    SDL_Renderer* renderer;
    std::unordered_map<std::string, std::unique_ptr<TTF_Font, decltype(&TTF_CloseFont)>> fonts;
    std::unordered_map<std::string, std::unique_ptr<SDL_Texture,decltype(&SDL_DestroyTexture)>> fontTextures;

    static std::string createTextureString(const std::string& str, const std::string& font, int size, SDL_Color color);
    SDL_Texture* createFontTexture(const std::string& str, const std::string& font, int size, SDL_Color color);
public:
    explicit SDLFontManager(SDL_Renderer* renderer);
    ~SDLFontManager() = default;

    SDL_Texture* getFontTexture(const std::string& str, const  std::string& font, int size, SDL_Color color = {255,255,255,255});
    int preGenFontTextures(std::vector<std::tuple<std::string, std::string, int, SDL_Color>>& fontTextureVector);

    bool addNewFont(const std::string& pathToFont, int fontSize);
    SDL_Texture* createTextTexture(const std::string& str, const std::string& font, int size, SDL_Color color = {0,0,0,255});

};