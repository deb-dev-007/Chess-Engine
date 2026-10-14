# Chess-Engine
A local two-player chess application in CPP with "all rules" engine, console based, and OOP-first design (use cases through implementation) built for an OOP coursework assignment.

## Features
 
- Two players take turns on the same computer
- Console board that shows pieces as letters (uppercase for White, lowercase for Black)
- Move validation, so illegal moves are rejected
- Check detection
- Pawn promotion
- Show legal moves for a square
- Move history
- Resign, offer a draw, accept a draw
- Restart without closing the program

## Commands
 
| Command | What it does |
|---------|--------------|
| e2 e4 | Move a piece |
| e7 e8 q | Move and promote a pawn (q, r, b or n) |
| moves e2 | Show legal moves for e2 |
| history | Show the move list |
| resign | Current player resigns |
| draw | Offer a draw, the other player types it again to accept |
| restart | Reset the board |

## Files
  - `chess.h` has the classes: `Position`, `Piece` and its child classes, `Board`, `Player`, `Game`
  - `main.cpp` has the console interface and the game loop


## OOP Concepts Used
 
- Abstract class and inheritance (`Piece` and its children, `Player` and `RankedPlayer`)
- Polymorphism with virtual functions
- Constructors and destructor(copy constructor and `operator=` in `Board`)
- Operator overloading (`==`, `!=` and `<<`) and a friend function
- Static data member and static function (piece counter)
- Object slicing demo in `main.cpp`





