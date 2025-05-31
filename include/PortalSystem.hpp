#pragma once

#include <queue>
#include <string>
#include <unordered_map>
#include "Piece.hpp"
#include "ConfigReader.hpp"

class PortalSystem{

    public:
    Position getExitPosition(const Position& entryPos);
    void triggerPortalCooldown(const Position& entryPos);

    PortalSystem(const std::vector<PortalConfig>& portals);
    //bool isPortalAvailable(const std::string& portalID , const std::string& color) const;
    //bool isPortalAvailable(const Position& entryPos, const Piece& piece) const;
    bool isPortalAvailable(const Position& entryPos, const Piece& piece) const;


    void updateCooldowns();                             //cooldownu her tur updatele
    void activatePortal(const std::string &portalID);   //portal acilinca cooldown baslatma

    private:
    struct PortalState{
        int cooldownRemaining;
        PortalProperties properties;
        std::queue<int> cooldownQueue;
    };

    std::unordered_map<std::string , PortalState> portalStates;
    std::vector<PortalConfig> portals;

};
