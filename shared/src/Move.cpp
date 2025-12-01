#include "../headers/Move.hpp"

Move::Move(Piece *piece, std::pair<int, int> from, std::pair<int, int> to) : piece(piece), from(from), to(to)
{
	if (from.first < 0 || from.first >= BOARD_SIZE ||
		from.second < 0 || from.second >= BOARD_SIZE ||
		to.first < 0 || to.first >= BOARD_SIZE ||
		to.second < 0 || to.second >= BOARD_SIZE)
	{
		throw std::invalid_argument("[Move::Move] Coordinates out of range");
	}

	if (piece == nullptr)
	{
		throw std::invalid_argument("[Move::Move] Null piece pointer");
	}
}

const Piece *Move::getPiece() const noexcept
{
	return piece;
}

const std::pair<int, int> Move::getFrom() const noexcept
{
	return from;
}

const std::pair<int, int> Move::getTo() const noexcept
{
	return to;
}
