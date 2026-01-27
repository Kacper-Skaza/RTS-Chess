#include <iostream>

#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

#include "../../shared/headers/dependencies/Json.hpp"

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

#include "../headers/RTS-Chess-MainLoop.hpp"

int main()
{
	SDL_SetHint(SDL_HINT_WINDOWS_DPI_AWARENESS, "permonitorv2");

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

	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
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

	//Scope to satisfy destructor
	{
		SDLFontManager fontMan(renderer);

		std::unique_ptr<View> currentView = std::make_unique<ConnectView>(window, renderer, &fontMan);

		// --- Main loop ---
		SDL_Event event;

		while (running)
		{
			mainLoop(window, renderer, &fontMan, &texMgr, currentView, event);
		}
	}

	mainLoopDestroy();
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	IMG_Quit();
	TTF_Quit();
	SDL_Quit();
	return 0;
}
