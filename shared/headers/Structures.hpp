#pragma once

#define PLATFORM_UNKNOWN 1
#define PLATFORM_WINDOWS 2
#define PLATFORM_LINUX 3

#if defined(_WIN32) || defined(_WIN64)
	#define PLATFORM PLATFORM_WINDOWS
#elif defined(LINUX) || defined(__linux__)
	#define PLATFORM PLATFORM_LINUX
#else
	#define PLATFORM PLATFORM_UNKNOWN
	#error "Unknown platform!"
#endif

constexpr int BOARD_SIZE = 8;

enum class MatchEndReasons
{
	WHITE_WON = 1,
	BLACK_WON = 2,
	PLAYER_LEFT = 4,
	CONNECTION_ERROR = 8,
	GAME_QUIT = 16,
	DATA_ERROR = 32,
	UNKNOWN = 2048
};

enum class ChessSide
{
	WHITE = 0,
	BLACK = 1
};
