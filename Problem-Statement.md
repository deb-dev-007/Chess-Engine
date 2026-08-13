### Chess Engine 


**Problem Statement**


# Overview
Two people who want to play a full game of chess against each other on a single
computer currently have to use a physical board, or open a heavyweight online
platform that assumes an internet connection, an opponent on another device, or a
built-in AI they don't want. 

# Solution/product overview
There is no lightweight way for two people sitting at
the same machine to play a properly-enforced, complete game of chess with the
visual conveniences (legal-move hints, move history, highlighting) that modern
players expect, without any of the online or AI overhead.


# Detailed explanation
Two people who want to play a full game of chess against each other on a single
computer currently have to use a physical board, or open a heavyweight online
platform that assumes an internet connection, an opponent on another device, or a
built-in AI they don't want. There is no lightweight way for two people sitting at
the same machine to play a properly-enforced, complete game of chess with the
visual conveniences (legal-move hints, move history, highlighting) that modern
players expect, without any of the online or AI overhead.


# What is it about
At any point, either player can start a new game or restart the current one
without closing the application. The system is explicitly not responsible for
teaching chess, evaluating position quality, suggesting moves, or playing against
either player — every decision on the board is made by the two humans; the system's
only job is to enforce legality and keep the game state visible and accurate.
