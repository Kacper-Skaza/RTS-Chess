#pragma once

#include <chrono>
#include <string>
#include <queue>

class ConnectionManager
{
private:
	int socketFd;
	std::chrono::steady_clock::time_point lastPing;

	std::queue<std::string> incomingQueue;
	std::queue<std::string> outgoingQueue;

	void sendRaw();
	void recvRaw();

public:
	explicit ConnectionManager(int socketFd);
	~ConnectionManager() = default;

	// Management
	void closeConnection();
	void update();

	// Ping
	void sendNewPing();
	std::chrono::steady_clock::time_point getLastPing();

	// Communication
	void sendMessage(const std::string &msg);
	std::string recvMessage();
};
