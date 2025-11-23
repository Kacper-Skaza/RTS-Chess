#include "../../headers/pieces/King.hpp"

King::King(const char side) : SYMBOL(std::toupper(side) == 'W' ? 'K' : 'k')
{
	// Throw exception when given wrong char for side
	if (std::toupper(side) != 'W' || std::toupper(side) != 'B')
	{
		throw std::invalid_argument("received different value in Bishop constructor than \'W\' or \'B\'");
	}
}

char King::getSymbol() const noexcept
{
	return SYMBOL;
}

bool King::validateMove(const Move &move, const std::vector<std::vector<std::unique_ptr<Piece>>> &board) const
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

	// Check if move is one square in any direction
	if (!(dx <= 1 && dy <= 1 && (dx != 0 || dy != 0)))
		return false;

	// Check if destination square is empty or colors are different
	if (target->getSymbol() == ' ')
		return true;
	if (std::isupper(piece->getSymbol()) != std::isupper(target->getSymbol()))
		return true;

	return false;
}
