#include "../../headers/pieces/Knight.hpp"

Knight::Knight(const char SIDE) : SYMBOL(std::toupper(SIDE) == 'W' ? 'N' : 'n')
{
	//Throw exception when given wrong char for SIDE
    if (std::toupper(SIDE) != 'W' && std::toupper(SIDE) != 'B')
    {
        throw std::invalid_argument("received different value in Bishop constructor than \'W\' or \'B\'");
    }
}

char Knight::getSymbol() const noexcept
{
	return SYMBOL;
}

bool Knight::getFirstMove() const noexcept
{
	return firstMove;
}

bool Knight::validateMove(const Move &move, const std::vector<std::vector<std::unique_ptr<Piece>>> &board) const
{
	const Piece *piece = move.getPiece();
	std::pair<int, int> from = move.getFrom();
	std::pair<int, int> to = move.getTo();

	const Piece *target = board[to.first][to.second].get();
	int dx = std::abs(to.first - from.first);
	int dy = std::abs(to.second - from.second);

	// Validate piece
	if (piece != this)
		return false;

	// Check if move is in L-shape
	if (!((dx == 2 && dy == 1) || (dx == 1 && dy == 2)))
		return false;

	// Check if destination square is empty or colors are different
	if (target->getSymbol() == ' ')
		return true;
	if (std::isupper(piece->getSymbol()) != std::isupper(target->getSymbol()))
		return true;

	return false;
}

void Knight::makeMove()
{
	firstMove = false;
}
