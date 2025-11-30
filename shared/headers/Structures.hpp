#pragma once

enum MatchEndReasons 
{
    WHITE_WON = 1,
    BLACK_WON = 2,
    PLAYER_LEFT = 4,
    CONNECTION_ERROR = 8,
    GAME_QUIT = 16,
    DATA_ERROR = 32,
    UNKNOWN = 2048
};

enum ChessSide
{
    WHITE = 0,
    BLACK = 1
};