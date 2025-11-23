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
public:
    bool enPassant = false;
    Pawn(const char SIDE);
    char getSymbol() const noexcept;
    bool validateMove(const Move &move, const std::vector<std::vector<std::unique_ptr<Piece>>> &board) const;
};