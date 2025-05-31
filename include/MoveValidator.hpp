#pragma once

#include "ChessBoard.hpp"
#include "ConfigReader.hpp"
#include "PortalSystem.hpp"

class MoveValidator{
    public:
    MoveValidator(const GameConfig& config , ChessBoard* board , PortalSystem* portalSystem);

    //Kontrol mekanizmasi
    bool isMoveValid(const Position& from , const Position& to , const Piece& piece);

    private:
    const GameConfig& config;
    ChessBoard *board;  //referansa da donebilirim bilmiyorum
    PortalSystem *portalSystem;

    bool isPathClear(const Position& from, const Position& to, const Piece& piece);
    bool isWithinBoard(const Position& pos) const;


    bool MoveValidator::isReachableWithPortals(const Position& from, const Position& to);
};