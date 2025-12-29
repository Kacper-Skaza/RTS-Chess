#include "../../headers/pieces/Rook.hpp"

Rook::Rook(const char SIDE, const int POS): Piece(SIDE, 'R', POS)
{
	// Throw exception when given wrong char for SIDE
	if (std::toupper(SIDE) != 'W' && std::toupper(SIDE) != 'B')
	{
		throw std::invalid_argument("received different value in Bishop constructor than \'W\' or \'B\'");
	}
}

bool Rook::validateMove(const Move &move, const std::vector<std::vector<Piece *>> &board) const
{
	const Piece *piece = move.getPiece();
	std::pair<int, int> from = move.getFrom();
	std::pair<int, int> to = move.getTo();

	const Piece *target = board[to.first][to.second];
	int fx = from.first;
	int fy = from.second;
	int tx = to.first;
	int ty = to.second;

	// Validate piece
	if (piece != this || piece == nullptr || target == nullptr)
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
			if (board[fx][y])
				if (board[fx][y]->getSymbol() != ' ')
					return false;

		// Left
		for (int y = fy - 1; y > ty; y--)
			if (board[fx][y])
				if (board[fx][y]->getSymbol() != ' ')
					return false;
	}
	else
	{
		// Vertical move
		// Down
		for (int x = fx + 1; x < tx; x++)
			if (board[x][fy])
				if (board[x][fy]->getSymbol() != ' ')
					return false;

		// Up
		for (int x = fx - 1; x > tx; x--)
			if (board[x][fy])
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

Rook Rook::from_json(nlohmann::json &j)
{
    return Rook(std::isupper(j.at("symbol").get<char>())? 'W': 'B', j.at("id").get<int>());
}