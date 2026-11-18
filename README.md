#  Terminal Chess — C++ Object-Oriented Programming Project

##  Project Overview

**Terminal Chess** is a two-player chess game developed in C++ that runs entirely in the terminal. The project implements the standard chessboard, chess pieces, movement rules, legal move validation, and game management functionalities.

The project was developed with the primary objective of demonstrating and implementing various **C++ programming concepts**, particularly Object-Oriented Programming (OOP), along with templates, operator overloading, typecasting, exception handling, static members, and memory management.

The game allows two players to play chess against each other using algebraic notation, such as `e2 e4`, while the program handles the board state, movement validation, captures, check, checkmate, stalemate, castling, en passant, and pawn promotion.

---

##  Objectives

- Implement a functional two-player chess game in C++.
- Understand and apply Object-Oriented Programming concepts.
- Implement inheritance and polymorphism through chess piece classes.
- Demonstrate operator overloading and typecasting.
- Use function and class templates.
- Implement exception handling for invalid inputs.
- Understand constructors, destructors, copy constructors, and assignment operators.
- Apply dynamic memory allocation and deep copying.
- Use static members, friend functions, nested classes, and enumerations.
- Develop a modular program using header files and source files.

---

##  Technologies Used

| Technology | Purpose |
|---|---|
| C++ | Core programming language |
| Standard Template Library (STL) | `vector`, `string`, `exception`, and other standard utilities |
| OOP | Structuring the chess game using classes and objects |
| Terminal / Command Line | User interface and gameplay |

---

##  Project Structure

### `chess.h`

Contains the declarations of classes, structures, enumerations, member functions, templates, and other components used throughout the project.

### `chess.cpp`

Contains the implementation of the chessboard, chess piece movement rules, game logic, move validation, and game status management.

### `main.cpp`

Contains the main function, player input handling, command-line interface, game loop, player ratings, scoreboard, and interaction with the user.

---

# ♟️ Features

- Two-player chess gameplay.
- Standard 8 × 8 chessboard.
- All six chess piece types:
  - Pawn
  - Knight
  - Bishop
  - Rook
  - Queen
  - King
- Legal move validation.
- Check detection.
- Checkmate detection.
- Stalemate detection.
- Castling.
- En passant.
- Pawn promotion to Queen, Rook, Bishop, or Knight.
- Move history.
- Resignation.
- Draw offer and acceptance.
- Restart functionality.
- Player ratings and rating levels.
- Score tracking across multiple games.
- Invalid square input handling.
- Piece counter displayed during gameplay.

---

#  C++ Concepts Used

This section explains the major C++ concepts implemented in the project, along with their locations and practical applications.

---

## 1. Classes and Objects

### Concept

A class is a user-defined data type that combines data members and member functions into a single entity.

An object is an instance of a class.

### Where it is used

Classes are used throughout the project to represent the different components of a chess game.

Some major classes include:

- `Position`
- `Piece`
- `Pawn`
- `Knight`
- `Bishop`
- `Rook`
- `Queen`
- `King`
- `Board`
- `Player`
- `RankedPlayer`
- `Game`
- `Rating`
- `Level`
- `Scoreboard`

### Example

```cpp
class Player {
public:
    string name;
    Color color;

    Player(string n, Color c) {
        name = n;
        color = c;
    }
};
```

An object of the `Player` class is created for each player in the game.

```cpp
Player white(whiteName, Color::WHITE);
Player black(blackName, Color::BLACK);
```

The `Game` class also contains objects of `Board` and `Player`, making it an example of objects interacting with each other.

---

## 2. Encapsulation

### Concept

Encapsulation is the process of combining data and the functions that operate on that data within a class, while controlling access to the internal implementation.

### Where it is used

Encapsulation is implemented in several classes, particularly:

- `Board`
- `Game`
- `Piece`
- `Scoreboard`

### Example

```cpp
class Board {
private:
    Piece* grid[8][8];

public:
    Piece* getPiece(Position pos) const;
    void applyMove(Move m);
};
```

The chessboard's internal grid is private.

Other parts of the program cannot directly access or modify the board's internal representation. Instead, they interact with it through functions such as:

```cpp
getPiece()
applyMove()
```

Similarly, the `Game` class maintains its internal game state privately.

```cpp
private:
    Board theBoard;
    Player white;
    Player black;
    Color turnColor;
    vector<string> moveHistory;
    GameStatus currentStatus;
    bool drawOffered;
```

This helps maintain the consistency of the game state.

---

## 3. Abstraction

### Concept

Abstraction means hiding unnecessary implementation details and exposing only the functionality required by the user or other parts of the program.

### Where it is used

The `Game` class provides an abstracted interface for managing the game.

```cpp
bool makeMove(Position from, Position to, char promotion = 'Q');

vector<Position> legalMoves(Position pos) const;

bool isInCheck(Color c) const;

void resign(Color who);

void offerDraw();

bool acceptDraw();

void restart();
```

The main program does not need to know how the program internally calculates legal moves, detects check, or updates the board.

It simply calls the appropriate function.

For example:

```cpp
game.makeMove(from, to, promo);
```

The internal implementation handles move validation, board updates, move history, turn switching, and game status updates.

---

## 4. Inheritance

### Concept

Inheritance allows one class to acquire the properties and behaviour of another class.

It promotes code reuse and establishes relationships between classes.

### Where it is used

The chess pieces use inheritance through the base class `Piece`.

```cpp
class Piece {
public:
    Color color;
    bool moved;

    virtual char symbol() const = 0;

    virtual vector<Position> pseudoMoves(
        Position pos,
        const Board& board
    ) const = 0;

    virtual Piece* clone() const = 0;

    virtual ~Piece();
};
```

The individual chess pieces inherit from `Piece`.

```cpp
class Pawn final : public Piece
```

```cpp
class Knight final : public Piece
```

```cpp
class Bishop final : public Piece
```

```cpp
class Rook final : public Piece
```

```cpp
class Queen final : public Piece
```

```cpp
class King final : public Piece
```

Each derived class represents a specific chess piece and implements its own movement logic.

### Another example

The `RankedPlayer` class inherits from `Player`.

```cpp
class RankedPlayer : public Player {
public:
    Rating rating;

    RankedPlayer(string n, Color c, Rating r)
        : Player(n, c), rating(r) {}
};
```

A `RankedPlayer` is a `Player` with additional rating information.

---

## 5. Polymorphism

### Concept

Polymorphism allows the same interface to represent different behaviours depending on the actual object type.

In this project, runtime polymorphism is implemented using virtual functions and overriding.

### Where it is used

The `Piece` class declares virtual functions:

```cpp
virtual char symbol() const = 0;

virtual vector<Position> pseudoMoves(
    Position pos,
    const Board& board
) const = 0;

virtual vector<Position> attackSquares(
    Position pos,
    const Board& board
) const;

virtual Piece* clone() const = 0;
```

Each derived piece implements its own version of these functions.

For example:

```cpp
char Pawn::symbol() const {
    return color == Color::WHITE ? 'P' : 'p';
}
```

```cpp
char Knight::symbol() const {
    return color == Color::WHITE ? 'N' : 'n';
}
```

```cpp
char Queen::symbol() const {
    return color == Color::WHITE ? 'Q' : 'q';
}
```

### Practical application

The board stores pieces using base-class pointers:

```cpp
Piece* grid[8][8];
```

When the program calls:

```cpp
piece->pseudoMoves(pos, board);
```

the appropriate derived-class implementation is executed based on the actual object pointed to by `piece`.

Thus, a pawn follows pawn movement rules, a knight follows knight movement rules, and so on, without requiring the board to separately implement the movement logic of every piece.

---

## 6. Abstract Class

### Concept

An abstract class is a class that contains at least one pure virtual function and cannot be instantiated directly.

It is generally used as a base class to define a common interface for derived classes.

### Where it is used

The `Piece` class is an abstract class.

```cpp
virtual char symbol() const = 0;

virtual vector<Position> pseudoMoves(
    Position pos,
    const Board& board
) const = 0;

virtual Piece* clone() const = 0;
```

The `= 0` syntax makes these functions pure virtual.

Therefore, an object of the `Piece` class cannot be created directly.

Instead, objects of classes such as `Pawn`, `Knight`, `Bishop`, `Rook`, `Queen`, and `King` are created.

---

## 7. Function Overriding

### Concept

Function overriding occurs when a derived class provides its own implementation of a virtual function declared in the base class.

### Where it is used

The derived chess piece classes override the virtual functions of `Piece`.

Example:

```cpp
vector<Position> Knight::pseudoMoves(
    Position pos,
    const Board& board
) const
```

```cpp
vector
