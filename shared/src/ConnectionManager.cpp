#include "../headers/ConnectionManager.hpp"

ConnectionManager::ConnectionManager(int socketFd) : socketFd(socketFd), lastPing(std::chrono::steady_clock::now()) {}

// Management
void ConnectionManager::closeConnection()
{
	close(socketFd);
}

void ConnectionManager::update()
{
	sendRaw();
	recvRaw();
	sendNewPing();
}

// Ping
void ConnectionManager::sendNewPing()
{
	sendMessage("{NEW_PING}\n");
}

std::chrono::steady_clock::time_point ConnectionManager::getLastPing()
{
	return lastPing;
}

// Communication
void ConnectionManager::sendMessage(const std::string &msg)
{
	outgoingQueue.push(msg);
	sendRaw();
}

std::string ConnectionManager::recvMessage()
{
	recvRaw();
	if (incomingQueue.empty())
        return "";

	std::string temp = incomingQueue.front();
	incomingQueue.pop();
	return temp;
}

// Private
void ConnectionManager::sendRaw()
{
	while (!outgoingQueue.empty())
	{
		std::string &msg = outgoingQueue.front();
		ssize_t sent = send(socketFd, msg.data(), msg.size(), MSG_NOSIGNAL);

		if (sent <= 0)
			return;

		// Delete only sent bytes
		msg.erase(0, sent);

		if (msg.empty())
			outgoingQueue.pop();
	}
}

void ConnectionManager::recvRaw()
{
	char buffer[512];

	while (true)
	{
		ssize_t recvd = recv(socketFd, buffer, sizeof(buffer), MSG_DONTWAIT);

		if (recvd > 0)
			incomingQueue.emplace(buffer, recvd);
		else
			return;
	}
}
