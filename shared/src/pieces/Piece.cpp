#include "../../headers/pieces/Piece.hpp"

Piece::Piece() : SYMBOL('P') {}

char Piece::getSymbol() const noexcept
{
	return SYMBOL;
}

bool Piece::validateMove(const Move &move, const std::vector<std::vector<std::unique_ptr<Piece>>> &board) const
{
	return false;
}
