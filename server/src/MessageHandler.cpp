#include "../headers/MessageHandler.hpp"

// ===== Main entry point =====

void MessageHandler::handle(Client *client, const std::string &jsonText, std::unordered_map<std::string, std::unique_ptr<Room>> &rooms)
{
    try
    {
        // Parse incoming string to JSON
        json j = json::parse(jsonText);

        // Extract type and data
        std::string type = j["type"];
        json data = j.contains("data") ? j["data"] : json();
        std::cout << "[SERVER] Handling: " << type << " for " << client->user->getUsername() << std::endl;

        // Handle specific request type
        if (type.rfind("ACK_", 0) == 0)
        {
            std::cout << "[DEBUG] Received ACK: " << type << " from " << client->user->getUsername() << std::endl;
        }
        else if (type == "REQUEST_NICK")
        {
            handleRequestNick(client, data);
        }
        else if (type == "ROOM_CREATE")
        {
            handleRoomCreate(client, data, rooms);
        }
        else if (type == "REQUEST_ROOMS")
        {
            handleRequestRooms(client, rooms);
        }
        else if (type == "ROOM_JOIN")
        {
            handleRoomJoin(client, data, rooms);
        }
        else if (type == "ROOM_LEAVE")
        {
            handleRoomLeave(client, data, rooms);
        }
        else if (type == "PLAYER_READY")
        {
            handlePlayerReady(client, rooms);
        }
        else if (type == "PLAYER_WANT")
        {
            handlePlayerWant(client, data, rooms);
        }
        else if (type == "CHAT_MESSAGE")
        {
            handleChatMessage(client, data, rooms);
        }
        else if (type == "MAKE_MOVE")
        {
            handleMakeMove(client, data, rooms);
        }
        else
        {
            std::cout << "[ERR] Unknown message type received: " << type << " from " << client->user->getUsername() << std::endl;
        }
    }
    catch (const json::parse_error &e)
    {
        std::cerr << "[ERR] JSON Parse Error: " << e.what() << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << "[ERR] Error in MessageHandler: " << e.what() << std::endl;
    }
}

// ===== Incoming messages =====

void MessageHandler::handleRequestNick(Client *client, const json &data)
{
    try
    {
        std::string newNick = data["nick"];

        if (client->user)
        {
            client->user->setUsername(newNick);

            // Prepare ACK_REQUEST_NICK response
            json response = {
                {"type", "ACK_REQUEST_NICK"},
                {"data", {"id", client->user->getPlayerID()}}};

            client->connection->sendMessage(response.dump());
            std::cout << "[DEBUG] User " << client->user->getPlayerID() << " set nickname to: " << newNick << std::endl;
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "[ERR] Error in handleRequestNick: " << e.what() << std::endl;
    }
}

void MessageHandler::handleRoomCreate(Client *client, const json &data, std::unordered_map<std::string, std::unique_ptr<Room>> &rooms)
{
    try
    {
        std::string newRoomName = data["roomName"];

        // Check if room name is already taken
        for (const auto &[name, roomPtr] : rooms)
        {
            if (name == newRoomName)
            {
                json response = {
                    {"type", "ERR_ROOM_CREATE"},
                    {"data", {"reason", "Room with this name already exists !!!"}}};

                client->connection->sendMessage(response.dump());
                return;
            }
        }

        // Create room and add owner
        rooms[newRoomName] = std::move(std::make_unique<Room>(newRoomName, *client->user));
        client->user->setInRoom(true);
        client->room = rooms[newRoomName].get();

        // Prepare ACK_ROOM_CREATE response
        json response = {
            {"type", "ACK_ROOM_CREATE"},
            {"data", nullptr}};

        client->connection->sendMessage(response.dump());
        std::cout << "[DEBUG] User " << client->user->getUsername() << " created room: " << newRoomName << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << "[ERR] Error in handleRoomCreate: " << e.what() << std::endl;
    }
}

void MessageHandler::handleRequestRooms(Client *client, std::unordered_map<std::string, std::unique_ptr<Room>> &rooms)
{
    try
    {
        json roomsArray = json::array();

        for (const auto &[name, roomPtr] : rooms)
        {
            roomsArray.push_back({name, *roomPtr});
        }

        // Prepare ACK_REQUEST_ROOMS response
        json response = {
            {"type", "ACK_REQUEST_ROOMS"},
            {"data", {"rooms", roomsArray}}};

        client->connection->sendMessage(response.dump());
        std::cout << "[DEBUG] Sent room list to " << client->user->getUsername() << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << "[ERR] Error in handleRequestRooms: " << e.what() << std::endl;
    }
}

void MessageHandler::handleRoomJoin(Client *client, const json &data, std::unordered_map<std::string, std::unique_ptr<Room>> &rooms)
{
    try
    {
        std::string roomName = data["roomName"];
        Room *targetRoom = nullptr;

        auto it = rooms.find(roomName);
        if (it != rooms.end())
        {
            targetRoom = it->second.get();
        }

        if (!targetRoom)
        {
            json response = {
                {"type", "ERR_ROOM_JOIN"},
                {"data", {"reason", "Room not found !!!"}}};

            client->connection->sendMessage(response.dump());
            return;
        }

        targetRoom->addPlayer(*client->user);
        client->user->setInRoom(true);
        client->room = targetRoom;

        // Prepare ACK_ROOM_JOIN response
        json response = {
            {"type", "ACK_ROOM_JOIN"},
            {"data", {"room", *targetRoom}}};

        client->connection->sendMessage(response.dump());
        std::cout << "[DEBUG] User " << client->user->getUsername() << " joined: " << roomName << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << "[ERR] Error in handleRoomJoin: " << e.what() << std::endl;
    }
}

void MessageHandler::handleRoomLeave(Client *client, const json &data, std::unordered_map<std::string, std::unique_ptr<Room>> &rooms)
{
    try
    {
        std::string roomName = data["roomName"];
        Room *targetRoom = nullptr;

        auto it = rooms.find(roomName);
        if (it != rooms.end())
        {
            targetRoom = it->second.get();
        }

        if (!targetRoom)
        {
            json response = {
                {"type", "ERR_ROOM_LEAVE"},
                {"data", {"reason", "Room not found !!!"}}};

            client->connection->sendMessage(response.dump());
            return;
        }

        targetRoom->removePlayer(*client->user);
        client->user->setInRoom(false);
        client->user->setReady(false);
        client->room = nullptr;

        // Prepare ACK_ROOM_LEAVE response
        json response = {
            {"type", "ACK_ROOM_LEAVE"},
            {"data", {"room", *targetRoom}}};

        client->connection->sendMessage(response.dump());
        std::cout << "[DEBUG] User " << client->user->getUsername() << " left room: " << roomName << std::endl;

        // If room is empty, remove it
        if (targetRoom->getPlayerCount() == 0)
        {
            std::cout << "[DEBUG] Room " << roomName << " is empty. Deleting..." << std::endl;
            rooms.erase(it);
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "[ERR] Error in handleRoomLeave: " << e.what() << std::endl;
    }
}

void MessageHandler::handlePlayerReady(Client *client, std::unordered_map<std::string, std::unique_ptr<Room>> &rooms)
{
    try
    {
        if (client->room)
        {
            client->user->setReady(!client->user->isReady());

            // Prepare ACK_PLAYER_READY response
            json response = {
                {"type", "ACK_PLAYER_READY"},
                {"data", nullptr}};

            client->connection->sendMessage(response.dump());
            std::cout << "[DEBUG] User " << client->user->getUsername() << " is ready? " << client->user->isReady() << std::endl;
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "[ERR] Error in handlePlayerReady: " << e.what() << std::endl;
    }
}

void MessageHandler::handlePlayerWant(Client *client, const json &data, std::unordered_map<std::string, std::unique_ptr<Room>> &rooms)
{
    try
    {
        bool player = data["player"];

        if (client->room)
        {
            client->user->setPlayer(player);

            // Prepare ACK_PLAYER_READY response
            json response = {
                {"type", "ACK_PLAYER_READY"},
                {"data", nullptr}};

            client->connection->sendMessage(response.dump());
            std::cout << "[DEBUG] User " << client->user->getUsername() << " is player? " << client->user->isPlayer() << std::endl;
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "[ERR] Error in handlePlayerWant: " << e.what() << std::endl;
    }
}

void MessageHandler::handleChatMessage(Client *client, const json &data, std::unordered_map<std::string, std::unique_ptr<Room>> &rooms)
{
    try
    {
        std::string newMessage = data["message"];

        // Broadcast to all in room
        broadcastUpdateChat(client->room, newMessage);

        // Prepare ACK_CHAT_MESSAGE response
        json response = {
            {"type", "ACK_CHAT_MESSAGE"},
            {"data", nullptr}};

        client->connection->sendMessage(response.dump());
        std::cout << "[DEBUG] User " << client->user->getUsername() << " send message. " << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << "[ERR] Error in handleChatMessage: " << e.what() << std::endl;
    }
}

// void MessageHandler::handleMakeMove(Client *client, const json &data, std::unordered_map<std::string, std::unique_ptr<Room>> &rooms)
// {
//     try
//     {
//         Move newMove = data["move"];

//         if (!client->room || !client->room->isMatchStarted())
//         {
//             json response = {
//                 {"type", "ERR_MAKE_MOVE"},
//                 {"data", {"reason", "Move not allowed at this time.+ !!!"}}};

//             client->connection->sendMessage(response.dump());
//             return;
//         }

//         if (client->room->getBoard().makeMove(newMove))
//         {
//             client->connection->sendMessage(json({{"type", "ACK_MAKE_MOVE"}, {"data", nullptr}}).dump());
//             // TODO: broadcastMoveMade(*targetRoom, data);
//         }
//         else
//         {
//             json errorResponse = {
//                 {"type", "ERR_MAKE_MOVE"},
//                 {"data", targetRoom->getBoardState()}};
//             client->connection->sendMessage(errorResponse.dump());
//         }
//     }
//     catch (const std::exception &e)
//     {
//         std::cerr << "[ERR] Error in handleMakeMove: " << e.what() << std::endl;
//     }
// }

// // ===== Outgoing messages =====

// void MessageHandler::broadcastUpdateMove(const Room* room, const Move &newMove)
// {
//     try
//     {
//         json roomList = json::array();
//         for (const auto &room : rooms)
//         {
//             roomList.push_back({{"name", room->getName()},
//                                 {"players", room->getPlayerCount()},
//                                 {"maxPlayers", 2},
//                                 {"status", room->isGameStarted() ? "in_game" : "waiting"}});
//         }

//         std::string serializedMsg = json({{"type", "UPDATE_ROOMS"}, {"data", roomList}}).dump();

//         for (const auto &c : allClients)
//         {
//             if (c && c->connection)
//                 c->connection->sendMessage(serializedMsg);
//         }
//     }
//     catch (const std::exception &e)
//     {
//         std::cerr << "[ERR] Error during broadcastUpdateRooms: " << e.what() << std::endl;
//     }
// }

// void MessageHandler::broadcastUpdateChat(const Room* room, const std::string &newMessage)
// {
//     try
//     {
//         json chatUpdate = {
//             {"type", "UPDATE_CHAT"},
//             {"data", {{"message", message}, {"user", {{"username", sender.getUsername()}, {"id", sender.getId()}}}}}};

//         std::string serializedMsg = chatUpdate.dump();
//         auto clientsInRoom = room.getAllClients();

//         for (auto *c : clientsInRoom)
//         {
//             if (c && c->connection)
//                 c->connection->sendMessage(serializedMsg);
//         }
//     }
//     catch (const std::exception &e)
//     {
//         std::cerr << "[ERR] Error in broadcastChat: " << e.what() << std::endl;
//     }
// }
