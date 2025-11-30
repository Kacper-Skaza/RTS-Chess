#include "../headers/User.hpp"

// Implement on server side as request to get ID (needs to be unique)
unsigned int User::generateID(const std::string &username) 
{
    unsigned int id = 0;
    for (size_t i = 0; i < username.size(); i++)
    {
        id +=  (unsigned int) username[i];
    }
    
    
    this->id = id;
}

User::User(const std::string &username)
{
    this->username = username;
    this->generateID(username);
    this->player = false;
    this->ready = false;
    this->inRoom = false;
    this->side = WHITE;
}

const bool User::operator==(const User &user) const
{
    if (this->id == user.id) return true;
    return false;
}

const unsigned int& User::getPlayerID() const noexcept 
{
    return this->id;
}

const std::string& User::getUsername() const noexcept
{
    return this->username;
}

const bool User::isPlayer() const noexcept
{
    return this->player;
}

const bool User::isReady() const noexcept
{
    return this->ready;
}

const bool User::isInRoom() const noexcept
{
    return this->inRoom;
}

const ChessSide User::getSide() const noexcept
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

void User::setUsername(std::string username)
{
    this->username = username;
}

void User::setSide(ChessSide side)
{
    this->side = side;
}