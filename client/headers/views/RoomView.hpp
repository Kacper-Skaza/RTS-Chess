#pragma once

#include <SDL2/SDL.h>
#include <vector>
#include <string>

#include "View.hpp"
#include "../SDLFontManager.hpp"
#include "../../../shared/headers/Room.hpp"
#include "../../../shared/headers/User.hpp"

class RoomView : public View
{
private:
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDLFontManager *fontManager;

    // List of rooms received from server
    Room room;
    User *user;

    // Helper structure used for detecting clicks on "Join" buttons
    struct Button
    {
        SDL_Rect rect;
        std::string name;
    };
    std::vector<Button> joinButtons;

public:
    explicit RoomView(SDL_Window *window, SDL_Renderer *renderer, SDLFontManager *fontManager);
    virtual ~RoomView() = default;

    void render() override;

    // Updates list of rooms displayed in the lobby
    void updateRoom(Room &newRoom);
    void updateUser(User *newUser);

    // Returns the room that was clicked by the user (if any)
    std::string getButtonClicked();
};
