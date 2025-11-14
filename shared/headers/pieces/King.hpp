#ifndef KING_HPP
#define KING_HPP
#include "../Move.hpp"
#include <cstdlib>
#include <utility>
#include <cctype>
#include <vector>

class King
{
private:
	const char symbol = 'K';

public:
	char getSymbol() const noexcept;
	bool validateMove(const Move &move, const std::vector<std::vector<char>> &board) const;
};

#endif // KING_HPP