#include "../../headers/views/RoomView.hpp"
#include <iostream>

RoomView::RoomView(SDL_Window *window, SDL_Renderer *renderer, SDLFontManager *fontManager)
    : window(window), renderer(renderer), fontManager(fontManager), room(), self(nullptr)
{
}

void RoomView::updateRoom(Room& newRoom)
{
    this->room = std::move(newRoom);
}

void RoomView::updateUser(User *newUser)
{
    this->self = newUser;
}

Room &RoomView::getRoom()
{
    return this->room;
}

User *RoomView::getSelf()
{
    return this->self;
}

User *RoomView::getOtherSelf()
{
    auto it = this->room.getUserList().find(this->self->getPlayerID());
    if (it != this->room.getUserList().end())
        return it->second;
    else
        throw std::logic_error("how are u not in room while beeing in room");
    return nullptr;
}

std::string RoomView::getButtonClicked()
{
    int mx, my;
    uint32_t mouseState = SDL_GetMouseState(&mx, &my);

    if (mouseState & SDL_BUTTON(SDL_BUTTON_LEFT))
    {
        for (const auto &btn : joinButtons)
        {
            if (mx >= btn.rect.x && mx <= btn.rect.x + btn.rect.w &&
                my >= btn.rect.y && my <= btn.rect.y + btn.rect.h)
            {
                return btn.name;
            }
        }
    }
    return "";
}

void RoomView::render()
{
    if (room.getRoomName() == "")
    {
        //add temporary rendering
        return;
    }

    // Tło
    SDL_SetRenderDrawColor(renderer, 30, 30, 35, 255);
    SDL_RenderClear(renderer);

    joinButtons.clear();
    int windowW, windowH;
    SDL_GetWindowSize(window, &windowW, &windowH);

    // --- 1. Nagłówek i Dane Pokoju ---
    SDL_Color white = {255, 255, 255, 255};
    SDL_Color green = {0, 255, 100, 255};

    SDL_Texture *titleTex = fontManager->getFontTexture("Room: " + room.getRoomName(), "Roboto/Roboto-Medium", 32, white);
    if (titleTex)
    {
        SDL_Rect dst = {50, 50, 0, 0};
        SDL_QueryTexture(titleTex, nullptr, nullptr, &dst.w, &dst.h);
        SDL_RenderCopy(renderer, titleTex, nullptr, &dst);
    }

    // --- 2. Lista Graczy ---
    int startY = 150;
    auto userList = room.getUserList();
    int iter = 0;
    for (auto &&i: userList)
    {
        std::string status = i.second->isReady() ? "[READY]" : "[NOT READY]";
        std::string role = i.second->isPlayer() ? "(Player)" : "(Spectator)";
        std::string userLine = i.second->getUsername() + " " + role + " " + status;

        SDL_Texture *userTex = fontManager->getFontTexture(userLine, "Roboto/Roboto-Medium", 20, white);
        if (userTex)
        {
            SDL_Rect dst = {70, startY + (static_cast<int>(iter) * 35), 0, 0};
            SDL_QueryTexture(userTex, nullptr, nullptr, &dst.w, &dst.h);
            SDL_RenderCopy(renderer, userTex, nullptr, &dst);
            iter++;
        }
    }

    // --- 3. Przyciski na dole ---
    // Definiujemy 3 przyciski: Ready/Unready, Role, Leave

    // Pobieramy dane lokalnego gracza (załóżmy, że to pierwszy na liście lub masz metodę by go znaleźć)
    // Na potrzeby UI musimy wiedzieć co wyświetlić. Załóżmy, że sprawdzamy stan "nasz" z obiektu pokoju.
    bool isReady = self->isReady();
    bool isPlayer = self->isPlayer();
    // Logika wyciągnięcia stanu 'self' powinna być tu dodana zależnie od Twojej klasy Room/User

    struct BtnDef
    {
        std::string label;
        std::string id;
        SDL_Color color;
    };
    std::vector<BtnDef> btnDefs = {
        {(isReady ? "NOT READY" : "READY"), "toggle_ready", {100, 100, 250, 255}},
        {(isPlayer ? "BE SPECTATOR" : "BE PLAYER"), "toggle_role", {150, 150, 150, 255}},
        {"LEAVE ROOM", "leave_room", {200, 50, 50, 255}}};

    int btnW = 200;
    int btnH = 50;
    int totalWidth = (btnW * 3) + (20 * 2); // 3 przyciski + odstępy
    int currentX = (windowW - totalWidth) / 2;
    int btnY = windowH - 100;

    for (const auto &def : btnDefs)
    {
        SDL_Rect r = {currentX, btnY, btnW, btnH};

        // Hover
        int mx, my;
        SDL_GetMouseState(&mx, &my);
        if (mx >= r.x && mx <= r.x + r.w && my >= r.y && my <= r.y + r.h)
        {
            SDL_SetRenderDrawColor(renderer, def.color.r + 30, def.color.g + 30, def.color.b + 30, 255);
        }
        else
        {
            SDL_SetRenderDrawColor(renderer, def.color.r, def.color.g, def.color.b, 255);
        }

        SDL_RenderFillRect(renderer, &r);

        SDL_Texture *bTex = fontManager->getFontTexture(def.label, "Roboto/Roboto-Medium", 18, white);
        if (bTex)
        {
            SDL_Rect tDst;
            SDL_QueryTexture(bTex, nullptr, nullptr, &tDst.w, &tDst.h);
            tDst.x = r.x + (r.w - tDst.w) / 2;
            tDst.y = r.y + (r.h - tDst.h) / 2;
            SDL_RenderCopy(renderer, bTex, nullptr, &tDst);
        }

        joinButtons.push_back({r, def.id});
        currentX += btnW + 20;
    }

    SDL_RenderPresent(renderer);
}
