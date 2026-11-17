#include "chess.h"


// the static counter is declared inside Piece, but it must be defined once here
int Piece::totalPiecesAlive = 0;

//BOARD

// sets up the standard starting position
// row 0 = rank 1 (White's back rank), row 7 = rank 8 (Black's back rank)
Board::Board() : lastDoubleStep(-1, -1) {
    for (int r = 0; r < 8; r++)
        for (int c = 0; c < 8; c++)
            grid[r][c] = nullptr;

    for (int c = 0; c < 8; c++) {
        grid[1][c] = new Pawn(Color::WHITE);
        grid[6][c] = new Pawn(Color::BLACK);
    }

    grid[0][0] = new Rook(Color::WHITE);   grid[7][0] = new Rook(Color::BLACK);
    grid[0][1] = new Knight(Color::WHITE); grid[7][1] = new Knight(Color::BLACK);
    grid[0][2] = new Bishop(Color::WHITE); grid[7][2] = new Bishop(Color::BLACK);
    grid[0][3] = new Queen(Color::WHITE);  grid[7][3] = new Queen(Color::BLACK);
    grid[0][4] = new King(Color::WHITE);   grid[7][4] = new King(Color::BLACK);
    grid[0][5] = new Bishop(Color::WHITE); grid[7][5] = new Bishop(Color::BLACK);
    grid[0][6] = new Knight(Color::WHITE); grid[7][6] = new Knight(Color::BLACK);
    grid[0][7] = new Rook(Color::WHITE);   grid[7][7] = new Rook(Color::BLACK);
}

// deep copy: clone() makes a copy of the right piece type through the base pointer
Board::Board(const Board& other) : lastDoubleStep(other.lastDoubleStep) {
    for (int r = 0; r < 8; r++)
        for (int c = 0; c < 8; c++) {
            if (other.grid[r][c] != nullptr) grid[r][c] = other.grid[r][c]->clone();
            else grid[r][c] = nullptr;
        }
}

Board& Board::operator=(const Board& other) {
    if (this == &other) return *this; // self-assignment check

    for (int r = 0; r < 8; r++)       // free what we own first
        for (int c = 0; c < 8; c++) {
            delete grid[r][c];
            grid[r][c] = nullptr;
        }

    for (int r = 0; r < 8; r++)       // then copy like the copy constructor
        for (int c = 0; c < 8; c++)
            if (other.grid[r][c] != nullptr) grid[r][c] = other.grid[r][c]->clone();

    lastDoubleStep = other.lastDoubleStep;
    return *this;
}

Board::~Board() {
    for (int r = 0; r < 8; r++)
        for (int c = 0; c < 8; c++)
            delete grid[r][c];
}

// returns nullptr for an empty square or a position off the board
Piece* Board::getPiece(Position pos) const {
    if (!pos.isValid()) return nullptr;
    return grid[pos.row][pos.col];
}

// friend function: prints the board with rank 8 at the top
// uses the Piece -> char typecast (char ch = *piece)
ostream& operator<<(ostream& os, const Board& board) {
    for (int r = 7; r >= 0; r--) {
        os << (r + 1) << "  ";
        for (int c = 0; c < 8; c++) {
            if (board.grid[r][c] == nullptr) {
                os << ". ";
            } else {
                char ch = *board.grid[r][c];
                os << ch << " ";
            }
        }
        os << "\n";
    }
    os << "\n   a b c d e f g h\n";
    return os;
}



// Board
Position Board::findKing(Color) const { return Position(); }
void Board::applyMove(Move) {}
bool Board::isSquareAttacked(Position, Color) const { return false; }

// Pieces: no moves yet
vector<Position> Pawn::pseudoMoves(Position, const Board&) const { return {}; }
vector<Position> Pawn::attackSquares(Position, const Board&) const { return {}; }
vector<Position> Knight::pseudoMoves(Position, const Board&) const { return {}; }
vector<Position> Bishop::pseudoMoves(Position, const Board&) const { return {}; }
vector<Position> Rook::pseudoMoves(Position, const Board&) const { return {}; }
vector<Position> Queen::pseudoMoves(Position, const Board&) const { return {}; }
vector<Position> King::pseudoMoves(Position, const Board&) const { return {}; }
vector<Position> King::attackSquares(Position, const Board&) const { return {}; }

// Game
Game::Game(string whiteName, string blackName)
    : theBoard(), white(whiteName, Color::WHITE), black(blackName, Color::BLACK),
      turnColor(Color::WHITE), currentStatus(GameStatus::ONGOING), drawOffered(false) {}

vector<Position> Game::legalMoves(Position) const { return {}; }
bool Game::isInCheck(Color) const { return false; }
bool Game::makeMove(Position, Position, char) { return false; } // every move is "illegal move" for now
bool Game::hasAnyLegalMove(Color) const { return true; }
void Game::resign(Color) {}
void Game::offerDraw() {}
bool Game::acceptDraw() { return false; }
void Game::restart() {}
void Game::updateStatus() {}
