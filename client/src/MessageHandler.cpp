#include "../headers/MessageHandler.hpp"
#include <iostream>

void MessageHandler::handlePing(ConnectionManager* connectionManager)
{
    //call connection manager to send ping 
}

void MessageHandler::handleReceivePing(ConnectionManager* connectionManager)
{
    //call connection manager to receive ping 
}

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

void MessageHandler::handleCreateRoom(ConnectionManager *connectionManager, const std::string &jsonText)
{
    handleGeneralSend(connectionManager, jsonText);
}

void MessageHandler::handleJoinRoom(ConnectionManager *connectionManager, const std::string &jsonText)
{
    handleGeneralSend(connectionManager, jsonText);
}

void MessageHandler::handleExitRoom(ConnectionManager *connectionManager, const std::string &jsonText)
{
    handleGeneralSend(connectionManager, jsonText);
}

void MessageHandler::handleReceiveRooms(LobbyView *view, ConnectionManager *connectionManager, const nlohmann::json &data)
{
    //To be implemented later when view is ready 
}

void MessageHandler::handleReceiveRoom(LobbyView *view, ConnectionManager *connectionManager, const nlohmann::json &data)
{
    //To be implemented later when view is ready
}

void MessageHandler::handleFlipReady(ConnectionManager *connectionManager, const std::string &jsonText)
{
    handleGeneralSend(connectionManager, jsonText);
}

void MessageHandler::handleFlipPlayerWant(ConnectionManager *connectionManager, const std::string &jsonText)
{
    handleGeneralSend(connectionManager, jsonText);
}

void MessageHandler::handleSetReady(User *user, const nlohmann::json &data)
{
    user->setReady(data.at("ready").get<bool>());
}

void MessageHandler::handleSetPlayerWant(User *user, const nlohmann::json &data)
{
    user->setPlayer(data.at("player").get<bool>());
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

void MessageHandler::handleBoardMissmatch(ConnectionManager* connectionManager, const std::string &jsonText)
{
    handleGeneralSend(connectionManager, jsonText);
}

void MessageHandler::handleMove(GameView *view, const nlohmann::json &data)
{
    view->getBoard()->makeMove(data.at("move").get<Move>());
}

void MessageHandler::handleGameFinale(GameView *view, const nlohmann::json &data)
{
    view->setGameState(data.at("reason").get<MatchEndReasons>());
}

void MessageHandler::handleAckErrMoveMade(GameView *view, const nlohmann::json &data)
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

void MessageHandler::handleView(View* view, ConnectionManager* connectionManager, User* user, const std::string &jsonText)
{
    try
    {
        nlohmann::json j = nlohmann::json::parse(jsonText);

        std::string type = j.at("type");
        nlohmann::json data = j.contains("data")? j.at("data"): nlohmann::json(); 
        
        if(ConnectView* connectView = dynamic_cast<ConnectView*>(view))
        {
            if (type == "REQUEST_NICK") handleGetUsernameID(connectionManager, jsonText);
            else if (type == "ACK_REQUEST_NICK") handleSetUsernameID(connectView, connectionManager, user, data);
            else if (type == "PING") handlePing(connectionManager);
            else if (type == "ACK_PING") handleReceivePing(connectionManager);
            else handleIgnore();
        }
        else if (LobbyView* lobbyView = dynamic_cast<LobbyView*>(view))
        {
            if (type == "REQUEST_ROOMS") handleListRooms(connectionManager, jsonText);
            else if (type == "ROOM_CREATE") handleCreateRoom(connectionManager, jsonText);
            else if (type == "ROOM_JOIN") handleJoinRoom(connectionManager, jsonText);
            else if (type == "ROOM_LEAVE") handleExitRoom(connectionManager, jsonText);
            else if (type == "UPDATE_ROOMS") handleReceiveRooms(lobbyView, connectionManager, data); // maybe should be different handler function
            else if (type == "ACK_REQUEST_ROOMS") handleReceiveRooms(lobbyView, connectionManager, data);
            else if (type == "ACK_ROOM_CREATE") handleReceiveRoom(lobbyView, connectionManager, data);
            else if (type == "ACK_ROOM_JOIN") handleReceiveRoom(lobbyView, connectionManager, data);
            else if (type == "ACK_ROOM_LEAVE") handleReceiveRoom(lobbyView, connectionManager, data); //potencially somethig different coz u leave or ignore
            else if (type == "PING") handlePing(connectionManager);
            else if (type == "ACK_PING") handleReceivePing(connectionManager);
            else handleIgnore();
        }
        else if (RoomView* roomView = dynamic_cast<RoomView*>(view))
        {
            if (type == "PLAYER_READY") handleFlipReady(connectionManager, jsonText);
            else if (type == "PLAYER_WANT") handleFlipPlayerWant(connectionManager, jsonText);
            else if (type == "ACK_PLAYER_READY") handleSetReady(user, data);
            else if (type == "ACK_PLAYER_WANT") handleSetPlayerWant(user, data);
            else if (type == "ERR_PLAYER_WANT") handleErrPlayerWant(user, data);
            else if (type == "PING") handlePing(connectionManager);
            else if (type == "ACK_PING") handleReceivePing(connectionManager);
            else handleIgnore();
        }
        else if(GameView* gameView = dynamic_cast<GameView*>(view))
        {
            if (type == "CHAT_MESSAGE") handleSendMessage(connectionManager, jsonText);
            else if (type == "MAKE_MOVE") handleMakeMove(connectionManager, jsonText);
            else if (type == "MOVE_MADE") handleMove(gameView, data); // mayube different handler function
            else if (type == "UPDATE_CHAT") handleChatMessage(gameView, connectionManager, data); //meybe different handler function
            else if (type == "GAME_FINALE") handleGameFinale(gameView, data);
            else if (type == "ACK_CHAT_MESSAGE") handleChatMessage(gameView, connectionManager, data);
            else if (type == "ACK_MAKE_MOVE") handleMove(gameView, data); //must be different function call
            else if (type == "ERR_MAKE_MOVE") handleBoardMissmatch(connectionManager, jsonText);
            else if (type == "ACK_ERR_MOVE_MADE") handleAckErrMoveMade(gameView, data);
            else if (type == "PING") handlePing(connectionManager);
            else if (type == "ACK_PING") handleReceivePing(connectionManager);
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