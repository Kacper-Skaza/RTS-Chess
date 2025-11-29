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
    bool validateMove(const Move &move, const std::vector<std::vector<std::unique_ptr<Piece>>> &board) const override;
};