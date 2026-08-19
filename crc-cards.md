# Chess Application: CRC Cards

One table per surviving class from noun-analysis.md. Position and Color are included for completeness even though, as noted there, they're plain value types — their "responsibilities" are just what data they hold, and they have effectively no collaborators of their own (everything else collaborates with them, not the other way around).

**ChessApplication**

*Responsibilities	Collaborators*
Start a new game
Restart the current game (discard it, create a fresh one)
Own the running application's lifetime, independent of any single game's outcome	Game
Player
Player

*Responsibilities	Collaborators*

Hold a display name
Hold a color (White or Black)	Color
Game

*Responsibilities	Collaborators*

Track whose turn it is
Validate a proposed move and apply it (enforce legality)
Compute the legal moves available to a piece, accounting for check
Detect check, checkmate, and stalemate
Handle resignation
Handle a draw offer and its acceptance
Maintain the move history	Board
Player
Move
DrawOffer
Color
Board

*Responsibilities	Collaborators*
Own the 8x8 grid of squares
Apply a move to the grid, including castling's rook move, en passant's extra capture, and pawn promotion
Detect whether a square is attacked by a given color
Locate a king
Render itself as text
Clone itself so Game can simulate a move safely	Piece (and its subclasses)
Move
Position
Color
Position

*Responsibilities	Collaborators*
Represent one square's coordinates (row, column)
Validate that a coordinate is on the board
Convert to/from algebraic notation (e.g. "e4")	(none — a value type used by, not collaborating with, other classes)
Piece (abstract)

*Responsibilities	Collaborators*
Know its own color
Know whether it has moved (needed for castling rights and a pawn's double-step)
Report every square it could geometrically reach from a given position
Report its own display symbol	Board
Position
Color
Pawn (Piece subclass)

*Responsibilities	Collaborators*
Move straight ahead, one square, or two from its starting row
Capture diagonally
Recognize an en passant capture opportunity
Recognize when it has reached the last rank	Board
Position
Move
Knight (Piece subclass)

*Responsibilities	Collaborators*
Report its eight L-shaped destination squares	Board
Position
Bishop (Piece subclass)

*Responsibilities	Collaborators*
Report the squares reachable by sliding diagonally until blocked	Board
Position
Rook (Piece subclass)

*Responsibilities	Collaborators*
Report the squares reachable by sliding horizontally or vertically until blocked
Participate in castling as the piece that moves alongside the King	Board
Position
King
Queen (Piece subclass)

*Responsibilities	Collaborators*
Report the squares reachable by sliding in any of the eight directions until blocked	Board
Position
King (Piece subclass)
Responsibilities	Collaborators
Report its one-square destination squares
Participate in castling	Board
Position
Rook
Game (Game determines whether the King is in check; the King doesn't judge this itself)
Move

*Responsibilities	Collaborators*
Record the start and end squares of one move
Record which piece moved and which piece, if any, was captured
Record whether the move was a castle, an en passant capture, or a promotion
Produce its own algebraic notation	Piece
Position
Color

*Responsibilities	Collaborators*
Represent White or Black
Report the opposite color	(none — a value type)
DrawOffer

*Responsibilities	Collaborators*
Remember which player offered a draw
Track whether the offer is pending, accepted, or declined	Game
Player