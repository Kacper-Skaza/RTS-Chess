#pragma once

#include "Piece.hpp"
#include "../Move.hpp"

#include <cstdlib>
#include <utility>
#include <cctype>
#include <vector>

class Knight : public Piece
{
private:
	const char SYMBOL;

public:
	Knight(const char side);
	char getSymbol() const noexcept;
	bool validateMove(const Move &move, const std::vector<std::vector<std::unique_ptr<Piece>>> &board) const;
};
