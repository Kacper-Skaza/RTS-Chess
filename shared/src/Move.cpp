#include "../headers/Move.hpp"

Move::Move()
{
	this->piece = nullptr;
	this->from = std::make_pair(-1,-1);
	this->to = std::make_pair(-1, -1);
	this->recreated = false;
}

Move::Move(Piece *piece, std::pair<int, int> from, std::pair<int, int> to) : piece(piece), from(from), to(to)
{
	if (from.first < 0 || from.first >= BOARD_SIZE ||
		from.second < 0 || from.second >= BOARD_SIZE ||
		to.first < 0 || to.first >= BOARD_SIZE ||
		to.second < 0 || to.second >= BOARD_SIZE)
	{
		throw std::invalid_argument("[Move::Move] Coordinates out of range");
	}

	if (piece == nullptr)
	{
		throw std::invalid_argument("[Move::Move] Null piece pointer");
	}
}

Move::~Move()
{
	if (recreated == true && piece != nullptr)
		delete piece;
}

const Piece *Move::getPiece() const noexcept
{
	return piece;
}

const std::pair<int, int> Move::getFrom() const noexcept
{
	return from;
}

const std::pair<int, int> Move::getTo() const noexcept
{
	return to;
}

void from_json(const nlohmann::json& j, Move& p)
{
	p.piece = new Piece(Piece::from_json(j.at("piece")));
	p.from = std::make_pair(j.at("from").at(0).get<int>(),j.at("from").at(1).get<int>()); 
	p.to = std::make_pair(j.at("to").at(0).get<int>(),j.at("to").at(1).get<int>());
	p.recreated = true;
}

void to_json(nlohmann::json &j, const Move &p)
{
	j = nlohmann::json{
		{"piece", *p.piece},
		{"from", p.from},
		{"to", p.to}
	};
}