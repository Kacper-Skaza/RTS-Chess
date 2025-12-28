#include "../headers/RTS-Chess-MainLoop.hpp"

namespace
{
    int mousePosX;
    int mousePosY;
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
                SDL_StartTextInput();
            else
                SDL_StopTextInput();
            if (connectView->getConnectButton().checkIfClicked(mousePosX, mousePosY))
            {
                if (connectView->validateConnectionData())
                {
                    //maybe should add some user or somethign to make it work with connect
                    if(connectView->connect())
                    {
                        view.release();
                        view = std::make_unique<LobbyView>();
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
            connectView->getConnectionBox().textListener(event);
    }
}

void roomLoop(SDL_Window* window, SDL_Renderer* renderer, SDLFontManager* fontManager, std::unique_ptr<View> &view, RoomView* roomView, SDL_Event& event)
{
    while (SDL_PollEvent(&event) != 0)
    {
        if (event.type == SDL_QUIT)
        {
            running = false;
        }
        if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT)
        {
            //here do some smart math to get what room to enter
        }
        
    }
    
    
}

void lobbyLoop(SDL_Window* window, SDL_Renderer* renderer, SDLFontManager* fontManager, std::unique_ptr<View> &view, LobbyView* lobbyView, SDL_Event& event)
{
    while (SDL_PollEvent(&event) != 0)
    {
        if (event.type == SDL_QUIT)
        {
            running = false;
        }
        if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT)
        {
            //here add some texture checking what was clicked and interact with it
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
            //here add some texture checking what was clicked and interact with it
        }
        if (event.type == SDL_MOUSEBUTTONDOWN && SDL_MOUSEMOTION)
        {
            /* code */
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