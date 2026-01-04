#pragma once

class Piece;
class Board;

#include "dependencies/Json.hpp"

#include "pieces/Piece.hpp"
#include "Board.hpp"

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

public:
	Move();
	Move(Piece *piece, std::pair<int, int> from, std::pair<int, int> to);
	~Move() = default;

	const Piece *getPiece() const noexcept;
	const std::pair<int, int> getFrom() const noexcept;
	const std::pair<int, int> getTo() const noexcept;

	static Move from_json(const nlohmann::json& j, Board* board);
	friend void to_json(nlohmann::json& j, const Move& p);
};