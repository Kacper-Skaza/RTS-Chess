#pragma once

#include <vector>
#include <memory>
#include <regex>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "../SDLFontManager.hpp"
#include "../SDLTextureManager.hpp"
#include "../TextBox.hpp"

#include "../../../shared/headers/ConnectionManager.hpp"

#include "View.hpp"

class ConnectView: public View
{
private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    
    TextBox connectionBox;
    TextBox userBox;
    TextBox connectButton;

    static const std::regex connectionRule;
public:
    explicit ConnectView(SDL_Window* window, SDL_Renderer* renderer, SDLFontManager* fontManager);
    ~ConnectView() = default;

    TextBox& getConnectionBox();
    TextBox& getUserBox();
    TextBox& getConnectButton();

    void render();
    bool validateConnectionData();
    SOCKET connectToServer();
};