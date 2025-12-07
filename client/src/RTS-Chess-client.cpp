#include <iostream>

#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

#include "../headers/SDLTextureManager.hpp"
#include "../headers/SDLFontManager.hpp"
#include "../headers/TextBox.hpp"

#include "../../shared/headers/dependencies/Json.hpp"

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

	SDL_Window *window = SDL_CreateWindow("Puste Okno", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN);
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

	SDL_Rect dst{100, 100, w, h};

	//Test for textbox
	//Scope to satisfy destructor
	{
		SDLFontManager fontMan(renderer);
		SDL_Rect rect{100, 100, 200, 50};
		int x, y;
		TextBox box(window, renderer, &fontMan, rect, "Roboto/Roboto-Medium", 20);

		// --- Main loop ---
		bool running = true;
		SDL_Event event;

		while (running)
		{
			while (SDL_PollEvent(&event) != 0)
			{
				// Close window event
				if (event.type == SDL_QUIT)
				{
					running = false;
				}
				
				if(event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT)
				{
					SDL_GetMouseState(&x, &y);
					if (box.checkIfClicked(x, y))
					{
						SDL_StartTextInput();
					}
					else
					{
						SDL_StopTextInput();
					}
				}
				if (SDL_IsTextInputActive() == SDL_TRUE)
				{
					box.textListener(event);
				}
			}

			// Clear screen
			SDL_RenderClear(renderer);

			// Draw texture
			SDL_RenderCopy(renderer, coconut, nullptr, &dst);
			
			//Test for textBox
			SDL_SetRenderDrawColor(renderer, 150, 150, 150, 255);
			SDL_RenderFillRect(renderer, &rect);
			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
			box.genTexture();
			SDL_RenderCopy(renderer, box.getTexture(), nullptr, &(box.getTextureRect()));

			// Present to screen
			SDL_RenderPresent(renderer);

			// 60 FPS <- should implement delta time for more stable fps
			SDL_Delay(16);
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
