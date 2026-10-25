# Tic Tac Toe

A tic tac toe game I made for Programming 1. It runs in the console and has a
two player mode, a mode against the computer, and a mode where the computer
plays itself. Your record gets saved so it doesn't reset every time you close
the program.

## Build and run

```
g++ -std=c++17 -o tictactoe main.cpp
./tictactoe
```

On Windows add .exe to the name:

```
g++ -std=c++17 -o tictactoe.exe main.cpp
```

## Menu

```
1. Player vs Player
2. Player vs AI
3. AI vs AI
4. Stats
5. Quit
```

Player vs Player asks for both names and then you take turns on the same
keyboard.

Player vs AI asks for your name, if you want to be X or O, and the difficulty.
X always goes first, so if you pick O the computer starts.

AI vs AI runs both sides by itself. On impossible it always ends in a draw if
you want to watch it.

Stats shows games, wins, losses, draws and your win rate.

## Board

The squares are numbered 1 to 9:

```
1 | 2 | 3
--+---+--
4 | 5 | 6
--+---+--
7 | 8 | 9
```

Type the number of the square you want. If you type a letter, a number that's
not on the board, or a square that's already taken, it just asks again.

## Difficulties

Easy: the AI picks a random empty square.

Hard: it looks one move ahead. It takes a win if it has one, blocks you if
you're about to win, and otherwise goes for the center or a corner. You can
still beat it with a fork because it only looks one move ahead.

Impossible: this is the minimax one. It plays out every possible way the rest
of the game can go and assumes you always answer with your best move, so it
never loses. The best you can do is a draw.

## How minimax works

minimax() scores a board from the AI's point of view. An AI win is positive, an
AI loss is negative, and a draw is 0. The score also subtracts the depth so it
prefers to win sooner (and delays losing if it's in a bad spot).

On the AI's turn it tries every empty square and keeps the highest score. On
the other player's turn it does the same thing but keeps the lowest score,
because it assumes they play their best move. Then bestMove() tries each move
once and plays the one that gives the best score.

There are only around 550,000 possible tic tac toe positions, so it finishes
right away without any fancy optimizations.

## Files

main.cpp - the game

stats.txt - created automatically after your first game
