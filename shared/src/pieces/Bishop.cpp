#include "../../headers/pieces/Bishop.hpp"

Bishop::Bishop(const char SIDE): SYMBOL(std::toupper(SIDE) == 'W'? 'B': 'b')
{
    //Throw exception when given wrong char for SIDE
    if (std::toupper(SIDE) != 'W' || std::toupper(SIDE) != 'B')
    {
        throw std::invalid_argument("received different value in Bishop constructor than \'W\' or \'B\'");
    }
}

char Bishop::getSymbol() const noexcept
{
    return this->SYMBOL;
}

bool Bishop::validateMove(const Move &move, const std::vector<std::vector<char>> &board) const
{
    //Check if correct piece is being moved (may be redundant later)
    if (move.getPiece() != this->SYMBOL) return false;

    std::pair<int, int> from = move.getFrom();
    std::pair<int, int> to = move.getTo();

    //Check if destination is valid (empty or other color)
    if (board[to.first][to.second] != ' ' && std::isupper(board[to.first][to.second]) == std::isupper(this->SYMBOL)) 
        return false;

    //Check if move is diagonal
    if ((std::abs(from.first - to.first) != std::abs(from.second - to.second)) && (from.first - to.first != 0)) return false;

    //Check if there is piece in the way
    const int dx = to.first - from.first;
    const int dy = to.second - from.second;

    for (   int i = dx >= 0? dx - 1: dx + 1, int j = dy >= 0? dy - 1: dy + 1;
            i != 0;
            i+= i >= 0? -1: 1, j+= j >= 0? -1: 1)
    {
        if (board[from.first + i][from.second + j] != ' ') return false;        
    }

    return true;
}