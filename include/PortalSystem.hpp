#pragma once

#include <queue>
#include <string>
#include <unordered_map>

#include "ConfigReader.hpp"

class PortalSystem{

    public:
    PortalSystem(const std::vector<PortalConfig>& portals);
    bool isPortalAvailable(const std::string& portalID , const std::string& color) const;
    void updateCooldowns();                             //cooldownu her tur updatele
    void activatePortal(const std::string &portalID);   //portal acilinca cooldown baslatma

    private:
    struct PortalState{
        int cooldownRemaining;
        PortalProperties properties;
        std::queue<int> cooldownQueue;
    };

    std::unordered_map<std::string , PortalState> portalStates;

};
