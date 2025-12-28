#pragma once

#include <iostream>

#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

#include "views/View.hpp"
#include "views/ConnectView.hpp"
#include "views/GameView.hpp"
#include "views/LobbyView.hpp"
#include "views/RoomView.hpp"

#include "SDLFontManager.hpp"
#include "SDLTextureManager.hpp"

#include "../../shared/headers/Board.hpp"
#include "../../shared/headers/Move.hpp"
#include "../../shared/headers/Room.hpp"
#include "../../shared/headers/Structures.hpp"
#include "../../shared/headers/User.hpp"
#include "../../shared/headers/ConnectionManager.hpp"

#include "../../shared/headers/dependencies/Json.hpp"

void mainLoop();