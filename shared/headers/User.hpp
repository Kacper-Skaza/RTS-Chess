#pragma once

#include "Structures.hpp"
#include "ConnectionManager.hpp"

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
    unsigned int id;
    std::string username;
    bool player = false;
    bool ready = false;
    bool inRoom = false;
    ChessSide side;
    
public:
    explicit User(const std::string &username);
    ~User() = default;
    
    const unsigned int &getPlayerID() const noexcept;
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
    
    // Operator overloading
    bool operator==(const User &user) const;
    
    // GenerateID
    unsigned int generateID(const std::string &username);
};