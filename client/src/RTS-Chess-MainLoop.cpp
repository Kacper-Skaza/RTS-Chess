#include "../headers/RTS-Chess-MainLoop.hpp"

bool running = true;

namespace
{
    User* me = nullptr;
    ConnectionManager* connectionManager = nullptr;
    bool boxIp = true;
    int mousePosX;
    int mousePosY;
    int toUpdateCounter = 10;
    int roomRefreshCounter = 5;
    int roomReturnCounter = 0;
    std::string messageContainer = "";
}

template<typename T> bool between(T x, T a, T b)
{
    return x >= a && x <= b;
}

void connectLoop(SDL_Window* window, SDL_Renderer* renderer, SDLFontManager* fontManager, std::unique_ptr<View> &view, ConnectView* connectView, SDL_Event& event)
{
    while (SDL_PollEvent(&event) != 0)
    {
        if (event.type == SDL_QUIT)
        {
            running = false;
        }
        if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT)
        {
            SDL_GetMouseState(&mousePosX, &mousePosY);
            if (connectView->getConnectionBox().checkIfClicked(mousePosX, mousePosY))
            {
                connectView->getUserBox().setSelected(false);
                connectView->getConnectionBox().setSelected(true);
                SDL_StartTextInput();
                boxIp = true;
            }
            else if(connectView->getUserBox().checkIfClicked(mousePosX, mousePosY))
            {
                connectView->getConnectionBox().setSelected(false);
                connectView->getUserBox().setSelected(true);
                SDL_StartTextInput();
                boxIp = false;
            }
            else
            {
                connectView->getConnectionBox().setSelected(false);
                connectView->getUserBox().setSelected(false);
                SDL_StopTextInput();
            }
            if (connectView->getConnectButton().checkIfClicked(mousePosX, mousePosY))
            {
                if (!connectView->getUserBox().getString().empty() &&
                     connectView->validateConnectionData())
                {
                    SOCKET sock;
                    if((sock = connectView->connectToServer()) != (SOCKET)SOCKET_ERROR_PLATFORM)
                    {
                        if (connectionManager == nullptr)
                            connectionManager = new ConnectionManager(sock);
                        if (me == nullptr)
                        {
                            nlohmann::json j = nlohmann::json{
                                {"type", "REQUEST_NICK"},
                                {"data", {{"nick", connectView->getUserBox().getString()}}}
                            };
                            roomRefreshCounter = 0;
                            unsigned long long id = 0;
                            me = new User(id, connectView->getUserBox().getString());
                            MessageHandler::handleView(connectView, connectionManager, me, j.dump());
                        }
                        view.release();
                        view = std::make_unique<LobbyView>(window, renderer, fontManager);
                    }
                    else
                    {
                        connectView->getConnectionBox().setText("");
                    }
                }
                else
                {
                    connectView->getConnectionBox().setText("");
                }
            }
        }
        if (SDL_IsTextInputActive() == SDL_TRUE)
        {
            if (boxIp)
                connectView->getConnectionBox().textListener(event);
            else
                connectView->getUserBox().textListener(event);
        }
    }
}

void lobbyLoop(SDL_Window* window, SDL_Renderer* renderer, SDLFontManager* fontManager, std::unique_ptr<View> &view, LobbyView* lobbyView, SDL_Event& event)
{
    if (roomRefreshCounter <= 0)
    {
        roomRefreshCounter = 20;
        nlohmann::json j = nlohmann::json{
            {"type", "REQUEST_ROOMS"},
            {"data", nullptr}
        };
        MessageHandler::handleView(lobbyView, connectionManager, me, j.dump());
    }
    else
        roomRefreshCounter--;

    while (SDL_PollEvent(&event) != 0)
    {
        if (event.type == SDL_QUIT)
        {
            running = false;
        }
        if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT)
        {
            SDL_GetMouseState(&mousePosX, &mousePosY);
            Room* room = lobbyView->getRoomClicked();
            if (room != nullptr)
            {
                //here join room
                nlohmann::json j = nlohmann::json{
                    {"type", "ROOM_JOIN"},
                    {"data", {{"roomName", room->getRoomName()}}}
                };
                MessageHandler::handleView(lobbyView, connectionManager, me, j.dump());
                view.release();
                view = std::make_unique<RoomView>(window, renderer, fontManager);
                ((RoomView*)(view.get()))->updateUser(me);
            }
            if (lobbyView->getCreateButton().checkIfClicked(mousePosX, mousePosY)) 
            {
                //here create room check
                if (!lobbyView->getcreateBox().getString().empty())
                {
                    nlohmann::json j = nlohmann::json{
                        {"type", "ROOM_CREATE"},
                        {"data", {{"roomName", lobbyView->getcreateBox().getString()}}}
                    };
                    MessageHandler::handleView(lobbyView, connectionManager, me, j.dump());
                    view.release();
                    view = std::make_unique<RoomView>(window, renderer, fontManager);
                    ((RoomView*)(view.get()))->updateUser(me);
                }
            }
            if (lobbyView->getcreateBox().checkIfClicked(mousePosX, mousePosY))
            {
                lobbyView->getcreateBox().setSelected(true);
                SDL_StartTextInput();
            }
            else
            {
                lobbyView->getcreateBox().setSelected(false);
                SDL_StopTextInput();
            }
        }
        
        if (event.type == SDL_MOUSEWHEEL)
        {
            // event.wheel.y to kierunek (1 w górę, -1 w dół)
            lobbyView->handleScroll(event.wheel.y);
        }
        if (SDL_IsTextInputActive() == SDL_TRUE)
        {
            lobbyView->getcreateBox().textListener(event);
        }
    }
}

void roomLoop(SDL_Window* window, SDL_Renderer* renderer, SDLFontManager* fontManager, SDLTextureManager* texturemanager, std::unique_ptr<View> &view, RoomView* roomView, SDL_Event& event)
{    
    if (roomView->getRoom().isMatchStarted())
    {
        // Switch to GameView
        me->setSide(roomView->getOtherSelf()->getSide());
        std::vector<std::string> whitePlayers;
        std::vector<std::string> blackPlayers;
        for (auto &&i : roomView->getRoom().getPlayerList())
        {
            if (i.second->getSide() == ChessSide::WHITE)
            {
                whitePlayers.push_back(i.second->getUsername());
            }
            else if (i.second->getSide() == ChessSide::BLACK)
            {
                blackPlayers.push_back(i.second->getUsername());
            }
            else
            {
                continue;
            }
        }
        Board b = std::move(roomView->getRoom().getBoard());
        view.release();
        view = std::make_unique<GameView>(window, renderer, fontManager, texturemanager, &b, whitePlayers, blackPlayers);
        roomReturnCounter = 300;
        return;
    }
    
    while (SDL_PollEvent(&event) != 0)
    {
        if (event.type == SDL_QUIT)
        {
            running = false;
        }
        if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT)
        {
            //get what button was pressed and do corresponding action
            std::string name = roomView->getButtonClicked();
            if (name == "toggle_ready")
            {
                roomView->getSelf()->setReady(!roomView->getSelf()->isReady());
                nlohmann::json j = nlohmann::json{
                    {"type", "PLAYER_WANT"},
                    {"data", {{"player", roomView->getSelf()->isReady() ? PlayerWant::PLAYER_READY : PlayerWant::PLAYER_NOT_READY}}}
                };
                MessageHandler::handleView(roomView, connectionManager, roomView->getSelf(), j.dump());
            }
            else if (name == "toggle_role")
            {
                roomView->getSelf()->setPlayer(!roomView->getSelf()->isPlayer());
                nlohmann::json j = nlohmann::json{
                    {"type", "PLAYER_WANT"},
                    {"data", {{"player", roomView->getSelf()->isPlayer() ? (roomView->getSelf()->isReady() ? PlayerWant::PLAYER_READY : PlayerWant::PLAYER_NOT_READY) : PlayerWant::SPECTATOR}}}
                };
                MessageHandler::handleView(roomView, connectionManager, roomView->getSelf(), j.dump());
            }
            else if (name == "leave_room")
            {
                nlohmann::json j = nlohmann::json{
                    {"type", "ROOM_LEAVE"},
                    {"data", nullptr}
                };
                MessageHandler::handleView(roomView, connectionManager, roomView->getSelf(), j.dump());
                roomRefreshCounter = 0;
                view.release();
                view = std::make_unique<LobbyView>(window, renderer, fontManager);
            }
            else if (name == "toggle_count")
            {
                roomView->getRoom().setMaxPlayerCount(roomView->getRoom().getMaxPlayerCount() % 8 + 2);
                nlohmann::json j = nlohmann::json{
                    {"type", "CHANGE_PLAYER_COUNT"},
                    {"data", nullptr}
                };
                MessageHandler::handleView(roomView, connectionManager, roomView->getSelf(), j.dump());
            }
            
        }
    }

}

void gameLoop(SDL_Window* window, SDL_Renderer* renderer, SDLFontManager* fontManager, std::unique_ptr<View> &view, GameView* gameView, SDL_Event& event)
{
    if (gameView->getBoard()->getGameState() != MatchEndReasons::NOT_ENDED)
    {
        // Game has ended
        roomReturnCounter--;
        if (roomReturnCounter <= 0)
        {
            // Return to RoomView
            nlohmann::json j = nlohmann::json{
                {"type", "REQUEST_ROOM"},
                {"data", {{"roomName", gameView->getOldRoomName()}}}
            };
            MessageHandler::handleView(gameView, connectionManager, me, j.dump());
            view.release();
            view = std::make_unique<RoomView>(window, renderer, fontManager);
            ((RoomView*)(view.get()))->updateUser(me);
            return;
        }
        
        return;
    }
    while (SDL_PollEvent(&event) != 0)
    {
        if (event.type == SDL_QUIT)
        {
            running = false;
        }
        if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT)
        {
            SDL_GetMouseState(&mousePosX, &mousePosY);
            if(between(mousePosX, 40, 1064) && between(mousePosY, 40, 1064))
            {
                if (gameView->getSelected() != std::make_pair(-1, -1))
                {
                    //do move
                    if(gameView->checkPiece() != ' ' && me->getSide() != ChessSide::UNKNOWN)
                    {
                        if ((std::isupper(gameView->checkPiece())? ChessSide::WHITE: ChessSide::BLACK) == me->getSide())
                        {
                            std::string s =(me->getSide() == ChessSide::BLACK)? "black\n": (me->getSide() == ChessSide::WHITE)? "white\n": "unknown\n";
                            Move move(gameView->getBoard()->getBoardFull()[gameView->getSelected().first][gameView->getSelected().second],
                                      gameView->getSelected(), std::make_pair((mousePosY - 40) / 128, (mousePosX - 40) / 128));
                            if (gameView->getBoard()->makeMove(move))
                            {
                                //send move to server
                                nlohmann::json j = nlohmann::json{
                                    {"type", "MAKE_MOVE"},
                                    {"data", {{"move", move}}}
                                };
                                MessageHandler::handleView(gameView, connectionManager, me, j.dump());
                            }
                        }
                        gameView->setSelected(-1, -1);
                    }
                    else
                    {
                        gameView->setSelected((mousePosY - 40) / 128, (mousePosX - 40) / 128);
                    }
                }
                else
                {
                    gameView->setSelected((mousePosY - 40) / 128, (mousePosX - 40) / 128);
                }
            }
            if (gameView->getChatBox().checkIfClicked(mousePosX, mousePosY))
            {
                gameView->getChatBox().setSelected(true);
                SDL_StartTextInput();
            }
            else
            {
                gameView->getChatBox().setSelected(false);
                SDL_StopTextInput();
            }
            
        }
        if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_RETURN && !gameView->getChatBox().getString().empty())
        {
            nlohmann::json j = nlohmann::json{
                {"type", "CHAT_MESSAGE"},
                {"data", {{"message", gameView->getChatBox().getString()}}}
            };
            MessageHandler::handleView(gameView, connectionManager, me, j.dump());
            gameView->getChatBox().setText("");
            gameView->getChatBox().setSelected(false);
            SDL_StopTextInput();
        }
        if (SDL_IsTextInputActive() == SDL_TRUE)
        {
            gameView->getChatBox().textListener(event);
        }
    }
}

void delay()
{
    //add delta time
    SDL_Delay(1000/TARGET_FPS);
}

void receiveLoop(std::unique_ptr<View> &view)
{
    messageContainer = connectionManager->recvMessage();
    while (messageContainer.empty() == false)
    {
        MessageHandler::handleView(view.get(), connectionManager, me, messageContainer);
        messageContainer = connectionManager->recvMessage();
    }
    messageContainer = "";
}

void mainLoop(SDL_Window* window, SDL_Renderer* renderer, SDLFontManager* fontManager, SDLTextureManager* textureManager, std::unique_ptr<View> &view, SDL_Event& event)
{
    if (GameView* gameView = dynamic_cast<GameView*>(view.get()))
        gameLoop(window, renderer, fontManager, view, gameView, event);
    else if (RoomView* roomView = dynamic_cast<RoomView*>(view.get()))
        roomLoop(window, renderer, fontManager, textureManager, view, roomView, event);
    else if(LobbyView* lobbyView = dynamic_cast<LobbyView*>(view.get()))
        lobbyLoop(window, renderer, fontManager, view, lobbyView, event);
    else if (ConnectView* connectView = dynamic_cast<ConnectView*>(view.get()))
        connectLoop(window, renderer, fontManager, view, connectView, event);
    else
        throw std::logic_error("View does not exist");
    view.get()->render();
    if (connectionManager != nullptr)
    {
        receiveLoop(view);   
        if (toUpdateCounter-- <= 0)
        {
            connectionManager->update();
            toUpdateCounter = 10;
        }
    }
    delay();
    if (connectionManager != nullptr)
    {
        receiveLoop(view);
        if ((0) /*jezeli 70 sekund od ostatniego ping*/)
        {
            view.release();
            view = std::make_unique<ConnectView>(window, renderer, fontManager);
            if (me != nullptr)
                delete me;
            delete connectionManager;
        }
    }
    return;
}

void mainLoopDestroy()
{
    if (me != nullptr)
        delete me;
    if (connectionManager != nullptr)
        delete connectionManager;
    #if PLATFORM == PLATFORM_WINDOWS
    WSACleanup();
    #endif
}