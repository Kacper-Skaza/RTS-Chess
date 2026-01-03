#pragma once

#include <chrono>
#include <string>
#include <queue>

#include "PlatformManager.hpp"

class ConnectionManager
{
private:
    // ===== STATIC =====
    static const std::string PING;
    static const std::string ACK_PING;

    // ===== SOCKET =====
    SOCKET socketFd;

    // ===== PING =====
    std::chrono::milliseconds pingMs;
    std::chrono::steady_clock::time_point lastPingSend;
    std::chrono::steady_clock::time_point lastPingRecv;

    // ===== BUFFERS =====
    std::string incomingBuffer;
    std::queue<std::string> incomingQueue;
    std::queue<std::string> outgoingQueue;

    // ===== PRIVATE DATA HANDLERS =====
    void sendRaw();
    void recvRaw();

public:
    // ===== CONSTRUCTOR =====
    explicit ConnectionManager(SOCKET socketFd);
    ~ConnectionManager() = default;

    // ===== MANAGEMENT =====
    static void setNonBlocking(SOCKET fd);
    static void closeConnection(SOCKET fd);
    void update();

    // ===== PING =====
    std::chrono::milliseconds getPingMs();
    std::chrono::seconds getTimeSinceLastPingSend();
    std::chrono::seconds getTimeSinceLastPingRecv();

    // ===== COMMUNICATION =====
    void sendMessage(const std::string &msg);
    std::string recvMessage();
};
