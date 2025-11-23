#include "../../headers/pieces/Pawn.hpp"

Pawn::Pawn(const char SIDE): SYMBOL(std::toupper(SIDE) == 'W'? 'P': 'p')
{
    //Throw exception when given wrong char for SIDE
    if (std::toupper(SIDE) != 'W' || std::toupper(SIDE) != 'B')
    {
        throw std::invalid_argument("received different value in Bishop constructor than \'W\' or \'B\'");
    }
}

char Pawn::getSymbol() const noexcept
{
    return this->SYMBOL;
}

bool Pawn::validateMove(const Move &move, const std::vector<std::vector<std::unique_ptr<Piece>>> &board) const
{
    //Check if correct piece is being moved
    if (move.getPiece() != this) return false;

    std::pair<int, int> from = move.getFrom();
    std::pair<int, int> to = move.getTo();

    const int dx = to.first - from.first;
    const int dy = to.second - from.second;

    //Check for move forward
    //THIS PROBABLY IS INCORRECT DUE TO BOARD DECLARATION
    if ((dx == 1 || dx == 2) && dy == 0)
    {
        //Its a move space need to be free
        if (board[to.first][to.second] != nullptr) return false;

        //Check if move is valid
        if (firstMove == false && dx !=1) return false;
        if (firstMove == true && dx != 2 && dx != 1) return false;
        return true;        
    }
    
    //Check for capture move
    if (dx == 1 && std::abs(dy) == 1)
    {
        //Check if space is same color
        if (std::isupper(board[to.first][to.second]->getSymbol()) == std::isupper(this->SYMBOL)) return false;
        
        //Check for en passant
        if (board[to.first][to.second] == nullptr && (1) /*check if pawn is next to this one*/ && (1) /*if its pawns 1st move*/)
            return true;
            
        //Check if space is empty
        if (board[to.first][to.second] == nullptr) return false;
        return true;
    }
}