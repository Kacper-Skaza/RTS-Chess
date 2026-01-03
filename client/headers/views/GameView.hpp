#pragma once

#include <vector>
#include <memory>
#include <algorithm>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "../SDLFontManager.hpp"
#include "../SDLTextureManager.hpp"

#include "../TextBox.hpp"

#include "../../../shared/headers/Board.hpp"
#include "../../../shared/headers/User.hpp"
#include "../../../shared/headers/Structures.hpp"
#include "View.hpp"

class GameView: public View
{
private:
    //SDL variables
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDLFontManager* fontManager;
    
    std::unordered_map<std::string, SDL_Texture*> textures;
    std::unordered_map<std::string, SDL_Rect> destinationRectangles;
    
    std::string oldRoomName;

    std::vector<std::string> whitePlayers;
    std::vector<std::string> blackPlayers;
    
    std::string chat;
    TextBox chatBox;
    
    //Game manipulation variables
    Board board;
    bool nullBoard;
    std::pair<int, int> selected;
    MatchEndReasons gameState = MatchEndReasons::NOT_ENDED;
    
    

public:
    explicit GameView(SDL_Window *window, SDL_Renderer *renderer, 
        SDLFontManager *fontManager, SDLTextureManager *textureManager, 
        Board *board = nullptr, 
        std::vector<std::string> whitePlayers = {}, std::vector<std::string> blackPlayers = {},
        std::string roomName = "");
    ~GameView() = default;

    void render();

    void updateChat(std::string& message, User& sender);

    std::string& getOldRoomName();

    char checkPiece();
    TextBox& getChatBox();
    void checkGameEnd(); 

    const MatchEndReasons& getGameState();
    void setGameState(MatchEndReasons state);

    std::pair<int, int>& getSelected();
    void setSelected(int x, int y);

    Board* getBoard();
};