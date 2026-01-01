#include "../headers/RTS-Chess-MainLoop.hpp"

bool running = true;

namespace
{
    User* me = nullptr;
    ConnectionManager* connectionManager = nullptr;
    bool boxIp = true;
    int mousePosX;
    int mousePosY;
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
                    if((sock = connectView->connectToServer()) != -1)
                    {
                        if (connectionManager == nullptr)
                            connectionManager = new ConnectionManager(sock);
                        if (me == nullptr)
                        {
                            nlohmann::json j = nlohmann::json{
                                {"type", "REQUEST_NICK"},
                                {"data", {"nick", connectView->getUserBox().getString()}}
                            };
                            MessageHandler::handleView(connectView, connectionManager, me, j);
                            unsigned long long id = 0;
                            me = new User(id, connectView->getUserBox().getString());
                        }
                        view.release();
                        view = std::make_unique<LobbyView>(window, renderer, fontManager);
                    }
                    else
                        connectView->getConnectionBox().setText("");
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

void roomLoop(SDL_Window* window, SDL_Renderer* renderer, SDLFontManager* fontManager, std::unique_ptr<View> &view, RoomView* roomView, SDL_Event& event)
{
    /*User dummyCreator(1ULL, "SystemAdmin");
    User* me = new User(2ULL, "Me");
    roomView->updateUser(me);

    // 2. Create a vector of Rooms
    Room* testRoom;

    // 3. Add rooms to the vector
    // We use emplace_back to construct the Room directly in the vector
    testRoom = new Room("Pro Only 2000+", dummyCreator);
    roomView->updateRoom(testRoom);*/

    while (SDL_PollEvent(&event) != 0)
    {
        if (event.type == SDL_QUIT)
        {
            running = false;
        }
        else if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT)
        {
            std::string name = roomView->getButtonClicked();
            if (name != "")
            {
                std::cout<<name<<std::endl;
            }
        }
    }


}

void lobbyLoop(SDL_Window* window, SDL_Renderer* renderer, SDLFontManager* fontManager, std::unique_ptr<View> &view, LobbyView* lobbyView, SDL_Event& event)
{
    /*
    User dummyCreator(1ULL, "SystemAdmin");

    // 2. Create a vector of Rooms
    std::vector<Room*> testRooms;

    // 3. Add rooms to the vector
    // We use emplace_back to construct the Room directly in the vector
    testRooms.emplace_back(new Room("Pro Only 2000+", dummyCreator));
    testRooms.emplace_back(new Room("Fast Blitz 3min", dummyCreator));
    testRooms.emplace_back(new Room("Casual Game", dummyCreator));
    testRooms.emplace_back(new Room("Casual Game", dummyCreator));
    testRooms.emplace_back(new Room("Casual Game", dummyCreator));
    testRooms.emplace_back(new Room("Casual Game", dummyCreator));
    testRooms.emplace_back(new Room("Casual Game", dummyCreator));
    testRooms.emplace_back(new Room("Casual Game", dummyCreator));
    testRooms.emplace_back(new Room("Casual Game", dummyCreator));
    testRooms.emplace_back(new Room("Casual Game", dummyCreator));
    testRooms.emplace_back(new Room("Casual Game", dummyCreator));
    testRooms.emplace_back(new Room("Casual Game", dummyCreator));
    testRooms.emplace_back(new Room("Casual Game", dummyCreator));
    testRooms.emplace_back(new Room("Casual Game", dummyCreator));
    testRooms.emplace_back(new Room("Casual Game", dummyCreator));
    testRooms.emplace_back(new Room("Casual Game", dummyCreator));
    testRooms.emplace_back(new Room("Casual Game", dummyCreator));
    testRooms.emplace_back(new Room("Casual Game", dummyCreator));
    testRooms.emplace_back(new Room("Casual Game", dummyCreator));
    testRooms.emplace_back(new Room("Casual Game", dummyCreator));
    testRooms.emplace_back(new Room("Casual Game", dummyCreator));
    testRooms.emplace_back(new Room("Casual Game", dummyCreator));
    testRooms.emplace_back(new Room("Casual Game", dummyCreator));
    testRooms.emplace_back(new Room("Casual Game", dummyCreator));
    lobbyView->updateRooms(testRooms);*/

    while (SDL_PollEvent(&event) != 0)
    {
        if (event.type == SDL_QUIT)
        {
            running = false;
        }
        else if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT)
        {
            Room* room = lobbyView->getRoomClicked();
            if (room != nullptr)
            {
                std::cout<<room->getRoomName()<<std::endl;
            }
        }
        else if (event.type == SDL_MOUSEWHEEL)
        {
            // event.wheel.y to kierunek (1 w górę, -1 w dół)
            lobbyView->handleScroll(event.wheel.y);
        }
    }
}

void gameLoop(SDL_Window* window, SDL_Renderer* renderer, SDLFontManager* fontManager, std::unique_ptr<View> &view, GameView* gameView, SDL_Event& event)
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
        }
        if (event.type == SDL_MOUSEBUTTONDOWN && SDL_MOUSEMOTION)
        {
            //maybe implemented later
        }

    }
}

void delay()
{
    //add delta time


    SDL_Delay(1000/TARGET_FPS);
}

void mainLoop(SDL_Window* window, SDL_Renderer* renderer, SDLFontManager* fontManager, std::unique_ptr<View> &view, SDL_Event& event)
{
    if (GameView* gameView = dynamic_cast<GameView*>(view.get()))
        gameLoop(window, renderer, fontManager, view, gameView, event);
    else if (RoomView* roomView = dynamic_cast<RoomView*>(view.get()))
        roomLoop(window, renderer, fontManager, view, roomView, event);
    else if(LobbyView* lobbyView = dynamic_cast<LobbyView*>(view.get()))
        lobbyLoop(window, renderer, fontManager, view, lobbyView, event);
    else if (ConnectView* connectView = dynamic_cast<ConnectView*>(view.get()))
        connectLoop(window, renderer, fontManager, view, connectView, event);
    else
        throw std::logic_error("View does not exist");
    view.get()->render();
    delay();
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
