#include "../headers/Room.hpp"

#include <iostream>

//Player count
#define MIN_PLAYER_COUNT 2
#define MAX_PLAYER_COUNT 8

Room::Room(): name(""), board()
{
    this->userList.clear();
    this->playerList.clear();
    this->maxPlayerCount = 0;
    this->matchStarted = false;
}

Room::Room(std::string name, User &creator): name(name), board()
{
    this->userList.insert({creator.getPlayerID(), &creator});
    this->playerList.push_back(creator.getPlayerID());
    this->maxPlayerCount = MIN_PLAYER_COUNT;
    this->matchStarted = false;
}

Room::~Room()
{

}

bool Room::isMatchReady() const
{
    for (unsigned int id : this->playerList)
    {
        auto it = this->userList.find(id);
        if (it != this->userList.end() && it->second->isReady() == false)
            return false;
    }

    return this->getPlayerCount() == this->maxPlayerCount;
}

const std::string Room::getRoomName() const noexcept
{
    return this->name;
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
        readyPlayersCount += (it != this->userList.end() && it->second->isReady() == true)? 1: 0;
    }
    return readyPlayersCount;
}

uint8_t Room::getMaxPlayerCount() const noexcept
{
    return this->maxPlayerCount;
}

const std::unordered_map<unsigned int, User*> &Room::getUserList() const
{
    return this->userList;
}

const std::unordered_map<unsigned int, User*> Room::getPlayerList()
{
    std::unordered_map<unsigned int, User*> tempUserList;
    auto it = this->userList.find(1);
    for (auto &&i : this->playerList)
    {
        it = this->userList.find(i);
        if (it != this->userList.end()) tempUserList.insert({it->first, it->second});
    }
    return tempUserList;
}

void Room::setMaxPlayerCount(const int count)
{
    if (count >= MIN_PLAYER_COUNT && count <= MAX_PLAYER_COUNT)
        return;

    this->maxPlayerCount = count;
}

void Room::bumpMaxPlayerCount()
{
    uint8_t nextValue = getMaxPlayerCount() + 2;

    if (nextValue > MAX_PLAYER_COUNT)
        nextValue = MIN_PLAYER_COUNT;

    while (nextValue < getPlayerCount())
        nextValue += 2;

    setMaxPlayerCount(nextValue);
}

void Room::addUserToRoom(const User& joining)
{
    this->userList.insert({joining.getPlayerID(), const_cast<User*>(&joining)});
}

void Room::addPlayer(const User &player)
{
    const unsigned long long id = player.getPlayerID();

    if (this->userList.find(id) == this->userList.end())
        return;

    if (std::find(this->playerList.begin(), this->playerList.end(), id) != this->playerList.end())
        return;

    this->playerList.push_back(id);
}

void Room::removePlayer(const User& player)
{
    const unsigned int ToRemoveID = player.getPlayerID();
    for (size_t i = 0; i < this->playerList.size(); i++)
    {
        if (this->playerList[i] == ToRemoveID)
        {
            this->playerList.erase(this->playerList.begin() + i);
            break;
        }
    }
}

void Room::removeUserFromRoom(const User& user)
{
    if (this->userList.find(user.getPlayerID()) != this->userList.end()) this->userList.erase(user.getPlayerID());
}

void Room::startMatch()
{
    this->matchStarted = true;

    const std::unordered_map<unsigned int, User *> players = this->getPlayerList();
    bool turn = true;

    for (auto &[_, user] : players)
    {
        user->setSide(turn ? ChessSide::WHITE : ChessSide::BLACK);
        turn = !turn;
    }

    // reinit board
}

void Room::stopMatch(MatchEndReasons reason)
{
    this->matchStarted = false;
    //destroy board
    //do not destroy room coz it will be done server side & client side elsewhere
}

const User* Room::getHost() const
{
    if (this->playerList.size() > 0)
        return this->userList.at(this->playerList[0]);
    return this->userList.begin()->second;
}

void Room::from_json(const nlohmann::json& j, Room& p)
{
    if (j.empty())
        return;
    p.name = j.at("name").get<std::string>();
    p.matchStarted = j.at("matchStarted").get<bool>();
    p.maxPlayerCount = static_cast<uint8_t>(j.at("maxPlayerCount").get<int>());
    p.playerList = j.at("playerList").get<std::vector<unsigned int>>();
    Board::from_json(j.at("board"), p.board);
    for (auto &&i : p.userList)
    {
        delete i.second;
    }
    p.userList.clear();
    for (auto &&mapItem : j.at("userList"))
    {
        User* user = new User();
        *user = mapItem.at(1).get<User>();
        p.userList.emplace(mapItem.at(0), user);
    }

    // p.userList = j.at("userList").get<std::unordered_map<unsigned int, User*>>();
}

void to_json(nlohmann::json& j, const Room& p)
{
    j = nlohmann::json{
        {"name", p.name},
        {"matchStarted", p.matchStarted},
        {"maxPlayerCount", p.maxPlayerCount},
        {"board", p.board},
        {"playerList", p.playerList},
        {"userList", p.userList}
    };
}
