#pragma once

class Move;

#include <cstdlib>
#include <utility>
#include <cctype>
#include <vector>
#include <stdexcept>
#include <memory>
#include <chrono>
#include "../dependencies/Json.hpp"

#define MAX_COOLDOWN 2000

class Piece
{
protected:
	const char SYMBOL;
	int moveCount = 0;
	const int ID; 

	std::chrono::system_clock::time_point lastMoveTime = std::chrono::system_clock::now();

public:
	Piece();
	Piece(const char SIDE, const char SYMBOL, const int ID);
	virtual ~Piece() = default;

	int  getID() const noexcept;
	char getSymbol() const noexcept;
	bool getMoveCount() const noexcept;
	std::chrono::system_clock::time_point getLastMoveTime() const noexcept;
	int getCooldown() const noexcept;
	
	void setMoveCount(int count);

	void makeMove();
	virtual bool validateMove(const Move &move, const std::vector<std::vector<Piece *>> &board) const;

	friend void to_json(nlohmann::json& j, const Piece& p);
	friend void to_json(nlohmann::json& j, const Piece* p);
	static Piece from_json(const nlohmann::json &j);
};
