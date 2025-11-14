#ifndef MOVE_HPP
#define MOVE_HPP
#include <utility>

class Move
{
private:
	char piece;
	std::pair<int, int> from;
	std::pair<int, int> to;

public:
	// Constructor
	Move(char piece, std::pair<int, int> from, std::pair<int, int> to);

	// Getters
	char getPiece() const noexcept;
	std::pair<int, int> getFrom() const noexcept;
	std::pair<int, int> getTo() const noexcept;
};

#endif // MOVE_HPP