#pragma once

#include "Move.hpp"
#include "pieces/Piece.hpp"

#include <vector>
#include <memory>

class Board
{
private:
	static constexpr int SIZE = 8;
	std::vector<std::vector<std::unique_ptr<Piece>>> board;

public:
	Board();
	~Board() = default;

	const std::vector<std::vector<std::unique_ptr<Piece>>> &getBoardFull() const noexcept;
	const std::vector<std::vector<char>> getBoardSymbol() const;
	const int getBoardSize() const noexcept;

	void makeMove(const Move &move);
};
