# System Boundary
### Chess Application


## *Actors*

Human Player — the only actor. The application has no AI, no external
system, and no scheduled/time-triggered actor (no game clock in this
version).

Use Cases (9)
Start Game
Make Move
View Legal Moves
Promote Pawn
Castle
Resign Game
Offer/Accept Draw
View Move History
Restart Game


## Relationships

Make Move <<include>> View Legal Moves
— every move requires the system to first determine legal destinations.
Promote Pawn <<extend>> Make Move
— guard condition: pawn reaches the last rank.
Castle <<extend>> Make Move
— guard condition: castling preconditions met (king/rook unmoved, no
pieces between them, king not in/through/into check).
