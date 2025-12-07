#pragma once

#include <vector>
#include <memory>
#include <regex>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "../SDLFontManager.hpp"
#include "../SDLTextureManager.hpp"
#include "../TextBox.hpp"

class ConnectView
{
private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    
    TextBox connectionBox;
    SDL_Rect connectButton;

    static const std::regex connectionRule;

    void drawButton();
public:
    explicit ConnectView(SDL_Window* window, SDL_Renderer* renderer, SDLFontManager* fontManager);
    ~ConnectView() = default;

    void render();
    void inputSanatizer();

    bool validateConnectionData();
    bool connect();
};