#pragma once

class Move;

#include <cstdlib>
#include <utility>
#include <cctype>
#include <vector>
#include <stdexcept>
#include <memory>

#define MAX_COOLDOWN 2000

class Piece
{
protected:
	const char SYMBOL;
	int moveCount = 0;
	const int ID; 

	// TO BE DETERMINED LATER (INT COZ DEPENDANT ON MS)
	int cooldown = MAX_COOLDOWN;

public:
	Piece();
	Piece(const char SIDE, const char SYMBOL, const int ID);
	virtual ~Piece() = default;

	int  getID() const noexcept;
	char getSymbol() const noexcept;
	bool getMoveCount() const noexcept;
	int getCooldown() const noexcept;

	void makeMove();
	virtual bool validateMove(const Move &move, const std::vector<std::vector<Piece *>> &board) const;
};
