#include "../headers/MessageHandler.hpp"

// ===== SERVER DATA =====

std::unordered_map<SOCKET, std::unique_ptr<Client>> *MessageHandler::clientsPtr = nullptr;
std::unordered_map<std::string, std::unique_ptr<Room>> *MessageHandler::roomsPtr = nullptr;

// ===== INIT SERVER DATA =====

void MessageHandler::init(std::unordered_map<SOCKET, std::unique_ptr<Client>> &clientsRef, std::unordered_map<std::string, std::unique_ptr<Room>> &roomsRef)
{
    clientsPtr = &clientsRef;
    roomsPtr = &roomsRef;
}

// ===== ENTRY POINT =====

void MessageHandler::handle(Client *client, const std::string &jsonText)
{
    if (!clientsPtr || !roomsPtr)
        throw std::runtime_error("[ERR] MessageHandler is missing 'clientsPtr' or 'roomsPtr' !!!");
    if (!client->connection || !client->user)
        throw std::runtime_error("[ERR] Client is missing 'connection' or 'user' !!!");

    try
    {
        // Parse incoming string to JSON
        json j = json::parse(jsonText);

        // Extract type and data
        std::string type = j.at("type");
        json data = j.contains("data") ? j.at("data") : json();
        std::cout << "[MESSAGE_HANDLER] Handling: " << type << " for " << client->user->getUsername() << std::endl;

        // DEBUG
        std::cout << "[DEBUG] Received message: " << j.dump() << std::endl;

        // Handle specific request type
        if (type.rfind("ACK_", 0) == 0)
        {
            std::cout << "[DEBUG] Received ACK: " << type << " from " << client->user->getUsername() << std::endl;
        }
        else if (type == "REQUEST_NICK")
        {
            handleRequestNick(client, data);
        }
        else if (type == "REQUEST_ROOMS")
        {
            handleRequestRooms(client);
        }
        else if (type == "REQUEST_ROOM")
        {
            handleRequestRoom(client);
        }
        else if (type == "ROOM_CREATE")
        {
            handleRoomCreate(client, data);
        }
        else if (type == "ROOM_JOIN")
        {
            handleRoomJoin(client, data);
        }
        else if (type == "ROOM_LEAVE")
        {
            handleRoomLeave(client);
        }
        else if (type == "PLAYER_WANT")
        {
            handlePlayerWant(client, data);
        }
        else if (type == "CHANGE_PLAYER_COUNT")
        {
            handleChangePlayerCount(client);
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
            std::cerr << "[ERR] Unknown message type received: " << type << " from " << client->user->getUsername() << std::endl;
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

// ===== INCOMING MESSAGES =====

void MessageHandler::handleRequestNick(Client *client, const json &data)
{
    try
    {
        // Data
        std::string newNick = data.at("nick");

        // Handle request
        client->user->setUsername(newNick);

        // Prepare ACK_REQUEST_NICK response
        json response = {
            {"type", "ACK_REQUEST_NICK"},
            {"data", {{"id", client->user->getPlayerID()}}}};

        // Send response
        client->connection->sendMessage(response.dump());
        std::cout << "[DEBUG] User on FD " << client->user->getPlayerID() << " set nickname to '" << newNick << "'" << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << "[ERR] Error in handleRequestNick: " << e.what() << std::endl;
    }
}

void MessageHandler::handleRequestRooms(Client *client)
{
    try
    {
        // Data
        std::unordered_map<std::string, std::unique_ptr<Room>> &rooms = *roomsPtr;

        // Handle request
        json roomsArray = json::array();
        for (const auto &[name, roomPtr] : rooms)
        {
            roomsArray.push_back({name, *roomPtr});
        }

        // Prepare ACK_REQUEST_ROOMS response
        json response = {
            {"type", "ACK_REQUEST_ROOMS"},
            {"data", {{"rooms", roomsArray}}}};

        // Send response
        client->connection->sendMessage(response.dump());
        std::cout << "[DEBUG] Sent room list to " << client->user->getUsername() << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << "[ERR] Error in handleRequestRooms: " << e.what() << std::endl;
    }
}

void MessageHandler::handleRequestRoom(Client *client)
{
    try
    {
        // Check if client is in the room
        if (!client->room)
        {
            json errResponse = {
                {"type", "ERR_REQUEST_ROOM"},
                {"data", {{"reason", "Room not found !!!"}}}};

            client->connection->sendMessage(errResponse.dump());
            return;
        }

        // Prepare ACK_REQUEST_ROOM response
        json response = {
            {"type", "ACK_REQUEST_ROOM"},
            {"data", {{"room", *client->room}}}};

        // Send response
        client->connection->sendMessage(response.dump());
        std::cout << "[DEBUG] Sent room to " << client->user->getUsername() << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << "[ERR] Error in handleRequestRoom: " << e.what() << std::endl;
    }
}

void MessageHandler::handleRoomCreate(Client *client, const json &data)
{
    try
    {
        // Data
        std::unordered_map<std::string, std::unique_ptr<Room>> &rooms = *roomsPtr;
        std::string newRoomName = data.at("roomName");

        // Trim spaces on newRoomName
        while (!newRoomName.empty() && newRoomName.front() == ' ')
            newRoomName.erase(newRoomName.begin());

        while (!newRoomName.empty() && newRoomName.back() == ' ')
            newRoomName.pop_back();

        // Check if newRoomName is empty
        if (newRoomName.empty())
        {
            json errResponse = {
                {"type", "ERR_ROOM_CREATE"},
                {"data", {{"reason", "Cannot create room with empty name !!!"}}}};

            client->connection->sendMessage(errResponse.dump());
            return;
        }

        // Check if newRoomName is already taken
        for (const auto &[name, _] : rooms)
        {
            if (name == newRoomName)
            {
                json errResponse = {
                    {"type", "ERR_ROOM_CREATE"},
                    {"data", {{"reason", "Room with this name already exists !!!"}}}};

                client->connection->sendMessage(errResponse.dump());
                return;
            }
        }

        // Create room and add owner
        rooms[newRoomName] = std::move(std::make_unique<Room>(newRoomName, *client->user));
        client->user->setInRoom(true);
        client->user->setPlayer(true);
        client->user->setReady(false);
        client->user->setSide(ChessSide::UNKNOWN);
        client->room = rooms[newRoomName].get();

        // Prepare ACK_ROOM_CREATE response
        json response = {
            {"type", "ACK_ROOM_CREATE"},
            {"data", {{"room", *client->room}}}};

        // Send response
        client->connection->sendMessage(response.dump());
        std::cout << "[DEBUG] User " << client->user->getUsername() << " created room: " << newRoomName << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << "[ERR] Error in handleRoomCreate: " << e.what() << std::endl;
    }
}

void MessageHandler::handleRoomJoin(Client *client, const json &data)
{
    try
    {
        // Data
        std::unordered_map<std::string, std::unique_ptr<Room>> &rooms = *roomsPtr;
        std::string roomName = data.at("roomName");
        Room *targetRoom = nullptr;

        // Handle request
        auto it = rooms.find(roomName);
        if (it != rooms.end())
        {
            targetRoom = it->second.get();
        }

        if (!targetRoom)
        {
            json errResponse = {
                {"type", "ERR_ROOM_JOIN"},
                {"data", {{"reason", "Room not found !!!"}}}};

            client->connection->sendMessage(errResponse.dump());
            return;
        }

        targetRoom->addUserToRoom(*client->user);
        client->user->setInRoom(true);
        client->user->setPlayer(false);
        client->user->setReady(false);
        client->user->setSide(ChessSide::UNKNOWN);
        client->room = targetRoom;

        // Broadcast update to everyone in the room
        broadcastUpdateRoom(client->room);

        // Prepare ACK_ROOM_JOIN response
        json response = {
            {"type", "ACK_ROOM_JOIN"},
            {"data", {{"room", *targetRoom}}}};

        // Send response
        client->connection->sendMessage(response.dump());
        std::cout << "[DEBUG] User " << client->user->getUsername() << " joined: " << roomName << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << "[ERR] Error in handleRoomJoin: " << e.what() << std::endl;
    }
}

void MessageHandler::handleRoomLeave(Client *client)
{
    try
    {
        // Data
        std::unordered_map<std::string, std::unique_ptr<Room>> &rooms = *roomsPtr;

        // Check if client is in the room
        if (!client->room)
        {
            json errResponse = {
                {"type", "ERR_ROOM_LEAVE"},
                {"data", {{"reason", "You are not in the room !!!"}}}};

            client->connection->sendMessage(errResponse.dump());
            return;
        }

        // Remove user
        client->room->removePlayer(*client->user);
        client->room->removeUserFromRoom(*client->user);

        // Broadcast update to everyone in the room
        broadcastUpdateRoom(client->room);

        // If room is empty, remove it
        if (client->room->getUserCount() == 0)
        {
            std::cout << "[DEBUG] Room " << client->room->getRoomName() << " is empty. Deleting..." << std::endl;
            rooms.erase(client->room->getRoomName());
        }

        // Set flags to default values
        client->user->setInRoom(false);
        client->user->setPlayer(false);
        client->user->setReady(false);
        client->room = nullptr;

        // Prepare ACK_ROOM_LEAVE response
        json response = {
            {"type", "ACK_ROOM_LEAVE"},
            {"data", nullptr}};

        // Send response
        client->connection->sendMessage(response.dump());
        std::cout << "[DEBUG] User " << client->user->getUsername() << " left room" << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << "[ERR] Error in handleRoomLeave: " << e.what() << std::endl;
    }
}

void MessageHandler::handlePlayerWant(Client *client, const json &data)
{
    try
    {
        // Data
        PlayerWant player = data.at("player");

        // Check if client is in the room
        if (!client->room)
        {
            json errResponse = {
                {"type", "ERR_PLAYER_WANT"},
                {"data", {{"reason", "User is not in the room !!!"}}}};

            client->connection->sendMessage(errResponse.dump());
            return;
        }

        // Set user to PlayerWant::SPECTATOR by default
        client->room->removePlayer(*client->user);
        client->user->setPlayer(false);
        client->user->setReady(false);

        // Check if user wants to become a player
        if (player == PlayerWant::PLAYER_READY || player == PlayerWant::PLAYER_NOT_READY)
        {
            if (client->room->getPlayerCount() >= client->room->getMaxPlayerCount())
            {
                json errResponse = {
                    {"type", "ERR_PLAYER_WANT"},
                    {"data", {{"reason", "Room already has maximum number of players !!!"}}}};

                client->connection->sendMessage(errResponse.dump());
                return;
            }

            // Set user to PlayerWant::PLAYER_NOT_READY
            client->room->addPlayer(*client->user);
            client->user->setPlayer(true);

            if (player == PlayerWant::PLAYER_READY)
            {
                // Set user to PlayerWant::PLAYER_READY
                client->user->setReady(true);
            }
        }

        // Broadcast update to everyone in the room
        broadcastUpdateRoom(client->room);

        // Prepare ACK_PLAYER_WANT response
        json response = {
            {"type", "ACK_PLAYER_WANT"},
            {"data", {{"player", player}}}};

        // Send response
        client->connection->sendMessage(response.dump());
        std::cout << "[DEBUG] User '" << client->user->getUsername() << "' in room '" << client->room->getRoomName() << "' ";
        std::cout << "is [" << client->user->isPlayer() << ", " << client->user->isReady() << "]" << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << "[ERR] Error in handlePlayerWant: " << e.what() << std::endl;
    }
}

void MessageHandler::handleChangePlayerCount(Client *client)
{
    try
    {
        // Check if client is in the room
        if (!client->room)
        {
            json errResponse = {
                {"type", "ERR_CHANGE_PLAYER_COUNT"},
                {"data", {{"reason", "User is not in the room !!!"}}}};

            client->connection->sendMessage(errResponse.dump());
            return;
        }

        // Bump MaxPlayerCount to next value
        client->room->bumpMaxPlayerCount();

        // Broadcast update to everyone in the room
        broadcastUpdateRoom(client->room);

        // Prepare ACK_CHANGE_PLAYER_COUNT response
        json response = {
            {"type", "ACK_CHANGE_PLAYER_COUNT"},
            {"data", nullptr}};

        // Send response
        client->connection->sendMessage(response.dump());
        std::cout << "[DEBUG] User '" << client->user->getUsername() << "' in room '" << client->room->getRoomName() << "' ";
        std::cout << "changed MaxPlayerCount in room " << client->room->getMaxPlayerCount() << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << "[ERR] Error in handleChangePlayerCount: " << e.what() << std::endl;
    }
}

void MessageHandler::handleChatMessage(Client *client, const json &data)
{
    try
    {
        // Data
        std::string newMessage = data.at("message");

        // Trim spaces on newMessage
        while (!newMessage.empty() && newMessage.front() == ' ')
            newMessage.erase(newMessage.begin());

        while (!newMessage.empty() && newMessage.back() == ' ')
            newMessage.pop_back();

        // Check if newMessage is empty
        if (newMessage.empty())
        {
            json errResponse = {
                {"type", "ERR_CHAT_MESSAGE"},
                {"data", {{"reason", "Cannot send an empty message!!!"}}}};

            client->connection->sendMessage(errResponse.dump());
            return;
        }

        // Broadcast update to everyone in the room
        broadcastUpdateChat(client->room, client->user.get(), newMessage);

        // Prepare ACK_CHAT_MESSAGE response
        json response = {
            {"type", "ACK_CHAT_MESSAGE"},
            {"data", nullptr}};

        // Send response
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
        // Data
        Move newMove = Move::from_json(data.at("move"), &client->room->getBoard());

        // Check if client is in the room
        if (!client->room || !client->room->isMatchStarted())
        {
            json errResponse = {
                {"type", "ERR_MAKE_MOVE"},
                {"data", {{"reason", "Move not allowed at this time !!!"}}}};

            client->connection->sendMessage(errResponse.dump());
            return;
        }

        // Check if move is valid
        if (!client->room->getBoard().makeMove(newMove))
        {
            json errResponse = {
                {"type", "ERR_MAKE_MOVE"},
                {"data", {{"reason", "Invalid move !!!"}, {"board", client->room->getBoard()}}}};

            client->connection->sendMessage(errResponse.dump());
            return;
        }

        // Broadcast update to everyone in the room
        broadcastMoveMade(client->room, client->user.get(), data.at("move"));

        // Prepare ACK_MAKE_MOVE response
        json response = {
            {"type", "ACK_MAKE_MOVE"},
            {"data", nullptr}};

        // Send response
        client->connection->sendMessage(response.dump());
        std::cout << "[DEBUG] User '" << client->user->getUsername() << "' in room '" << client->room->getRoomName() << "' ";
        std::cout << "made a move from ";
        std::cout << "[" << newMove.getFrom().first << ", " << newMove.getFrom().second << "] to ";
        std::cout << "[" << newMove.getTo().first << ", " << newMove.getTo().second << "]" << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << "[ERR] Error in handleMakeMove: " << e.what() << std::endl;
    }
}

// ===== OUTGOING MESSAGES =====

void MessageHandler::broadcastMoveMade(const Room *room, const User *user, const json &newMove)
{
    try
    {
        // Data
        std::unordered_map<SOCKET, std::unique_ptr<Client>> &clients = *clientsPtr;
        const std::unordered_map<unsigned int, User *> &roomUsers = room->getUserList();

        // Prepare MOVE_MADE broadcast
        json broadcastData = {
            {"type", "MOVE_MADE"},
            {"data", {{"move", newMove}}}};

        // Broadcast update to everyone in the room
        for (const auto &[_, roomUser] : roomUsers)
        {
            unsigned long long id = roomUser->getPlayerID();

            // Do not send a move to the client making it
            if (id != user->getPlayerID())
                clients[static_cast<SOCKET>(id)]->connection->sendMessage(broadcastData.dump());
        }
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
        // Data
        std::unordered_map<SOCKET, std::unique_ptr<Client>> &clients = *clientsPtr;
        const std::unordered_map<unsigned int, User *> &roomUsers = room->getUserList();

        // Prepare UPDATE_CHAT broadcast
        json broadcastData = {
            {"type", "UPDATE_CHAT"},
            {"data", {{"message", newMessage}, {"user", *user}}}};

        // Broadcast update to everyone in the room
        for (const auto &[_, roomUser] : roomUsers)
        {
            unsigned long long id = roomUser->getPlayerID();
            clients[static_cast<SOCKET>(id)]->connection->sendMessage(broadcastData.dump());
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "[ERR] Error in broadcastUpdateChat: " << e.what() << std::endl;
    }
}

void MessageHandler::broadcastUpdateRoom(Room *room)
{
    try
    {
        // Data
        std::unordered_map<SOCKET, std::unique_ptr<Client>> &clients = *clientsPtr;
        const std::unordered_map<unsigned int, User *> &roomUsers = room->getUserList();

        // Check if match is ready to start
        if (!room->isMatchStarted() && room->isMatchReady())
        {
            room->startMatch();
        }

        // Prepare UPDATE_ROOM broadcast
        json broadcastData = {
            {"type", "UPDATE_ROOM"},
            {"data", {{"room", *room}}}};

        // Broadcast update to everyone in the room
        for (const auto &[_, roomUser] : roomUsers)
        {
            unsigned long long id = roomUser->getPlayerID();
            clients[static_cast<SOCKET>(id)]->connection->sendMessage(broadcastData.dump());
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "[ERR] Error in broadcastUpdateRoom: " << e.what() << std::endl;
    }
}
