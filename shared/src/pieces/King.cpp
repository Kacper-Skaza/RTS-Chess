#include "../../headers/pieces/King.hpp"

King::King(const char SIDE) : SYMBOL(std::toupper(SIDE) == 'W' ? 'K' : 'k')
{
	// Throw exception when given wrong char for SIDE
	if (std::toupper(SIDE) != 'W' && std::toupper(SIDE) != 'B')
	{
		throw std::invalid_argument("received different value in Bishop constructor than \'W\' or \'B\'");
	}
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
	if (piece != this || piece == nullptr || target == nullptr)
		return false;

	// Check if move is castling
	if (std::toupper(target->getSymbol()) == 'R' &&
		std::isupper(piece->getSymbol()) == std::isupper(target->getSymbol()) &&
		piece->getFirstMove() == true &&
		target->getFirstMove() == true)
	{
		return true;
	}

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
