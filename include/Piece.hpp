#pragma once

#include <string>
#include "ConfigReader.hpp"

class Piece{
    public:
    Piece(const std::string& type , const std::string& color , const Movement& movement , const SpecialAbilities& abilities);

    std::string getType() const;
    std::string getColor() const;
    const Movement& getMovement() const;
    const SpecialAbilities& getAbilities() const;

    private:
    std::string type;       //Piyon, At, Fil vsvs
    std::string color;      //white or black
    Movement movement;
    SpecialAbilities abilities;
};