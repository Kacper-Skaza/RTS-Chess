#include "../../headers/views/LobbyView.hpp"
#include <iostream>

LobbyView::LobbyView(SDL_Window *window, SDL_Renderer *renderer, SDLFontManager *fontManager) : window(window), renderer(renderer), fontManager(fontManager),
    createBox   (window, renderer, fontManager, {50, 980, 600, 50}, "Roboto/Roboto-Medium", 24, true),
    createButton(window, renderer, fontManager, {700, 980, 150, 50}, "Roboto/Roboto-Medium", 24, false, {255, 255, 255, 255})
{
    createButton.setText("Create Room");
}

void LobbyView::updateRooms(std::vector<Room>& newRooms)
{
    for (auto &&i : this->rooms)
        for (auto &&j : i.getUserList())
            delete j.second;
    this->rooms.clear();

    for (auto& room : newRooms)
    {
        rooms.push_back(std::move(room));
    }
}

TextBox &LobbyView::getcreateBox()
{
    return createBox;
}

TextBox &LobbyView::getCreateButton()
{
    return createButton;
}

void LobbyView::setJoinRequested(bool requested)
{
    joinRequested = requested;
}

bool LobbyView::isJoinRequested() const
{
    return joinRequested;
}

Room *LobbyView::getRoomClicked()
{
    int mx, my;
    // Pobieramy stan myszy i sprawdzamy czy lewy przycisk jest wciśnięty
    uint32_t mouseState = SDL_GetMouseState(&mx, &my);

    if (mouseState)
    {
        for (const auto &btn : joinButtons)
        {
            if (mx >= btn.rect.x && mx <= btn.rect.x + btn.rect.w && my >= btn.rect.y && my <= btn.rect.y + btn.rect.h)
            {
                return btn.room; // Zwracamy wskaźnik do pokoju przypisany do przycisku
            }
        }
    }

    return nullptr;
}

void LobbyView::handleScroll(int mouseWheelY)
{
    // Zmieniamy offset na podstawie ruchu kółka
    scrollOffset += mouseWheelY * 30;

    // Blokada: nie scrolluj powyżej pierwszego elementu
    if (scrollOffset > 0)
        scrollOffset = 0;

    // Blokada: nie scrolluj poniżej ostatniego elementu
    int windowH;
    SDL_GetWindowSize(window, nullptr, &windowH);
    int contentHeight = static_cast<int>(rooms.size()) * itemHeight;
    int visibleArea = windowH - 200; // 50 to nasz startowy Y

    if (contentHeight > visibleArea)
    {
        if (scrollOffset < -(contentHeight - visibleArea))
        {
            scrollOffset = -(contentHeight - visibleArea);
        }
    }
    else
    {
        scrollOffset = 0;
    }
}

void LobbyView::render()
{
    // 1. Tło lobby
    SDL_SetRenderDrawColor(renderer, 25, 25, 25, 255);
    SDL_RenderClear(renderer);

    // Czyścimy listę przycisków przed każdym renderowaniem
    joinButtons.clear();

    // Pobieramy aktualny rozmiar okna dla dynamicznego "cullingu" i scrolla
    int windowW, windowH;
    SDL_GetWindowSize(window, &windowW, &windowH);

    int startY = 50 + scrollOffset;

    for (size_t i = 0; i < rooms.size(); ++i)
    {
        int currentY = startY + (static_cast<int>(i) * itemHeight);

        // Optymalizacja: Nie rysuj, jeśli element jest poza ekranem
        if (currentY + itemHeight < 0 || currentY > windowH - 150)
            continue;

        // Kontener pokoju (tło wiersza)
        SDL_Rect roomRect = {50, currentY, windowW - 100, itemHeight - 10};
        SDL_SetRenderDrawColor(renderer, 45, 45, 48, 255);
        SDL_RenderFillRect(renderer, &roomRect);

        // --- Renderowanie tekstu ---
        SDL_Color white = {255, 255, 255, 255};
        SDL_Color gray = {180, 180, 180, 255};
        SDL_Color cyan = {0, 200, 255, 255};

        // 1. Nazwa pokoju
        SDL_Texture *nameTex = fontManager->getFontTexture("Room: " + rooms[i].getRoomName(), "Roboto/Roboto-Medium", 24, white);
        if (nameTex)
        {
            SDL_Rect dst = {70, currentY + 10, 0, 0};
            SDL_QueryTexture(nameTex, nullptr, nullptr, &dst.w, &dst.h);
            SDL_RenderCopy(renderer, nameTex, nullptr, &dst);
        }

        // 2. Lista graczy
        std::string playersStr = "Players: ";
        auto userList = rooms[i].getUserList();
        for (auto &&j : userList)
        {
            playersStr += j.second->getUsername() + ", ";
        }
        if (playersStr.size() >= 11 )
        {
            playersStr.resize(playersStr.size() - 2);
        }
        SDL_Texture *playersTex = fontManager->getFontTexture(playersStr, "Roboto/Roboto-Medium", 18, gray);
        if (playersTex)
        {
            SDL_Rect dst = {70, currentY + 45, 0, 0};
            SDL_QueryTexture(playersTex, nullptr, nullptr, &dst.w, &dst.h);
            SDL_RenderCopy(renderer, playersTex, nullptr, &dst);
        }

        // 3. Pojemność (Capacity)
        std::string capStr = std::to_string(rooms[i].getPlayerCount()) + "/" + std::to_string(rooms[i].getMaxPlayerCount());
        SDL_Texture *capTex = fontManager->getFontTexture(capStr, "Roboto/Roboto-Medium", 22, cyan);
        if (capTex)
        {
            SDL_Rect dst = {roomRect.x + roomRect.w - 280, currentY + 30, 0, 0};
            SDL_QueryTexture(capTex, nullptr, nullptr, &dst.w, &dst.h);
            SDL_RenderCopy(renderer, capTex, nullptr, &dst);
        }

        // 4. Status meczu
        std::string statusStr = rooms[i].isMatchStarted() ? "IN GAME" : "IN LOBBY";
        SDL_Texture *statusTex = fontManager->getFontTexture(statusStr, "Roboto/Roboto-Medium", 22, cyan);
        if (statusTex)
        {
            SDL_Rect dst = {roomRect.x + roomRect.w - 225, currentY + 30, 0, 0};
            SDL_QueryTexture(statusTex, nullptr, nullptr, &dst.w, &dst.h);
            SDL_RenderCopy(renderer, statusTex, nullptr, &dst);
        }

        // --- Przycisk JOIN ---
        SDL_Rect btnRect = {roomRect.x + roomRect.w - 110, currentY + 25, 90, 40};

        // Prosty hover effect
        int mx, my;
        SDL_GetMouseState(&mx, &my);
        if (mx >= btnRect.x && mx <= btnRect.x + btnRect.w && my >= btnRect.y && my <= btnRect.y + btnRect.h)
        {
            SDL_SetRenderDrawColor(renderer, 0, 150, 255, 255);
        }
        else
        {
            SDL_SetRenderDrawColor(renderer, 0, 122, 204, 255);
        }

        SDL_RenderFillRect(renderer, &btnRect);

        SDL_Texture *joinTex = fontManager->getFontTexture("JOIN", "Roboto/Roboto-Medium", 20, white);
        if (joinTex)
        {
            SDL_Rect dst;
            SDL_QueryTexture(joinTex, nullptr, nullptr, &dst.w, &dst.h);
            dst.x = btnRect.x + (btnRect.w - dst.w) / 2;
            dst.y = btnRect.y + (btnRect.h - dst.h) / 2;
            SDL_RenderCopy(renderer, joinTex, nullptr, &dst);
        }

        // Dodajemy przycisk do listy klikalnych obiektów
        joinButtons.push_back({btnRect, &rooms[i]});
    }

    // Bottom Bar
    SDL_SetRenderDrawColor(renderer, 25, 25, 25, 255);
    SDL_Rect tempRect = {0, 920, windowW, 150};
    SDL_RenderFillRect(renderer, &tempRect);

    // Create Box Render
    if (createBox.getSelected())
        SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
    else
        SDL_SetRenderDrawColor(renderer, 180, 180, 180, 255);
    SDL_RenderFillRect(renderer, &createBox.getBoxPos());
    createBox.genTexture();
    SDL_RenderCopy(renderer, createBox.getTexture(), nullptr, &(createBox.getTextureRect()));

    // Create Button Render
    SDL_SetRenderDrawColor(renderer, 46, 87, 25, 255);
    SDL_RenderFillRect(renderer, &createButton.getBoxPos());
    createButton.genTexture();
    SDL_RenderCopy(renderer, createButton.getTexture(), nullptr, &(createButton.getTextureRect()));

    SDL_RenderPresent(renderer);
}
