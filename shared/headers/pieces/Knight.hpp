#ifndef KNIGHT_HPP
#define KNIGHT_HPP
#include "../Move.hpp"
#include <cstdlib>
#include <utility>
#include <cctype>
#include <vector>

class Knight
{
private:
	const char symbol = 'N';

public:
	char getSymbol() const noexcept;
	bool validateMove(const Move &move, const std::vector<std::vector<char>> &board) const;
};

#endif // KNIGHT_HPP