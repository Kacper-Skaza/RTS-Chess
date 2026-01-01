#include "../headers/MessageHandler.hpp"

// ===== Init pointers =====

std::unordered_map<SOCKET, std::unique_ptr<Client>> *MessageHandler::clientsPtr = nullptr;
std::unordered_map<std::string, std::unique_ptr<Room>> *MessageHandler::roomsPtr = nullptr;

void MessageHandler::init(std::unordered_map<SOCKET, std::unique_ptr<Client>> &clientsRef, std::unordered_map<std::string, std::unique_ptr<Room>> &roomsRef)
{
    clientsPtr = &clientsRef;
    roomsPtr = &roomsRef;
}

// ===== Main entry point =====

void MessageHandler::handle(Client *client, const std::string &jsonText)
{
    if (!clientsPtr || !roomsPtr)
        return;

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
            handleRoomCreate(client, data);
        }
        else if (type == "REQUEST_ROOMS")
        {
            handleRequestRooms(client);
        }
        else if (type == "ROOM_JOIN")
        {
            handleRoomJoin(client, data);
        }
        else if (type == "ROOM_LEAVE")
        {
            handleRoomLeave(client, data);
        }
        else if (type == "PLAYER_READY")
        {
            handlePlayerReady(client);
        }
        else if (type == "PLAYER_WANT")
        {
            handlePlayerWant(client, data);
        }
        else if (type == "CHAT_MESSAGE")
        {
            handleChatMessage(client, data);
        }
        else if (type == "MAKE_MOVE")
        {
            handleMakeMove(client, data);
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

void MessageHandler::handleRoomCreate(Client *client, const json &data)
{
    try
    {
        std::unordered_map<std::string, std::unique_ptr<Room>> &rooms = *roomsPtr;
        std::string newRoomName = data["roomName"];

        // Check if room name is already taken
        for (const auto &[name, _] : rooms)
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
            {"data", {"room", *client->room}}};

        client->connection->sendMessage(response.dump());
        std::cout << "[DEBUG] User " << client->user->getUsername() << " created room: " << newRoomName << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << "[ERR] Error in handleRoomCreate: " << e.what() << std::endl;
    }
}

void MessageHandler::handleRequestRooms(Client *client)
{
    try
    {
        std::unordered_map<std::string, std::unique_ptr<Room>> &rooms = *roomsPtr;
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

void MessageHandler::handleRoomJoin(Client *client, const json &data)
{
    try
    {
        std::unordered_map<std::string, std::unique_ptr<Room>> &rooms = *roomsPtr;
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

void MessageHandler::handleRoomLeave(Client *client, const json &data)
{
    try
    {
        std::unordered_map<std::string, std::unique_ptr<Room>> &rooms = *roomsPtr;
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
            {"data", nullptr}};

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

void MessageHandler::handlePlayerReady(Client *client)
{
    try
    {
        if (client->room)
        {
            client->user->setReady(!client->user->isReady());

            // Prepare ACK_PLAYER_READY response
            json response = {
                {"type", "ACK_PLAYER_READY"},
                {"data", {"ready", client->user->isReady()}}};

            client->connection->sendMessage(response.dump());
            std::cout << "[DEBUG] User " << client->user->getUsername() << " is ready? " << client->user->isReady() << std::endl;
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "[ERR] Error in handlePlayerReady: " << e.what() << std::endl;
    }
}

void MessageHandler::handlePlayerWant(Client *client, const json &data)
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

void MessageHandler::handleChatMessage(Client *client, const json &data)
{
    try
    {
        std::string newMessage = data["message"];

        // Broadcast to all in room
        broadcastUpdateChat(client->room, client->user.get(), newMessage);

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

void MessageHandler::handleMakeMove(Client *client, const json &data)
{
    try
    {
        Move newMove = data["move"];

        if (!client->room || !client->room->isMatchStarted())
        {
            json response = {
                {"type", "ERR_MAKE_MOVE"},
                {"data", {"reason", "Move not allowed at this time.+ !!!"}}};

            client->connection->sendMessage(response.dump());
            return;
        }

        if (client->room->getBoard().makeMove(newMove))
        {
            // Broadcast to all in room
            broadcastMoveMade(client->room, client->user.get(), newMove);

            // Prepare ACK_MAKE_MOVE response
            json response = {
                {"type", "ACK_MAKE_MOVE"},
                {"data", nullptr}};

            client->connection->sendMessage(response.dump());
            std::cout << "[DEBUG] User " << client->user->getUsername() << " made move. " << std::endl;
        }
        else
        {
            json response = {
                {"type", "ERR_MAKE_MOVE"},
                {"data", {"board", client->room->getBoard()}}};

            client->connection->sendMessage(response.dump());
            return;
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "[ERR] Error in handleMakeMove: " << e.what() << std::endl;
    }
}

// ===== Outgoing messages =====

void MessageHandler::broadcastMoveMade(const Room *room, const User *user, const Move &newMove)
{
    try
    {
        std::unordered_map<SOCKET, std::unique_ptr<Client>> &clients = *clientsPtr;
        json broadcastData = {
            {"type", "MOVE_MADE"},
            {"data", {{"move", newMove}}}};

        const std::unordered_map<unsigned int, User> &roomUsers = room->getUserList();

        // Send newMove to everyone
        for (const auto &[_, roomUser] : roomUsers)
        {
            unsigned long long id = roomUser.getPlayerID();

            if (id != user->getPlayerID())
                clients[static_cast<SOCKET>(id)]->connection->sendMessage(broadcastData.dump());
        }

        std::cout << "[DEBUG] Broadcasted move in room: " << room->getRoomName() << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << "[ERR] Error in broadcastMoveMade: " << e.what() << std::endl;
    }
}

void MessageHandler::broadcastUpdateChat(const Room *room, const User *user, const std::string &newMessage)
{
    try
    {
        std::unordered_map<SOCKET, std::unique_ptr<Client>> &clients = *clientsPtr;
        json broadcastData = {
            {"type", "UPDATE_CHAT"},
            {"data", {{"message", newMessage}, {"user", *user}}}};

        const std::unordered_map<unsigned int, User> &roomUsers = room->getUserList();

        // Send newMessage to everyone
        for (const auto &[_, roomUser] : roomUsers)
        {
            unsigned long long id = roomUser.getPlayerID();

            if (id != user->getPlayerID())
                clients[static_cast<SOCKET>(id)]->connection->sendMessage(broadcastData.dump());
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "[ERR] Error in broadcastChat: " << e.what() << std::endl;
    }
}
