#include "../../headers/pieces/Rook.hpp"

Rook::Rook(const char SIDE) : SYMBOL(std::toupper(SIDE) == 'W' ? 'R' : 'r')
{
	// Throw exception when given wrong char for SIDE
	if (std::toupper(SIDE) != 'W' && std::toupper(SIDE) != 'B')
	{
		throw std::invalid_argument("received different value in Bishop constructor than \'W\' or \'B\'");
	}
}

bool Rook::validateMove(const Move &move, const std::vector<std::vector<std::unique_ptr<Piece>>> &board) const
{
	const Piece *piece = move.getPiece();
	std::pair<int, int> from = move.getFrom();
	std::pair<int, int> to = move.getTo();

	const Piece *target = board[to.first][to.second].get();
	int fx = from.first;
	int fy = from.second;
	int tx = to.first;
	int ty = to.second;

	// Validate piece
	if (piece != this)
		return false;

	// Check if move is in straight line
	if (!((fx == tx && fy != ty) || (fx != tx && fy == ty)))
		return false;

	// Check for obstruction
	if (fx == tx)
	{
		// Horizontal move
		// Right
		for (int y = fy + 1; y < ty; y++)
			if (board[fx][y]->getSymbol() != ' ')
				return false;

		// Left
		for (int y = fy - 1; y > ty; y--)
			if (board[fx][y]->getSymbol() != ' ')
				return false;
	}
	else
	{
		// Vertical move
		// Down
		for (int x = fx + 1; x < tx; x++)
			if (board[x][fy]->getSymbol() != ' ')
				return false;

		// Up
		for (int x = fx - 1; x > tx; x--)
			if (board[x][fy]->getSymbol() != ' ')
				return false;
	}

	// Check if destination square is empty or colors are different
	if (target->getSymbol() == ' ')
		return true;
	if (std::isupper(piece->getSymbol()) != std::isupper(target->getSymbol()))
		return true;

	return false;
}
