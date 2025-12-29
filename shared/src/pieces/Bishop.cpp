#include "../../headers/pieces/Bishop.hpp"

Bishop::Bishop(const char SIDE, const int POS): Piece(SIDE, 'B', POS)
{
    //Throw exception when given wrong char for SIDE
    if (std::toupper(SIDE) != 'W' && std::toupper(SIDE) != 'B')
    {
        throw std::invalid_argument("received different value in Bishop constructor than \'W\' or \'B\'");
    }
}

bool Bishop::validateMove(const Move &move, const std::vector<std::vector<Piece *>> &board) const
{
    //Check if correct piece is being moved
    if (move.getPiece() != this) return false;

    std::pair<int, int> from = move.getFrom();
    std::pair<int, int> to = move.getTo();

    //Check if destination is valid (empty or other color)
    if (board[to.first][to.second] != nullptr && std::isupper(board[to.first][to.second]->getSymbol()) == std::isupper(this->SYMBOL)) 
        return false;

    //Check if move in place
    if ((from.first - to.first == 0) || (from.second - to.second == 0)) return false;

    //Check if move is diagonal
    if ((std::abs(from.first - to.first) != std::abs(from.second - to.second))) return false;

    //Check if there is piece in the way
    const int dx = to.first - from.first;
    const int dy = to.second - from.second;

    for (   int i = dx >= 0? dx - 1: dx + 1, j = dy >= 0? dy - 1: dy + 1;
            i != 0;
            i+= i >= 0? -1: 1, j+= j >= 0? -1: 1)
    {
        if (board[from.first + i][from.second + j] != nullptr) return false;        
    }

    return true;
}

Bishop Bishop::from_json(nlohmann::json &j)
{
    return Bishop(std::isupper(j.at("symbol").get<char>())? 'W': 'B', j.at("id").get<int>());
}