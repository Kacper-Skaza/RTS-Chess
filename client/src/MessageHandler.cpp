#include "../headers/MessageHandler.hpp"
#include <iostream>

void MessageHandler::handlePing()
{

}

void MessageHandler::handleView(View* view, ConnectionManager* connectionManager, const std::string &jsonText)
{
    try
    {
        nlohmann::json j = nlohmann::json::parse(jsonText);

        std::string type = j.at("type");
        nlohmann::json data = j.contains("data")? j.at("data"): nlohmann::json(); 
        
        if(ConnectView* connectView = dynamic_cast<ConnectView*>(view))
        {
            if (type == "REQUEST_NICK") handleGetUsernameID();
            else if (type == "ACK_REQUEST_NICK") handleSetUsernameID();
            else handleIgnore();
        }
        else if (GameView* gameView = dynamic_cast<GameView*>(view))
        {
            if (type == "REQUEST_ROOMS") handleListRooms();
            else if (type == "ROOM_CREATE") handleCreateRoom();
            else if (type == "ROOM_JOIN") handleJoinRoom();
            else if (type == "ROOM_LEAVE") handleExitRoom();
            else if (type == "ACK_REQUEST_ROOMS")
                handleReceiveRooms();
            else if (type == "ACK_ROOM_CREATE")
                handleReceiveRoom();
            else if (type == "ACK_ROOM_JOIN")
                handleReceiveRoom();
            else if (type == "ACK_ROOM_LEAVE")
                handleReceiveRoom(); //potencially somethig different coz u leave or ignore
            else handleIgnore();
        }
        else if (LobbyView* lobbyView = dynamic_cast<LobbyView*>(view))
        {
            if (type == "")
            {
                /* code */
            }
            
        }
        else if(RoomView* roomView = dynamic_cast<RoomView*>(view))
        {
            
        }
        else
        {
            //here throw error or something idk
            throw std::logic_error("Handling this view is not supported");
        }

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