#pragma once

#include "Structures.hpp"
#include "Move.hpp"
#include "pieces/Piece.hpp"

#include <vector>
#include <memory>

class Board
{
private:
	std::vector<std::vector<std::unique_ptr<Piece>>> board;

public:
	Board();
	~Board() = default;

	const std::vector<std::vector<Piece *>> &getBoardFull() const;
	const std::vector<std::vector<char>> getBoardSymbol() const;

	void makeMove(const Move &move);
};
