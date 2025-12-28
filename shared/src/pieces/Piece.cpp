#include "../../headers/pieces/Piece.hpp"

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

int Piece::getCooldown() const noexcept
{
	auto timeDif = std::chrono::duration_cast<std::chrono::milliseconds>(lastMoveTime - std::chrono::system_clock::now()).count();
	return  timeDif - MAX_COOLDOWN;
}

void Piece::makeMove()
{
	moveCount++;
	lastMoveTime = std::chrono::system_clock::now();
}

bool Piece::validateMove(const Move &move, const std::vector<std::vector<Piece *>> &board) const
{
	// Suppress warnings
	(void)move;
	(void)board;

	// Return
	return false;
}
