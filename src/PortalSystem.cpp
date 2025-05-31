#include "PortalSystem.hpp"

PortalSystem::PortalSystem(const std::vector<PortalConfig>& portals){
    for(const auto& portal : portals){
        PortalState state;
        state.cooldownRemaining = 0;
        state.properties =  portal.properties;
        portalStates[portal.id] = state;
    }
}


bool PortalSystem::isPortalAvailable(const std::string& portalID, const std::string& color) const {
    auto it = portalStates.find(portalID);
    if (it == portalStates.end()) {
        return false;
    }

    const PortalState& state = it->second;

    // Cooldown aktif mi?
    if (state.cooldownRemaining > 0) {
        return false;
    }

    // Renk uyumlu mu?
    for (const auto& allowed : state.properties.allowed_colors) {
        if (allowed == color) {
            return true;
        }
    }

    return false;
}


void PortalSystem::activatePortal(const std::string& portalId) {
    auto it = portalStates.find(portalId);
    if (it != portalStates.end()) {
        it->second.cooldownRemaining = it->second.properties.cooldown;
    }
}


void PortalSystem::updateCooldowns() {
    for (auto& entry : portalStates) {
        if (entry.second.cooldownRemaining > 0) {
            entry.second.cooldownRemaining--;
        }
    }
}
