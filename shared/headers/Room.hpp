#pragma once

#include "Board.hpp"
#include "User.hpp"
#include "Structures.hpp"

#include <cstdlib>
#include <cstdint>
#include <utility>
#include <cctype>
#include <vector>
#include <stdexcept>
#include <memory>
#include <unordered_map>

class Room
{
private:
    bool matchStarted = false;
    uint8_t maxPlayerCount;
    Board board;
    std::unordered_map<unsigned int, User> userList;
    std::vector<unsigned int> playerList;

public:
    // Constructor & Destructor
    Room(User creator);
    ~Room() = default;

    // Get structure Data
    bool isMatchReady() const;
    bool isMatchStarted() const noexcept;
    Board &getBoard();
    unsigned int getUserCount() const;
    unsigned int getPlayerCount() const;
    unsigned int getPlayerReadyCount() const;
    uint8_t getMaxPlayerCount() const noexcept;
    std::unordered_map<unsigned int, User> &getUserList();
    std::unordered_map<unsigned int, User> &getPlayerList();

    // Set structure data
    void addUserToRoom(User& joining);
    void addPlayer(User& player);
    void removePlayer(User& player, const bool quit = false);
    void removeUserFromRoom(User& player);

    // Other metchods
    bool startMatch();
    bool stopMatch(MatchEndReasons reason);


};