#include "../../headers/views/ConnectView.hpp"

#if PLATFORM == PLATFORM_WINDOWS
    #include <ws2tcpip.h>
#endif
#if PLATFORM == PLATFORM_LINUX
    #include <arpa/inet.h>
#endif


const std::regex ConnectView::connectionRule("^((1[0-9]{2}|[0-9]{1,2}|2[0-4][0-9]|25[0-5])\\.){3}(1[0-9]{2}|[0-9]{1,2}|2[0-4][0-9]|25[0-5]):([0-9]{1,4}|[1-5][0-9]{4}|6[0-4][0-9]{3}|65[0-4][0-9]{2}|655[0-2][0-9]|6553[0-5])$", std::regex::optimize);

ConnectView::ConnectView(SDL_Window *window, SDL_Renderer *renderer, SDLFontManager* fontManager)
    : connectionBox(window, renderer, fontManager, {660, 490, 300, 50}, "Roboto/Roboto-Medium", 20, true),
      connectButton(window, renderer, fontManager, {660, 590, 300, 50}, "Roboto/Roboto-Medium", 20),
            userBox(window, renderer, fontManager, {660, 390, 300, 50}, "Roboto/Roboto-Medium", 20, true)
{
    this->connectButton.setText("Connect");
    this->window = window;
    this->renderer = renderer;
}

TextBox& ConnectView::getConnectionBox()
{
    return this->connectionBox;
}

TextBox &ConnectView::getUserBox()
{
    return this->userBox;
}
TextBox &ConnectView::getConnectButton()
{
    return this->connectButton;
}

void ConnectView::render()
{
    //Clear screen
    SDL_SetRenderDrawColor(renderer, 60, 60, 60, 255);
    SDL_RenderClear(renderer);

    //user box render code
    SDL_SetRenderDrawColor(renderer, 90, 90, 90, 255);
    SDL_RenderFillRect(renderer, &userBox.getBoxPos());
    userBox.genTexture();
    SDL_RenderCopy(renderer, userBox.getTexture(), nullptr, &(userBox.getTextureRect()));


    //connetion box render code
    SDL_SetRenderDrawColor(renderer, 90, 90, 90, 255);
    SDL_RenderFillRect(renderer, &connectionBox.getBoxPos());
    connectionBox.genTexture();
    SDL_RenderCopy(renderer, connectionBox.getTexture(), nullptr, &(connectionBox.getTextureRect()));

    //connect button render code
    SDL_SetRenderDrawColor(renderer, 46, 87, 25, 255);
    SDL_RenderFillRect(renderer, &connectButton.getBoxPos());
    connectButton.genTexture();
    SDL_RenderCopy(renderer, connectButton.getTexture(), nullptr, &(connectButton.getTextureRect()));

    //Present to screen
    SDL_RenderPresent(renderer);
}

/**
 * This method may be unoptimized
 */
bool ConnectView::validateConnectionData()
{
    return std::regex_match(this->connectionBox.getString(), this->connectionRule);
}

SOCKET ConnectView::connectToServer()
{
    #if PLATFORM == PLATFORM_WINDOWS
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
        return 1;
    #endif

    const int separatorIndex = this->connectionBox.getString().find(':');
    std::string ip = this->connectionBox.getString().substr(0, separatorIndex);
    int port = std::stoi(this->connectionBox.getString().substr(separatorIndex + 1));
    SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock == INVALID_SOCKET_PLATFORM)
        return sock;
    sockaddr_in addr{
        .sin_family = AF_INET,
        .sin_port = htons(port),
    };
    inet_pton(AF_INET, ip.c_str(), &addr.sin_addr);

    if (connect(sock, (sockaddr*)&addr, sizeof(addr)) == SOCKET_ERROR_PLATFORM)
    {
        close(sock);
        return -1;
    }

    return sock;
}
