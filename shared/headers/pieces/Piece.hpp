#pragma once

#include "../Move.hpp"

#include <cstdlib>
#include <utility>
#include <cctype>
#include <vector>
#include <stdexcept>

class Piece
{
private:
	const char SYMBOL;

public:
	Piece();
	~Piece() = default;
	char getSymbol() const noexcept;
	bool validateMove(const Move &move, const std::vector<std::vector<std::unique_ptr<Piece>>> &board) const;
};
