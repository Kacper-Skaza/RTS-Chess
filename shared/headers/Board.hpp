#pragma once

#include "dependencies/Json.hpp"

#include "Structures.hpp"
#include "Move.hpp"
#include "pieces/Piece.hpp"

#include <vector>
#include <memory>
#include <algorithm>
#include <cctype>

class Board
{
private:
	std::vector<std::vector<std::unique_ptr<Piece>>> board;

public:
	Board();
	~Board() = default;

	void setSpace(int row, int col, std::unique_ptr<Piece>& piece);
	std::unique_ptr<Piece>& getSpace(int row, int col);

	const std::vector<std::vector<Piece *>> getBoardFull() const;
	const std::vector<std::vector<char>> getBoardSymbol() const;

	bool makeMove(const Move &move);

	static void from_json(const nlohmann::json& j, Board& b);
	friend void to_json(nlohmann::json& j, const Board& p);
};
