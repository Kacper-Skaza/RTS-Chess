#include "../../headers/pieces/King.hpp"

King::King(const char SIDE, const int POS): Piece(SIDE, 'K', POS)
{
	// Throw exception when given wrong char for SIDE
	if (std::toupper(SIDE) != 'W' && std::toupper(SIDE) != 'B')
	{
		throw std::invalid_argument("received different value in Bishop constructor than \'W\' or \'B\'");
	}
}

bool King::validateMove(const Move &move, const std::vector<std::vector<Piece *>> &board) const
{
	const Piece *piece = move.getPiece();
	std::pair<int, int> from = move.getFrom();
	std::pair<int, int> to = move.getTo();

	const Piece *target = board[to.first][to.second];
	int dx = std::abs(to.first - from.first);
	int dy = std::abs(to.second - from.second);

	// Validate piece
	if (piece != this || piece == nullptr)
		return false;

    // Check if move is castling
    if (target)
    {
        if (from.first == to.first &&
            std::toupper(target->getSymbol()) == 'R' &&
            std::isupper(piece->getSymbol()) == std::isupper(target->getSymbol()) &&
            piece->getMoveCount() == 0 &&
            target->getMoveCount() == 0)
        {
            // Check for obstruction in horizontal move
            // Right
            for (int y = from.second + 1; y < to.second; y++)
                if (board[from.first][y])
                    return false;

            // Left
            for (int y = from.second - 1; y > to.second; y--)
                if (board[from.first][y])
                    return false;

            return true;
        }
    }

	// Check if move is one square in any direction
	if (!(dx <= 1 && dy <= 1 && (dx != 0 || dy != 0)))
		return false;

	// Check if destination square is empty or colors are different
	if (target == nullptr)
		return true;
	if (std::isupper(piece->getSymbol()) != std::isupper(target->getSymbol()))
		return true;

	return false;
}

King King::from_json(nlohmann::json &j)
{
    return King(std::isupper(j.at("symbol").get<char>())? 'W': 'B', j.at("id").get<int>());
}
