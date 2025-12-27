#pragma once

#include "Piece.hpp"
#include "../Move.hpp"

#include <cstdlib>
#include <utility>
#include <cctype>
#include <vector>

class Rook : public Piece
{
public:
	explicit Rook(const char SIDE, const int POS);

	bool validateMove(const Move &move, const std::vector<std::vector<Piece *>> &board) const override;
};
