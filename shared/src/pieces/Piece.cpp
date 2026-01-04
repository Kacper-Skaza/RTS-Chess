#include "../../headers/pieces/Piece.hpp"

Piece::Piece() : SYMBOL('p'), ID(-1) {}

Piece::Piece(const char SIDE, const char SYMBOL, const int ID): SYMBOL(std::toupper(SIDE) == 'W'? std::toupper(SYMBOL): std::tolower(SYMBOL)), ID(ID) {}

char Piece::getSymbol() const noexcept
{
	return SYMBOL;
}

int Piece::getID() const noexcept
{
    return ID;
}

bool Piece::getMoveCount() const noexcept
{
	return moveCount;
}

std::chrono::system_clock::time_point Piece::getLastMoveTime() const noexcept
{
    return this->lastMoveTime;
}

int Piece::getCooldown() const noexcept
{
	auto timeDif = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - lastMoveTime).count();
	return  timeDif - MAX_COOLDOWN;
}

void Piece::setLastMoveTime(std::chrono::system_clock::time_point time)
{
	this->lastMoveTime = time;
}

void Piece::setMoveCount(int count)
{
	this->moveCount = count;
}

void Piece::makeMove()
{
	moveCount++;
	lastMoveTime = std::chrono::system_clock::now();
}

bool Piece::validateMove(const Move &move, const std::vector<std::vector<Piece *>> &board) const
{
	// Suppress warnings
	(void)move;
	(void)board;

	// Return
	return false;
}

void to_json(nlohmann::json& j, const Piece* p)
{
	if (p == nullptr)
	{
		j = Piece();
		return;
	}
	
	j = nlohmann::json{
		{"symbol", p->getSymbol()},
		{"id", p->getID()},
		{"moveCount", p->moveCount}
	};
}

void to_json(nlohmann::json &j, const Piece &p)
{
	j = nlohmann::json{
		{"symbol", p.getSymbol()},
		{"id", p.getID()},
		{"moveCount", p.moveCount}
	};
}

Piece Piece::from_json(const nlohmann::json &j)
{
	Piece p(std::isupper(j.at("symbol").get<char>()) == true? 'W': 'B', j.at("symbol").get<char>(), j.at("id").get<const int>());
	p.setMoveCount(j.at("moveCount").get<int>());
	return p;
}