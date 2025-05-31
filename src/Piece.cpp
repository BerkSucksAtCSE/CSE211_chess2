#include "Piece.hpp"

//Constructor:
Piece::Piece(const std::string& type , const std::string& color , const Movement& movement , const SpecialAbilities& abilities)
: type(type), color(color), movement(movement), abilities(abilities){}

std::string Piece::getType()const{return type;}
std::string Piece::getColor()const{return color;}
const Movement& Piece::getMovement()const{return movement;}
const SpecialAbilities& Piece::getAbilities() const{return abilities;}