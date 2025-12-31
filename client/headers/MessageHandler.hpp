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
    static void handlePing();
    static void handleReceivePing();
    static void handleIgnore();

    //Connect view handles
    static void handleConnectView();
    //Outgoing
    static void handleGetUsernameID(); 
    //Incomming
    static void handleSetUsernameID();

    //Room view handles
    static void handleRoomView();
    //Outgoing
    static void handleListRooms();
    static void handleCreateRoom();
    static void handleJoinRoom();
    static void handleExitRoom();
    //Incomming
    static void handleReceiveRooms();
    static void handleReceiveRoom();

    //Lobby view handles
    static void handleLobbyView();
    //Outgoing
    static void handleFlipReady();
    static void handleFlipPlayerWant();
    //Incomming
    static void handleSetReady();
    static void handleSetPlayerWant();

    //Room view handles
    static void handleGameView();
    //Outgoing
    static void handleSendMessage();
    static void handleMakeMove();
    //Incomming
    static void handleChatMessage();
    static void handleBoardMissmatch();
    static void handleMove();
public:
    MessageHandler() = default;
    ~MessageHandler() = default;

    static void handleView(View* view, ConnectionManager* connectionManager, const std::string &jsonText);
};