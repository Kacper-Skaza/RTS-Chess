#pragma once

#include "Piece.hpp"
#include "../Move.hpp"

#include <vector>
#include <cctype>
#include <stdexcept>

class Pawn: public Piece
{
private:
    const char SYMBOL;
    bool firstMove = true;
    bool enPassant = false;
public:
    Pawn(const char SIDE);
    char getSymbol() const noexcept;
    bool validateMove(const Move &move, const std::vector<std::vector<char>> &board) const;
};