//
// Created by larsm on 31.12.2023.
//

#ifndef EXAMAUTUMN2023_CHESSPIECE_H
#define EXAMAUTUMN2023_CHESSPIECE_H

#include "vector"

enum ChessPieceType {
    PAWN,
    ROOK,
    KNIGHT,
    BISHOP,
    QUEEN,
    KING
};

class ChessPiece {
private:
    int pos;
    ChessPieceType type;
    bool white;
public:
    ChessPiece(int pos, int type, bool white);
    ~ChessPiece();
    int getPos() const;
    ChessPieceType getType();
    bool isWhite() const;
    void setPos(int pos);
    void setType(int type);
};


#endif //EXAMAUTUMN2023_CHESSPIECE_H
