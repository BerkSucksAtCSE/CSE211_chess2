#pragma once

#include <string>
#include <unordered_map>
#include <memory>
#include "Piece.hpp"

class ChessBoard{
    public:
    
    ChessBoard();

    //Tahtaya tas koyma
    void addPiece(const std::string& position , std::shared_ptr<Piece> piece);

    //Belli bi konumdaki tasi getir
    std::shared_ptr<Piece> getPieceAt(const std::string& position) const;

    //Belli bi tasi kaldir
    void removePiece(const std::string& position);

    //Belli bi tasin yerini degistir
    void movePiece(const std::string& from , const std::string& to);

    //Tahtayi printle
    void printBoard() const;

    private:

    //pos->piece eslesmesi
    std::unordered_map<std::string , std::shared_ptr<Piece>> board;
};


