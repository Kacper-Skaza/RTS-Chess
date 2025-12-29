#include "../headers/ConnectionManager.hpp"

ConnectionManager::ConnectionManager(SOCKET socketFd) : socketFd(socketFd), lastPingSend(std::chrono::steady_clock::now()), lastPingRecv(std::chrono::steady_clock::now())
{
    this->setNonBlocking(this->socketFd);
    this->sendNewPing();
    this->update();
}

// Management
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

void ConnectionManager::closeConnection()
{
    close(this->socketFd);
}

void ConnectionManager::update()
{
    // Send every 5 seconds
    if (this->getTimeSinceLastPingRecv().count() >= 5 && this->getTimeSinceLastPingSend().count() >= 5)
    {
        this->lastPingSend = std::chrono::steady_clock::now();
        this->sendNewPing();
    }

    this->recvRaw();
    this->sendRaw();
}

// Ping
void ConnectionManager::sendNewPing()
{
    this->sendMessage("{PING}");
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

// Communication
void ConnectionManager::sendMessage(const std::string &msg)
{
    this->outgoingQueue.push(msg + '\n');
    this->sendRaw();
}

std::string ConnectionManager::recvMessage()
{
    this->recvRaw();
    if (this->incomingQueue.empty())
        return "";

    std::string temp = this->incomingQueue.front();
    this->incomingQueue.pop();
    return temp;
}

// Private
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
                    if (message == "{PING}")
                    {
                        // Handle PING
                        this->sendMessage("{ACK_PING}");
                    }
                    else if (message == "{ACK_PING}")
                    {
                        // Handle ACK_PING
                        this->lastPingRecv = std::chrono::steady_clock::now();
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
