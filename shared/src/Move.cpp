#include "../headers/Move.hpp"

// Konstruktor
Move::Move(char piece, std::pair<int, int> from, std::pair<int, int> to)
	: piece(piece), from(from), to(to) {}

// Gettery
char Move::getPiece() const noexcept
{
	return piece;
}
std::pair<int, int> Move::getFrom() const noexcept
{
	return from;
}
std::pair<int, int> Move::getTo() const noexcept
{
	return to;
}
