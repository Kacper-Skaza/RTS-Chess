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
    std::string name;
    bool matchStarted = false;
    uint8_t maxPlayerCount;
    Board board;
    std::unordered_map<unsigned int, User> userList;
    std::vector<unsigned int> playerList;

public:
    // Constructor & Destructor
    explicit Room();
    explicit Room(std::string name, User &creator);
    ~Room() = default;

    // Get structure Data
    const std::string getRoomName() const noexcept;
    bool isMatchReady() const;
    bool isMatchStarted() const noexcept;
    Board &getBoard();
    unsigned int getUserCount() const;
    unsigned int getPlayerCount() const;
    unsigned int getPlayerReadyCount() const;
    uint8_t getMaxPlayerCount() const noexcept;
    const std::unordered_map<unsigned int, User> &getUserList() const;
    const std::unordered_map<unsigned int, User*> getPlayerList();

    // Set structure data
    void addUserToRoom(const User& joining);
    void addPlayer(const User& player);
    void removePlayer(const User& player, const bool quit = false);
    void removeUserFromRoom(const User& player);
    void setMaxPlayerCount(const int count);

    // Other metchods
    void startMatch();
    void stopMatch(MatchEndReasons reason);

    static void from_json(const nlohmann::json& j, Room& p);
	friend void to_json(nlohmann::json& j, const Room& p);
};
