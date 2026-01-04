#pragma once

#include "dependencies/Json.hpp"

#include "Structures.hpp"
#include "Move.hpp"
#include "pieces/Piece.hpp"

#include <vector>
#include <memory>
#include <algorithm>
#include <cctype>

class Board
{
private:
	std::vector<std::vector<std::unique_ptr<Piece>>> board;
	MatchEndReasons gameState = MatchEndReasons::NOT_ENDED;

public:
	Board();
	~Board() = default;

	Board(Board&& other) noexcept = default;
	Board& operator=(Board&& other) noexcept = default;

	Board(const Board &other) = delete;
	Board& operator=(const Board &other) = delete;

	void setSpace(int row, int col, std::unique_ptr<Piece>& piece);
	std::unique_ptr<Piece>& getSpace(int row, int col);

	const std::vector<std::vector<Piece *>> getBoardFull() const;
	const std::vector<std::vector<char>> getBoardSymbol() const;
	const std::vector<std::vector<bool>> getBoardCooldown() const;

	void checkGameEnd();
	const MatchEndReasons& getGameState();
    void setGameState(MatchEndReasons state);
    void updateGameState(MatchEndReasons state);

	bool makeMove(const Move &move);

	static void from_json(const nlohmann::json& j, Board& b);
	friend void to_json(nlohmann::json& j, const Board& p);
};
