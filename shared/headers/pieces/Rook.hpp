#ifndef ROOK_HPP
#define ROOK_HPP
#include "../Move.hpp"
#include <cstdlib>
#include <utility>
#include <cctype>
#include <vector>

class Rook
{
private:
	const char symbol = 'R';

public:
	char getSymbol() const noexcept;
	bool validateMove(const Move &move, const std::vector<std::vector<char>> &board) const;
};

#endif // ROOK_HPP