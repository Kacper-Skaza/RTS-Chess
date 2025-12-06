#pragma once

#include <vector>
#include <memory>
#include <string>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "../headers/SDLFontManager.hpp"

class TextBox
{
private:
    const SDL_Rect boxPos;
    const std::string font;
    const SDL_Color color;
    const int fontSize;
    
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDLFontManager* fontManager;
    SDL_Texture* texture;
    
    // data for fontManager
    SDL_Rect textureSize;
    std::string text;
    bool render;

    
    void handleSpecialKeyPress(SDL_Event &e);
    
public:
    TextBox(SDL_Window* window, SDL_Renderer* renderer, SDLFontManager* fontManager, SDL_Rect& pos, 
        std::string font, int size, SDL_Color color = {0,0,0,255});
    ~TextBox();

    bool checkIfClicked(int x, int y);
    void genTexture();
    SDL_Texture* getTexture();
    SDL_Rect& getTextureRect();

    void setText(const std::string& text);
    void textListener(SDL_Event& e);
};