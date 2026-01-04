#include "../headers/SDLFontManager.hpp"
#include "SDLFontManager.hpp"

SDLFontManager::SDLFontManager(SDL_Renderer* renderer)
{
    this->renderer = renderer;
}

/**
 * Finds or creates a fontTexture
 * \param str string that will be presented
 * \param font used font as string
 * \param size int that represents size must be >0
 * \param color color of text 
 * \returns SDL_Texture* on success, nullptr on errror
 */
SDL_Texture* SDLFontManager::getFontTexture(const std::string& str, const std::string& font, int size, SDL_Color color)
{

    auto it = this->fontTextures.find(this->createTextureString(str, font, size, color));
    if (it != this->fontTextures.end()) return it->second.get();    
    return this->createFontTexture(str, font, size, color);
}

/**
 * Generates all textures provided in param
 * 
 * \param fontTextureVector vector of tuples with structure (string string int SDL_Color)
 * \returns 0 successfully generated all of textures, n failed to load n textures
 */
int SDLFontManager::preGenFontTextures(std::vector<std::tuple<std::string, std::string, int, SDL_Color>>& fontTextureVector)
{
    int fails = 0;
    for (auto &&i : fontTextureVector)
    {
        if(this->getFontTexture(std::get<0>(i), std::get<1>(i), std::get<2>(i), std::get<3>(i)) == nullptr) fails++;
    }
    return fails;
}

/**
 * Creates new texture from font with specific font size
 *
 * \param str string that will be presented
 * \param font used font as string
 * \param size int that represents size must be >0
 * \param color color of text 
 * \returns SDL_Texture* on success, nullptr on errror
 */
SDL_Texture *SDLFontManager::createFontTexture(const std::string& str, const std::string& font, int size, SDL_Color color)
{
    //Size must be bigger than 0
    if (size <= 0) return nullptr;

    //Try to find the font 
    if (this->fonts.find(std::string(str + std::to_string(size))) == this->fonts.end())
    {
        if (this->addNewFont(font, size) == false) return nullptr;
    }

    //Adding texture to map
    SDL_Texture* fontTexture = this->createTextTexture(str, font, size, color);

    this->fontTextures.emplace(
        this->createTextureString(str, font, size, color), 
        std::unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)>(fontTexture, SDL_DestroyTexture));

    return fontTexture;
}

/**
 * Creates string for searching unordered_map of fontTextures
 * 
 * \param str string that will be presented
 * \param font used font as string
 * \param size int that represents size must be >0
 * \param color color of text 
 * \returns string that represents key value to fontTexture
 */
std::string SDLFontManager::createTextureString(const std::string& str, const std::string& font, int size, SDL_Color color)
{
    return std::string(
        str + font + std::to_string(size) + 
        std::to_string(color.r) + std::to_string(color.g) +std::to_string(color.b) + std::to_string(color.a));
}

/**
 * Use if only adding font is needed, not texture else use getFontTexture
 * \param font used font
 * \param fontSize size of font in ptsize
 * \returns true on success, false on error 
 */
bool SDLFontManager::addNewFont(const std::string &font, int fontSize)
{
    std::string pathToFont = "./resources/fonts/" + font + ".ttf";
    TTF_Font* newFont = TTF_OpenFont(pathToFont.c_str(), fontSize);
    if (newFont == nullptr) return false;        
    this->fonts.emplace(
        std::string(font + std::to_string(fontSize)), 
        std::unique_ptr<TTF_Font, decltype(&TTF_CloseFont)>(newFont, TTF_CloseFont));
    return true;
}

/**
 * This is unsafe texture creating method do not use if speed and memory usage isn't a concern.
 * No data validation is done.
 * \param str string that will be presented
 * \param font used font as string
 * \param size int that represents size must be >0
 * \param color color of text
 * \returns SDL_Texture* on success, nullptr on errror
 */
SDL_Texture *SDLFontManager::createTextTexture(const std::string &str, const std::string &font, int size, SDL_Color color)
{
    //Creating texture
    SDL_Surface* surface = TTF_RenderText_Solid(
        this->fonts.find(std::string(font + std::to_string(size)))->second.get(), 
        str.c_str(), color);
    SDL_Texture* fontTexture = SDL_CreateTextureFromSurface(this->renderer, surface);
    
    SDL_FreeSurface(surface);
    return fontTexture;
} 

/**
 * This creates wrapped text it is not saved every time generate new and remember to free this
 */
SDL_Texture *SDLFontManager::createWrappedTextTexture(const std::string &str, const std::string &font, int size, SDL_Color color, uint32_t wraplength)
{
    SDL_Surface* surface = TTF_RenderText_Solid_Wrapped(
        this->fonts.find(std::string(font + std::to_string(size)))->second.get(),
        str.c_str(), color, wraplength
    );
    SDL_Texture* fontTexture = SDL_CreateTextureFromSurface(this->renderer, surface);

    this->fontTextures.emplace(
        this->createTextureString(str+"wrapped", font, size, color), 
        std::unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)>(fontTexture, SDL_DestroyTexture));

    SDL_FreeSurface(surface);
    return fontTexture;
}