#include "../../headers/pieces/Rook.hpp"

char Rook::getSymbol() const noexcept
{
	return symbol;
}

bool Rook::validateMove(const Move &move, const std::vector<std::vector<char>> &board) const
{
	std::pair<int, int> from = move.getFrom();
	std::pair<int, int> to = move.getTo();

	char target = board[to.first][to.second];
	int fx = from.first;
	int fy = from.second;
	int tx = to.first;
	int ty = to.second;

	// Check if move is in straight line
	if (!((fx == tx && fy != ty) || (fx != tx && fy == ty)))
		return false;

	// Check for obstruction
	if (fx == tx)
	{
		// Horizontal move
		// Right
		for (int y = fy + 1; y < ty; y++)
			if (board[fx][y] != ' ')
				return false;

		// Left
		for (int y = fy - 1; y > ty; y--)
			if (board[fx][y] != ' ')
				return false;
	}
	else
	{
		// Vertical move
		// Down
		for (int x = fx + 1; x < tx; x++)
			if (board[x][fy] != ' ')
				return false;

		// Up
		for (int x = fx - 1; x > tx; x--)
			if (board[x][fy] != ' ')
				return false;
	}

	// Check if destination square is empty or colors are different
	if (target == ' ')
		return true;
	if (std::isupper(move.getPiece()) != std::isupper(target))
		return true;

	return false;
}
