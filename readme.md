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

#### Date : 11/18/2025

So after a chat with my professor, I realized that I was handling movement fairly incorrectly. Whilst I'd like to say that it was eventful for at least the local portion of the code, many sections of the code say other wise.

With that in mind, here's a list of problems that spewed up:

- ***Still a WIP***