//
// Created by larsm on 31.12.2023.
//

#include "ChessEngine.h"
#include "fstream"
#include "set"
#include "iostream"
#include "algorithm"

ChessEngine::ChessEngine() {
    whiteTurn = true;
    whiteCheck = false;
    whitePieces = std::vector<ChessPiece*>();
    whiteMoves = 0;

    blackCheck = false;
    blackPieces = std::vector<ChessPiece*>();
    blackMoves = 0;

    selectedPiece = nullptr;

    logFile = "logFile.txt";

    resetBoard();
}

ChessEngine::~ChessEngine() {
    for(auto piece : whitePieces){
        delete piece;
    }
    for(auto piece : blackPieces){
        delete piece;
    }
}

void ChessEngine::resetBoard() {
    // Clear board
    for(auto piece : whitePieces){
        delete piece;
    }
    for(auto piece : blackPieces){
        delete piece;
    }
    whitePieces.clear();
    blackPieces.clear();

    whiteMoves = 0;
    blackMoves = 0;
    whiteTurn = true;
    whiteCheck = false;
    blackCheck = false;
    logFile = "logFile.txt";

    // Initialize white pieces
    whitePieces.push_back(new ChessPiece(0, ROOK, true));
    whitePieces.push_back(new ChessPiece(1, KNIGHT, true));
    whitePieces.push_back(new ChessPiece(2, BISHOP, true));
    whitePieces.push_back(new ChessPiece(3, QUEEN, true));
    whitePieces.push_back(new ChessPiece(4, KING, true));
    whitePieces.push_back(new ChessPiece(5, BISHOP, true));
    whitePieces.push_back(new ChessPiece(6, KNIGHT, true));
    whitePieces.push_back(new ChessPiece(7, ROOK, true));
    for(int i = 8; i < 16; i++){
        whitePieces.push_back(new ChessPiece(i, PAWN, true));
    }
    // Initialize black pieces
    blackPieces.push_back(new ChessPiece(56, ROOK, false));
    blackPieces.push_back(new ChessPiece(57, KNIGHT, false));
    blackPieces.push_back(new ChessPiece(58, BISHOP, false));
    blackPieces.push_back(new ChessPiece(59, QUEEN, false));
    blackPieces.push_back(new ChessPiece(60, KING, false));
    blackPieces.push_back(new ChessPiece(61, BISHOP, false));
    blackPieces.push_back(new ChessPiece(62, KNIGHT, false));
    blackPieces.push_back(new ChessPiece(63, ROOK, false));
    for(int i = 48; i < 56; i++){
        blackPieces.push_back(new ChessPiece(i, PAWN, false));
    }
}

void ChessEngine::getLegalMoves(ChessPiece* piece, std::vector<int> &legalMoves){
    auto type = piece->getType();
    auto pos = piece->getPos();
    auto white = piece->isWhite();

    if(whiteCheck && white){
        std::cout << "White is in check" << std::endl;
        return;
    } else if(blackCheck && !white){
        std::cout << "Black is in check" << std::endl;
        return;
    }

    switch (type) {
        case PAWN:
            // Pawn moves
            if (white) {
                if (pos / 8 == 1) {
                    // Initial double move for white pawn
                    legalMoves.push_back(pos + 16);
                }
                // Single move for white pawn
                legalMoves.push_back(pos + 8);
            } else {
                if (pos / 8 == 6) {
                    // Initial double move for black pawn
                    legalMoves.push_back(pos - 16);
                }
                // Single move for black pawn
                legalMoves.push_back(pos - 8);
            }
            break;

        case KNIGHT:
            // Knight moves
            if (pos % 8 > 1) {
                if (pos / 8 > 0) {
                    legalMoves.push_back(pos - 17);
                }
                if (pos / 8 < 7) {
                    legalMoves.push_back(pos + 15);
                }
            }
            if (pos % 8 > 0) {
                if (pos / 8 > 1) {
                    legalMoves.push_back(pos - 10);
                }
                if (pos / 8 < 6) {
                    legalMoves.push_back(pos + 6);
                }
            }
            if (pos % 8 < 6) {
                if (pos / 8 > 0) {
                    legalMoves.push_back(pos - 15);
                }
                if (pos / 8 < 7) {
                    legalMoves.push_back(pos + 17);
                }
            }
            if (pos % 8 < 7) {
                if (pos / 8 > 1) {
                    legalMoves.push_back(pos - 6);
                }
                if (pos / 8 < 6) {
                    legalMoves.push_back(pos + 10);
                }
            }
            break;

        case ROOK:
            // Rook moves
            for (int i = 0; i < 8; i++) {
                if (i != pos % 8) {
                    // Horizontal moves
                    legalMoves.push_back(pos - (pos % 8) + i);
                }
                if (i != pos / 8) {
                    // Vertical moves
                    legalMoves.push_back(pos % 8 + i * 8);
                }
            }
            break;

        case BISHOP:
            // Bishop moves
            for (int i = 0; i < 8; i++) {
                if (pos % 8 + i < 8 && pos / 8 + i < 8) {
                    legalMoves.push_back(pos + i * 9);
                }
                if (pos % 8 - i >= 0 && pos / 8 + i < 8) {
                    legalMoves.push_back(pos + i * 7);
                }
                if (pos % 8 + i < 8 && pos / 8 - i >= 0) {
                    legalMoves.push_back(pos - i * 7);
                }
                if (pos % 8 - i >= 0 && pos / 8 - i >= 0) {
                    legalMoves.push_back(pos - i * 9);
                }
            }
            break;

        case QUEEN:
            // Queen moves (combination of rook and bishop)
            for (int i = 0; i < 8; i++) {
                if (i != pos % 8) {
                    // Horizontal moves
                    legalMoves.push_back(pos - (pos % 8) + i);
                }
                if (i != pos / 8) {
                    // Vertical moves
                    legalMoves.push_back(pos % 8 + i * 8);
                }
            }
            for (int i = 0; i < 8; i++) {
                if (pos % 8 + i < 8 && pos / 8 + i < 8) {
                    legalMoves.push_back(pos + i * 9);
                }
                if (pos % 8 - i >= 0 && pos / 8 + i < 8) {
                    legalMoves.push_back(pos + i * 7);
                }
                if (pos % 8 + i < 8 && pos / 8 - i >= 0) {
                    legalMoves.push_back(pos - i * 7);
                }
                if (pos % 8 - i >= 0 && pos / 8 - i >= 0) {
                    legalMoves.push_back(pos - i * 9);
                }
            }
            break;

        case KING:
            // King moves
            if (pos % 8 > 0) {
                legalMoves.push_back(pos - 1);
                if (pos / 8 > 0) {
                    legalMoves.push_back(pos - 9);
                }
                if (pos / 8 < 7) {
                    legalMoves.push_back(pos + 7);
                }
            }
            if (pos % 8 < 7) {
                legalMoves.push_back(pos + 1);
                if (pos / 8 > 0) {
                    legalMoves.push_back(pos - 7);
                }
                if (pos / 8 < 7) {
                    legalMoves.push_back(pos + 9);
                }
            }
            if (pos / 8 > 0) {
                legalMoves.push_back(pos - 8);
            }
            if (pos / 8 < 7) {
                legalMoves.push_back(pos + 8);
            }
            break;
    }
}

void ChessEngine::logMove(ChessPiece* piece, int pos){
    std::fstream *file = new std::fstream(logFile, std::ios::out | std::ios::app);
    if (!file->is_open()) {
        std::cout << "Could not open file" << std::endl;
        return;
    }
    std::string move = "";
    char x = 'a' + (pos % 8);
    int y = (pos / 8) + 1;


    ChessPieceType type = piece->getType();
    switch(type){
        case PAWN:
            move = x + std::to_string(y);
            break;
        case ROOK:
            move = &"R" [ x] + std::to_string(y);
            break;
        case KNIGHT:
            move = &"N" [ x] + std::to_string(y);
            break;
        case BISHOP:
            move = &"B" [ x] + std::to_string(y);
            break;
        case QUEEN:
            move = &"Q" [ x] + std::to_string(y);
            break;
        case KING:
            move = &"K" [ x] + std::to_string(y);
            break;
    }

    *file << (whiteTurn ? "White: " : "Black: ") << move << std::endl;

}

std::vector<ChessPiece *> ChessEngine::getPieces() {
    std::vector<ChessPiece*> pieces = std::vector<ChessPiece*>();
    pieces.insert(pieces.end(), whitePieces.begin(), whitePieces.end());
    pieces.insert(pieces.end(), blackPieces.begin(), blackPieces.end());
    return pieces;
}

void ChessEngine::setLogFile(std::string logFile) {
    std::fstream *file = new std::fstream(logFile, std::ios::out | std::ios::app);
    if (!file->is_open()) {
        std::cout << "Could not open file" << std::endl;
        return;
    }
    this->logFile = logFile;
}

std::string ChessEngine::getLogFile() {
    return logFile;
}

bool ChessEngine::checkMove(ChessPiece* piece, int pos){
    for(auto p : whitePieces){
        if(p->getPos() == pos){
            if(p->isWhite() == piece->isWhite()){
                return false;
            }
        }
    }
    for(auto p : blackPieces){
        if(p->getPos() == pos){
            if(p->isWhite() == piece->isWhite()){
                return false;
            }
        }
    }
    return true;
}

void ChessEngine::tryMove(int pos) {
    std::vector<int> legalMoves = std::vector<int>();
    if(selectedPiece == nullptr){
        for(auto piece : whitePieces){
            if(piece->getPos() == pos){
                selectedPiece = piece;
                getLegalMoves(piece, legalMoves);
                break;
            }
        }
        for(auto piece : blackPieces){
            if(piece->getPos() == pos){
                selectedPiece = piece;
                getLegalMoves(piece, legalMoves);
                break;
            }
        }
        if(selectedPiece == nullptr){
            return;
        }
        char* type;
        switch(selectedPiece->getType()){
            case PAWN:
                type = "Pawn";
                break;
            case ROOK:
                type = "Rook";
                break;
            case KNIGHT:
                type = "Knight";
                break;
            case BISHOP:
                type = "Bishop";
                break;
            case QUEEN:
                type = "Queen";
                break;
            case KING:
                type = "King";
                break;
        }


        printf("Selected piece of type %s(%s) on %d\n", type, selectedPiece->isWhite() ? "White" : "Black", selectedPiece->getPos());
    } else {
        // Cancel move if same target pos is same as start pos
        if(selectedPiece->getPos() == pos){
            selectedPiece = nullptr;
            return;
        }
        if(checkMove(selectedPiece, pos)){
            movePiece(selectedPiece, pos);
        }
    }
}

void ChessEngine::movePiece(ChessPiece *piece, int pos) {
    if(piece->isWhite()){
        whiteMoves++;
        for(auto p : blackPieces){
            if(p->getPos() == pos){
                blackPieces.erase(std::remove(blackPieces.begin(), blackPieces.end(), p), blackPieces.end());
                break;
            }
        }
    } else {
        blackMoves++;
        for(auto p : whitePieces){
            if(p->getPos() == pos){
                whitePieces.erase(std::remove(whitePieces.begin(), whitePieces.end(), p), whitePieces.end());
                break;
            }
        }
    }
    char* type;
    switch(piece->getType()){
        case PAWN:
            type = "Pawn";
            break;
        case ROOK:
            type = "Rook";
            break;
        case KNIGHT:
            type = "Knight";
            break;
        case BISHOP:
            type = "Bishop";
            break;
        case QUEEN:
            type = "Queen";
            break;
        case KING:
            type = "King";
            break;
    }


    printf("Moving piece of type %s(%s)from %d to %d\n", type, piece->isWhite() ? "White" : "Black", piece->getPos(), pos);
    piece->setPos(pos);

    selectedPiece = nullptr;
    whiteTurn = !whiteTurn;

    if(piece->getType() == PAWN && (pos / 8 == 0 || pos / 8 == 7)){
        // Promote pawn TODO: MORE OPTIONS FOR PROMOTION
        piece->setType(QUEEN);
    }
}

ChessPiece *const &ChessEngine::getSelectedPiece() const {
    return selectedPiece;
}
