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
    const SDL_Color tempColor;
    const int fontSize;
    const bool editable;
    
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDLFontManager* fontManager;
    SDL_Texture* texture;

    // data for fontManager
    SDL_Rect textureSize;
    std::string text;
    std::string tempText;
    bool render;

    bool selected;
    
    void handleSpecialKeyPress(const SDL_Event &e);
    
public:
    TextBox(SDL_Window* window, SDL_Renderer* renderer, SDLFontManager* fontManager, const SDL_Rect& pos, 
        const std::string& font, int size, const bool editable = false, SDL_Color color = {0,0,0,255}, bool select = false, 
        const std::string& temp= "", SDL_Color tempColor = {75, 75, 75, 255});
    ~TextBox();

    bool checkIfClicked(int x, int y) const noexcept; 
    void genTexture();
    SDL_Texture* getTexture();
    SDL_Rect& getTextureRect();
    const SDL_Rect& getBoxPos() const;
    bool getSelected() const;

    void setSelected(bool select);
    void setText(const std::string& text);
    void textListener(SDL_Event& e);

    std::string& getString();
};