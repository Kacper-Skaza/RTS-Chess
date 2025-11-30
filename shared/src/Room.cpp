#include "../headers/Room.hpp"

#define MIN_PLAYER_COUNT 2

Room::Room(User creator)
{
    this->userList.push_back(creator);
    this->playerList.push_back(0);
    this->maxPlayerCount = MIN_PLAYER_COUNT;
    this->matchStarted = false;
}


bool Room::isMatchReady() const
{
    for (auto &&i : this->playerList)
    {
        if (this->userList[i].isReady() == false) return false;         
    }
    return true;
}

const bool Room::isMatchStarted() const noexcept
{
    return this->matchStarted;
}

const unsigned int Room::getUserCount() const
{
    return this->userList.size();
}

const unsigned int Room::getPlayerCount() const
{
    return this->playerList.size();
}

const Board &Room::getBoard() const noexcept
{
    return this->board;
}

const unsigned int Room::getPlayerReadyCount() const
{
    unsigned int readyPlayersCount = 0;
    for (auto &&i : this->playerList)
    {
        readyPlayersCount += this->userList[i].isReady() == true? 1: 0; 
    }
    return readyPlayersCount;
}

const uint8_t Room::getMaxPlayerCount() const noexcept
{
    return this->maxPlayerCount;
}

const std::vector<User> &Room::getUserList() const
{
    return this->userList;
}

const std::vector<User> &Room::getPlayerList() const
{
    std::vector<User> tempUserList;
    for (auto &&i : this->playerList)
    {
        tempUserList.push_back(this->userList[i]);
    }
    return tempUserList;
}

void Room::addUserToRoom(User& joining)
{
    this->userList.push_back(joining);
}

void Room::addPlayer(User& player)
{
    for (unsigned int i = 0; i < this->userList.size(); i++)
    {
        if (this->userList[i] == player) this->playerList.push_back(i);
    }
}

// TODO
void Room::removePlayer(User& player, const bool quit)
{

    for (size_t i = 0; i < this->playerList.size(); i++)
    {
        //find player to remove
        if (this->userList[this->playerList[i]] == player)
        {
            this->playerList.erase(this->playerList.begin() + i);
        }

        //fix rest of players slots
    }
    
    for (auto &&i : this->playerList)
    {
        if (this->userList[i] == player)
        {

        }
    }
    
}

//  TODO
void Room::removeUserFromRoom(User& player)
{
    for (size_t i = 0; i < this->userList.size(); i++)
    {
        //find player to remove
        if (this->userList[i] == player)
        {
            
        }
        //fix player structure

    }
    
}

bool Room::startMatch()
{

}

bool Room::stopMatch(MatchEndReasons reason)
{

}