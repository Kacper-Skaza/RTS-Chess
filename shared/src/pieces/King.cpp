#include "../../headers/pieces/King.hpp"

char King::getSymbol() const noexcept
{
	return symbol;
}

bool King::validateMove(const Move &move, const std::vector<std::vector<char>> &board) const
{
	std::pair<int, int> from = move.getFrom();
	std::pair<int, int> to = move.getTo();

	char target = board[to.first][to.second];
	int dx = std::abs(to.first - from.first);
	int dy = std::abs(to.second - from.second);

	// Check if move is one square in any direction
	if (!(dx <= 1 && dy <= 1 && (dx != 0 || dy != 0)))
		return false;

	// Check if destination square is empty or colors are different
	if (target == ' ')
		return true;
	if (std::isupper(move.getPiece()) != std::isupper(target))
		return true;

	return false;
}
