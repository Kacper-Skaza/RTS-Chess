#include "../../headers/pieces/Piece.hpp"

Piece::Piece() : SYMBOL('P') {}

char Piece::getSymbol() const noexcept
{
	return SYMBOL;
}

bool Piece::getFirstMove() const noexcept
{
	return firstMove;
}

void Piece::makeMove()
{
	firstMove = false;
}

bool Piece::validateMove(const Move &move, const std::vector<std::vector<std::unique_ptr<Piece>>> &board) const
{
	// Avoid warnings
	(void)move;
	(void)board;

	// Return
	return false;
}
