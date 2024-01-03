//
// Created by larsm on 31.12.2023.
//

#ifndef EXAMAUTUMN2023_CHESSENGINE_H
#define EXAMAUTUMN2023_CHESSENGINE_H

#include <vector>
#include <string>
#include "ChessPiece.h"

class ChessEngine {
private:
    std::vector <ChessPiece*> whitePieces;
    int whiteMoves;
    int blackMoves;
    bool whiteTurn;
    bool whiteCheck;
    bool blackCheck;
    std::string logFile;
    ChessPiece* selectedPiece;
    std::vector <ChessPiece*> blackPieces;
public:
    ChessEngine();
    ~ChessEngine();
    void setLogFile(std::string logFile);
    std::string getLogFile();
    void logMove(ChessPiece* piece, int pos);
    void movePiece(ChessPiece* piece, int pos);
    void updateBoard();
    void tryMove(int pos);
    std::vector<ChessPiece*> getPieces();
    void getLegalMoves(ChessPiece* piece, std::vector<int> &legalMoves);
    bool checkMove(ChessPiece* piece, int pos);
    void resetBoard();

    ChessPiece *const &getSelectedPiece() const;
};


#endif //EXAMAUTUMN2023_CHESSENGINE_H
