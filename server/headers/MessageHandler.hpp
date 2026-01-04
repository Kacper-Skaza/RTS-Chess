#pragma once

#include <iostream>

#include "../../shared/headers/ConnectionManager.hpp"
#include "../../shared/headers/Structures.hpp"
#include "../../shared/headers/User.hpp"
#include "../../shared/headers/Room.hpp"

#include "../../shared/headers/dependencies/Json.hpp"
using json = nlohmann::json;

class MessageHandler
{
private:
    // ===== SERVER DATA =====
    static std::unordered_map<SOCKET, std::unique_ptr<Client>> *clientsPtr;
    static std::unordered_map<std::string, std::unique_ptr<Room>> *roomsPtr;

    // ===== INCOMING MESSAGES =====
    static void handleErrorMoveMade(Client *client);
    static void handleRequestNick(Client *client, const json &data);
    static void handleRequestRooms(Client *client);
    static void handleRequestRoom(Client *client);
    static void handleRoomCreate(Client *client, const json &data);
    static void handleRoomJoin(Client *client, const json &data);
    static void handleRoomLeave(Client *client);
    static void handlePlayerWant(Client *client, const json &data);
    static void handleChangePlayerCount(Client *client);
    static void handleChatMessage(Client *client, const json &data);
    static void handleMakeMove(Client *client, const json &data);

    // ===== OUTGOING MESSAGES =====
    static void broadcastUpdateRoom(Room *room);
    static void broadcastUpdateChat(const Room *room, const User *user, const std::string &newMessage);
    static void broadcastMoveMade(const Room *room, const User *user, const json &newMove);
    static void broadcastGameFinale(Room *room);

public:
    MessageHandler() = delete;

    // ===== INIT SERVER DATA =====
    static void init(std::unordered_map<SOCKET, std::unique_ptr<Client>> &clientsRef, std::unordered_map<std::string, std::unique_ptr<Room>> &roomsRef);

    // ===== ENTRY POINTS =====
    static void handleMessage(Client *client, const std::string &jsonText);
    static void handleDisconnect(Client *client, const SOCKET &fd);
};
