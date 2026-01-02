#include "../headers/User.hpp"

User::User()
{
    this->id = 0;
    this->username = "";
    this->player = false;
    this->ready = false;
    this->inRoom = false;
    this->side = ChessSide::UNKNOWN;
}

User::User(const unsigned long long &id, const std::string &username)
{
    this->id = id;
    this->username = username;
    this->player = false;
    this->ready = false;
    this->inRoom = false;
    this->side = ChessSide::UNKNOWN;
}

bool User::operator==(const User &user) const
{
    if (this->id == user.id) return true;
    return false;
}

const unsigned long long& User::getPlayerID() const noexcept
{
    return this->id;
}

const std::string& User::getUsername() const noexcept
{
    return this->username;
}

bool User::isPlayer() const noexcept
{
    return this->player;
}

bool User::isReady() const noexcept
{
    return this->ready;
}

bool User::isInRoom() const noexcept
{
    return this->inRoom;
}

ChessSide User::getSide() const noexcept
{
    return this->side;
}

void User::setPlayer(bool player)
{
    this->player = player;
}

void User::setReady(bool ready)
{
    this->ready = ready;
}

void User::setInRoom(bool inRoom)
{
    this->inRoom = inRoom;
}

void User::setUsername(const std::string &username)
{
    this->username = username;
}

void User::setSide(ChessSide side)
{
    this->side = side;
}

void User::setPlayerID(const unsigned long long &ID)
{
    this->id = ID;
}

void from_json(const nlohmann::json& j, User& p)
{
    p.id = j.at("id").get<unsigned long long>();
    p.username = j.at("username").get<std::string>();
    p.player = j.at("player").get<bool>();
    p.ready = j.at("ready").get<bool>();
    p.inRoom = j.at("inRoom").get<bool>();
    p.side = j.at("side").get<ChessSide>();
}

void to_json(nlohmann::json& j, const User* p)
{
    
}

void to_json(nlohmann::json& j, const User& p)
{
    j = nlohmann::json{
        {"id", p.id},
        {"username", p.username},
        {"player", p.player},
        {"ready", p.ready},
        {"inRoom", p.inRoom},
        {"side", p.side}
    };
}
