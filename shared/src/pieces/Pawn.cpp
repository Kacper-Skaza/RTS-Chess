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
        //Check if there is something and that thing is different color
        if (board[to.first][to.second] != nullptr && std::isupper(board[to.first][to.second]->getSymbol()) != std::isupper(this->SYMBOL)) return true;
        
        //Check for en passant
        if (board[to.first][to.second] == nullptr && board[from.first][to.second] != nullptr)
        {
            if (Pawn* pawn = dynamic_cast<Pawn*>(board[from.first][to.second])) /*check if it is a pawn*/
            {
                if (std::isupper(pawn->getSymbol()) != std::isupper(this->SYMBOL)  /*is different color*/
                    && pawn->getMoveCount() == 1 /*its move count is 1*/
                    && pawn->getID() / 10 - 2 == from.first || pawn->getID() / 10 + 2 == from.first) /*was move far*/
                    return true;
            }
        }
    }
    return false;
}

bool Pawn::getEnPassant()
{
    return this->enPassant;
}