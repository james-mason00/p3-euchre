# Euchre in C++

A command-line implementation of Euchre built for EECS 280 at the University of Michigan. The program simulates a complete game with four players, including trump selection, card dealing, trick-taking, and scoring.

The game supports both human-controlled players and a simple AI that makes decisions based on the rules of Euchre.

## Features

- **Card representation:** Models cards by rank and suit, with comparison logic that accounts for trump and the right and left bowers.
- **Deck management:** Builds a 24-card Euchre deck, deals hands, and supports the project's in-shuffle algorithm.
- **Trump selection:** Implements both rounds of making trump, including the dealer's forced decision when everyone else passes.
- **Player strategies:** Supports human players who interact through the terminal and simple AI players that make decisions based on their hands and the current game situation.
- **Trick-taking:** Enforces following suit when possible and determines the winner of each trick.
- **Scoring:** Tracks team scores, including marches and euchres, until a team reaches the specified winning score.
- **Game configuration:** Accepts command-line arguments to configure the deck, shuffle behavior, target score, and players.

## Implementation

The project is written in C++ and uses object-oriented programming to separate the main parts of the game.

- `Card.cpp` implements card behavior, suit relationships, and card comparisons.
- `Pack.cpp` manages the deck, including resetting, dealing, and shuffling.
- `Player.cpp` implements the abstract player interface, the human and simple player types, and the player factory.
- `euchre.cpp` coordinates the game, from dealing and selecting trump to playing tricks and updating scores.

Inheritance and virtual functions allow the game to interact with different player types through a shared interface. This keeps the game logic separate from the decisions made by each player.

## Building and Running

Compile the program:

```bash
make euchre.exe
```

Run a game with four simple AI players and no shuffling:

```bash
./euchre.exe pack.in noshuffle 1 \
  Alice Simple Bob Simple \
  Charlie Simple David Simple
```

The arguments specify the pack file, shuffle setting, points needed to win, and each player's name and strategy. Replace `noshuffle` with `shuffle` to enable shuffling.

To use human-controlled players, replace `Simple` with `Human`. Human players enter their decisions and select cards through the terminal.

## Testing

The project includes unit tests for cards, packs, and player behavior, along with system tests that compare complete game output against expected results.

Run the test suite with:

```bash
make test
```

## What I Learned

This project gave me experience with object-oriented design, inheritance, polymorphism, operator overloading, and testing individual components of a larger program. It also required careful handling of Euchre's rules, especially card comparisons involving trump, the bowers, and the led suit.
