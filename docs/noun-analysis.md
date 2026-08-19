# Chess Application - Noun-Verb Analysis

Scope: full problem statement (lightweight local 2-player chess, no online/AI
overhead, enforces legality, visible/accurate state, in-app restart) plus the
use-case diagram (actor: Human Player; use cases: Start Game, Resign Game,
Offer/Accept Draw, View Move History, Restart Game, Castle and Promote Pawn
as extensions of Make Move, Make Move includes View Legal Moves).

Filters applied to discard candidates:
1. **Outside the system** — not part of what this application is responsible for
2. **A property of something else** — data/behavior that belongs on an existing class, not a class of its own
3. **A synonym already listed** — same concept as a candidate already kept, under a different name
4. **Vague or whole system** — too abstract, or describes a condition/state rather than a discrete thing

---

## 1. Raw Candidate List (Nouns)

Chess Application , Human Player, Game , Board , Square/Position , Piece ,
Pawn , Knight , Bishop , Rook , Queen , King , Move , Color , Move History ,
Resignation , Draw Offer , Legal-move hints/highlighting , Game state ,
Check , Checkmate , Stalemate , AI/Computer player

---

## 2. Surviving Classes

| Noun (source) | Becomes | Reasoning |
|---|---|---|
| Chess Application | `ChessApplication` class | The system boundary — owns app lifecycle (start/restart), distinct from one game's rules |
| Human Player | `Player` class | Identity only: name + color |
| Game (a full game of chess) | `Game` class | The rules engine — scoped to *one* game's lifetime, not the app's |
| Board | `Board` class | Owns the 8x8 grid, the one thing that changes state every move |
| Square / Position | `Position` class | A value, not a behaviour — needs equality, not logic |
| Piece | `Piece` (abstract) | Every piece shares color + "can I move here?", differs in *how* |
| Pawn, Knight, Bishop, Rook, Queen, King | `Piece` subclasses | Each has genuinely different movement rules |
| Move | `Move` class | A record of "what happened", separate from the act of applying it |
| Color (White/Black) | `Color` enum | A closed, fixed set of two values |
| Draw offer | `DrawOffer` class | Persists across a turn boundary (A offers, B must respond before A moves again) — enough state to justify a type |

---

## 3. Discarded Candidates — Filter Applied

| Discarded Candidate | Filter | Reasoning |
|---|---|---|
| Square / Tile | **Synonym already listed** | Same concept as `Position`, no behaviour beyond being a coordinate |
| Move History | **A property of something else** | A plain `List[Move]` attribute on `Game`; nothing for a separate class to own |
| Resignation | **A property of something else** | One boolean flip + a result string on `Game`; a method, not an object |
| Legal-move hints / highlighting | **Outside the system** | A rendering concern for the UI layer, not the domain model — presentation of data the engine already computes |
| Game state (is_over, result, current_turn) | **A property of something else** | Just fields on `Game`, not a separate class |
| Check / Checkmate / Stalemate | **Vague or whole system** | Conditions, not things — verbs pretending to be nouns |
| AI / Computer player | **Outside the system** | Explicitly out of scope; `Player` stays thin enough for a future `AIPlayer(Player)` subclass, but no such class exists now |

---

## 4. Verbs → Candidate Methods

| Verb (from use cases) | Becomes | On which class |
|---|---|---|
| Start Game | `start_game()` | `ChessApplication` |
| Restart Game | `restart_game()` | `ChessApplication` (discards current `Game`, creates a fresh one) |
| Resign Game | `resign(color)` | `Game` |
| Offer Draw | `offer_draw(color)` | `Game`, via `DrawOffer` |
| Accept Draw | `accept_draw()` | `Game`, via `DrawOffer` |
| View Move History | `get_move_history()` | `Game` |
| Make Move | `make_move()` | `Game` |
| View Legal Moves | `get_legal_moves()` | `Game` |
| Castle | handled inside `make_move()` via `Move` flags | `Game` / `Board` |
| Promote Pawn | handled inside `make_move()` via `Move` flags | `Game` / `Board` |
| Highlight / hint | rendering only, not a rules concern | UI layer |
| Attack (a square) | `is_square_attacked()` | `Board` |
| Clone / simulate a position | `clone()` | `Board` |
| Switch turns | `_switch_turn()` | `Game` |

**Key design implication:** `Start Game` / `Restart Game` sit outside the
`Make Move`/`Castle`/`Promote` cluster on the use-case diagram — that's the
diagram signalling that the app has two different lifetimes. `ChessApplication`
owns the outer loop and holds a `Game` instance it can discard and replace;
`Game` itself shouldn't know how to restart itself.