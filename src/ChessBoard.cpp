//Hours spent just to learn what the hell is auto: "4"
#include "ChessBoard.hpp"
#include <iostream>

ChessBoard::ChessBoard(){}//CONSTRUCTOR

void ChessBoard::addPiece(const std::string& position , std::shared_ptr<Piece> piece){
    board[position] = piece;
}

std::shared_ptr<Piece> ChessBoard::getPieceAt(const std::string& position) const{
    auto it = board.find(position);//(??)
    if(it != board.end()){
        return it->second;
    }
    return nullptr;
}

void ChessBoard::removePiece(const std::string& position){
    board.erase(position);
}

void ChessBoard::movePiece(const std::string& from , const std::string& to){
    auto piece = getPieceAt(from);
    if(piece){
        board[to] = piece;
        removePiece(from);
    }

}

void ChessBoard::printBoard() const{
    std::cout << "*-*-* Current Board State *-*-*\n";
    for(const auto& entry : board){
        std::cout << entry.first << ": "
                  << entry.second->getType() << " ("
                  << entry.second->getColor() << ")\n";
    }
}