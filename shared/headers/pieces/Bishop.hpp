#pragma once

#include "Piece.hpp"
#include "../Move.hpp"

#include <vector>
#include <cctype>
#include <stdexcept>

class Bishop: public Piece
{
public:
    explicit Bishop(const char SIDE, const int POS);
    bool validateMove(const Move &move, const std::vector<std::vector<Piece *>> &board) const override;

    static Bishop from_json(nlohmann::json& j);
};