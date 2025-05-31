#pragma once

#include <string>
#include <stack>

#include "ChessBoard.hpp"
#include "ConfigReader.hpp"
#include "MoveValidator.hpp"
#include "Piece.hpp"
#include "PortalSystem.hpp"

struct Move{
    Position from;
    Position to;
    std::shared_ptr<Piece> capturedPiece;
};

class GameManager{
    public:
    GameManager(const GameConfig &config);
    void startGame();

    private:
    const GameConfig &config;
    ChessBoard board;
    PortalSystem system;
    MoveValidator validator;
    int turnCount;
    std::stack<Move> moveHistory;
    //
    void printBoard();
    void loadInitialBoard();    //onemli! taslari boarda yerlestir
    void processCommand(const std::string &command);    //kullanici komut sistemi
    void handleMove(const std::string& fromStr, const std::string& toStr);  //hamle yapma
    void handleUndo();  //hamleyi geri alma
};