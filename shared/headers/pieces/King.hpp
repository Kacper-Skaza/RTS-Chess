#pragma once

#include "Piece.hpp"
#include "../Move.hpp"

#include <cstdlib>
#include <utility>
#include <cctype>
#include <vector>

class King : public Piece
{
public:
	explicit King(const char SIDE, const int POS);

	bool validateMove(const Move &move, const std::vector<std::vector<Piece *>> &board) const override;

    static King from_json(nlohmann::json &j);
};
