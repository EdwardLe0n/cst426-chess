#include "Chess.h"
#include <limits>
#include <cmath>
#include <iostream>

Chess::Chess()
{
    _grid = new Grid(8, 8);
    negamaxCalls = 0;
}

Chess::~Chess()
{
    delete _grid;
}

void Chess::generateBitBoards() {

    Chess::generatePawnBitBoards();

    Chess::generateKnightBitBoards();
    Chess::generateKingBitBoards();

}

void Chess::generateBitBoards(int target) {

    // Add logic to only do it based on the player playing

    // Chess::generateKnightBitBoards(target);

}

void Chess::generatePawnBitBoards() {
    Chess::generateWhitePawnBitBoards();
    Chess::generateBlackPawnBitBoards();
}

void Chess::generateWhitePawnBitBoards() {

    for (int y = 1; y < 7; y++) {

        for (int x = 0; x < 8; x++) {
        
            whitePawnMoveBitBoards[(y * 8) + x] = 0;

            whitePawnMoveBitBoards[(y * 8) + x] ^= (1ULL << (uint64_t)( (y + 1) * 8 + x ) );

            if (y == 1) {
                whitePawnMoveBitBoards[(y * 8) + x] ^= (1ULL << (uint64_t)( (y + 2) * 8 + x ) );
            }

            // Sanity
            // Chess::printBitboard(whitePawnMoveBitBoards[(y * 8) + x]);

        }

    }

    for (int y = 1; y < 7; y++) {

        for (int x = 0; x < 8; x++) {
        
            whitePawnAttackBitBoards[(y * 8) + x] = 0;

            if (x != 0) {
                whitePawnAttackBitBoards[(y * 8) + x] ^= (1ULL << (uint64_t)( (y + 1) * 8 + (x - 1) ) );
            }

            if (x != 7) {
                whitePawnAttackBitBoards[(y * 8) + x] ^= (1ULL << (uint64_t)( (y + 1) * 8 + (x + 1) ) );
            }

            // Sanity
            // Chess::printBitboard(whitePawnAttackBitBoards[(y * 8) + x]);

        }

    }

}

void Chess::generateBlackPawnBitBoards() {

    for (int y = 6; y > 0; y--) {

        for (int x = 0; x < 8; x++) {
        
            blackPawnMoveBitBoards[(y * 8) + x] = 0;

            blackPawnMoveBitBoards[(y * 8) + x] ^= (1ULL << (uint64_t)( (y - 1) * 8 + x ) );

            if (y == 6) {
                blackPawnMoveBitBoards[(y * 8) + x] ^= (1ULL << (uint64_t)( (y - 2) * 8 + x ) );
            }

            // Sanity
            // Chess::printBitboard(blackPawnMoveBitBoards[(y * 8) + x]);

        }

    }

    for (int y = 6; y > 0; y--) {

        for (int x = 0; x < 8; x++) {
        
            blackPawnAttackBitBoards[(y * 8) + x] = 0;

            if (x != 0) {
                blackPawnAttackBitBoards[(y * 8) + x] ^= (1ULL << (uint64_t)( (y - 1) * 8 + (x - 1) ) );
            }

            if (x != 7) {
                blackPawnAttackBitBoards[(y * 8) + x] ^= (1ULL << (uint64_t)( (y - 1) * 8 + (x + 1) ) );
            }

            // Sanity
            // Chess::printBitboard(blackPawnAttackBitBoards[(y * 8) + x]);

        }

    }

}

void Chess::generateKingBitBoards() {

    std::pair<int, int>offsets[] = {
        {-1, -1}, {0, -1}, {1, -1}, 
        {-1, 0}, {1, 0}, 
        {-1, 1}, {0, 1}, {1, 1}, 
    };

    for (int y = 0; y < 8; y++) {

        for (int x = 0; x < 8; x++) {

            kingBitBoards[(y * 8) + x] = 0;

            for (std::pair<int, int> someElement : offsets) {

                if (x + someElement.first < 0 || x + someElement.first >= 8) {
                    continue;
                }

                if (y + someElement.second < 0 || y + someElement.second >= 8) {
                    continue;
                }

                kingBitBoards[(y * 8) + x] ^= (1ULL << (uint64_t)( (y + someElement.second) * 8 + (x + someElement.first) ) );

            }

            // Sanity
            // Chess::printBitboard(kingBitBoards[(y * 8) + x]);

        }

    }

}

void Chess::generateKnightBitBoards() {

    std::pair<int, int>offsets[] = {
        {-2, -1}, {-1, -2}, {1, -2}, {2, -1},
        {-2, 1}, {-1, 2}, {1, 2}, {2, 1}
    };

    for (int y = 0; y < 8; y++) {

        for (int x = 0; x < 8; x++) {

            knightBitBoards[(y * 8) + x] = 0;

            for (std::pair<int, int> someElement : offsets) {

                if (x + someElement.first < 0 || x + someElement.first >= 8) {
                    continue;
                }

                if (y + someElement.second < 0 || y + someElement.second >= 8) {
                    continue;
                }

                knightBitBoards[(y * 8) + x] ^= (1ULL << (uint64_t)( (y + someElement.second) * 8 + (x + someElement.first) ) );

            }

            // Sanity
            // Chess::printBitboard(knightBitBoards[(y * 8) + x]);

        }

    }

}

void Chess::generateKnightMoves(std::vector<BitMove>& moves, int target, const std::string &state, bool isBlack) {

    std::pair<int, int>offsets[] = {
        {-2, -1}, {-1, -2}, {1, -2}, {2, -1},
        {-2, 1}, {-1, 2}, {1, 2}, {2, 1}
    };

    for (std::pair<int, int> someElement : offsets) {

        if ((target % 8) + someElement.first < 0 || (target % 8) + someElement.first >= 8) {
            continue;
        }

        if ((target / 8) + someElement.second < 0 || (target / 8) + someElement.second >= 8) {
            continue;
        }

        int looking_at = target + someElement.first + (someElement.second * 8);

        if (state[looking_at] != '0') {
            if (isBlack) {
                if (std::islower(state[looking_at])) {
                    continue;
                }
            }
            else {
                if (std::isupper(state[looking_at])) {
                    continue;
                }
            }
        }

        moves.emplace_back(target, looking_at, ChessPiece::Knight);

    }

}

void Chess::generateWhitePawnMoves(std::vector<BitMove>& moves, int target, const std::string &state, bool isBlack) {

    if (state[target + 8] == '0') {
        
        moves.emplace_back(target, target + 8, ChessPiece::Pawn);

        if (target + 16 < 64) {

            if (state[target + 16] == '0' && target / 8 == 1) {

                moves.emplace_back(target, target + 16, ChessPiece::Pawn);

            }

        }
        
    }
    
    // Check left up
    if (target + 7 < 64 && ((target % 8) - 1) > 0) {
        if(std::islower(state[target + 7])) {
            moves.emplace_back(target, target + 7, ChessPiece::Pawn);
        }
    }

    // Check right up
    if (target + 9 < 64 && ((target % 8) + 1) < 8) {
        if(std::islower(state[target + 9])) {
            moves.emplace_back(target, target + 9, ChessPiece::Pawn);
        }
    }

}

void Chess::generateBlackPawnMoves(std::vector<BitMove>& moves, int target, const std::string &state, bool isBlack) {

    if (target - 8 < 0) {
        return;
    }

    if (state[target - 8] == '0') {
        
        moves.emplace_back(target, target - 8, ChessPiece::Pawn);

        if (target - 16 >= 0) {

            if (state[target - 16] == '0' && target / 8 == 6) {

                moves.emplace_back(target, target - 16, ChessPiece::Pawn);

            }

        } 
        
    }

    // Check left down
    if (target - 9 >= 0 && ((target % 8) - 1) > 0) {
        if(std::isupper(state[target - 9])) {
            moves.emplace_back(target, target - 9, ChessPiece::Pawn);
        }
    }

    // Check right down
    if (target - 7 >= 0 && ((target % 8) + 1) < 8) {
        if(std::isupper(state[target - 7])) {
            moves.emplace_back(target, target - 7, ChessPiece::Pawn);
        }
    }

}

void Chess::generateKingMoves(std::vector<BitMove>& moves, int target, const std::string &state, bool isBlack) {

    std::pair<int, int>offsets[] = {
        {-1, 0}, {-1, -1}, {0, -1}, {1, -1},
        {1, 0}, {1, 1}, {0, 1}, {-1, 1}
    };

    for (std::pair<int, int> someElement : offsets) {

        if ((target % 8) + someElement.first < 0 || (target % 8) + someElement.first >= 8) {
            continue;
        }

        if ((target / 8) + someElement.second < 0 || (target / 8) + someElement.second >= 8) {
            continue;
        }

        int looking_at = target + someElement.first + (someElement.second * 8);

        if (state[looking_at] != '0') {
            if (isBlack) {
                if (std::islower(state[looking_at])) {
                    continue;
                }
            }
            else {
                if (std::isupper(state[looking_at])) {
                    continue;
                }
            }
        }

        moves.emplace_back(target, looking_at, ChessPiece::King);

    }

}

char Chess::pieceNotation(int x, int y) const
{
    const char *wpieces = { "0PNBRQK" };
    const char *bpieces = { "0pnbrqk" };
    Bit *bit = _grid->getSquare(x, y)->bit();
    char notation = '0';
    if (bit) {
        notation = bit->gameTag() < 128 ? wpieces[bit->gameTag()] : bpieces[bit->gameTag()-128];
    }
    return notation;
}

Bit* Chess::PieceForPlayer(const int playerNumber, ChessPiece piece)
{
    const char* pieces[] = { "pawn.png", "knight.png", "bishop.png", "rook.png", "queen.png", "king.png" };

    Bit* bit = new Bit();
    // should possibly be cached from player class?
    const char* pieceName = pieces[piece - 1];
    std::string spritePath = std::string("") + (playerNumber == 0 ? "w_" : "b_") + pieceName;
    bit->LoadTextureFromFile(spritePath.c_str());
    bit->setOwner(getPlayerAt(playerNumber));
    bit->setSize(pieceSize, pieceSize);

    int to_add = playerNumber == 1 ? 128 : 0;

    to_add += piece;

    bit->setGameTag(to_add);

    // Sanity
    // std::cout << "testing game tag : " << bit->gameTag() << std::endl;

    return bit;
}

void Chess::setUpBoard()
{
    setNumberOfPlayers(2);
    _gameOptions.rowX = 8;
    _gameOptions.rowY = 8;

    _grid->initializeChessSquares(pieceSize, "boardsquare.png");
    FENtoBoard("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR");

    Chess::generateBitBoards();
    _isWhitePlaying = true;

    startGame();
    generateMoves();
}

void Chess::FENtoBoard(const std::string& fen) {
    // convert a FEN string to a board
    // FEN is a space delimited string with 6 fields
    // 1: piece placement (from white's perspective)

    int targetX = 0;
    int targetY = 7;

    for (int i = 0; i < fen.length(); i++) {

        // Extra info logic
        if (targetX == 8 && targetY == -1) {

        }
        // main driver code for setting up the board
        else {
            
            // If we've reached the end of this line, adjust info to go to the next one
            if (fen[i] == '/') {
                
                // Sanity
                // std::cout << "Going to next line" <<std::endl;
                
                targetX = 0;
                targetY--;
                
                continue;
            }

            // if there's a number, adjust the target x location
            if (fen[i] - '0' < 10) {

                // Sanity
                // std::cout << "A number has been found" << std::endl;

                targetX += fen[i] - '0';
            }
            // otherwise start handling piece placement
            else {

                bool is_black = false;
                char piece = fen[i];

                if (piece >= 'a') {
                    is_black = true;
                    
                    // Sanity
                    // std::cout << "found a black piece" << std::endl;
                }

                piece = std::tolower(piece);
                Bit* somePiece;

                switch (piece)
                {
                case 'p':
                    
                    somePiece = PieceForPlayer(is_black, ChessPiece::Pawn);

                    break;
                
                case 'r':
                    
                    somePiece = PieceForPlayer(is_black, ChessPiece::Rook);

                    break;
                
                case 'b':
                    
                    somePiece = PieceForPlayer(is_black, ChessPiece::Bishop);

                    break;
                
                case 'k':
                    
                    somePiece = PieceForPlayer(is_black, ChessPiece::King);

                    break;

                case 'n':
                    
                    somePiece = PieceForPlayer(is_black, ChessPiece::Knight);

                    break;

                case 'q':
                    
                    somePiece = PieceForPlayer(is_black, ChessPiece::Queen);

                    break;

                default:
                    break;
                }

                // somePiece->setPosition(
                //     ImVec2(
                //         (float)(targetX * pieceSize) + (float)(pieceSize / 2), 
                //         (float)((targetY - 7) * pieceSize) + (float)(pieceSize / 2)
                //     )
                // );

                ChessSquare* some_square =  _grid->getSquare(targetX, targetY);
                
                somePiece->setPosition(some_square->getPosition());

                some_square->setBit(somePiece);

                targetX++;

            }

        }

    }

    // NOT PART OF THIS ASSIGNMENT BUT OTHER THINGS THAT CAN BE IN A FEN STRING
    // ARE BELOW
    // 2: active color (W or B)
    // 3: castling availability (KQkq or -)
    // 4: en passant target square (in algebraic notation, or -)
    // 5: halfmove clock (number of halfmoves since the last capture or pawn advance)
}

bool Chess::actionForEmptyHolder(BitHolder &holder)
{
    return false;
}

bool Chess::canBitMoveFrom(Bit &bit, BitHolder &src)
{

    ChessSquare* temp = (ChessSquare*)(&src);
    int from_locat = temp->getSquareIndex();

    for (auto element : moves) {

        if (element.from == from_locat) {
            return true;
        }

    }
    
    return false;
}

void Chess::bitMovedFromTo(Bit &bit, BitHolder &src, BitHolder &dst)
{

    endTurn();

    // do AI stuff here
    if (gameHasAI()) {
        updateAI();
    }

    generateMoves();

}

bool Chess::canBitMoveFromTo(Bit &bit, BitHolder &src, BitHolder &dst)
{

    ChessPiece pieceType = (ChessPiece)(bit.gameTag() < 128 ? bit.gameTag() : bit.gameTag() - 128);

    // get chess piece data from bitholder
        
    ChessSquare *from = (ChessSquare*)(&src);
    int from_locat = from->getSquareIndex();

    ChessSquare *to = (ChessSquare*)(&dst);
    int to_locat = to->getSquareIndex();
    
    for (auto element : moves) {

        if (element.from == from_locat) {
            if (element.to == to_locat) {
                return true;
            }
        }

    }

    return false;
}

void Chess::generateMoves() {

    moves.clear();

    int player = getCurrentPlayer()->playerNumber() == 0 ? WHITE : BLACK;

    GameState gs;
    gs.init(stateString().c_str(), player);

    moves = gs.generateAllMoves();

}

void Chess::stopGame()
{
    _grid->forEachSquare([](ChessSquare* square, int x, int y) {
        square->destroyBit();
    });
}

Player* Chess::ownerAt(int x, int y) const
{
    if (x < 0 || x >= 8 || y < 0 || y >= 8) {
        return nullptr;
    }

    auto square = _grid->getSquare(x, y);
    if (!square || !square->bit()) {
        return nullptr;
    }
    return square->bit()->getOwner();
}

Player* Chess::checkForWinner()
{
    return nullptr;
}

bool Chess::checkForDraw()
{
    return false;
}

std::string Chess::initialStateString()
{
    return stateString();
}

std::string Chess::stateString()
{
    std::string s;
    s.reserve(64);
    _grid->forEachSquare([&](ChessSquare* square, int x, int y) {
            s += pieceNotation( x, y );
        }
    );
    return s;
}

uint64_t Chess::whiteOccupancy()
{
    uint64_t to_return = 0;

    _grid->forEachSquare([&](ChessSquare* square, int x, int y) {

            if (square->bit()) {

                if (square->bit()->gameTag() < 128) {

                    to_return = to_return | (1ULL << (y * 8 + x));

                }

            }

        }
    );

    return to_return;
}

uint64_t Chess::blackOccupancy()
{
    uint64_t to_return = 0;

    _grid->forEachSquare([&](ChessSquare* square, int x, int y) {

            if (square->bit()) {

                if (square->bit()->gameTag() > 128) {

                    to_return = to_return | (1ULL << (y * 8 + x));

                }

            }

        }
    );

    return to_return;
}

void Chess::setStateString(const std::string &s)
{
    _grid->forEachSquare([&](ChessSquare* square, int x, int y) {
        int index = y * 8 + x;
        char playerNumber = s[index] - '0';
        if (playerNumber) {
            square->setBit(PieceForPlayer(playerNumber - 1, Pawn));
        } else {
            square->setBit(nullptr);
        }
    });
}

void Chess::printBitboard(uint64_t some_board) {
    std::cout << "\n  a b c d e f g h\n";
    for (int rank = 7; rank >= 0; rank--) {
        std::cout << (rank + 1) << " ";
        for (int file = 0; file < 8; file++) {
            int square = rank * 8 + file;
            if (some_board & (1ULL << square)) {
                std::cout << "X ";
            } else {
                std::cout << ". ";
            }
        }
        std::cout << (rank + 1) << "\n";
        std::cout << std::flush;
    }
    std::cout << "  a b c d e f g h\n";
    std::cout << std::flush;
}

void Chess::updateAI() {

    _lastAIMove = BitMove(); // Reset last AI move (add this at the top of updateAI())

    const auto searchStart = std::chrono::steady_clock::now();
    
    negamaxCalls = 0;
    int player = getCurrentPlayer()->playerNumber() == 0 ? WHITE : BLACK;

    GameState gs;
    gs.init(stateString().c_str(), player);

    int bestMove = -100000;
    BitMove bestSquare = BitMove();

    int alpha = -100000;
    int beta = 100000;

    std::vector moves = gs.generateAllMoves();

    for (auto element : moves) {

        // puts most recent move into the game state
        gs.pushMove(element);

        int currentMove = -negamax(gs, 0, alpha, beta, BLACK);

        if (currentMove > bestMove) {
            bestMove = currentMove;
            bestSquare = element;
        }

        // clears past move from state
        gs.popState();

    }

    std::cout << "total negamax calls : " << negamaxCalls << std::endl;

    if (bestMove != -100000) {

        _lastAIMove = bestSquare;

        const double seconds = std::chrono::duration<double>(std::chrono::steady_clock::now() - searchStart).count();
        const double boardsPerSecond = seconds > 0.0 ? static_cast<double>(negamaxCalls) / seconds : 0.0;
        std::cout << "Moves checked: " << negamaxCalls
              << " (" << std::fixed << std::setprecision(2) << boardsPerSecond
              << " boards/s)" << std::defaultfloat << std::endl;

        std::cout << "Best move was from : " << std::to_string(bestSquare.from) << " to " << std::to_string(bestSquare.to) << std::endl;

        ChessSquare* fromSquare = _grid->getSquareByIndex(bestSquare.from);
        ChessSquare* toSquare = _grid->getSquareByIndex(bestSquare.to);

        toSquare->setBit(fromSquare->bit());
        toSquare->bit()->moveTo(toSquare->getPosition());
        
        fromSquare->setBit(nullptr);

    } 

    endTurn();

}

int Chess::negamax(GameState& gs, int depth, int alpha, int beta, int playerColor) {

    negamaxCalls++;

    // run initial checks

    if (depth >= 5) {

        return -evaluateBoard(gs.state);

    }

    // then do more negamax

    int bestMove = -100000;

    std::vector moves = gs.generateAllMoves();

    if (moves.size() == 0) {
        return -evaluateBoard(gs.state);
    }

    for (auto element : moves) {

        // puts most recent move into the game state
        gs.pushMove(element);

        bestMove = std::max(bestMove, -negamax(gs, depth + 1, -beta, -alpha, -playerColor));

        alpha = std::max(alpha, bestMove);

        if (alpha > beta) {
            gs.popState();
            break;
        }

        // clears past move from state
        gs.popState();

    }

    return bestMove;

}

int Chess::evaluateBoard(const char state[64]) {

    int boardValues[128];
 
    boardValues['P'] = 10  ;
    boardValues['N'] = 30  ;
    boardValues['B'] = 30  ;
    boardValues['R'] = 50  ;
    boardValues['Q'] = 90  ;
    boardValues['K'] = 900 ;
    boardValues['p'] = -10 ;
    boardValues['n'] = -30 ;
    boardValues['b'] = -30 ;
    boardValues['r'] = -50 ;
    boardValues['q'] = -90 ;
    boardValues['k'] = -900;
    boardValues['0'] = 0;

    int sum = 0;

    for(int i = 0; i < 64; i++) {

        sum += boardValues[state[i]];

        switch (state[i])
        {
            case 'P':
                sum += whitePawnBoard[i];
                break;

            case 'N':
                sum += whiteKnightBoard[i];
                break;

            case 'B':
                sum += whiteBishopBoard[i];
                break;

            case 'R':
                sum += whiteRookBoard[i];
                break;

            case 'Q':
                sum += whiteQueenBoard[i];
                break;

            case 'K':
                sum += whiteKingBoard[i];
                break;

            case 'p':
                sum += blackPawnBoard[i];
                break;

            case 'n':
                sum += blackKnightBoard[i];
                break;

            case 'b':
                sum += blackBishopBoard[i];
                break;

            case 'r':
                sum += blackRookBoard[i];
                break;

            case 'q':
                sum += blackQueenBoard[i];
                break;

            case 'k':
                sum += blackKingBoard[i];
                break;
            
            default:
                break;
        }

    } 

    // std::cout<< "board value is " << std::to_string(sum) << std::endl; 

    return sum;

}

std::vector<BitMove> Chess::generateAllMoves() {

    std::vector<BitMove> moveList;

    return moveList;

}

// Tournament support: Set board from FEN and reinitialize game state for AI
void Chess::setBoardFromFEN(const std::string& fen) {
 // Parse FEN string - can be full FEN or just piece placement
std::string piecePlacement = fen;
std::string activeColor = "w";
std::string castling = "KQkq";
std::string enPassant = "-";
 // Check if this is a full FEN string (has spaces)
size_t spacePos = fen.find(' ');
if (spacePos != std::string::npos) {
 // Parse full FEN
std::istringstream fenStream(fen);
 fenStream >> piecePlacement >> activeColor >> castling >> enPassant;
 }
 // Set visual board from piece placement
FENtoBoard(piecePlacement);
 // Determine current player from FEN
 _currentPlayer = (activeColor == "w" || activeColor == "W") ? WHITE : BLACK;
 // Reinitialize game state so AI sees correct board

GameState _gamestate; 
_gamestate.init(stateString().c_str(), _currentPlayer);
 // TODO: Parse castling rights and en passant from FEN for more accurate state
 // For now, the basic state is sufficient for AI to calculate moves
 // Generate legal moves for the new position
 moves.clear();
 moves = _gamestate.generateAllMoves();
std::cout << "[Tournament] Board set from FEN. Player: "
 << (_currentPlayer == WHITE ? "White" : "Black")
 << ", Legal moves: " << moves.size() << std::endl;
}
// Tournament support: Generate FEN string from current board
std::string Chess::getFEN() const {
std::string fen;
fen.reserve(90);
 // Piece placement (from rank 8 to rank 1)
for (int rank = 7; rank >= 0; --rank) {
int emptyCount = 0;
for (int file = 0; file < 8; ++file) {
char piece = pieceNotation(file, rank);
if (piece == '0') {
 emptyCount++;
 } else {
if (emptyCount > 0) {
 fen += std::to_string(emptyCount);
 emptyCount = 0;
 }
 fen += piece;
 }
 }
if (emptyCount > 0) {
 fen += std::to_string(emptyCount);
 }
if (rank > 0) {
 fen += '/';
 }
 }
 // Active color
 fen += ' ';
 fen += (_currentPlayer == WHITE) ? 'w' : 'b';
 // Castling availability (simplified - always report based on piece positions)
 fen += ' ';
std::string castling;
 // Check if white can castle (king on e1, rooks on a1/h1)
char e1 = pieceNotation(4, 0);
char a1 = pieceNotation(0, 0);
char h1 = pieceNotation(7, 0);
if (e1 == 'K') {
if (h1 == 'R') castling += 'K';
if (a1 == 'R') castling += 'Q';
 }
 // Check if black can castle (king on e8, rooks on a8/h8)
char e8 = pieceNotation(4, 7);
char a8 = pieceNotation(0, 7);
char h8 = pieceNotation(7, 7);
if (e8 == 'k') {
if (h8 == 'r') castling += 'k';
if (a8 == 'r') castling += 'q';
 }
 fen += castling.empty() ? "-" : castling;
 // En passant target square (simplified - report as '-')
 fen += " -";
 // Halfmove clock (simplified)
 fen += " 0";
 // Fullmove number (simplified)
 fen += " 1";
return fen;
}
