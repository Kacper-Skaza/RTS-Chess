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
                SDL_StartTextInput();
                boxIp = true;
            }
            else if(connectView->getUserBox().checkIfClicked(mousePosX, mousePosY))
            {
                SDL_StartTextInput();
                boxIp = false;
            }
            else
                SDL_StopTextInput();
            if (connectView->getConnectButton().checkIfClicked(mousePosX, mousePosY))
            {
                if (!connectView->getUserBox().getString().empty() &&
                     connectView->validateConnectionData())
                {
                    SOCKET sock;
                    if((sock = connectView->connectToServer()) != SOCKET_ERROR_PLATFORM)
                    {
                        if (connectionManager == nullptr)
                            connectionManager = new ConnectionManager(sock);
                        if (me == nullptr)
                        {
                            nlohmann::json j = nlohmann::json{
                                {"type", "REQUEST_NICK"},
                                {"data", {"nick", connectView->getUserBox().getString()}}
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
    
    // User dummyCreator(1ULL, "SystemAdmin");

    // // 2. Create a vector of Rooms
    // std::vector<Room> testRooms;

    // // 3. Add rooms to the vector
    // // We use emplace_back to construct the Room directly in the vector
    // testRooms.emplace_back(Room("Pro Only 2000+", dummyCreator));
    // testRooms.emplace_back(Room("Fast Blitz 3min", dummyCreator));
    // testRooms.emplace_back(Room("Casual Game", dummyCreator));
    // testRooms.emplace_back(Room("Casual Game", dummyCreator));
    // testRooms.emplace_back(Room("Casual Game", dummyCreator));
    // testRooms.emplace_back(Room("Casual Game", dummyCreator));
    // testRooms.emplace_back(Room("Casual Game", dummyCreator));
    // testRooms.emplace_back(Room("Casual Game", dummyCreator));
    // testRooms.emplace_back(Room("Casual Game", dummyCreator));
    // testRooms.emplace_back(Room("Casual Game", dummyCreator));
    // testRooms.emplace_back(Room("Casual Game", dummyCreator));
    // testRooms.emplace_back(Room("Casual Game", dummyCreator));
    // testRooms.emplace_back(Room("Casual Game", dummyCreator));
    // testRooms.emplace_back(Room("Casual Game", dummyCreator));
    // testRooms.emplace_back(Room("Casual Game", dummyCreator));
    // testRooms.emplace_back(Room("Casual Game", dummyCreator));
    // testRooms.emplace_back(Room("Casual Game", dummyCreator));
    // testRooms.emplace_back(Room("Casual Game", dummyCreator));
    // testRooms.emplace_back(Room("Casual Game", dummyCreator));
    // testRooms.emplace_back(Room("Casual Game", dummyCreator));
    // testRooms.emplace_back(Room("Casual Game", dummyCreator));
    // testRooms.emplace_back(Room("Casual Game", dummyCreator));
    // testRooms.emplace_back(Room("Casual Game", dummyCreator));
    // testRooms.emplace_back(Room("Casual Game1", dummyCreator));
    // lobbyView->updateRooms(testRooms);

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
                    {"data", {"room_name", room->getRoomName()}}
                };
                MessageHandler::handleView(lobbyView, connectionManager, me, j.dump());
                view.release();
                view = std::make_unique<RoomView>(window, renderer, fontManager);
            }
            if (lobbyView->getCreateButton().checkIfClicked(mousePosX, mousePosY)) 
            {
                //here create room check
                nlohmann::json j = nlohmann::json{
                    {"type", "ROOM_CREATE"},
                    {"data", {"room_name", lobbyView->getcreateBox().getString()}}
                };
                MessageHandler::handleView(lobbyView, connectionManager, me, j.dump());
                view.release();
                view = std::make_unique<RoomView>(window, renderer, fontManager);
            }
            if (roomRefreshCounter <= 0)
            {
                roomRefreshCounter = 5;
                nlohmann::json j = nlohmann::json{
                    {"type", "REQUEST_ROOMS"},
                    {"data", nullptr}
                };
                // MessageHandler::handleView(lobbyView, connectionManager, me, j.dump());
            }
            else
                roomRefreshCounter--;
            
            if (lobbyView->getcreateBox().checkIfClicked(mousePosX, mousePosY))
                SDL_StartTextInput();
            else
                SDL_StopTextInput();
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
        view.release();
        view = std::make_unique<GameView>(window, renderer, fontManager, texturemanager, &roomView->getRoom().getBoard());
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
                    {"data", roomView->getSelf()->isReady() ? "PLAYER_READY" : "PLAYER_NOT_READY"}
                };
                // MessageHandler::handleView(roomView, connectionManager, roomView->getSelf(), j.dump());
            }
            else if (name == "toggle_role")
            {
                roomView->getSelf()->setPlayer(!roomView->getSelf()->isPlayer());
                nlohmann::json j = nlohmann::json{
                    {"type", "PLAYER_WANT"},
                    {"data", roomView->getSelf()->isPlayer() ? (roomView->getSelf()->isReady() ? "PLAYER_READY" : "PLAYER_NOT_READY") : "SPECTATOR"}
                };
                // MessageHandler::handleView(roomView, connectionManager, roomView->getSelf(), j.dump());
            }
            else if (name == "leave_room")
            {
                nlohmann::json j = nlohmann::json{
                    {"type", "ROOM_LEAVE"},
                    {"data", nullptr}
                };
                // MessageHandler::handleView(roomView, connectionManager, roomView->getSelf(), j.dump());
                roomRefreshCounter = 0;
                view.release();
                view = std::make_unique<LobbyView>(window, renderer, fontManager);
            }
        }
    }

}

void gameLoop(SDL_Window* window, SDL_Renderer* renderer, SDLFontManager* fontManager, std::unique_ptr<View> &view, GameView* gameView, SDL_Event& event)
{
    if (gameView->getGameState() != MatchEndReasons::NOT_ENDED)
    {
        // Game has ended
        roomReturnCounter--;
        if (roomReturnCounter <= 0)
        {
            // Return to RoomView
            nlohmann::json j = nlohmann::json{
                {"type", "ROOM_REQUEST"},
                {"data", {"room_name", gameView->getOldRoomName()}}
            };
            // MessageHandler::handleView(gameView, connectionManager, me, j.dump());
            view.release();
            view = std::make_unique<RoomView>(window, renderer, fontManager);
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
                    if(gameView->checkPiece() != ' ')
                    {
                        Move move(gameView->getBoard()->getBoardFull()[gameView->getSelected().first][gameView->getSelected().second],
                                  gameView->getSelected(), std::make_pair((mousePosY - 40) / 128, (mousePosX - 40) / 128));
                        gameView->getBoard()->makeMove(move);
                        gameView->setSelected(-1, -1);
                        //send move to server
                        nlohmann::json j = nlohmann::json{
                            {"type", "MAKE_MOVE"},
                            {"data", {"move", move}}
                        };
                        // MessageHandler::handleView(gameView, connectionManager, me, j.dump());
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
                SDL_StartTextInput();
            }
            else
            {
                SDL_StopTextInput();
            }
            
        }
        if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_RETURN)
        {
            nlohmann::json j = nlohmann::json{
                {"type", "CHAT_MESSAGE"},
                {"data", gameView->getChatBox().getString()}
            };
            // MessageHandler::handleView(gameView, connectionManager, me, j.dump());
            //temp later remove
            gameView->updateChat(gameView->getChatBox().getString(), *me);
            gameView->getChatBox().setText("");
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
        receiveLoop(view);
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