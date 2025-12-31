#pragma once

#include "../../shared/headers/Structures.hpp"
#include "../../shared/headers/User.hpp"
#include "../../shared/headers/Room.hpp"

#include <iostream>
#include <string>
#include <vector>
#include <memory>

#include "../../shared/headers/dependencies/Json.hpp"
using json = nlohmann::json;

class MessageHandler
{
private:
    // ===== Incoming messages =====
    void handleRequestNick(Client *client, const json &data);
    void handleRoomCreate(Client *client, const json &data, std::unordered_map<std::string, std::unique_ptr<Room>> &rooms);
    void handleRequestRooms(Client *client, std::unordered_map<std::string, std::unique_ptr<Room>> &rooms);
    void handleRoomJoin(Client *client, const json &data, std::unordered_map<std::string, std::unique_ptr<Room>> &rooms);
    void handleRoomLeave(Client *client, const json &data, std::unordered_map<std::string, std::unique_ptr<Room>> &rooms);
    void handlePlayerReady(Client *client, std::unordered_map<std::string, std::unique_ptr<Room>> &rooms);
    void handlePlayerWant(Client *client, const json &data, std::unordered_map<std::string, std::unique_ptr<Room>> &rooms);
    void handleChatMessage(Client *client, const json &data, std::unordered_map<std::string, std::unique_ptr<Room>> &rooms);
    void handleMakeMove(Client *client, const json &data, std::unordered_map<std::string, std::unique_ptr<Room>> &rooms);

    // ===== Outgoing messages =====
    void broadcastUpdateMove(const Room* room, const Move &newMove);
    void broadcastUpdateChat(const Room* room, const std::string &newMessage);

public:
    MessageHandler() = default;
    ~MessageHandler() = default;

    // ===== Main entry point =====
    void handle(Client *client, const std::string &jsonText, std::unordered_map<std::string, std::unique_ptr<Room>> &rooms);
};
