#include "../../headers/pieces/Piece.hpp"

Piece::Piece(): SYMBOL('P'){}

char Piece::getSymbol() const noexcept
{
    return this->SYMBOL;
}

bool Piece::validateMove(const Move &move, const std::vector<std::vector<char>> &board) const
{
    return true;
}