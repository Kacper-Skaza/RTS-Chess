#include "../headers/MessageHandler.hpp"
#include <iostream>

void MessageHandler::handleIgnore()
{
    //ignore message (maybe call comething from connection manager)
}

void MessageHandler::handleGeneralSend(ConnectionManager *connectionManager, const std::string &jsonText)
{
    connectionManager->sendMessage(jsonText);
}

void MessageHandler::handleGetUsernameID(ConnectionManager *connectionManager, const std::string &jsonText)
{
    handleGeneralSend(connectionManager, jsonText);
}

void MessageHandler::handleSetUsernameID(ConnectView *view, ConnectionManager *connectionManager, User* user, const nlohmann::json &data)
{
    user->setPlayerID(data.at("id").get<unsigned long long>());
}

void MessageHandler::handleListRooms(ConnectionManager *connectionManager, const std::string &jsonText)
{
    handleGeneralSend(connectionManager, jsonText);
}

void MessageHandler::handleCreateRoom(LobbyView *view, ConnectionManager *connectionManager, const std::string &jsonText)
{
    handleGeneralSend(connectionManager, jsonText);
    view->setJoinRequested(true);
}

void MessageHandler::handleJoinRoom(LobbyView *view, ConnectionManager *connectionManager, const std::string &jsonText)
{
    handleGeneralSend(connectionManager, jsonText);
    view->setJoinRequested(true);
}

void MessageHandler::handleExitRoom(ConnectionManager *connectionManager, const std::string &jsonText)
{
    handleGeneralSend(connectionManager, jsonText);
}

void MessageHandler::handleReceiveRooms(LobbyView *view, ConnectionManager *connectionManager, const nlohmann::json &data)
{
    std::vector<Room> rooms;
    for (const auto& roomJson : data.at("rooms"))
    {
        std::cout << roomJson.at(1).dump() << "\n";
        rooms.push_back(Room());
        Room::from_json(roomJson.at(1), rooms[rooms.size() - 1]);
    }
    view->updateRooms(rooms);
}

void MessageHandler::handleReceiveRoom(RoomView *view, ConnectionManager *connectionManager, const nlohmann::json &data)
{
    Room room;
    Room::from_json(data.at("room"), room);
    view->updateRoom(room);
}

void MessageHandler::handleSpecialReceiveRoom(RoomView *view, ConnectionManager *connectionManager, const nlohmann::json &data)
{
    handleReceiveRoom(view, connectionManager, data);
    nlohmann::json j = nlohmann::json{
        {"type", "ACK_UPDATE_ROOM"},
        {"data", nullptr}
    };
    connectionManager->sendMessage(j.dump());
}


void MessageHandler::handleFlipReady(ConnectionManager *connectionManager, const std::string &jsonText)
{
    handleGeneralSend(connectionManager, jsonText);
}

void MessageHandler::handleFlipPlayerWant(ConnectionManager *connectionManager, const std::string &jsonText)
{
    handleGeneralSend(connectionManager, jsonText);
}

void MessageHandler::handleSetPlayerWant(User *user, const nlohmann::json &data)
{
    PlayerWant want = data.at("player").get<PlayerWant>();
    switch (want)
    {
    case PlayerWant::SPECTATOR:
        user->setPlayer(false);
        user->setReady(false);
        break;
    case PlayerWant::PLAYER_READY:
        user->setPlayer(true);
        user->setReady(true);
        break;
    case PlayerWant::PLAYER_NOT_READY:
        user->setPlayer(true);
        user->setReady(false);
        break;
    default:
        break;
    }
}

void MessageHandler::handleErrPlayerWant(User *user, const nlohmann::json &data)
{
    //add some error handling logic based on reason in string
    //full team cant join
    //game already started
    //unknown error
}

void MessageHandler::handleSendMessage(ConnectionManager *connectionManager, const std::string &jsonText)
{
    handleGeneralSend(connectionManager, jsonText);
}

void MessageHandler::handleMakeMove(ConnectionManager *connectionManager, const std::string &jsonText)
{
    handleGeneralSend(connectionManager, jsonText);
}

void MessageHandler::handleChatMessage(GameView* view, ConnectionManager* connectionManager, const nlohmann::json &data)
{
    std::string message = data.at("message").get<std::string>();
    User sender = data.at("user").get<User>();
    view->updateChat(message, sender);

    //send ACK back to server
    nlohmann::json response = {
        {"type", "ACK_UPDATE_CHAT"},
        {"data", nullptr}};
    connectionManager->sendMessage(response.dump());
}

void MessageHandler::handleBoardMissmatch(GameView *view, const nlohmann::json &data)
{
    Board newBoard;
    Board::from_json(data.at("board"), newBoard);

    for (size_t i = 0; i < BOARD_SIZE; i++)
    {
        for (size_t j = 0; j < BOARD_SIZE; j++)
        {
            view->getBoard()->setSpace(i, j, newBoard.getSpace(i, j));
        }
    }
}

void MessageHandler::handleMove(GameView *view, ConnectionManager* connectionManager, const nlohmann::json &data)
{
    nlohmann::json response;
    if (view->getBoard()->makeMove(data.at("move").get<Move>()))
    {
        response = {
            {"type", "ACK_MOVE_MADE"},
            {"data", nullptr}};
    }
    else
    {
        response = {
            {"type", "ERR_MOVE_MADE"},
            {"data", nullptr}};
    }
    connectionManager->sendMessage(response.dump());

}

void MessageHandler::handleGameFinale(GameView *view, const nlohmann::json &data)
{
    view->setGameState(data.at("reason").get<MatchEndReasons>());
}

void MessageHandler::handleView(View* view, ConnectionManager* connectionManager, User* user, const std::string &jsonText)
{
    if (connectionManager == nullptr || user == nullptr || view == nullptr)
        throw std::invalid_argument("Null pointer passed to MessageHandler::handleView");    
    try
    {
        nlohmann::json j = nlohmann::json::parse(jsonText);

        std::string type = j.at("type");
        nlohmann::json data = j.contains("data")? j.at("data"): nlohmann::json(); 
        
        if(ConnectView* connectView = dynamic_cast<ConnectView*>(view))
        {
            if (type == "REQUEST_NICK") handleGetUsernameID(connectionManager, jsonText);
            else handleIgnore();
        }
        else if (LobbyView* lobbyView = dynamic_cast<LobbyView*>(view))
        {
            if (type == "REQUEST_ROOMS") handleListRooms(connectionManager, jsonText);
            else if (type == "ACK_REQUEST_NICK") handleSetUsernameID(connectView, connectionManager, user, data);
            else if (type == "ROOM_CREATE") handleCreateRoom(lobbyView, connectionManager, jsonText);
            else if (type == "ROOM_JOIN") handleJoinRoom(lobbyView, connectionManager, jsonText);
            else if (type == "ACK_REQUEST_ROOMS") handleReceiveRooms(lobbyView, connectionManager, data);
            else handleIgnore();
        }
        else if (RoomView* roomView = dynamic_cast<RoomView*>(view))
        {
            if (type == "PLAYER_WANT") handleFlipPlayerWant(connectionManager, jsonText);
            else if (type == "ACK_PLAYER_WANT") handleSetPlayerWant(user, data);
            else if (type == "ERR_PLAYER_WANT") handleErrPlayerWant(user, data);
            else if (type == "ROOM_LEAVE") handleExitRoom(connectionManager, jsonText);
            else if (type == "ACK_ROOM_CREATE") handleReceiveRoom(roomView, connectionManager, data);
            else if (type == "ACK_ROOM_JOIN") handleReceiveRoom(roomView, connectionManager, data);
            else if (type == "ACK_ROOM_REQUEST") handleReceiveRoom(roomView, connectionManager, data);
            else if (type == "UPDATE_ROOM") handleSpecialReceiveRoom(roomView, connectionManager, data);
            else handleIgnore();
        }
        else if(GameView* gameView = dynamic_cast<GameView*>(view))
        {
            if (type == "CHAT_MESSAGE") handleSendMessage(connectionManager, jsonText);
            else if (type == "MAKE_MOVE") handleMakeMove(connectionManager, jsonText);
            else if (type == "MOVE_MADE") handleMove(gameView, connectionManager, data);
            else if (type == "UPDATE_CHAT") handleChatMessage(gameView, connectionManager, data);
            else if (type == "GAME_FINALE") handleGameFinale(gameView, data);
            else if (type == "ERR_MAKE_MOVE") handleBoardMissmatch(gameView, data);
            else if (type == "ACK_ERR_MOVE_MADE") handleBoardMissmatch(gameView, data);
            else if (type == "ROOM_REQUEST") handleGeneralSend(connectionManager, jsonText);
            else handleIgnore();
        }
        else throw std::logic_error("Handling this view is not supported");
    }
    catch (const nlohmann::json::parse_error &e)
    {
        std::cerr << "[ERR] JSON Parse Error: " << e.what() << "\n";
    }
    catch(const std::exception& e)
    {
        std::cerr << "[ERR] Error in MessageHandler:"<< e.what() << '\n';
    }
}