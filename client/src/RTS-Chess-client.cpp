#include <iostream>
#include "../../shared/headers/Board.hpp"
#include "../../shared/headers/pieces/Pawn.hpp"

int main()
{
	Board board = {};
	std::cout << ((Pawn *)(board.getBoardFull()[1][1].get()))->enPassant;
	return 0;
}
