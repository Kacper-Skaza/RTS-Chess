#include <iostream>

#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

#include "../headers/SDLTextureManager.hpp"
#include "../headers/SDLFontManager.hpp"
#include "../headers/TextBox.hpp"

#include "../../shared/headers/Board.hpp"
#include "../../shared/headers/Move.hpp"
#include "../../shared/headers/User.hpp"
#include "../../shared/headers/Room.hpp"

#include "../headers/views/View.hpp"
#include "../headers/views/ConnectView.hpp"
#include "../headers/views/GameView.hpp"
#include "../headers/views/LobbyView.hpp"
#include "../headers/views/RoomView.hpp"

#include "../../shared/headers/dependencies/Json.hpp"

#include "RTS-Chess-MainLoop.hpp"

int main()
{
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
		return 1;
	}

	if (IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG) == 0)
	{
		std::cerr << "IMG_Init Error: " << IMG_GetError() << std::endl;
		return 1;
	}

	if (TTF_Init() == -1)
	{
		std::cerr << "TTF_Init Error: " << TTF_GetError() << "\n";
	}

	SDL_Window *window = SDL_CreateWindow("RTS Chess", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1920, 1080,
		 SDL_WINDOW_SHOWN);
	if (!window)
	{
		std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
		SDL_Quit();
		return 1;
	}

	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (!renderer)
	{
		std::cerr << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
		SDL_DestroyWindow(window);
		SDL_Quit();
		return 1;
	}

	SDL_RenderSetLogicalSize(renderer, 1920, 1080);

	// --- Texture Manager ---
	SDLTextureManager texMgr(renderer);

	// Try to get the texture; it will be loaded on demand
	SDL_Texture *coconut = texMgr.getTexture("Coconut");
	if (!coconut)
	{
		std::cerr << "Coconut.jpg not found" << std::endl;
		return 1;
	}

	// Get the texture size
	int w, h;
	SDL_QueryTexture(coconut, nullptr, nullptr, &w, &h);

	//Test for textbox
	//Scope to satisfy destructor
	{
		SDLFontManager fontMan(renderer);
		int x, y;

		std::unique_ptr<View> currentView = std::make_unique<GameView>(window, renderer, &fontMan, &texMgr);

		// --- Main loop ---
		bool running = true;
		SDL_Event event;

		while (running)
		{
			mainLoop(window, renderer, &fontMan, currentView, event);
			// while (SDL_PollEvent(&event) != 0)
			// {
			// 	// Close window event
			// 	if (event.type == SDL_QUIT)
			// 	{
			// 		running = false;
			// 	}

			// 	if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_F12)
			// 	{
			// 		if (SDL_GetWindowFlags(window) & SDL_WINDOW_FULLSCREEN_DESKTOP)
			// 		{
			// 			SDL_SetWindowFullscreen(window, 0);
			// 			SDL_SetWindowSize(window, 1920, 1080);
			// 			SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
			// 		}
			// 		else
			// 		{
			// 			SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
			// 		}
			// 	}
				
				
			// 	if(event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT)
			// 	{
			// 		SDL_GetMouseState(&x, &y);
			// 		//check if connectView
			// 		if (ConnectView* conView = dynamic_cast<ConnectView*>(currentView.get()))
			// 		{
			// 			if (conView->getConnectionBox().checkIfClicked(x, y))
			// 			{
			// 				SDL_StartTextInput();
			// 			}
			// 			else
			// 			{
			// 				SDL_StopTextInput();
			// 			}
			// 			if (conView->getConnectButton().checkIfClicked(x,y))
			// 			{
			// 				if (conView->validateConnectionData())
			// 				{
			// 					currentView.release();
			// 					currentView = std::make_unique<LobbyView>();
			// 				}
			// 				else
			// 				{
			// 					conView->getConnectionBox().setText("");
			// 				}							
			// 			}
						
			// 		}
			// 	}
			// 	if (SDL_IsTextInputActive() == SDL_TRUE)
			// 	{
			// 		if (ConnectView* conView = dynamic_cast<ConnectView*>(currentView.get()))
			// 		{
			// 			conView->getConnectionBox().textListener(event);
			// 		}
			// 	}
			// 	if (GameView* gameView = dynamic_cast<GameView*>(currentView.get()))
			// 	{
			// 		gameView->handleEvent(event);
			// 	}
				
			// }

			// currentView.get()->render();			

			// // 60 FPS <- should implement delta time for more stable fps
			// SDL_Delay(16);
		}
	}

	{
		

	}
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	IMG_Quit();
	TTF_Quit();
	SDL_Quit();
	return 0;
}
