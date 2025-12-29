#pragma once

class Piece;

#include "dependencies/Json.hpp"

#include "pieces/Piece.hpp"

#include "Structures.hpp"

#include <stdexcept>
#include <utility>
#include <memory>

class Move
{
private:
	Piece *piece;
	std::pair<int, int> from;
	std::pair<int, int> to;
	bool recreated = false;

public:
	Move();
	Move(Piece *piece, std::pair<int, int> from, std::pair<int, int> to);
	~Move();

	const Piece *getPiece() const noexcept;
	const std::pair<int, int> getFrom() const noexcept;
	const std::pair<int, int> getTo() const noexcept;

	friend void from_json(const nlohmann::json& j, Move& p);
	friend void to_json(nlohmann::json& j, const Move& p);
};