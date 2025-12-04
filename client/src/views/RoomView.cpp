#include "../../headers/views/RoomView.hpp"

#include <iostream>
#include <string>

RoomView::RoomView(SDL_Window *win, SDL_Renderer *rend) : window(win), renderer(rend) {}

void RoomView::updateRoom(std::unique_ptr<Room> room)
{
	rooms.push_back(room);
}

void RoomView::render()
{
	SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
	SDL_RenderClear(renderer);

	int y = 50;
	for (size_t i = 0; i < rooms.size(); i++)
	{
		SDL_Rect rect = {50, y, 500, 60};

		if (rooms[i]->isMatchStarted())
			SDL_SetRenderDrawColor(renderer, 200, 100, 100, 255);
		else if (rooms[i]->isMatchReady())
			SDL_SetRenderDrawColor(renderer, 100, 200, 100, 255);
		else
			SDL_SetRenderDrawColor(renderer, 100, 100, 200, 255);

		SDL_RenderFillRect(renderer, &rect);

		std::string text = "Room " + std::to_string(i) + ": " + std::to_string(rooms[i]->getPlayerCount()) + "/" + std::to_string(rooms[i]->getMaxPlayerCount()) + " players";

		// SDL_Color white = {255, 255, 255, 255};
		// SDL_Surface *surf = TTF_RenderText_Blended(font, text.c_str(), white);
		// SDL_Texture *tex = SDL_CreateTextureFromSurface(renderer, surf);
		// SDL_Rect textRect = {rect.x + 10, rect.y + 10, surf->w, surf->h};
		// SDL_RenderCopy(renderer, tex, nullptr, &textRect);
		// SDL_FreeSurface(surf);
		// SDL_DestroyTexture(tex);

		y += 80;
	}

	SDL_RenderPresent(renderer);
}

void RoomView::handleEvent(const SDL_Event &e)
{
	if (e.type == SDL_MOUSEBUTTONDOWN)
	{
		int mx = e.button.x;
		int my = e.button.y;
		int y = 50;

		for (size_t i = 0; i < rooms.size(); i++)
		{
			SDL_Rect rect = {50, y, 500, 60};
			if (mx >= rect.x && mx <= rect.x + rect.w &&
				my >= rect.y && my <= rect.y + rect.h)
			{
				std::cout << "Clicked on room " << i << "\n";
			}
			y += 80;
		}
	}
}
