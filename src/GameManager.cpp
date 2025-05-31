#include "GameManager.hpp"

#include <iostream>
#include <sstream>//komutlar icin


GameManager::GameManager(const GameConfig &config) : 
config(config),
board(),
system(config.portals),
validator(config , &board , &system),
turnCount(0)
{loadInitialBoard();}


void GameManager::loadInitialBoard() {
    // Standart taslar
    for (const auto& pieceConfig : config.pieces) {
        for (const auto& colorPositions : pieceConfig.positions) {
            std::string color = colorPositions.first;

            for (const auto& pos : colorPositions.second) {
                auto piece = std::make_shared<Piece>(
                    pieceConfig.type,
                    color,
                    pieceConfig.movement,
                    pieceConfig.special_abilities
                );

                std::string key = std::to_string(pos.x) + "," + std::to_string(pos.y);
                board.addPiece(key, piece);
            }
        }
    }


    // Custom taslar (ayni mantik)
    for (const auto& pieceConfig : config.custom_pieces) {
        for (const auto& colorPositions : pieceConfig.positions) {
            std::string color = colorPositions.first;

            for (const auto& pos : colorPositions.second) {
                auto piece = std::make_shared<Piece>(
                    pieceConfig.type,
                    color,
                    pieceConfig.movement,
                    pieceConfig.special_abilities
                );

                std::string key = std::to_string(pos.x) + "," + std::to_string(pos.y);
                board.addPiece(key, piece);
            }
        }
    }


    std::cout << "Initial board loaded!" << std::endl;

}


void GameManager::startGame() {
    std::cout << "Welcome to the Chess Game with Portals!" << std::endl;
    printBoard();

    std::string command;
    while (true) {
        std::cout << "Enter command (move, undo, quit): ";
        std::getline(std::cin, command);

        processCommand(command);
    }
}


void GameManager::processCommand(const std::string& command) {
    std::istringstream iss(command);
    std::string action;
    iss >> action;

    if (action == "move") {
        std::string from, to;
        iss >> from >> to;
        handleMove(from, to);
    }
    else if (action == "undo") {
        handleUndo();
    }
    else if (action == "quit") {
        std::cout << "Goodbye!" << std::endl;
        exit(0);
    }
    else {
        std::cout << "Invalid command." << std::endl;
    }
}


void GameManager::handleMove(const std::string& fromStr, const std::string& toStr) {
    // Pozisyonları parse et (string → Position)
    auto parsePos = [](const std::string& str) -> Position {
        size_t commaPos = str.find(',');
        int x = std::stoi(str.substr(0, commaPos));
        int y = std::stoi(str.substr(commaPos + 1));
        return {x, y};
    };

    Position from = parsePos(fromStr);
    Position to = parsePos(toStr);

    auto piece = board.getPieceAt(fromStr);
    if (!piece) {
        std::cout << "No piece at source position." << std::endl;
        return;
    }

    // Hamle gecerli mi?
    if (!validator.isMoveValid(from, to, *piece)) {
        std::cout << "Invalid move." << std::endl;
        return;
    }

    // Hedefte tas varsa (capture)
    auto captured = board.getPieceAt(toStr);

    // Tasi gotur
    board.movePiece(fromStr, toStr);


    // Portaldan geçiş kontrolü
if (system.isPortalAvailable(to, *piece)) {
    Position exitPos = system.getExitPosition(to);
    std::string exitKey = std::to_string(exitPos.x) + "," + std::to_string(exitPos.y);

    // Hedefte başka taş varsa yakalıyoruz (çünkü exit'e ışınlanıyoruz)
    auto capturedAtExit = board.getPieceAt(exitKey);
    if (capturedAtExit) {
        board.removePiece(exitKey);
    }

    // Taşı portal çıkışına taşıyoruz
    board.movePiece(toStr, exitKey);

    // Move kaydını güncelle (undo için)
    moveHistory.top().to = exitPos;

    // Portala girildi → cooldown başlat
    system.triggerPortalCooldown(to);
    
    std::cout << "Portal used! Teleported to: " << exitPos.x << "," << exitPos.y << std::endl;
}




    // Move stackine kaydet (undo icin)
    moveHistory.push({from, to, captured});

    // Portallari guncelle (wip kanka)
    system.updateCooldowns();

    // Tur sayısını guncelle
    turnCount++;

    std::cout << "Move completed." << std::endl;
    printBoard();
}


void GameManager::handleUndo() {
    if (moveHistory.empty()) {
        std::cout << "No moves to undo." << std::endl;
        return;
    }

    Move lastMove = moveHistory.top();
    moveHistory.pop();

    // Tasi eski yerine geri al
    std::string fromStr = std::to_string(lastMove.from.x) + "," + std::to_string(lastMove.from.y);
    std::string toStr = std::to_string(lastMove.to.x) + "," + std::to_string(lastMove.to.y);

    auto movedPiece = board.getPieceAt(toStr);
    board.movePiece(toStr, fromStr);

    // Eger hamlede tas alindiysa onu da geri koy
    if (lastMove.capturedPiece) {
        board.addPiece(toStr, lastMove.capturedPiece);
    }

    // Portal cooldownlari geri almak icin su anlik boyle
    // Gelismis undoya girerim belki

    turnCount--;
    std::cout << "Undo successful." << std::endl;
    printBoard();
}


void GameManager::printBoard() {
    int boardSize = config.game_settings.board_size;

    // Kolon basliklari
    std::cout << "  ";
    for (int x = 0; x < boardSize; ++x)
        std::cout << x << " ";
    std::cout << std::endl;

    // Satirlar
    for (int y = 0; y < boardSize; ++y) {
        std::cout << y << " ";
        for (int x = 0; x < boardSize; ++x) {
            std::string key = std::to_string(x) + "," + std::to_string(y);
            auto piece = board.getPieceAt(key);
            if (piece) {
                std::cout << piece->getType().at(0) << " ";  // ilk harfi bastiriyoruz
            } else {
                std::cout << ". ";
            }
        }
        std::cout << std::endl;
    }
}

