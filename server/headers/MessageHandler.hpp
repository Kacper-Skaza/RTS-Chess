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
    // ===== Pointers to server data =====
    static std::unordered_map<SOCKET, std::unique_ptr<Client>> *clientsPtr;
    static std::unordered_map<std::string, std::unique_ptr<Room>> *roomsPtr;

    // ===== Incoming messages =====
    static void handleRequestNick(Client *client, const json &data);
    static void handleRoomCreate(Client *client, const json &data);
    static void handleRequestRooms(Client *client);
    static void handleRoomJoin(Client *client, const json &data);
    static void handleRoomLeave(Client *client, const json &data);
    static void handlePlayerReady(Client *client);
    static void handlePlayerWant(Client *client, const json &data);
    static void handleChatMessage(Client *client, const json &data);
    static void handleMakeMove(Client *client, const json &data);

    // ===== Outgoing messages =====
    static void broadcastMoveMade(const Room *room, const User *user, const Move &newMove);
    static void broadcastUpdateChat(const Room *room, const User *user, const std::string &newMessage);

public:
    MessageHandler() = delete;

    // ===== Init pointers =====
    static void init(std::unordered_map<SOCKET, std::unique_ptr<Client>> &clientsRef, std::unordered_map<std::string, std::unique_ptr<Room>> &roomsRef);

    // ===== Main entry point =====
    static void handle(Client *client, const std::string &jsonText);
};
