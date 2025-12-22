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
    explicit Pawn(const char SIDE);
    bool validateMove(const Move &move, const std::vector<std::vector<Piece *>> &board) const override;
};