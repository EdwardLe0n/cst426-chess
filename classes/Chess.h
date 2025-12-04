#pragma once

#include "Game.h"
#include "Grid.h"
#include "GameState.h"

constexpr int pieceSize = 80;

constexpr int tempBoard[64] = {
    0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 
};

constexpr int whiteKingBoard[64] = {
    -6, -8, -8, -10, -10, -8, -8, -6, 
    -6, -8, -8, -10, -10, -8, -8, -6, 
    -6, -8, -8, -10, -10, -8, -8, -6, 
    -6, -8, -8, -10, -10, -8, -8, -6, 
    -4, -6, -6,  -8,  -8, -6, -6, -4, 
    -2, -4, -4,  -4,  -4, -4, -4,  2, 
     4,  4,  0,   0,   0,  0,  4,  4, 
     4,  3,  1,   0,   0,  1,  3,  4
};

constexpr int whiteQueenBoard[64] = {
    -4, -2, -2, -1, -1, -2, -2, -4, 
    -2,  0,  0,  0,  0,  0,  0, -2, 
    -2,  0,  1,  1,  1,  1,  0, -2, 
    -1,  0,  1,  1,  1,  1,  0, -1, 
    -1,  0,  1,  1,  1,  1,  0, -1, 
    -2,  0,  1,  1,  1,  1,  0, -2, 
    -2,  0,  0,  0,  0,  0,  0, -2, 
    -4, -2, -2, -1, -1, -2, -2, -4, 
};

constexpr int whiteRookBoard[64] = {
     0, 0, 0, 0, 0, 0, 0,  0, 
     1, 2, 2, 2, 2, 2, 2,  1, 
    -1, 0, 0, 0, 0, 0, 0, -1, 
    -1, 0, 0, 0, 0, 0, 0, -1, 
    -1, 0, 0, 0, 0, 0, 0, -1, 
    -1, 0, 0, 0, 0, 0, 0, -1, 
    -1, 0, 0, 0, 0, 0, 0, -1, 
     0, 0, 0, 1, 1, 0, 0,  0, 
};

constexpr int whiteBishopBoard[64] = {
    -4, -2, -2, -2, -2, -2, -2, -4, 
    -2,  0,  0,  0,  0,  0,  0, -2, 
    -2,  0,  1,  2,  2,  1,  0, -2, 
    -2,  1,  1,  2,  2,  1,  1, -2, 
    -2,  0,  2,  2,  2,  2,  0, -2, 
    -2,  2,  2,  2,  2,  2,  2, -2, 
    -2,  1,  0,  0,  0,  0,  1, -2, 
    -4, -2, -2, -2, -2, -2, -2, -4, 
};

constexpr int whiteKnightBoard[64] = {
    -10, -8, -6, -6, -6, -6, -8, -10, 
    -8,  -4,  0,  0,  0,  0, -4,  -8, 
    -6,   0,  2,  3,  4,  2,  0,  -6, 
    -6,   1,  3,  4,  4,  3,  0,  -6, 
    -6,   0,  3,  4,  4,  3,  0,  -6, 
    -6,   1,  0,  0,  0,  0,  1,  -6, 
    -8,  -4,  0,  1,  1,  0, -4,  -8, 
    -10, -8, -6, -6, -6, -6, -8, -10, 
};

constexpr int whitePawnBoard[64] = {
     0,  0,  0,  0,  0,  0,  0,  0, 
    10, 10, 10, 10, 10, 10, 10, 10, 
     2,  2,  4,  6,  6,  4,  2,  2, 
     1,  1,  2,  5,  5,  2,  1,  1, 
     0,  0,  0,  4,  4,  0,  0,  0, 
     1, -1, -2,  0,  0, -2, -1,  1, 
     1,  2,  2, -4, -4,  2,  2,  1, 
     0,  0,  0,  0,  0,  0,  0,  0, 
};

constexpr int blackKingBoard[64] = {         
     4,  3,  1,   0,   0,  1,  3,  4,
     4,  4,  0,   0,   0,  0,  4,  4,
    -2, -4, -4,  -4,  -4, -4, -4,  2,
    -4, -6, -6,  -8,  -8, -6, -6, -4,
    -6, -8, -8, -10, -10, -8, -8, -6,
    -6, -8, -8, -10, -10, -8, -8, -6,
    -6, -8, -8, -10, -10, -8, -8, -6,
    -6, -8, -8, -10, -10, -8, -8, -6, 
};

constexpr int blackQueenBoard[64] = {    
    -4, -2, -2, -1, -1, -2, -2, -4,
    -2,  0,  0,  0,  0,  0,  0, -2,
    -2,  0,  1,  1,  1,  1,  0, -2, 
    -1,  0,  1,  1,  1,  1,  0, -1, 
    -1,  0,  1,  1,  1,  1,  0, -1,
    -2,  0,  1,  1,  1,  1,  0, -2,
    -2,  0,  0,  0,  0,  0,  0, -2,
    -4, -2, -2, -1, -1, -2, -2, -4,
};

constexpr int blackRookBoard[64] = { 
     0, 0, 0, 1, 1, 0, 0,  0,
    -1, 0, 0, 0, 0, 0, 0, -1, 
    -1, 0, 0, 0, 0, 0, 0, -1, 
    -1, 0, 0, 0, 0, 0, 0, -1, 
    -1, 0, 0, 0, 0, 0, 0, -1, 
    -1, 0, 0, 0, 0, 0, 0, -1, 
     1, 2, 2, 2, 2, 2, 2,  1,
     0, 0, 0, 0, 0, 0, 0,  0,  
};

constexpr int blackBishopBoard[64] = {
    -4, -2, -2, -2, -2, -2, -2, -4, 
    -2,  1,  0,  0,  0,  0,  1, -2,
    -2,  2,  2,  2,  2,  2,  2, -2,
    -2,  0,  2,  2,  2,  2,  0, -2,
    -2,  1,  1,  2,  2,  1,  1, -2,
    -2,  0,  1,  2,  2,  1,  0, -2,
    -2,  0,  0,  0,  0,  0,  0, -2,
    -4, -2, -2, -2, -2, -2, -2, -4, 
};

constexpr int blackKnightBoard[64] = {
    -10, -8, -6, -6, -6, -6, -8, -10,  
    -8,  -4,  0,  1,  1,  0, -4,  -8,
    -6,   1,  0,  0,  0,  0,  1,  -6,
    -6,   0,  3,  4,  4,  3,  0,  -6, 
    -6,   0,  3,  4,  4,  3,  0,  -6,  
    -6,   0,  2,  3,  4,  2,  0,  -6,
    -8,  -4,  0,  0,  0,  0, -4,  -8,
    -10, -8, -6, -6, -6, -6, -8, -10, 
};

constexpr int blackPawnBoard[64] = {
     0,  0,  0,  0,  0,  0,  0,  0,   
     1,  2,  2, -4, -4,  2,  2,  1,
     1, -1, -2,  0,  0, -2, -1,  1,
     0,  0,  0,  4,  4,  0,  0,  0,
     1,  1,  2,  5,  5,  2,  1,  1,
     2,  2,  4,  6,  6,  4,  2,  2,
    10, 10, 10, 10, 10, 10, 10, 10,
     0,  0,  0,  0,  0,  0,  0,  0, 
};

class Chess : public Game
{
public:
    Chess();
    ~Chess();

    void setUpBoard() override;

    bool canBitMoveFrom(Bit &bit, BitHolder &src) override;
    bool canBitMoveFromTo(Bit &bit, BitHolder &src, BitHolder &dst) override;

    virtual void bitMovedFromTo(Bit &bit, BitHolder &src, BitHolder &dst) override;

    bool actionForEmptyHolder(BitHolder &holder) override;

    void stopGame() override;

    Player *checkForWinner() override;
    bool checkForDraw() override;

    std::string initialStateString() override;
    std::string stateString() override;
    void setStateString(const std::string &s) override;

    Grid* getGrid() override { return _grid; }

    void generateBitBoards();

    void generateKnightBitBoards();
    void generateKingBitBoards();
    void generatePawnBitBoards();
    void generateWhitePawnBitBoards();
    void generateBlackPawnBitBoards();

    void generateBitBoards(int target);

    void generateKnightMoves(std::vector<BitMove>& moves, int target, const std::string &state, bool isBlack);
    void generateKingMoves(std::vector<BitMove>& moves, int target, const std::string &state, bool isBlack);
    void generateWhitePawnMoves(std::vector<BitMove>& moves, int target, const std::string &state, bool isBlack);
    void generateBlackPawnMoves(std::vector<BitMove>& moves, int target, const std::string &state, bool isBlack);

    void generateMoves();

    void printBitboard(uint64_t some_board);

    void updateAI();
    int negamax(GameState& gamestate, int depth, int alpha, int beta, int playerColor);
    
    int evaluateBoard(const char state[64]);
    std::vector<BitMove> generateAllMoves();

    uint64_t whiteOccupancy();
    uint64_t blackOccupancy();

private:
    Bit* PieceForPlayer(const int playerNumber, ChessPiece piece);
    Player* ownerAt(int x, int y) const;
    void FENtoBoard(const std::string& fen);
    char pieceNotation(int x, int y) const;

    Grid* _grid;

    BitBoard knightBitBoards[64];

    BitBoard kingBitBoards[64];

    BitBoard whitePawnMoveBitBoards[64];
    BitBoard whitePawnAttackBitBoards[64];

    BitBoard blackPawnMoveBitBoards[64];
    BitBoard blackPawnAttackBitBoards[64];

    bool _isWhitePlaying;

    std::vector<BitMove> moves;

    int negamaxCalls;
    
};