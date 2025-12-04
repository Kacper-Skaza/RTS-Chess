#pragma once

#include <vector>
#include <memory>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "../../../shared/headers/Room.hpp"

class RoomView
{
private:
	SDL_Window *window;
	SDL_Renderer *renderer;
	std::vector<std::unique_ptr<Room>> rooms;

public:
	RoomView(SDL_Window *window, SDL_Renderer *renderer);
	~RoomView() = default;

	void render();
	void handleEvent(const SDL_Event &e);

	void updateRoom(std::unique_ptr<Room> room);
};
