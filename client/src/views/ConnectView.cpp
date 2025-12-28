#include "../../headers/views/ConnectView.hpp"

const std::regex ConnectView::connectionRule("^((1[0-9]{2}|[0-9]{1,2}|2[0-4][0-9]|25[0-5])\\.){3}(1[0-9]{2}|[0-9]{1,2}|2[0-4][0-9]|25[0-5]):([0-9]{1,4}|[1-5][0-9]{4}|6[0-4][0-9]{3}|65[0-4][0-9]{2}|655[0-2][0-9]|6553[0-5])$", std::regex::optimize);

ConnectView::ConnectView(SDL_Window *window, SDL_Renderer *renderer, SDLFontManager* fontManager)
    : connectionBox(window, renderer, fontManager, {100, 100, 300, 50}, "Roboto/Roboto-Medium", 20, true),
      connectButton(window, renderer, fontManager, {100, 200, 300, 50}, "Roboto/Roboto-Medium", 20)
{
    this->connectButton.setText("Button");
    this->window = window;
    this->renderer = renderer;
}

TextBox& ConnectView::getConnectionBox()
{
    return this->connectionBox;
}

TextBox& ConnectView::getConnectButton()
{
    return this->connectButton;
}

void ConnectView::render()
{
    //Clear screen
    SDL_SetRenderDrawColor(renderer, 150, 150, 150, 255);
    SDL_RenderClear(renderer);

    //connetion box render code
    SDL_SetRenderDrawColor(renderer, 150, 150, 150, 255);
    SDL_RenderFillRect(renderer, &connectionBox.getBoxPos());
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    connectionBox.genTexture();
    SDL_RenderCopy(renderer, connectionBox.getTexture(), nullptr, &(connectionBox.getTextureRect()));

    //connect button render code
    SDL_SetRenderDrawColor(renderer, 150, 50, 150, 255);
    SDL_RenderFillRect(renderer, &connectButton.getBoxPos());
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    connectButton.genTexture();
    SDL_RenderCopy(renderer, connectButton.getTexture(), nullptr, &(connectButton.getTextureRect()));

    //Present to screen
    SDL_RenderPresent(renderer);
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
