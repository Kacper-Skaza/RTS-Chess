#pragma once

class Piece;

#include <stdexcept>
#include <utility>
#include <memory>

class Move
{
private:
	Piece *piece;
	std::pair<int, int> from;
	std::pair<int, int> to;

public:
	Move(Piece *piece, std::pair<int, int> from, std::pair<int, int> to);
	~Move() = default;

	const Piece *getPiece() const noexcept;
	const std::pair<int, int> getFrom() const noexcept;
	const std::pair<int, int> getTo() const noexcept;
};
