#ifndef CHESS_H
#define CHESS_H

#include <iostream>
#include <string>
#include <vector>
using namespace std;

//nested class: Game::MoveRecord inside the Game class
//all the concrete piece classes (Pawn, Knight, etc)

//position class, just a row col pair basically
class Position {
public:
    int row, col;

    Position(int r = -1, int c = -1) {
        row = r;
        col = c;
    }

    bool isValid() const {
        return row >= 0 && row < 8 && col >= 0 && col < 8;
    }

    static Position fromAlgebraic(string s) {
        if (s.size() != 2) return Position();
        int c = s[0] - 'a';
        int r = s[1] - '1';
        return Position(r, c);
    }

    string toAlgebraic() const {
        string s = "";
        s += (char)('a' + col);
        s += (char)('1' + row);
        return s;
    }

    //needed this to check if two squares are the same, used all over the place
    //we tried to implement operator overloading here
    bool operator==(const Position& o) const {
        return row == o.row && col == o.col;
    }

    
    bool operator!=(const Position& o) const {
        return !(row == o.row && col == o.col);
    }
};

enum class Color { WHITE, BLACK };

inline Color opposite(Color c) {
    if (c == Color::WHITE) return Color::BLACK;
    return Color::WHITE;
}

class Board;

class Piece {
public:
    Color color;
    bool moved;

    Piece(Color c) {
        color = c;
        moved = false;
        totalPiecesAlive++;//piece counter
    }

    Piece(const Piece& other) {
        color = other.color;
        moved = other.moved;
        totalPiecesAlive++;
    }

    virtual ~Piece() {
        totalPiecesAlive--; // and one less now that this one's gone
    }

    // = 0 means no body here, every piece HAS to define its own version
    virtual char symbol() const = 0;
    virtual vector<Position> pseudoMoves(Position pos, const Board& board) const = 0;
    virtual vector<Position> attackSquares(Position pos, const Board& board) const {
        return pseudoMoves(pos, board); // most pieces attack same squares they move to, pawn/king are the odd ones out
    }
    virtual Piece* clone() const = 0;

    //static function
    //initiated without any object
    static int getTotalPieces() { return totalPiecesAlive; }

private:
    //static variable
    static int totalPiecesAlive;
};




//each piece is overriding its properties 
//also, we use "final" keyword
//no other class can inherit from these classes
class Pawn final : public Piece {
public:
    Pawn(Color c) : Piece(c) {}
    char symbol() const override { return color == Color::WHITE ? 'P' : 'p'; }
    vector<Position> pseudoMoves(Position pos, const Board& board) const override;
    vector<Position> attackSquares(Position pos, const Board& board) const override;
    Piece* clone() const override { return new Pawn(*this); }
};

class Knight final : public Piece {
public:
    Knight(Color c) : Piece(c) {}
    char symbol() const override { return color == Color::WHITE ? 'N' : 'n'; }
    vector<Position> pseudoMoves(Position pos, const Board& board) const override;
    Piece* clone() const override { return new Knight(*this); }
};

class Bishop final : public Piece {
public:
    Bishop(Color c) : Piece(c) {}
    char symbol() const override { return color == Color::WHITE ? 'B' : 'b'; }
    vector<Position> pseudoMoves(Position pos, const Board& board) const override;
    Piece* clone() const override { return new Bishop(*this); }
};

class Rook final : public Piece {
public:
    Rook(Color c) : Piece(c) {}
    char symbol() const override { return color == Color::WHITE ? 'R' : 'r'; }
    vector<Position> pseudoMoves(Position pos, const Board& board) const override;
    Piece* clone() const override { return new Rook(*this); }
};

class Queen final : public Piece {
public:
    Queen(Color c) : Piece(c) {}
    char symbol() const override { return color == Color::WHITE ? 'Q' : 'q'; }
    vector<Position> pseudoMoves(Position pos, const Board& board) const override;
    Piece* clone() const override { return new Queen(*this); }
};

class King final : public Piece {
public:
    King(Color c) : Piece(c) {}
    char symbol() const override { return color == Color::WHITE ? 'K' : 'k'; }
    vector<Position> pseudoMoves(Position pos, const Board& board) const override;
    vector<Position> attackSquares(Position pos, const Board& board) const override;
    Piece* clone() const override { return new King(*this); }
};

struct Move {
    Position from;
    Position to;
    char promotion = 'Q';
};

// board just holds the 8x8 grid and knows how to apply a move to it.
//grid is private here
class Board {
public:
    Board(); //starts the board-game 
    Board(const Board& other);     // deep copy - needed for the check-simulation stuff in Game
    Board& operator=(const Board& other); // same deal but for assignment
    ~Board();                      // frees all the piece pointers

    Piece* getPiece(Position pos) const;
    void applyMove(Move m);

    bool isSquareAttacked(Position pos, Color byColor) const;
    Position findKing(Color color) const;
    bool wasDoubleStepTo(Position p) const { return lastDoubleStep == p; }

    //into our private grid, and we didnt want to write a getter just for that. so its declared
    ///// we tried to add a friend here this is the "stream operator overloading using a friend function" part.
    friend ostream& operator<<(ostream& os, const Board& board);

private:
    Piece* grid[8][8];
    Position lastDoubleStep;
};


class Player {
public:
    string name;
    Color color;
    Player(string n, Color c) {
        name = n;
        color = c;
    }
};

class RankedPlayer : public Player {
public:
    int rating;
    RankedPlayer(string n, Color c, int r) : Player(n, c), rating(r) {}
};

enum class GameStatus { ONGOING, WHITE_WINS, BLACK_WINS, DRAW };

//main class of header file
//this is basically the main stuff of the game
class Game {
public:
    Game(string whiteName, string blackName);

    // nested class - bundles up what happened in a single move (which piece, from where, to
    // where) before its turned into the plain string that goes into moveHistory. put it inside
    // Game (instead of a free-standing class) since it only makes sense in the context of a
    // Game's history, nothing else in the project needs it
    class MoveRecord {
    public:
        MoveRecord(char sym, Position f, Position t) : pieceSymbol(sym), from(f), to(t) {}

        string toString() const {
            string s = "";
            s += pieceSymbol;
            s += " " + from.toAlgebraic() + "-" + to.toAlgebraic();
            return s;
        }

    private:
        char pieceSymbol;
        Position from, to;
    };

    vector<Position> legalMoves(Position pos) const;
    bool makeMove(Position from, Position to, char promotion = 'Q');

    bool isInCheck(Color c) const;
    bool hasAnyLegalMove(Color c) const;

    GameStatus status() const { return currentStatus; }
    Color turn() const { return turnColor; }

    void resign(Color who);
    void offerDraw();
    bool acceptDraw();
    void restart();

    const Board& board() const { return theBoard; }
    const vector<string>& history() const { return moveHistory; }

private:
    Board theBoard;
    Player white;
    Player black;
    Color turnColor;
    vector<string> moveHistory;
    GameStatus currentStatus;
    bool drawOffered;

    void updateStatus();
};

#endif
