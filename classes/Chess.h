#pragma once

#include "Game.h"
#include "Grid.h"
#include "Bitboard.h"

constexpr int pieceSize = 80;

typedef uint64_t BitBoard;

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
    void generateKnightBitBoards(int target);

    void printBitboard(uint64_t some_board);

    void updateAI();
    int negamax();
    
    int evaluateBoard(const std::string &state);
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
    
};