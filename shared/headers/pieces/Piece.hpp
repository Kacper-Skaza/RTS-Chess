#pragma once

class Move;

#include <cstdlib>
#include <utility>
#include <cctype>
#include <vector>
#include <stdexcept>
#include <memory>

class Piece
{
private:
	const char SYMBOL;
	bool firstMove = true;

public:
	Piece();
	~Piece() = default;

	char getSymbol() const noexcept;
	bool getFirstMove() const noexcept;

	bool validateMove(const Move &move, const std::vector<std::vector<std::unique_ptr<Piece>>> &board) const;
	void makeMove();
};
