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
	board.resize(BOARD_SIZE);
	for (int r = 0; r < BOARD_SIZE; r++)
		board[r].resize(BOARD_SIZE);

	// White pieces
	board[0][0] = std::make_unique<Rook>('W');
	board[0][1] = std::make_unique<Knight>('W');
	board[0][2] = std::make_unique<Bishop>('W');
	board[0][3] = std::make_unique<Queen>('W');
	board[0][4] = std::make_unique<King>('W');
	board[0][5] = std::make_unique<Bishop>('W');
	board[0][6] = std::make_unique<Knight>('W');
	board[0][7] = std::make_unique<Rook>('W');
	for (int c = 0; c < BOARD_SIZE; c++)
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
	for (int c = 0; c < BOARD_SIZE; c++)
		board[6][c] = std::make_unique<Pawn>('B');
}

const std::vector<std::vector<std::unique_ptr<Piece>>> &Board::getBoardFull() const noexcept
{
	return board;
}

const std::vector<std::vector<char>> Board::getBoardSymbol() const
{
	std::vector<std::vector<char>> symbolBoard(BOARD_SIZE, std::vector<char>(BOARD_SIZE));

	for (int r = 0; r < BOARD_SIZE; r++)
	{
		for (int c = 0; c < BOARD_SIZE; c++)
		{
			if (board[r][c])
				symbolBoard[r][c] = board[r][c]->getSymbol();
			else
				symbolBoard[r][c] = ' ';
		}
	}

	return symbolBoard;
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

	// Check if move is castling
	if (std::toupper(board[from.first][from.second]->getSymbol()) == 'K' &&
		std::toupper(board[to.first][to.second]->getSymbol()) == 'R' &&
		std::isupper(board[from.first][from.second]->getSymbol()) == std::isupper(board[to.first][to.second]->getSymbol()))
	{
		// Make castling move
		int kingFromCol = from.second, kingToCol = 0;
		int rookFromCol = to.second, rookToCol = 0;

		if (to.second > from.second)
		{
			kingToCol = kingFromCol + 2;
			rookToCol = to.second - 1;
		}
		else
		{
			kingToCol = kingFromCol - 2;
			rookToCol = to.second + 1;
		}

		// Move rook
		this->board[from.first][rookFromCol]->makeMove();
		this->board[to.first][rookToCol] = std::move(board[from.first][rookFromCol]);
		this->board[from.first][rookFromCol].reset();
		// Move king
		this->board[from.first][kingFromCol]->makeMove();
		this->board[to.first][kingToCol] = std::move(board[from.first][kingFromCol]);
		this->board[from.first][kingFromCol].reset();
		return;
	}

	// Make default move
	this->board[from.first][from.second]->makeMove();
	this->board[to.first][to.second] = std::move(board[from.first][from.second]);
	this->board[from.first][from.second].reset();
}
