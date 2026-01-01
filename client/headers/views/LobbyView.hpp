#pragma once

#include <SDL2/SDL.h>
#include <vector>
#include <string>

#include "View.hpp"
#include "../SDLFontManager.hpp"
#include "../../../shared/headers/Room.hpp"

class LobbyView : public View
{
private:
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDLFontManager *fontManager;

    // List of rooms received from server
    std::vector<Room *> rooms;

    // Scrolling state for the room list
    int scrollOffset = 0;
    const int itemHeight = 100;

    // Helper structure used for detecting clicks on "Join" buttons
    struct Button
    {
        SDL_Rect rect;
        Room *room;
    };
    std::vector<Button> joinButtons;

public:
    explicit LobbyView(SDL_Window *window, SDL_Renderer *renderer, SDLFontManager *fontManager);
    virtual ~LobbyView() = default;

    void render() override;

    // Updates list of rooms displayed in the lobby
    void updateRooms(std::vector<Room *> newRooms);

    // Returns the room that was clicked by the user (if any)
    Room *getRoomClicked();
    void handleScroll(int mouseWheelY);
};
