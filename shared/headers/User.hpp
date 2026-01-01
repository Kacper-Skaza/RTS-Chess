#pragma once

#include "Structures.hpp"
#include "ConnectionManager.hpp"

#include "dependencies/Json.hpp"

#include <cstdlib>
#include <utility>
#include <cctype>
#include <vector>
#include <stdexcept>
#include <memory>

// May require adding server side methods for requesting data

class User
{
private:
    unsigned long long id;
    std::string username;
    bool player = false;
    bool ready = false;
    bool inRoom = false;
    ChessSide side;

public:
    explicit User();
    explicit User(const unsigned long long &ID, const std::string &username);
    ~User() = default;

    const unsigned long long &getPlayerID() const noexcept;
    const std::string &getUsername() const noexcept;
    bool isPlayer() const noexcept;
    bool isReady() const noexcept;
    bool isInRoom() const noexcept;
    ChessSide getSide() const noexcept;

    void setPlayer(bool player);
    void setReady(bool ready);
    void setInRoom(bool inRoom);
    void setUsername(const std::string &username);
    void setSide(ChessSide side);
    void setPlayerID(const unsigned long long &ID);

    // Operator overloading
    bool operator==(const User &user) const;

    friend void from_json(const nlohmann::json &j, User &p);
    friend void to_json(nlohmann::json &j, const User &p);
};
