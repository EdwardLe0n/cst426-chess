# The Chess Assignment(s)

## Directory

- [Setting up the Chess Board](#chess-board-instantiation)
- [Moving Some Pieces](#chess-movement---part-1)
- [Fixing Movement](#chess-movement---part-1--12)

## Chess Board Instantiation

#### Date : 11/3/2025

Whilst a read me wasn't necessarily required for this segment, I felt that it's be nice to look back on it and reflect on some of the initial work.

Looking back, I quite enjoyed my methodology of tackling what needed to be done in the given order, which was:

1. Parse through the given FEN string without storing any date
2. Bring in pieces piece by piece
3. Tie in the Chess Piece enum
4. Bring in all the pieces

By grouping up what needed to be done into smaller chunks, it then allowed me to better attack the problem at hand by completing issues one by one.

With that in mind, there weren't many issues, as in all honesty, it was a pretty easy assignment.

## Chess Movement - Part 1

#### Date : 11/3 - 11/18/2025

Maybe I deserve to be put in a blender.

Anyways, so I might have misinterpreted bit boards and their use case, but for the sake of this portion of the README, we'll glide past that very fact.

To show off the list in order of how I tackled the first assignment, it goes from:

1. Knights
2. King
3. Pawn

At this point in time, it was of my understanding that one was to generate all the possible moves a piece could make at the start of the game, and that those moves would then be stored in an array of similar move lists.

With that in mind, I first tackled the movement code for the knights, with a fair amount of help from the knight starter code given in class. And due to the fact that all king movement is, is simply adjusting knight offsets, I went ahead and based my king code off of that work.

Then from here, I'd say I entered the challenging chunk of code being:

- Developing the bit board systems for the pawns
- Adjusting the chess game to give more data on occupancy

### Pawn Time!

So, in order to account for the fact that both black and white pawns move differently, as well as the fact that attacks are in different directions, and need to have enemies on them in order to move, I then made 4 total bit board arrays for pawns. Two for each color, one being a movement focused one and one being an attack focused one.

#### Movement of Le Pawns

When addressing the movement of these buggers, I focused on the fact there are two standard move cases

1. Moving forward a space (normale)
2. Moving forward two spaces (only at the start of a pawns lifecycle of movement)

So, in order to achieve this, I looped through all valid rows and adjusted that appropriate bit boards to allow the pawn to move forward one space. However, in the scenario in which we're at the first space a pawn would move from, then move forward two spaces.

In practice, it looks like this!

```
void Chess::generateWhitePawnBitBoards() {

    for (int y = 1; y < 7; y++) {

        for (int x = 0; x < 8; x++) {
        
            whitePawnMoveBitBoards[(y * 8) + x] = 0;

            whitePawnMoveBitBoards[(y * 8) + x] ^= (1ULL << (uint64_t)( (y + 1) * 8 + x ) );

            if (y == 1) {
                whitePawnMoveBitBoards[(y * 8) + x] ^= (1ULL << (uint64_t)( (y + 2) * 8 + x ) );
            }

        }

    }

    ...

}
```

#### Attackment of Le Pawns

In this case, we're also looping through each one of the rows, but we're just checking if left or right movement is valid before we toss it in the attack bit board.

In practice, it looks like this!

```
void Chess::generateWhitePawnBitBoards() {

    ...

    for (int y = 1; y < 7; y++) {

        for (int x = 0; x < 8; x++) {
        
            whitePawnAttackBitBoards[(y * 8) + x] = 0;

            if (x != 0) {
                whitePawnAttackBitBoards[(y * 8) + x] ^= (1ULL << (uint64_t)( (y + 1) * 8 + (x - 1) ) );
            }

            if (x != 7) {
                whitePawnAttackBitBoards[(y * 8) + x] ^= (1ULL << (uint64_t)( (y + 1) * 8 + (x + 1) ) );
            }

        }

    }

}
```

### And Finally, Occupancy!

```
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
```

*bazinga*

To go into actual detail on what's happening here, within the state string system, I noticed that there was a function that was checking the game tag of a given piece. 

Noticing this, I then decided to do a little copy and paste magic, as well as a some adjustments to how I instantiate new pieces to then store the owner/color of a piece within the game tag. I then use that into to adjust the to_return variable to let the game know whether I should care about it occupancy wise.

So yeah, debugging now calls my name.

### Anyways, it's blender time

## Chess Movement - Part 1 + 1/2

#### Date : 11/18/2025 - 11/19/2025

So after a chat with my professor, I realized that I was handling movement fairly incorrectly. Whilst I'd like to say that it was eventful for at least the local portion of the code, many sections of the code say other wise.

With that in mind, here's a list of problems that spewed up:

- Reused code in multiple sections
- Interpreting the state string incorrectly

To fix redundant code, I simply moved many duplicate move calls of new bits to one generalized location.

Yet, for some reason, when dealing with movement generation, the state string seemed to be off.

With that in mind, I went to bed.

## Chess Movement - Part 2

#### Date : 11/19/2025 - 11/27/2025

After some work, I went ahead and chatted with my professor about some random bug in which the state string was not reflective of the visible game.

Those chats then brought in this philosophy about the code base:

- It's genuinely garbage oh my god

### Why you may be wondering?

Well, upon further investigation, it was found that during the initialization of my board, I was unintentionally flipping the entire board during the fen to string process.

I wish I was kidding cause dear lord.

Proof be below: 

```
void Chess::FENtoBoard(const std::string& fen) {
    
    ...
    
    int targetX = 0;
    int targetY = 0;

    for (int i = 0; i < fen.length(); i++) {

        // Extra info logic
        if (targetX == 8 && targetY == 7) {
            
            ...

        }
        // main driver code for setting up the board
        else {

        somePiece->setPosition(
                ImVec2(
                    (float)(targetX * pieceSize) + (float)(pieceSize / 2), 
                    (float)(targetY * pieceSize) + (float)(pieceSize / 2)
                )
            );

            _grid->getSquare(targetX, targetY)->setBit(somePiece);

            targetX++;

        }

    }

}

```

This was done by placing pieces on the opposite side of the board, but setting their position to be on the correct side. So whilst visually it all seemed to mesh well and that there some bug with the state string, in reality, the state string was warning me, but hey. We live and lear i guess.

Following this incident and the changes I made to clean it up, I then focused on...

- Implementing black pawns into the code base

However, this development was short lived as...

## Chess Movement - Part 3 (we like BitMoves now)

#### Date : 11/27/2025 - 12/2/2025

Frankly, I like being able to read code.

This

```
bool Chess::canBitMoveFromTo(Bit &bit, BitHolder &src, BitHolder &dst)
{

    ChessPiece pieceType = (ChessPiece)(bit.gameTag() < 128 ? bit.gameTag() : bit.gameTag() - 128);

    // get chess piece data from bitholder
        
    ChessSquare *from = (ChessSquare*)(&src);

    std::pair<int, int> from_locat = {from->getColumn(), from->getRow()};

    ChessSquare *to = (ChessSquare*)(&dst);
    std::pair<int, int> to_locat = {to->getColumn(), to->getRow()};

    Player* currentPlayer = Game::getCurrentPlayer();

    uint64_t friendly = 0;

    if (currentPlayer->playerNumber() == 0) {
        friendly = Chess::whiteOccupancy();
    }
    else {
        friendly = Chess::blackOccupancy();
    }

    int from_index = (from_locat.second * 8) + from_locat.first;
    int to_index = (to_locat.second * 8) + to_locat.first;


    switch (pieceType) {

        case ChessPiece::Pawn: 

            // White pawn code
            if (currentPlayer->playerNumber() == 0) {

                // first, check move
                if (
                    ( ( whitePawnMoveBitBoards[from_index]
                    & (1ULL << (uint64_t)(to_index)) )
                    & ( ~( friendly | Chess::blackOccupancy() ) ) ) != 0
                ) {

                    if ((to_index / 8) - 1 == (from_index / 8)) {
                        return true;
                    }
                    else {

                        if (
                            ( ( whitePawnMoveBitBoards[from_index]
                            & (1ULL << (uint64_t)(to_index - 8)) )
                            & ( ~( friendly | Chess::blackOccupancy() ) ) ) != 0
                        ) {

                            return true;

                        }
                        else {

                            return false;

                        }

                    }

                }
                // then check attack

                else {

                    if (
                        ( ( whitePawnAttackBitBoards[from_index]
                        & (1ULL << (uint64_t)(to_index)) )
                        & ( Chess::blackOccupancy() ) ) != 0
                    ) {

                        return true;

                    }
                    else {

                        return false;

                    }

                }

            }

            // Black pawn code
            else {

                // first, check move
                if (
                    ( ( blackPawnMoveBitBoards[from_index]
                    & (1ULL << (uint64_t)(to_index)) )
                    & ( ~( friendly | Chess::whiteOccupancy() ) ) ) != 0
                ) {

                    if ((to_index / 8) + 1 == (from_index / 8)) {
                        return true;
                    }
                    else {

                        if (
                            ( ( blackPawnMoveBitBoards[from_index]
                            & (1ULL << (uint64_t)(to_index + 8)) )
                            & ( ~( friendly | Chess::whiteOccupancy() ) ) ) != 0
                        ) {

                            return true;

                        }
                        else {

                            return false;

                        }

                    }

                }
                // then check attack

                else {

                    if (
                        ( ( blackPawnAttackBitBoards[from_index]
                        & (1ULL << (uint64_t)(to_index)) )
                        & ( Chess::whiteOccupancy() ) ) != 0
                    ) {

                        return true;

                    }
                    else {

                        return false;

                    }

                }

            }

            break;

        case ChessPiece::Knight:

            if ((knightBitBoards[from_index] & (1ULL << (uint64_t)(to_index)) & ~friendly) != 0) {

                return true;
            }

        break;

        case ChessPiece::King:

            if ((kingBitBoards[from_index] & (1ULL << (uint64_t)(to_index)) & ~friendly) != 0) {

                return true;
            }

            break;

        default: 
            break;

    }

    return false;
}

```

is not readable

And it had a large flaw, being that 

## Chess Movement - Part 4 + AI - So we using GameState now

#### Date : 12/2/2025 - 12/3/2025

Upon much deliberation, I determined that I like sleep.

This then took form in me scrapping all of my movement code that I developed and rewrote over the course of a few weeks in order to import and utilize movement code within the new GameState. 

great

In all honesty, I didn't mind this change in direction as instead of attempting to make my own rook/bishop/queen movement code, I could instead jump straight into working on the AI functionality to get it battle ready.

So to prep it for the tourney, I did these three things:

1. Implement the standard negamax functionality
2. Implement piece weights
3. Implement weight maps for each individual piece