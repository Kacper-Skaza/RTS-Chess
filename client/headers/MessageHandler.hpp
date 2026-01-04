#pragma once

#include "views/ConnectView.hpp"
#include "views/GameView.hpp"
#include "views/LobbyView.hpp"
#include "views/RoomView.hpp"
#include "views/View.hpp"

#include "../../shared/headers/ConnectionManager.hpp"
#include "../../shared/headers/Structures.hpp"

#include "../../shared/headers/dependencies/Json.hpp"

class MessageHandler
{
private:
    static void handleIgnore(const std::string &jsonText);
    static void handleGeneralSend(ConnectionManager* connectionManager, const std::string &jsonText);
    // static void handleNotInRoom(View* view, const nlohmann::json &data);

    //Connect view handles
    //Outgoing
    static void handleGetUsernameID(ConnectionManager *connectionManager, const std::string &jsonText); 
    //Incomming
    static void handleSetUsernameID(User* user, const nlohmann::json &data);

    //Lobby view handles
    //Outgoing
    static void handleListRooms(ConnectionManager* connectionManager, const std::string &jsonText);
    static void handleCreateRoom(LobbyView *view, ConnectionManager *connectionManager, const std::string &jsonText);
    static void handleJoinRoom(LobbyView *view, ConnectionManager *connectionManager, const std::string &jsonText);
    //Incomming
    static void handleReceiveRooms(LobbyView* view, const nlohmann::json &data);
    
    //Room view handles
    //Outgoing
    static void handleFlipReady(ConnectionManager* connectionManager, const std::string &jsonText);
    static void handleFlipPlayerWant(ConnectionManager* connectionManager, const std::string &jsonText);
    static void handleExitRoom(ConnectionManager* connectionManager, const std::string &jsonText);
    static void handlePlayerCountChange(ConnectionManager* connectionManager, const std::string &jsonText);
    //Incomming
    static void handleReceiveRoom(RoomView* view, const nlohmann::json &data);
    static void handleSpecialReceiveRoom(RoomView *view, ConnectionManager *connectionManager, const nlohmann::json &data);
    static void handleSetPlayerWant(User* user, const nlohmann::json &data);
    static void handleErrPlayerWant(RoomView* view, ConnectionManager* connectionManager);

    //Game view handles
    //Outgoing
    static void handleSendMessage(ConnectionManager* connectionManager, const std::string &jsonText);
    static void handleMakeMove(ConnectionManager* connectionManager, const std::string &jsonText);
    //Incomming
    static void handleChatMessage(GameView* view, ConnectionManager* connectionManager, const nlohmann::json &data);
    static void handleBoardMissmatch(GameView *view, const nlohmann::json &data);
    static void handleMove(GameView* view, ConnectionManager* connectionManager, const nlohmann::json &data);
    static void handleGameFinale(GameView* view, const nlohmann::json &data);
public:
    MessageHandler() = default;
    ~MessageHandler() = default;

    static void handleView(View* view, ConnectionManager* connectionManager, User* user, const std::string &jsonText);
};