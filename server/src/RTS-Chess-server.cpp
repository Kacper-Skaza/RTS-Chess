#include <iostream>

#include "../headers/MessageHandler.hpp"

#include "../../shared/headers/Structures.hpp"
#include "../../shared/headers/ConnectionManager.hpp"
#include "../../shared/headers/User.hpp"
#include "../../shared/headers/Room.hpp"

#if PLATFORM == PLATFORM_WINDOWS
    typedef int nfds_t;
    #define poll WSAPoll
#endif

#if PLATFORM == PLATFORM_LINUX
    #include <poll.h>
    #include <netinet/in.h>
#endif

// ===== MAIN =====

int main()
{
    // Start WSA for Windows
#if PLATFORM == PLATFORM_WINDOWS
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
        return 1;
#endif

    // Port number
    const int PORT = 1337;

    // Create listening socket
    SOCKET listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    // Allow address reuse
    int opt = 1;
    setsockopt(listenSocket, SOL_SOCKET, SO_REUSEADDR, (const char *)&opt, sizeof(opt));

    // Set socket to non-blocking mode
    ConnectionManager::setNonBlocking(listenSocket);

    // Server address configuration
    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    // Bind socket
    if (bind(listenSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
    {
        std::cout << ">> RTS Chess Server failed to bind server socket on port " << PORT << std::endl;
        return 1;
    }

    // Start listening
    listen(listenSocket, SOMAXCONN);
    std::cout << ">> RTS Chess Server started listening on port " << PORT << std::endl;

    // Initialize server data
    std::chrono::steady_clock::time_point lastAllCheck = {};      // Time of last full server check
    std::unordered_map<SOCKET, std::unique_ptr<Client>> clients;  // Active clients mapped by socket FD
    std::unordered_map<std::string, std::unique_ptr<Room>> rooms; // Active game rooms mapped by roomName

    // Initialize central message handler
    MessageHandler::init(clients, rooms);

    while (true)
    {
        // Prepare pollfd vector
        std::vector<pollfd> fds;

        // Add listening socket
        fds.push_back({listenSocket, POLLIN, 0});

        // Add client sockets
        for (auto const &[fd, client] : clients)
        {
            fds.push_back({fd, POLLIN, 0});
        }

        // Wait up to 100ms for events
        int pollResult = poll(fds.data(), static_cast<nfds_t>(fds.size()), 100);

        // Process socket events
        if (pollResult > 0)
        {
            // Handle incoming connections
            if (fds[0].revents & POLLIN)
            {
                sockaddr_in clientAddr;
                SocketLen addrLen = sizeof(clientAddr);

                SOCKET newSock = accept(listenSocket, (struct sockaddr *)&clientAddr, &addrLen);

                if (newSock != INVALID_SOCKET_HANDLE)
                {
                    ConnectionManager::setNonBlocking(newSock);

                    std::unique_ptr<Client> newClient = std::make_unique<Client>();
                    newClient->connection = std::make_unique<ConnectionManager>(newSock);
                    newClient->user = std::make_unique<User>("Guest", static_cast<unsigned long long>(newSock));
                    newClient->room = nullptr;

                    std::cout << "[SERVER] Client " << newClient->user->getUsername() << " on FD " << newSock << " connected :)" << std::endl;
                    clients[newSock] = std::move(newClient);
                }
            }

            // Handle client events
            for (size_t i = 1; i < fds.size(); i++)
            {
                if (fds[i].revents & POLLIN)
                {
                    SOCKET fd = fds[i].fd;

                    if (clients.count(fd))
                    {
                        Client *clientPtr = clients[fd].get();

                        // Update client data
                        clientPtr->connection->update();

                        // Process all queued messages
                        std::string msg;
                        while (!(msg = clientPtr->connection->recvMessage()).empty())
                        {
                            MessageHandler::handle(clientPtr, msg);
                        }
                    }
                }
            }
        }

        // Check all once every 1000ms
        std::chrono::steady_clock::time_point currentTime = std::chrono::steady_clock::now();
        std::chrono::milliseconds elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastAllCheck);

        if (elapsed.count() >= 1000)
        {
            for (auto it = clients.begin(); it != clients.end();)
            {
                Client *clientPtr = it->second.get();

                // Update client data
                clientPtr->connection->update();

                // Process all queued messages
                std::string msg;
                while (!(msg = clientPtr->connection->recvMessage()).empty())
                {
                    MessageHandler::handle(clientPtr, msg);
                }

                // Remove client on timeout (1 minute)
                if (clientPtr->connection->getTimeSinceLastPingRecv().count() > 60)
                {
                    std::cout << "[SERVER] Client " << clientPtr->user->getUsername() << " on FD " << it->first << " disconnected :( [timeout]" << std::endl;
                    it = clients.erase(it);
                }
                else
                {
                    it++;
                }
            }
        }
    }

    return 0;
}
