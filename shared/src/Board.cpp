#include "../headers/Board.hpp"

#include "../headers/pieces/Bishop.hpp"
#include "../headers/pieces/King.hpp"
#include "../headers/pieces/Knight.hpp"
#include "../headers/pieces/Pawn.hpp"
#include "../headers/pieces/Queen.hpp"
#include "../headers/pieces/Rook.hpp"

Board::Board()
{
	// Initialize board
	// board.assign(SIZE, std::vector<std::unique_ptr<Piece>>(SIZE, nullptr));
	board.resize(SIZE);
    for (auto& row : board)
        row.resize(SIZE); // wszystkie elementy są nullptr

	// White pieces
	board[0][0] = std::make_unique<Rook>('W');
	board[0][1] = std::make_unique<Knight>('W');
	board[0][2] = std::make_unique<Bishop>('W');
	board[0][3] = std::make_unique<Queen>('W');
	board[0][4] = std::make_unique<King>('W');
	board[0][5] = std::make_unique<Bishop>('W');
	board[0][6] = std::make_unique<Knight>('W');
	board[0][7] = std::make_unique<Rook>('W');
	for (int c = 0; c < SIZE; c++)
		board[1][c] = std::make_unique<Pawn>('W');

	// Black pieces
	board[7][0] = std::make_unique<Rook>('B');
	board[7][1] = std::make_unique<Knight>('B');
	board[7][2] = std::make_unique<Bishop>('B');
	board[7][3] = std::make_unique<Queen>('B');
	board[7][4] = std::make_unique<King>('B');
	board[7][5] = std::make_unique<Bishop>('B');
	board[7][6] = std::make_unique<Knight>('B');
	board[7][7] = std::make_unique<Rook>('B');
	for (int c = 0; c < SIZE; c++)
		board[6][c] = std::make_unique<Pawn>('B');
}

const std::vector<std::vector<std::unique_ptr<Piece>>> &Board::getBoardFull() const noexcept
{
	return board;
}

const std::vector<std::vector<char>> Board::getBoardSymbol() const
{
	std::vector<std::vector<char>> symbolBoard(SIZE, std::vector<char>(SIZE));

	for (int r = 0; r < SIZE; r++)
	{
		for (int c = 0; c < SIZE; c++)
		{
			if (board[r][c])
				symbolBoard[r][c] = board[r][c]->getSymbol();
			else
				symbolBoard[r][c] = ' ';
		}
	}

	return symbolBoard;
}

const int Board::getBoardSize() const noexcept
{
	return SIZE;
}

void Board::makeMove(const Move &move)
{
	const Piece *piece = move.getPiece();
	std::pair<int, int> from = move.getFrom();
	std::pair<int, int> to = move.getTo();

	// Validate move
	if (!board[from.first][from.second])
	{
		return;
	}
	if (board[from.first][from.second].get() != piece)
	{
		return;
	}
	if (!board[from.first][from.second]->validateMove(move, this->board))
	{
		return;
	}

	// Make move
	this->board[to.first][to.second] = std::move(board[from.first][from.second]);
	this->board[from.first][from.second].reset();
}
