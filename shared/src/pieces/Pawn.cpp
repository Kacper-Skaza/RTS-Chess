#include "../../headers/pieces/Pawn.hpp"

Pawn::Pawn(const char SIDE, const int POS): Piece(SIDE, 'P', POS)
{
	//Throw exception when given wrong char for SIDE
    if (std::toupper(SIDE) != 'W' && std::toupper(SIDE) != 'B')
    {
        throw std::invalid_argument("received different value in Bishop constructor than \'W\' or \'B\'");
    }
}

bool Pawn::validateMove(const Move &move, const std::vector<std::vector<Piece *>> &board) const
{
    //Check if correct piece is being moved
    if (move.getPiece() != this) return false;

    std::pair<int, int> from = move.getFrom();
    std::pair<int, int> to = move.getTo();

    //dx is top down movement where lower number is more to the bottom
    //dy is left right movement where lower number is more to the left
    const int dx = std::abs(to.first - from.first);
    const int dy = std::abs(to.second - from.second);

    //Check for move forward
    //THIS PROBABLY IS INCORRECT DUE TO BOARD DECLARATION
    if ((dx == 1 || dx == 2) && dy == 0)
    {
        //Its a move space need to be free
        if (board[to.first][to.second] != nullptr) return false;

        //Check if move is valid
        if (moveCount != 0 && dx !=1) return false;
        if (moveCount == 0 && dx != 2 && dx != 1) return false;
        return true;
    }
    
    //Check for capture move
    if (dx == 1 && dy == 1)
    {
        //Check if space is same color
        if (std::isupper(board[to.first][to.second]->getSymbol()) == std::isupper(this->SYMBOL)) return false;
        
        //Check for en passant
        if (board[to.first][to.second] == nullptr 
            && (board[to.first][from.second]->getSymbol() == 'P' || board[to.first][from.second]->getSymbol() == 'p') 
            && (board[to.first][from.second]->getMoveCount() == 1))
            return true;
            
        //Check if space is empty
        if (board[to.first][to.second] == nullptr) return false;
        return true;
    }
    return false;
}