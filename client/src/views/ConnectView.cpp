#include "../../headers/views/ConnectView.hpp"

const std::regex ConnectView::connectionRule("^((1[0-9]{2}|[0-9]{1,2}|2[0-4][0-9]|25[0-5])\\.){3}(1[0-9]{2}|[0-9]{1,2}|2[0-4][0-9]|25[0-5]):([0-9]{1,4}|[1-5][0-9]{4}|6[0-4][0-9]{3}|65[0-4][0-9]{2}|655[0-2][0-9]|6553[0-5])$", std::regex::optimize);

ConnectView::ConnectView(SDL_Window *window, SDL_Renderer *renderer, SDLFontManager* fontManager)
    : connectionBox(window, renderer, fontManager, {100, 100, 300, 50}, "Roboto/Roboto-Medium", 20)
{
    this->window = window;
    this->renderer = renderer;
}

void ConnectView::render()
{
    
}

void ConnectView::inputSanatizer()
{

}

/**
 * This method may be unoptimized
 */
bool ConnectView::validateConnectionData()
{
    return std::regex_match(this->connectionBox.getString(), this->connectionRule);
}

bool ConnectView::connect()
{
    //pure intenet function
    return false;
}
