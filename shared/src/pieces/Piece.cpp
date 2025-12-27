#include "../../headers/pieces/Piece.hpp"
#include "Piece.hpp"

Piece::Piece() : SYMBOL('P'), ID(00) {}

Piece::Piece(const char SIDE, const char SYMBOL, const int ID): SYMBOL(std::toupper(SIDE) == 'W'? std::toupper(SYMBOL): std::tolower(SYMBOL)), ID(ID) {}

char Piece::getSymbol() const noexcept
{
	return SYMBOL;
}

int Piece::getID() const noexcept
{
    return ID;
}

bool Piece::getMoveCount() const noexcept
{
	return moveCount;
}

void Piece::makeMove()
{
	moveCount++;
}

bool Piece::validateMove(const Move &move, const std::vector<std::vector<Piece *>> &board) const
{
	// Suppress warnings
	(void)move;
	(void)board;

	// Return
	return false;
}
