#include "../headers/Move.hpp"

Move::Move(Piece *piece, std::pair<int, int> from, std::pair<int, int> to) : piece(piece), from(from), to(to) {}

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
