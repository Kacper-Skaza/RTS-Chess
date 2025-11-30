#include "../headers/Room.hpp"

#define MIN_PLAYER_COUNT 2
#define MAX_PLAYER_COUNT 8

Room::Room(std::string name, User creator): NAME(name)
{
    this->userList.insert({creator.getPlayerID(), creator});
    this->playerList.push_back(creator.getPlayerID());
    this->maxPlayerCount = MIN_PLAYER_COUNT;
    this->matchStarted = false;
}


bool Room::isMatchReady() const
{
    auto it = this->userList.find(1);
    for (auto &&i : this->playerList)
    {
        it = this->userList.find(i);
        if (it != this->userList.end() && it->second.isReady() == false) return false; 
    }
    return true;
}

const std::string Room::getRoomName() const noexcept
{
    return this->NAME;
}

bool Room::isMatchStarted() const noexcept
{
    return this->matchStarted;
}

unsigned int Room::getUserCount() const
{
    return this->userList.size();
}

unsigned int Room::getPlayerCount() const
{
    return this->playerList.size();
}

Board &Room::getBoard()
{
    return this->board;
}

unsigned int Room::getPlayerReadyCount() const
{
    unsigned int readyPlayersCount = 0;
    auto it = this->userList.find(1);
    for (auto &&i : this->playerList)
    {
        it = this->userList.find(i);
        readyPlayersCount += (it != this->userList.end() && it->second.isReady() == true)? 1: 0; 
    }
    return readyPlayersCount;
}

uint8_t Room::getMaxPlayerCount() const noexcept
{
    return this->maxPlayerCount;
}

std::unordered_map<unsigned int, User> &Room::getUserList()
{
    return this->userList;
}

std::unordered_map<unsigned int, User*> Room::getPlayerList()
{
    std::unordered_map<unsigned int, User*> tempUserList;
    auto it = this->userList.find(1);
    for (auto &&i : this->playerList)
    {
        it = this->userList.find(i);
        if (it != this->userList.end()) tempUserList.insert({it->first, &(it->second)});
    }
    return tempUserList;
}

void Room::setMaxPlayerCount(const int count)
{
    if (count >= MIN_PLAYER_COUNT && count <= MAX_PLAYER_COUNT) return;
    this->maxPlayerCount = count;
}

void Room::addUserToRoom(User& joining)
{
    this->userList.insert({joining.getPlayerID(), joining});
}

void Room::addPlayer(User& player)
{
    if (this->userList.find(player.getPlayerID()) != this->userList.end())
    {
        this->playerList.push_back(player.getPlayerID());
    }
}

void Room::removePlayer(User& player, const bool quit)
{
    const unsigned int ToRemoveID = player.getPlayerID();
    for (size_t i = 0; i < this->playerList.size(); i++)
    {
        if (this->playerList[i] == ToRemoveID)
        {
            this->playerList.erase(this->playerList.begin() + i);
        }
    }
    if (quit == true) this->userList.erase(ToRemoveID);
}

void Room::removeUserFromRoom(User& player)
{
    if (this->userList.find(player.getPlayerID()) != this->userList.end()) this->userList.erase(player.getPlayerID());   
}

bool Room::startMatch()
{

}

bool Room::stopMatch(MatchEndReasons reason)
{

}