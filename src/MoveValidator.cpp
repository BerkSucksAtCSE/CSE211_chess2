#include "MoveValidator.hpp"

#include <cmath>    //abs()
#include <set>      //isReachableWithPortals

MoveValidator::MoveValidator(const GameConfig &config , ChessBoard *board , PortalSystem* portalSystem)
: config(config) , board(board) , portalSystem(portalSystem){}


bool MoveValidator::isWithinBoard(const Position &pos) const{
    return
    pos.x >= 0 && pos.x < config.game_settings.board_size
    &&
    pos.y >= 0 && pos.y < config.game_settings.board_size;
}//tahtanin disina tas cikmasin diye pozisyon tahtada gecerli mi kontrol eder


bool MoveValidator::isMoveValid(const Position &from , const Position &to , const Piece &piece){
    if(!isWithinBoard(to)){
        return false;
    }if (from.x == to.x && from.y == to.y) {//ayni pos gecersiz
        return false;
    }

    if (isReachableWithPortals(from, to)) {
    return true;
    }

    //hareket patternleri
    const Movement& move = piece.getMovement();

    int dx = std::abs(to.x - from.x);
    int dy = std::abs(to.y - from.y);

    
    if (move.l_shape) {
        if ((dx == 2 && dy == 1) || (dx == 1 && dy == 2)) {
            return true;
        }// L-shape kontrol
    }if (from.y == to.y && dx <= move.forward) {
        if (isPathClear(from, to, piece)) {
            return true;
        }//Forward kontrol
    }if (from.x == to.x && dy <= move.sideways) {
        if (isPathClear(from, to, piece)) {
            return true;
        }//Sideways kotnrol
    } if (dx == dy && dx <= move.diagonal) {
        if (isPathClear(from, to, piece)) {
            return true;
        }//diagonal kontrol
    }if (move.first_move_forward > 0 && from.y == 1) {
        if (from.y + move.first_move_forward == to.y && from.x == to.x) {
            if (isPathClear(from, to, piece)) {
                return true;
            }
        }//pawn icin ilk hamle kontrolu
    }

    //

    return false;
}


bool MoveValidator::isPathClear(const Position& from, const Position& to, const Piece& piece) {
    // L-shape kontrol (at)
    if (piece.getMovement().l_shape) {
        int dx = std::abs(to.x - from.x);
        int dy = std::abs(to.y - from.y);
        return (dx == 2 && dy == 1) || (dx == 1 && dy == 2);
    }

    // Diagonal hareket kontrol
    if (from.x != to.x && from.y != to.y) {
        int dx = to.x > from.x ? 1 : -1;
        int dy = to.y > from.y ? 1 : -1;

        int steps = std::abs(to.x - from.x);
        for (int i = 1; i < steps; i++) {
            Position intermediate = {from.x + i * dx, from.y + i * dy};
            if (board->getPieceAt(std::to_string(intermediate.x) + "," + std::to_string(intermediate.y))) {
                return false; // Yol dolu
            }
        }
        return true;
    }

    // Sideways veya Forward hareket kontrol
    if (from.x == to.x || from.y == to.y) {
        int dx = (to.x == from.x) ? 0 : (to.x > from.x ? 1 : -1);
        int dy = (to.y == from.y) ? 0 : (to.y > from.y ? 1 : -1);

        int steps = std::max(std::abs(to.x - from.x), std::abs(to.y - from.y));
        for (int i = 1; i < steps; i++) {
            Position intermediate = {from.x + i * dx, from.y + i * dy};
            if (board->getPieceAt(std::to_string(intermediate.x) + "," + std::to_string(intermediate.y))) {
                return false;
            }
        }
        return true;
    }

    return false;  // Uygun pattern bulunamadi
}


bool MoveValidator::isReachableWithPortals(const Position& from, const Position& to) {
    std::queue<Position> q;
    std::set<std::string> visited;

    auto posToString = [](const Position& p) {
        return std::to_string(p.x) + "," + std::to_string(p.y);
    };

    q.push(from);
    visited.insert(posToString(from));

    while (!q.empty()) {
        Position current = q.front();
        q.pop();

        // Hedefe ulaştık mı?
        if (current.x == to.x && current.y == to.y) {
            return true;
        }

        // Portal geçişlerini kontrol et
        for (const auto& portal : config.portals) {
            Position entry = portal.positions.entry;
            Position exit = portal.positions.exit;

            if (current.x == entry.x && current.y == entry.y) {
                // Portala giriyoruz

                // Portalı kullandıktan sonra çıkış noktasını ekle
                if (visited.count(posToString(exit)) == 0) {
                    q.push(exit);
                    visited.insert(posToString(exit));
                }
            }
        }
    }

    return false;
}