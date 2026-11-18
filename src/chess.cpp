#include "chess.h"
#include <cctype>
#include <cstdlib>

// ===================== STATIC MEMBER DEFINITION =====================
// the static counter is declared inside Piece, but it must be defined once here
int Piece::totalPiecesAlive = 0;

// ===================== BOARD =====================

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

Board::Board(const Board& other) : lastDoubleStep(other.lastDoubleStep) {
    for (int r = 0; r < 8; r++)
        for (int c = 0; c < 8; c++) {
            if (other.grid[r][c] != nullptr) grid[r][c] = other.grid[r][c]->clone();
            else grid[r][c] = nullptr;
        }
}

Board& Board::operator=(const Board& other) {
    if (this == &other) return *this; 

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

// ===================== HELPERS (only visible inside this file) =====================
namespace {

// true if a piece of colour c may move onto p: p is on the board and is empty or holds an enemy
bool canLandOn(Position p, Color c, const Board& board) {
    if (!p.isValid()) return false;
    Piece* target = board.getPiece(p);
    return target == nullptr || target->color != c;
}

// walks from 'from' in direction (dr, dc) until it hits the edge or a piece
// empty squares are added; an enemy piece is added (capture) and then we stop; own piece just stops
void slide(Position from, Color c, const Board& board, int dr, int dc, vector<Position>& out) {
    Position p(from.row + dr, from.col + dc);
    while (p.isValid()) {
        Piece* target = board.getPiece(p);
        if (target == nullptr) {
            out.push_back(p);
        } else {
            if (target->color != c) out.push_back(p);
            break;
        }
        p = Position(p.row + dr, p.col + dc);
    }
}



} // namespace

// ===================== BOARD: RULES =====================

// scans the grid for the king of this colour; returns an invalid Position if there is none
Position Board::findKing(Color color) const {
    for (int r = 0; r < 8; r++)
        for (int c = 0; c < 8; c++) {
            Piece* p = grid[r][c];
            if (p != nullptr && p->color == color && dynamic_cast<King*>(p) != nullptr)
                return Position(r, c);
        }
    return Position();
}

// is 'pos' attacked by any piece of colour byColor?
// uses attackSquares (not pseudoMoves) so that pawns attack diagonally even onto empty squares

bool Board::isSquareAttacked(Position pos, Color byColor) const {
    for (int r = 0; r < 8; r++)
        for (int c = 0; c < 8; c++) {
            Piece* p = grid[r][c];
            if (p == nullptr || p->color != byColor) continue;
            vector<Position> targets = p->attackSquares(Position(r, c), *this);
            for (int i = 0; i < (int)targets.size(); i++)
                if (targets[i] == pos) return true;
        }
    return false;
    
}

// handles: normal moves, captures, en passant, castling (moves the rook too)
void Board::applyMove(Move m) {
    Piece* mover = grid[m.from.row][m.from.col];
    if (mover == nullptr) return;

    lastDoubleStep = Position(); // only the very last move can be captured en passant

    bool isPawn = dynamic_cast<Pawn*>(mover) != nullptr;
    bool isKing = dynamic_cast<King*>(mover) != nullptr;


    // castling: king moves two columns -> also move the matching rook
    if (isKing && abs(m.to.col - m.from.col) == 2) {
        int row = m.from.row;
        int rookFrom = (m.to.col > m.from.col) ? 7 : 0;
        int rookTo   = (m.to.col > m.from.col) ? 5 : 3;
        grid[row][rookTo] = grid[row][rookFrom];
        grid[row][rookFrom] = nullptr;
        if (grid[row][rookTo] != nullptr) grid[row][rookTo]->moved = true;
    }

    // normal move / capture: whatever stands on the target square is destroyed
    delete grid[m.to.row][m.to.col];
    grid[m.to.row][m.to.col] = mover;
    grid[m.from.row][m.from.col] = nullptr;
    mover->moved = true;

    // remember a two-square pawn push so the enemy can capture it en passant next move
    if (isPawn && abs(m.to.row - m.from.row) == 2) lastDoubleStep = m.to;

    // promotion: pawn reached the last rank -> replace it with the chosen piece (default queen)
    if (isPawn) {
        int lastRank = (mover->color == Color::WHITE) ? 7 : 0;
        if (m.to.row == lastRank) {
            Piece* promoted;
            switch (toupper(m.promotion)) {
                case 'R': promoted = new Rook(mover->color);   break;
                case 'B': promoted = new Bishop(mover->color); break;
                case 'N': promoted = new Knight(mover->color); break;
                default:  promoted = new Queen(mover->color);  break;
            }
            promoted->moved = true;
            delete mover;
            grid[m.to.row][m.to.col] = promoted;
        }
    }
}

// ===================== PIECES: MOVEMENT =====================

// Pawn: forward 1, forward 2 from the start rank, diagonal capture, en passant
vector<Position> Pawn::pseudoMoves(Position pos, const Board& board) const {
    vector<Position> out;
    int dir = (color == Color::WHITE) ? 1 : -1;
    int startRow = (color == Color::WHITE) ? 1 : 6;

    Position one(pos.row + dir, pos.col);
    if (one.isValid() && board.getPiece(one) == nullptr) {
        out.push_back(one);
        Position two(pos.row + 2 * dir, pos.col);
        if (pos.row == startRow && board.getPiece(two) == nullptr) out.push_back(two);
    }

    for (int dc = -1; dc <= 1; dc += 2) {
        Position diag(pos.row + dir, pos.col + dc);
        if (!diag.isValid()) continue;

        Piece* target = board.getPiece(diag);
        if (target != nullptr) {
            if (target->color != color) out.push_back(diag); // normal capture
        } 
    }
    return out;
}

// squares a pawn threatens: the two forward diagonals (even if empty)
vector<Position> Pawn::attackSquares(Position pos, const Board&) const {
    vector<Position> out;
    int dir = (color == Color::WHITE) ? 1 : -1;
    for (int dc = -1; dc <= 1; dc += 2) {
        Position diag(pos.row + dir, pos.col + dc);
        if (diag.isValid()) out.push_back(diag);
    }
    return out;
}

vector<Position> Knight::pseudoMoves(Position pos, const Board& board) const {
    vector<Position> out;
    int dr[8] = {-2, -2, -1, -1, 1, 1, 2, 2};
    int dc[8] = {-1, 1, -2, 2, -2, 2, -1, 1};
    for (int i = 0; i < 8; i++) {
        Position p(pos.row + dr[i], pos.col + dc[i]);
        if (canLandOn(p, color, board)) out.push_back(p);
    }
    return out;
}

vector<Position> Bishop::pseudoMoves(Position pos, const Board& board) const {
    vector<Position> out;
    slide(pos, color, board, 1, 1, out);
    slide(pos, color, board, 1, -1, out);
    slide(pos, color, board, -1, 1, out);
    slide(pos, color, board, -1, -1, out);
    return out;
}

vector<Position> Rook::pseudoMoves(Position pos, const Board& board) const {
    vector<Position> out;
    slide(pos, color, board, 1, 0, out);
    slide(pos, color, board, -1, 0, out);
    slide(pos, color, board, 0, 1, out);
    slide(pos, color, board, 0, -1, out);
    return out;
}

vector<Position> Queen::pseudoMoves(Position pos, const Board& board) const {
    vector<Position> out;
    for (int dr = -1; dr <= 1; dr++)
        for (int dc = -1; dc <= 1; dc++)
            if (dr != 0 || dc != 0) slide(pos, color, board, dr, dc, out);
    return out;
}

//attack sqaures of the king
//king kaha attack kar sakta hain
vector<Position> King::attackSquares(Position pos, const Board& board) const {
    vector<Position> out;
    for (int dr = -1; dr <= 1; dr++)
        for (int dc = -1; dc <= 1; dc++) {
            if (dr == 0 && dc == 0) continue;
            Position p(pos.row + dr, pos.col + dc);
            if (canLandOn(p, color, board)) out.push_back(p);
        }
    return out;
}

// normal king steps + castling
// castling needs: king and rook never moved, empty squares between, king not in check,
// and the squares the king crosses/lands on are not attacked
vector<Position> King::pseudoMoves(Position pos, const Board& board) const {
    vector<Position> out = attackSquares(pos, board);

    int home = (color == Color::WHITE) ? 0 : 7;
    if (moved || pos.row != home || pos.col != 4) return out;

    Color enemy = opposite(color);
    if (board.isSquareAttacked(pos, enemy)) return out; // cannot castle out of check

    // king side (towards the h-file)
    Piece* kRook = board.getPiece(Position(home, 7));
    if (kRook != nullptr && kRook->color == color && !kRook->moved &&
        dynamic_cast<Rook*>(kRook) != nullptr &&
        board.getPiece(Position(home, 5)) == nullptr &&
        board.getPiece(Position(home, 6)) == nullptr &&
        !board.isSquareAttacked(Position(home, 5), enemy) &&
        !board.isSquareAttacked(Position(home, 6), enemy))
        out.push_back(Position(home, 6));

    //queen side (towards the a-file); the b-file square only has to be empty, not safe
    Piece* qRook = board.getPiece(Position(home, 0));
    if (qRook != nullptr && qRook->color == color && !qRook->moved &&
        dynamic_cast<Rook*>(qRook) != nullptr &&
        board.getPiece(Position(home, 1)) == nullptr &&
        board.getPiece(Position(home, 2)) == nullptr &&
        board.getPiece(Position(home, 3)) == nullptr &&
        !board.isSquareAttacked(Position(home, 3), enemy) &&
        !board.isSquareAttacked(Position(home, 2), enemy))
        out.push_back(Position(home, 2));

    return out;
}

// ===================== GAME =====================

Game::Game(string whiteName, string blackName)
    : theBoard(), white(whiteName, Color::WHITE), black(blackName, Color::BLACK),
      turnColor(Color::WHITE), currentStatus(GameStatus::ONGOING), drawOffered(false) {}

// all FULLY legal destinations for the piece on 'pos':
// take every pseudo-legal move, play it on a copy of the board,
// and keep it only if our own king is not attacked afterwards
// (this single test handles pins, moving into check and escaping check)
vector<Position> Game::legalMoves(Position pos) const {
    vector<Position> legal;
    Piece* piece = theBoard.getPiece(pos);
    if (piece == nullptr) return legal;

    Color mine = piece->color;
    vector<Position> candidates = piece->pseudoMoves(pos, theBoard);

    for (int i = 0; i < (int)candidates.size(); i++) {
        Board copy(theBoard);
        Move m;
        m.from = pos;
        m.to = candidates[i];
        m.promotion = 'Q';
        copy.applyMove(m);

        Position king = copy.findKing(mine);
        if (king.isValid() &&
        !copy.isSquareAttacked(king, opposite(mine))) {
            legal.push_back(candidates[i]);
        }
    }
    return legal;
}

bool Game::isInCheck(Color c) const {
    Position king = theBoard.findKing(c);
    if (!king.isValid()) return false;
    return theBoard.isSquareAttacked(king, opposite(c));
}

// true if colour c has at least one legal move with any piece
bool Game::hasAnyLegalMove(Color c) const {
    for (int r = 0; r < 8; r++)
        for (int col = 0; col < 8; col++) {
            Position p(r, col);
            Piece* piece = theBoard.getPiece(p);
            if (piece != nullptr && piece->color == c && !legalMoves(p).empty())
                return true;
        }
    return false;
}

// returns false (= "Illegal move.") when: game is over, a square is off-board, the start square
// is empty, the piece is not the current player's, or the target is not in legalMoves()
bool Game::makeMove(Position from, Position to, char promotion) {
    if (currentStatus != GameStatus::ONGOING) return false;
    if (!from.isValid() || !to.isValid()) return false;

    Piece* piece = theBoard.getPiece(from);
    if (piece == nullptr) return false;
    if (piece->color != turnColor) return false;

    vector<Position> legal = legalMoves(from);
    bool found = false;
    for (int i = 0; i < (int)legal.size(); i++)
        if (legal[i] == to) { found = true; break; }
    if (!found) return false;

    // only Q/R/B/N are valid promotion letters; anything else becomes a queen
    char promo = toupper(promotion);
    if (promo != 'Q' && promo != 'R' && promo != 'B' && promo != 'N') promo = 'Q';

    // write the move into the history before the board changes
    MoveRecord record(piece->symbol(), from, to);
    string text = record.toString();
    bool isPawn = dynamic_cast<Pawn*>(piece) != nullptr;
    int lastRank = (turnColor == Color::WHITE) ? 7 : 0;
    if (isPawn && to.row == lastRank) text += string("=") + promo;
    if (dynamic_cast<King*>(piece) != nullptr && abs(to.col - from.col) == 2)
        text += (to.col > from.col) ? " (O-O)" : " (O-O-O)";

    Move m;
    m.from = from;
    m.to = to;
    m.promotion = promo;
    theBoard.applyMove(m);

    moveHistory.push_back(text);
    drawOffered = false;               // a pending draw offer is dropped once a move is played
    turnColor = opposite(turnColor);
    updateStatus();
    return true;
}

// resigning gives the win to the other side
void Game::resign(Color who) {
    if (currentStatus != GameStatus::ONGOING) return;
    currentStatus = (who == Color::WHITE) ? GameStatus::BLACK_WINS : GameStatus::WHITE_WINS;
}

void Game::offerDraw() {
    if (currentStatus == GameStatus::ONGOING) drawOffered = true;
}

bool Game::acceptDraw() {
    if (currentStatus != GameStatus::ONGOING || !drawOffered) return false;
    currentStatus = GameStatus::DRAW;
    drawOffered = false;
    return true;
}

// back to the starting position, White to move
void Game::restart() {
    theBoard = Board();
    turnColor = Color::WHITE;
    moveHistory.clear();
    currentStatus = GameStatus::ONGOING;
    drawOffered = false;
}

// called after every move: decides checkmate and stalemate draw
void Game::updateStatus() {
    if (!hasAnyLegalMove(turnColor)) {
        if (isInCheck(turnColor)) {
            // side to move is checkmated -> the other side wins
            cout<<"Checkmate!!\n";
            currentStatus = (turnColor == Color::WHITE) ? GameStatus::BLACK_WINS : GameStatus::WHITE_WINS;
        } else {
            currentStatus = GameStatus::DRAW; // stalemate
        }
        return;
    }
    
}
