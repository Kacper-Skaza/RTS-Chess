#pragma once

#include "Piece.hpp"
#include "../Move.hpp"

#include <vector>
#include <cctype>
#include <stdexcept>

class Bishop: public Piece
{
private:
    const char SYMBOL;
public:
    explicit Bishop(const char SIDE);
    bool validateMove(const Move &move, const std::vector<std::vector<Piece *>> &board) const override;
};