#include "Chess.h"
#include <sstream>

string colorName(Color c) {
    if (c == Color::WHITE) return "White";
    return "Black";
}

//takes a Player BY VALUE on purpose, not by reference or pointer. if you hand this a
//RankedPlayer, the extra "rating" part just gets chopped off on the way in and all thats left
//inside the function is a plain Player - that's object slicing. added this specifically to
//show we understand the concept, the chess logic itself doesnt actually need this function
void announcePlayer(Player p) {
    cout << p.name << " is ready to play.\n";
}

void printHelp() {
    cout << "Commands:\n";
    cout << "  e2 e4        move a piece\n";
    cout << "  e7 e8 q      promote to Q/R/B/N\n";
    cout << "  moves e2     show legal destinations for e2\n";
    cout << "  resign       current player resigns\n";
    cout << "  draw         offer/accept a draw\n";
    cout << "  history      show move list\n";
    cout << "  restart      reset the board\n";
    cout << "  quit         exit\n";
}

int main() {
    cout << "=== Terminal Chess (2 Player) ===\n";
    cout << "White name: ";
    string whiteName, blackName;
    getline(cin, whiteName);
    cout << "Black name: ";
    getline(cin, blackName);
    if (whiteName == "") whiteName = "White";
    if (blackName == "") blackName = "Black";

    Game game(whiteName, blackName);

    //giving both players a made-up rating just so there's actually something to slice off below.
    //real rating doesnt matter here, its just a number to prove the point
    RankedPlayer whiteRanked(whiteName, Color::WHITE, 1200);
    RankedPlayer blackRanked(blackName, Color::BLACK, 1200);
    announcePlayer(whiteRanked); //slicing happens right at this call, rating is gone the moment it enters announcePlayer
    announcePlayer(blackRanked);

    printHelp();
    bool drawPending = false;

    while (true) {
        cout << "\n" << game.board();
        cout << "(pieces on board: " << Piece::getTotalPieces() << ")\n"; // static function - no Piece object needed to call this

        if (game.status() != GameStatus::ONGOING) {
            if (game.status() == GameStatus::WHITE_WINS) cout << whiteName << " (White) wins!\n";
            else if (game.status() == GameStatus::BLACK_WINS) cout << blackName << " (Black) wins!\n";
            else cout << "Game drawn.\n";
            cout << "Type 'restart' to play again or 'quit' to exit.\n";
        } else {
            string toMove = (game.turn() == Color::WHITE) ? whiteName : blackName;
            cout << toMove << " (" << colorName(game.turn()) << ") to move";
            if (game.isInCheck(game.turn())) cout << " - CHECK";
            cout << ". > ";
        }

        string line;
        if (!getline(cin, line)) break;
        istringstream iss(line);
        string first;
        iss >> first;
        if (first == "") continue;

        if (first == "quit") {
            break;

        } else if (first == "help") {
            printHelp();

        } else if (first == "history") {
            const vector<string>& h = game.history();
            if (h.empty()) cout << "(no moves yet)\n";
            for (int i = 0; i < (int)h.size(); i++) cout << (i + 1) << ". " << h[i] << "\n";

        } else if (first == "restart") {
            game.restart();
            drawPending = false;
            cout << "Board reset.\n";

        } else if (first == "resign") {
            if (game.status() == GameStatus::ONGOING) {
                game.resign(game.turn());
                cout << colorName(game.turn()) << " resigned.\n";
            }

        } else if (first == "draw") {
            if (game.status() != GameStatus::ONGOING) continue;
            if (drawPending) {
                game.acceptDraw();
                drawPending = false;
                cout << "Draw accepted.\n";
            } else {
                game.offerDraw();
                drawPending = true;
                cout << "Draw offered. Other player: type 'draw' to accept.\n";
            }

        } else if (first == "moves") {
            string sq;
            iss >> sq;
            Position p = Position::fromAlgebraic(sq);
            if (!p.isValid()) { cout << "Bad square.\n"; continue; }
            vector<Position> legal = game.legalMoves(p);
            if (legal.empty()) { cout << "No legal moves for " << sq << ".\n"; continue; }
            cout << "Legal moves for " << sq << ": ";
            for (int i = 0; i < (int)legal.size(); i++) cout << legal[i].toAlgebraic() << " ";
            cout << "\n";

        } else {
            string second, third;
            iss >> second;
            iss >> third;
            Position from = Position::fromAlgebraic(first);
            Position to = Position::fromAlgebraic(second);
            if (!from.isValid() || !to.isValid()) {
                cout << "Didn't understand that. Type 'help' for commands.\n";
                continue;
            }
            char promo = 'Q';
            if (third != "") promo = toupper(third[0]);
            drawPending = false;
            if (!game.makeMove(from, to, promo)) cout << "Illegal move.\n";
        }
    }

    cout << "Goodbye.\n";
    return 0;
}
