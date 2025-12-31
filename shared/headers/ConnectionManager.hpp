#pragma once

#include <chrono>
#include <string>
#include <queue>
#include <unistd.h>

#include "Structures.hpp"

#if PLATFORM == PLATFORM_WINDOWS
    #include <WinSock2.h>
    #define MSG_NOSIGNAL_PLATFORM 0
    #define INVALID_SOCKET_HANDLE INVALID_SOCKET
    using SocketResult = int;
    using SocketLen = int;
    // SOCKET is already defined by <WinSock2.h>
#endif

#if PLATFORM == PLATFORM_LINUX
    #include <sys/socket.h>
    #include <fcntl.h>
    #define MSG_NOSIGNAL_PLATFORM MSG_NOSIGNAL
    #define INVALID_SOCKET_HANDLE -1
    using SocketResult = ssize_t;
    using SocketLen = socklen_t;
    using SOCKET = int;
#endif

class ConnectionManager
{
private:
    SOCKET socketFd;

    std::chrono::steady_clock::time_point lastPingSend;
    std::chrono::steady_clock::time_point lastPingRecv;

    std::string incomingBuffer;
    std::queue<std::string> incomingQueue;
    std::queue<std::string> outgoingQueue;

    void sendRaw();
    void recvRaw();

public:
    explicit ConnectionManager(SOCKET socketFd);
    ~ConnectionManager() = default;

    // ===== Management =====
    static void setNonBlocking(SOCKET fd);
    void closeConnection();
    void update();

    // ===== Ping =====
    void sendNewPing();
    std::chrono::seconds getTimeSinceLastPingSend();
    std::chrono::seconds getTimeSinceLastPingRecv();

    // ===== Communication =====
    void sendMessage(const std::string &msg);
    std::string recvMessage();
};
