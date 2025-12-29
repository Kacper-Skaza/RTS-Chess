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
	board[0][0] = std::make_unique<Rook>    ('W', 0);
	board[0][1] = std::make_unique<Knight>  ('W', 1);
	board[0][2] = std::make_unique<Bishop>  ('W', 2);
	board[0][3] = std::make_unique<Queen>   ('W', 3);
	board[0][4] = std::make_unique<King>    ('W', 4);
	board[0][5] = std::make_unique<Bishop>  ('W', 5);
	board[0][6] = std::make_unique<Knight>  ('W', 6);
	board[0][7] = std::make_unique<Rook>    ('W', 7);
	for (int c = 0; c < BOARD_SIZE; c++)
		board[1][c] = std::make_unique<Pawn>('W', 10+c);

	// Black pieces
	board[7][0] = std::make_unique<Rook>    ('B', 70);
	board[7][1] = std::make_unique<Knight>  ('B', 71);
	board[7][2] = std::make_unique<Bishop>  ('B', 72);
	board[7][3] = std::make_unique<Queen>   ('B', 73);
	board[7][4] = std::make_unique<King>    ('B', 74);
	board[7][5] = std::make_unique<Bishop>  ('B', 75);
	board[7][6] = std::make_unique<Knight>  ('B', 76);
	board[7][7] = std::make_unique<Rook>    ('B', 77);
	for (int c = 0; c < BOARD_SIZE; c++)
		board[6][c] = std::make_unique<Pawn>('B', 60+c);
}

const std::vector<std::vector<Piece *>> Board::getBoardFull() const
{
	std::vector<std::vector<Piece *>> pointerBoard(BOARD_SIZE, std::vector<Piece*>(BOARD_SIZE));

	for (int r = 0; r < BOARD_SIZE; r++)
	{
		for (int c = 0; c < BOARD_SIZE; c++)
		{
			if (board[r][c])
				pointerBoard[r][c] = board[r][c].get();
			else
				pointerBoard[r][c] = nullptr;
		}
	}

	return pointerBoard;
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
	if (!board[from.first][from.second]->validateMove(move, this->getBoardFull()))
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

	//Check if move is en passant
	if (this->board[to.first][to.second] == nullptr && 
		this->board[from.first][to.second] != nullptr &&
		std::toupper(this->board[from.first][to.second].get()->getSymbol()) == 'P' &&
		std::toupper(this->board[from.first][from.second].get()->getSymbol()) == 'P' &&
		std::isupper(this->board[from.first][to.second].get()->getSymbol()) != std::isupper(this->board[from.first][from.second].get()->getSymbol()) &&
		this->board[from.first][to.second].get()->getMoveCount() == 1)
	{
		this->board[from.first][from.second]->makeMove();
		this->board[to.first][to.second] = std::move(board[from.first][from.second]);
		this->board[from.first][from.second].reset();
		this->board[from.first][to.second].reset();
		return;
	}

	// Make default move
	this->board[from.first][from.second]->makeMove();
	this->board[to.first][to.second] = std::move(board[from.first][from.second]);
	this->board[from.first][from.second].reset();
}

void Board::from_json(const nlohmann::json &j, Board& b)
{
	int i = 0, k = 0;
	for (const auto &row : j.at("board"))
	{
		k = 0;
		for (const auto &cell : row)
		{
			b.board[i][k].reset();
			if (cell.at("id").get<const int>() != -1)
			{
				b.board[i][k] = std::make_unique<Piece>(
					std::isupper(cell.at("symbol").get<char>()) ? 'W' : 'B', 
					cell.at("symbol").get<char>(), cell.at("id").get<int>());
			}
			k++;
		}
		i++;
	}
}

void to_json(nlohmann::json &j, const Board &p)
{
	// nlohmann::json jpart = nlohmann::json::array();
	// for (auto& p : p.getBoardFull())
	// {
	// 	jpart.push_back(nlohmann::json::array());
	// 	for (auto &&pp : p)
	// 	{
	// 		jpart.push_back(pp);
	// 	}
	// }
	
	// j = nlohmann::json{{"board", jpart}};
	j = nlohmann::json{{"board", p.getBoardFull()}};
}
