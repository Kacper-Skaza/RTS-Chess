#include "../headers/ConnectionManager.hpp"

// ===== STATIC =====

const std::string ConnectionManager::PING = "{PING}";
const std::string ConnectionManager::ACK_PING = "{ACK_PING}";

// ===== INIT CONNECTION MANAGER =====

ConnectionManager::ConnectionManager(SOCKET socketFd) : socketFd(socketFd)
{
    std::chrono::steady_clock::time_point currentTime = std::chrono::steady_clock::now();
    this->lastPingSend = currentTime;
    this->lastPingRecv = currentTime;

    this->setNonBlocking(this->socketFd);
    this->update();
}

// ===== MANAGEMENT =====

void ConnectionManager::setNonBlocking(SOCKET fd)
{
#if PLATFORM == PLATFORM_WINDOWS
    unsigned long mode = 1;
    ioctlsocket(fd, FIONBIO, &mode);
#endif

#if PLATFORM == PLATFORM_LINUX
    int flags = fcntl(fd, F_GETFL, 0);
    fcntl(fd, F_SETFL, flags | O_NONBLOCK);
#endif
}

void ConnectionManager::closeConnection(SOCKET fd)
{
    // Check if socket is already closed
    if (fd == INVALID_SOCKET_PLATFORM)
        return;

#if PLATFORM == PLATFORM_WINDOWS
    closesocket(fd);
#elif PLATFORM == PLATFORM_LINUX
    close(fd);
#endif

    // Mark as closed
    fd = INVALID_SOCKET_PLATFORM;
}

void ConnectionManager::update()
{
    // Send PING every 2 seconds
    if (this->getTimeSinceLastPingRecv().count() >= 2 && this->getTimeSinceLastPingSend().count() >= 2)
    {
        this->lastPingSend = std::chrono::steady_clock::now();
        this->sendMessage(PING);
    }

    this->recvRaw();
    this->sendRaw();
}

// ===== PING =====

std::chrono::milliseconds ConnectionManager::getPingMs()
{
    // Check if ACK_PING was received in the last 5 seconds
    if (this->getTimeSinceLastPingRecv().count() <= 5)
        return this->pingMs + std::chrono::milliseconds(1);

    return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - this->lastPingRecv);
}

std::chrono::seconds ConnectionManager::getTimeSinceLastPingSend()
{
    std::chrono::steady_clock::time_point currentTime = std::chrono::steady_clock::now();
    std::chrono::seconds elapsed = std::chrono::duration_cast<std::chrono::seconds>(currentTime - this->lastPingSend);

    return elapsed;
}

std::chrono::seconds ConnectionManager::getTimeSinceLastPingRecv()
{
    std::chrono::steady_clock::time_point currentTime = std::chrono::steady_clock::now();
    std::chrono::seconds elapsed = std::chrono::duration_cast<std::chrono::seconds>(currentTime - this->lastPingRecv);

    return elapsed;
}

// ===== COMMUNICATION =====

void ConnectionManager::sendMessage(const std::string &msg)
{
    this->outgoingQueue.push(msg + '\n');
    this->update();
}

std::string ConnectionManager::recvMessage()
{
    this->update();
    if (this->incomingQueue.empty())
        return "";

    std::string temp = this->incomingQueue.front();
    this->incomingQueue.pop();
    return temp;
}

// ===== PRIVATE DATA HANDLERS =====

void ConnectionManager::sendRaw()
{
    while (!this->outgoingQueue.empty())
    {
        std::string &msg = this->outgoingQueue.front();

        // Attempt to send data using the platform-specific flags
        // static_cast<int> is used for compatibility with Windows signature
        SocketResult sent = send(this->socketFd, msg.data(), static_cast<int>(msg.size()), MSG_NOSIGNAL_PLATFORM);

        if (sent > 0)
        {
            // Remove only the bytes that were successfully sent from the string
            msg.erase(0, static_cast<size_t>(sent));

            // Message fully sent or kernel buffer is full
            if (msg.empty())
                this->outgoingQueue.pop();
            else
                return;
        }
        else
        {
            return;
        }
    }
}

void ConnectionManager::recvRaw()
{
    char buffer[512];

    while (true)
    {
        SocketResult recvd = recv(this->socketFd, buffer, sizeof(buffer), 0);

        if (recvd > 0)
        {
            // Append newly received bytes to the persistent class buffer
            this->incomingBuffer.append(buffer, static_cast<size_t>(recvd));

            // Look for newlines in the buffer to extract complete messages
            size_t pos;
            while ((pos = this->incomingBuffer.find('\n')) != std::string::npos)
            {
                // Extract the message excluding the '\n'
                std::string message = this->incomingBuffer.substr(0, pos);

                // We remove '\r' if it exists at the end (Windows standard \r\n)
                if (!message.empty() && message.back() == '\r')
                {
                    message.pop_back();
                }

                if (!message.empty())
                {
                    if (message == PING)
                    {
                        // Handle PING
                        this->sendMessage(ACK_PING);
                    }
                    else if (message == ACK_PING)
                    {
                        // Handle ACK_PING
                        this->lastPingRecv = std::chrono::steady_clock::now();
                        this->pingMs = std::chrono::duration_cast<std::chrono::milliseconds>(this->lastPingRecv - this->lastPingSend);
                    }
                    else
                    {
                        // Push message to the queue
                        this->incomingQueue.push(std::move(message));
                    }
                }

                // Remove the processed message and the '\n' from the buffer
                this->incomingBuffer.erase(0, pos + 1);
            }
        }
        else
        {
            return;
        }
    }
}
