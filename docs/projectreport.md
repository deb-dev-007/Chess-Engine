# Progress Report: Chess Engine

## 1. Project Overview

The Chess Engine is a console-based C++ application designed for two human players sharing one computer. Its design uses object-oriented programming to represent chess pieces, players, the board, moves, and the overall game. The interface is command-driven: players enter moves and commands, while the game structure is designed to manage turns, validate moves, report game status, and support common chess interactions.

The project does not include a computer opponent, online multiplayer, or a game clock. Its focus is a local two-player chess experience and the application of OOP concepts in a practical C++ project.

## 2. Project Structure and Workflow

The project has been approached from requirements and design through class modelling and the console interface. The following flowcharts summarize the design and the intended interaction between a player and the program.

### 2.1 Development and design workflow

```text
+-----------------------------+
| Problem Statement and Scope |
+-----------------------------+
              |
              v
+-----------------------------+
|       Use-Case Model        |
+-----------------------------+
              |
              v
+-----------------------------+
|      Noun-Verb Analysis     |
+-----------------------------+
              |
              v
+-----------------------------+
| Identify Classes and        |
| Responsibilities            |
+-----------------------------+
              |
              v
+-----------------------------+
|          CRC Cards          |
+-----------------------------+
              |
              v
+-----------------------------+
|        Class Diagram        |
+-----------------------------+
              |
              v
+-----------------------------+
|        Object Diagram       |
+-----------------------------+
              |
              v
+-----------------------------+
| C++ Class Structure         |
| (chess.h)                   |
+-----------------------------+
              |
              v
+-----------------------------+
| Console Interface and       |
| Game Loop (main.cpp)        |
+-----------------------------+
```

### 2.2 Player interaction flow

```text
              +----------------+
              |  Start Program |
              +----------------+
                      |
                      v
             +-------------------+
             | Create Game/Board |
             +-------------------+
                      |
                      v
             +-------------------+
       +---->| Display Board and |
       |     | Status            |
       |     +-------------------+
       |              |
       |              v
       |     +-------------------+
       |     | Read Player       |
       |     | Command           |
       |     +-------------------+
       |              |
       |              v
       |     +-------------------+
       |     | Select Command    |
       |     +-------------------+
       |       /    |    |    \
       |      /     |    |     \
       |     v      v    v      v
       |   Move   Legal History Resign/
       |          Moves         Draw
       |     |      |    |      |
       |     v      v    v      v
       |  Validate Show Show  Process
       |   Move   Moves History Command
       |     |
       |     v
       |  +------------------+
       |  | Move Accepted?   |
       |  +------------------+
       |    | Yes       | No
       |    v           v
       | Update Board  Show Invalid
       | Turn/History  Move Feedback
       |    |           |
       |    v           |
       | Refresh Board  |
       | and Status     |
       |    |           |
       |    v           |
       |  +------------------+
       |  | Game Over?       |
       |  +------------------+
       |    | No       | Yes
       +----+           v
                  +----------------+
                  | Show Final     |
                  | Result         |
                  +----------------+
                          |
                          v
                   +-------------+
                   | End/Restart |
                   +-------------+

Restart command: reset game state, then return to board display.
```

*The interaction flow represents the intended command flow. The console commands are wired in `main.cpp`, while several underlying game-engine method definitions are still declared rather than implemented in the reviewed repository snapshot.*

## 3. Design and Documentation

The design documents establish the problem scope, identify user interactions, and map responsibilities to classes. They are summarized here without repeating the same information in the feature tables.

| Design artifact | Purpose in this project |
|---|---|
| Problem statement | Defines the need for a lightweight local chess application and clarifies what is included and excluded. |
| Use-case model | Describes the Human Player as the actor and captures interactions such as making moves, viewing legal moves, promoting a pawn, castling, resigning, offering a draw, and restarting. |
| Noun-verb analysis | Filters candidate nouns into classes and verbs into operations, helping derive the software structure from the problem description. |
| CRC cards | Records the responsibilities of classes and the collaborators they work with. |
| Class diagram | Shows class relationships, including inheritance and ownership relationships. |
| Object diagram | Gives a runtime snapshot of a game after the moves `1. e4 e5 2. Nf3`. |
| README | Provides a project overview, command reference, file overview, and summary of the OOP concepts used. |

## 4. Main Classes and Responsibilities

- **`Position` and `Color`:** `Position` represents a board square using row and column values and supports algebraic-square conversion such as `e4`. `Color` represents the side associated with a player or piece.
- **`Piece`:** An abstract base class that defines the common interface for chess pieces. Its pure virtual operations include `symbol()`, `pseudoMoves()`, and `clone()`.
- **Concrete piece classes:** `Pawn`, `Knight`, `Bishop`, `Rook`, `Queen`, and `King` specialize the common `Piece` interface. Each is declared `final`.
- **`Player` and `RankedPlayer`:** Represent a player and a specialized player that also carries a rating.
- **`Board`:** Represents the 8×8 board through a grid of `Piece*` pointers. It includes copy-related operations and tracks the last double-step pawn move as groundwork for en passant.
- **`Game`:** Coordinates the board, players, current turn, move history, game status, and draw state. Its interface includes operations such as `legalMoves`, `makeMove`, `isInCheck`, `hasAnyLegalMove`, `resign`, `offerDraw`, `acceptDraw`, and `restart`.
- **`Game::MoveRecord`:** A nested class used to represent and format an entry in the move history.
- **`main.cpp`:** Contains the console input loop and connects user commands to the `Game` and `Board` interfaces.

## 5. OOP Concepts Demonstrated in the Project

The concepts below are tied to concrete classes or code patterns in the reviewed project. A declaration or design-level feature is identified as such rather than being described as a fully working engine behaviour.

| OOP / C++ concept | How it appears in the project |
|---|---|
| **Classes and objects** | `Game`, `Board`, `Position`, `Player`, and the piece classes model the main entities in a chess game. Their objects represent the current game, board, players, squares, and pieces. |
| **Abstraction** | The `Piece` base class exposes a common interface for different kinds of pieces without requiring callers to depend on each piece's internal movement implementation. |
| **Abstract class** | `Piece` is abstract because it declares pure virtual operations and therefore cannot be instantiated directly as a complete piece. |
| **Pure virtual functions** | `symbol()`, `pseudoMoves()`, and `clone()` are pure virtual in `Piece`, requiring concrete piece types to provide their own implementations. |
| **Inheritance** | `Pawn`, `Knight`, `Bishop`, `Rook`, `Queen`, and `King` derive from `Piece`; `RankedPlayer` derives from `Player`. |
| **Runtime polymorphism** | Base-class `Piece*` pointers can refer to objects of derived piece classes. Virtual operations provide a common call interface for piece-specific behaviour. |
| **Virtual functions and overriding** | `symbol()`, `pseudoMoves()`, and `clone()` form the polymorphic piece interface. `attackSquares()` is virtual and is overridden by `Pawn` and `King`; `Piece` also has a virtual destructor. |
| **Encapsulation** | Board and game state, along with the piece counter, are private. Public methods such as `getPiece`, `status`, and `turn` provide controlled access. |
| **Access specifiers** | `private` protects internal state, while `public` methods expose the operations needed by other parts of the program. The stream output operator is declared as a friend to access the board's private representation. |
| **Constructors** | Constructors initialize positions, pieces, players, the board, and the game. Examples include `Position(r, c)`, `Piece(Color)`, `Player(name, color)`, `RankedPlayer(name, color, rating)`, `Game(whiteName, blackName)`, and `Board()`. |
| **Constructor overloading** | Multiple constructor signatures support different forms of construction, including `Piece(Color)` versus `Piece(const Piece&)`, and `Board()` versus `Board(const Board&)`. |
| **Copy constructor** | `Board(const Board&)` and `Piece(const Piece&)` provide copy-construction paths. Board copying is intended to duplicate the board's pieces rather than merely copy pointer addresses. |
| **Destructor and resource management** | `Board` has a destructor that frees its pieces. `Piece` has a virtual destructor so derived pieces can be destroyed safely through a base-class pointer. |
| **Operator overloading** | `Position` overloads `==` and `!=` to compare squares. `Board::operator=` supports assignment with deep-copy behaviour. |
| **Friend function** | The board output operator is declared as a friend of `Board`, giving it access to private board data when formatting the board. |
| **Stream insertion operator overloading** | `operator<<(ostream&, const Board&)` supports a stream-style expression such as `cout << game.board()` for board display. |
| **Nested class** | `Game::MoveRecord` is declared inside `Game` and stores move-display information, including the piece symbol and source/destination squares. |
| **Object slicing** | `announcePlayer(Player p)` receives a `Player` by value. Passing a `RankedPlayer` to it slices off the derived-only rating information. |
| **Static data member** | `Piece::totalPiecesAlive` is shared across piece instances and is incremented/decremented by piece construction and destruction. It is used for the displayed piece count. |
| **Static member functions** | `Piece::getTotalPieces()` reads the shared counter without requiring a particular piece object. `Position::fromAlgebraic()` converts text such as `e4` into a position. |
| **`final` keyword** | The six concrete piece classes are declared `final`, preventing further derivation from those piece types. |
| **`this` pointer** | The `Piece(Color)` constructor uses `this->color` and `this->moved` to refer explicitly to the current object's members. |
| **Pointers to objects** | The board grid stores pointers to piece objects, allowing the board to hold and manage the pieces occupying its squares. |
| **Pointers to base and derived objects** | `Piece*` can refer to derived objects such as `Pawn` or `Knight`. The `clone()` interface is designed to return copies through a base-class pointer. |
| **Deep copying** | The board's copy operations duplicate piece objects so a copied board can be used independently, including for safe move simulation. |
| **Composition / ownership** | `Game` owns the board and coordinates players and game state; `Board` owns the pieces represented in its grid. These relationships express whole-part ownership in the design. |
| **Standard library containers** | Move history uses a `vector<string>` in the current code, and the game structure keeps a record of moves for later display. |
| **Structs and data modelling** | `Move` is represented as a struct containing fields such as `from`, `to`, and `promotion`, grouping data about a requested move. |
| **Enumerations** | `Color` is an enum used to represent the two sides, making the meaning of player/piece colour explicit. |
| **Virtual base class** | **Not currently used.** The reviewed class hierarchies use single inheritance, so virtual inheritance is not part of the present implementation. |

## 6. Game Features and User Interaction

This section describes what a player can do through the console and the role each feature has in a chess session. It avoids repeating the OOP-concept explanations above.

| Game feature | Player interaction | Purpose and current implementation status |
|---|---|---|
| **Board display** | The board is displayed through the console interface. | Uses the board representation and the stream insertion operator interface to format the board for output. |
| **Move entry** | Enter a source and destination, for example `e2 e4`. | The input path is wired in `main.cpp` and includes feedback for an illegal move. The `Game::makeMove` method is declared, but its implementation body is absent in the reviewed snapshot. |
| **Legal-move hints** | Enter `moves e2` to request moves for a square. | The interface is wired and includes handling for an invalid square. The engine method `legalMoves` is declared but not yet defined in the reviewed snapshot. |
| **Turn and status display** | Check the status line after a command. | The interface includes a check banner and result messaging; the underlying status-update and check-detection methods are declared in the header. |
| **Check detection** | The game is intended to indicate when a king is in check. | The design includes `isInCheck` and `Board::isSquareAttacked`; their declarations are present, but the engine definitions are not present in the reviewed snapshot. |
| **Checkmate and stalemate** | The game is intended to announce a winner or a draw when no legal continuation exists. | The interface includes result messaging. `hasAnyLegalMove` and `updateStatus` are declared, but full engine enforcement is not yet implemented in the reviewed snapshot. |
| **Pawn promotion** | Enter a move such as `e7 e8 q`; Queen is the default when no promotion piece is supplied. | Promotion input is wired into the interface and `Move::promotion` exists. Full promotion behaviour depends on the move-engine implementation. |
| **Move history** | Enter `history`. | The interface is wired to display the move record list. `Game::MoveRecord` formats entries such as `P e2-e4`; the `history()` method is declared. |
| **Resignation** | Enter `resign`. | The command is connected to the declared `Game::resign` operation, intended to end the game by resignation. |
| **Draw offer and acceptance** | Enter `draw` to access draw handling. | The design and interface include draw offers and acceptance through `offerDraw` and `acceptDraw`; full engine behaviour depends on their definitions. |
| **Restart** | Enter `restart`. | The command is wired to the declared restart operation, intended to reset the board and game state without requiring the program to be closed. |
| **En passant groundwork** | No separate command; the capture would be handled as part of a move. | The board tracks the last double-step pawn move and includes `wasDoubleStepTo` groundwork. Complete en passant move execution is not confirmed in the reviewed snapshot. |
| **Castling groundwork** | Intended to be performed as a king move. | Castling is included in the use cases and design materials, and piece movement state is tracked. The reviewed `Move` structure has no dedicated castling flag and the console path has no dedicated castling handling. |
| **Two-player local play** | Two people take turns on the same computer. | This is the intended operating mode; no AI opponent, network play, or clock is included in the project scope. |

**Implementation note:** the console command paths and class declarations exist, but a declared method is not the same as a completed engine feature. The reviewed repository snapshot does not contain definitions for several core rules-engine methods, so features that depend on those methods should be described as wired, designed, or prepared rather than as fully verified gameplay.

## 7. Conclusion

The Chess Engine project establishes a clear object-oriented structure for a local two-player chess application. Its design work connects the problem statement to user interactions, class responsibilities, and relationships between objects. The problem statement defines the intended scope; the use-case model identifies how a player interacts with the application; noun-verb analysis helps derive the class and method structure; CRC cards allocate responsibilities; and the class and object diagrams document both static relationships and a sample runtime state.

At the code level, the project provides a practical demonstration of a broad range of C++ OOP concepts. The abstract `Piece` class and its pure virtual functions establish a common interface for different chess pieces, while inheritance and virtual functions allow the concrete piece types to provide specialized behaviour. Encapsulation is reflected in private game and board state, with public operations used to interact with that state. Constructor overloading, copy constructors, operator overloading, a friend stream operator, nested move-history records, static members, the `this` pointer, `final` classes, and base-class pointers all connect course concepts to a concrete application rather than isolated examples.

The project also models meaningful chess interactions at the console level: entering moves, requesting legal-move hints, viewing history, handling resignation and draw commands, restarting a game, and displaying status or result messages. Special chess rules such as promotion, en passant, and castling have different levels of readiness in the reviewed snapshot; their presence in the interface or design should not be confused with fully implemented and tested rules-engine behaviour. Similarly, virtual base class inheritance is not used in the current class hierarchy, which relies on single inheritance.

Overall, the project brings together analysis, design, class modelling, C++ implementation patterns, and a command-based user interface in one application. It provides a strong foundation for demonstrating how OOP supports separation of responsibilities, reusable interfaces, state management, and extensible modelling. The report therefore distinguishes the features and concepts represented in the current code from those that remain at declaration or design level, giving an accurate picture of the project's progress as of the reviewed repository snapshot.
