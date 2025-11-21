#pragma once

#include "Piece.hpp"
#include "../Move.hpp"

#include <vector>
#include <cctype>
#include <stdexcept>

class Queen : public Piece 
{
private:
    const char SYMBOL;
public:
    Queen(const char SIDE);
    char getSymbol() const noexcept;
    bool validateMove(const Move &move, const std::vector<std::vector<char>> &board) const;
};