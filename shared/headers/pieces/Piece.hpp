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

	// TO BE DETERMINED LATER (INT COZ DEPENDANT ON MS)
	int cooldown = 2000;

public:
	Piece();
	virtual ~Piece() = default;

	char getSymbol() const noexcept;
	bool getFirstMove() const noexcept;

	void makeMove();
	virtual bool validateMove(const Move &move, const std::vector<std::vector<std::unique_ptr<Piece>>> &board) const;
};
