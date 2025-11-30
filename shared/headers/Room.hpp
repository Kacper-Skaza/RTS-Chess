#pragma once

#include "Board.hpp"
#include "User.hpp"
#include "Structures.hpp"

#include <cstdlib>
#include <utility>
#include <cctype>
#include <vector>
#include <stdexcept>
#include <memory>

class Room
{
private:
    bool matchStarted = false;
    uint8_t maxPlayerCount;
    Board board;
    std::vector<User> userList;
    std::vector<unsigned int> playerList;

public:
    // Constructor & Destructor
    Room(User creator);
    ~Room() = default;

    // Get structure Data
    bool isMatchReady() const;
    const bool isMatchStarted() const noexcept;
    const Board &getBoard() const noexcept;
    const unsigned int getUserCount() const;
    const unsigned int getPlayerCount() const;
    const unsigned int getPlayerReadyCount() const;
    const uint8_t getMaxPlayerCount() const noexcept;
    const std::vector<User> &getUserList() const;
    const std::vector<User> &getPlayerList() const;

    // Set structure data
    void addUserToRoom(User& joining);
    void addPlayer(User& player);
    void removePlayer(User& player, const bool quit = false);
    void removeUserFromRoom(User& player);

    // Other metchods
    bool startMatch();
    bool stopMatch(MatchEndReasons reason);


};