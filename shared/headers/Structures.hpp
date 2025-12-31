#pragma once

#include <memory>
class ConnectionManager;
class User;
class Room;

// Platform identifiers
#define PLATFORM_UNKNOWN 1
#define PLATFORM_WINDOWS 2
#define PLATFORM_LINUX 3

// Detect current platform
#if defined(_WIN32) || defined(_WIN64)
    #define PLATFORM PLATFORM_WINDOWS
#elif defined(LINUX) || defined(__linux__)
    #define PLATFORM PLATFORM_LINUX
#else
    #define PLATFORM PLATFORM_UNKNOWN
    #error "Unknown platform!"
#endif

// Default chessboard size
constexpr int BOARD_SIZE = 8;

// Structures
struct Client
{
    std::unique_ptr<ConnectionManager> connection;
    std::unique_ptr<User> user;
    Room *room;
};

// Enums
enum class MatchEndReasons
{
    NOT_ENDED = 0,
    WHITE_WON = 1,
    BLACK_WON = 2,
    PLAYER_LEFT = 4,
    CONNECTION_ERROR = 8,
    GAME_QUIT = 16,
    DATA_ERROR = 32,
    KING_DIED = 64,
    UNKNOWN = 2048
};

enum class ChessSide
{
    UNKNOWN = 0,
    WHITE = 1,
    BLACK = 2
};
