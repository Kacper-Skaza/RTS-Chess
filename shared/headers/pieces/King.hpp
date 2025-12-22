#pragma once

#include "Piece.hpp"
#include "../Move.hpp"

#include <cstdlib>
#include <utility>
#include <cctype>
#include <vector>

class King : public Piece
{
private:
	const char SYMBOL;

public:
	explicit King(const char SIDE);

	bool validateMove(const Move &move, const std::vector<std::vector<Piece *>> &board) const override;
};
