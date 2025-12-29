#pragma once

#include "Piece.hpp"
#include "../Move.hpp"

#include <vector>
#include <cctype>
#include <stdexcept>

class Pawn: public Piece
{
public:
    explicit Pawn(const char SIDE, const int POS);
    bool validateMove(const Move &move, const std::vector<std::vector<Piece *>> &board) const override;
    static Pawn from_json(nlohmann::json &j);
};