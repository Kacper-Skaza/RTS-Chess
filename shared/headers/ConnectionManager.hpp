#pragma once

#include <chrono>
#include <string>
#include <queue>

#include "PlatformManager.hpp"

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

    // ===== MANAGEMENT =====
    static void setNonBlocking(SOCKET fd);
    static void closeConnection(SOCKET fd);
    void update();

    // ===== PING =====
    void sendNewPing();
    std::chrono::seconds getTimeSinceLastPingSend();
    std::chrono::seconds getTimeSinceLastPingRecv();

    // ===== COMMUNICATION =====
    void sendMessage(const std::string &msg);
    std::string recvMessage();
};
