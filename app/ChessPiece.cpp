//
// Created by larsm on 31.12.2023.
//

#include "ChessPiece.h"

bool ChessPiece::isWhite() const {
    return white;
}

ChessPiece::ChessPiece(int pos, int type, bool white) {
    this->pos = pos;
    this->type = (ChessPieceType) type;
    this->white = white;
}

ChessPiece::~ChessPiece() = default;

int ChessPiece::getPos() const {
    return pos;
}

ChessPieceType ChessPiece::getType() {
    return type;
}

void ChessPiece::setPos(int pos) {
    this->pos = pos;
}

void ChessPiece::setType(int type) {
    this->type = (ChessPieceType) type;
}
