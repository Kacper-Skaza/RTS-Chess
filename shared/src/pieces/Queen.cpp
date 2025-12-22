#include "../../headers/pieces/Queen.hpp"

Queen::Queen(const char SIDE): SYMBOL(std::toupper(SIDE) == 'W'? 'Q': 'q')
{
	//Throw exception when given wrong char for SIDE
    if (std::toupper(SIDE) != 'W' && std::toupper(SIDE) != 'B')
    {
        throw std::invalid_argument("received different value in Bishop constructor than \'W\' or \'B\'");
    }
}

bool Queen::validateMove(const Move &move, const std::vector<std::vector<Piece *>> &board) const
{
    //Check if correct piece is being moved
    if (move.getPiece() != this) return false;

    std::pair<int, int> from = move.getFrom();
    std::pair<int, int> to = move.getTo();

    const int dx = to.first - from.first;
    const int dy = to.second - from.second;

    //Check if destination is valid (empty or other color)
    if (board[to.first][to.second] != nullptr && std::isupper(board[to.first][to.second]->getSymbol()) == std::isupper(this->SYMBOL)) 
        return false;

    //Check if move is diagonal or othogonal
    if (std::abs(dx) == std::abs(dy)) //diagonal
    {
        //Check if there is piece in the way diagonally
        for (   int i = dx, j = dy;
                i != 0;
                i+= i >= 0? -1: 1, j+= j >= 0? -1: 1)
        {
            if (board[from.first + i][from.second + j] != nullptr) return false;        
        }
        return true;
    } 
    else if ((dx == 0 && dy != 0) || (dy == 0 && dx != 0)) //orthogonal
    {
        //Check if there is piece in the way orthogonally
        for (int i = dx == 0? (dy >= 0? dy - 1: dy + 1): (dx >= 0? dx - 1: dx + 1); i != 0; i += i >= 0? -1: 1)
        {
            if (board[from.first + (dx == 0? 0: i)][from.second + (dx == 0? i: 0)] != nullptr) return false;
        }
        return true;
    } 
    
    return false;
}