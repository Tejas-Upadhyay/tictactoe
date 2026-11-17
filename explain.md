# Main.cpp Function Explanations

This document provides a detailed, function-by-function explanation of the `Main.cpp` file, which implements a Tic-Tac-Toe game with three modes (PVP, PVAI, AIVAI) and a robust game statistics system.

## 🎯 Core Game Logic Functions

*   **`checkwin(char board[9], char XorO)`**:
    *   **Purpose**: Checks if the given player (`XorO`) has won on the current `board`.
    *   **Mechanism**: Checks all 3 rows, 3 columns, and 2 diagonals for three identical markers.
    *   **Returns**: `true` if a win is detected, `false` otherwise.
*   **`checkdraw(char board[9])`**:
    *   **Purpose**: Determines if the game is a draw (i.e., the board is full).
    *   **Mechanism**: Iterates through all 9 positions, returning `false` if any spot is still marked as available (not 'X' or 'O').
    *   **Returns**: `true` if the board is full, `false` otherwise.
*   **`printboard(const char board[9])`**:
    *   **Purpose**: Prints the current state of the 3x3 game board to the console.
    *   **Mechanism**: Nested loops print the characters row by row.
*   **`availability(char board[9], int position)`**:
    *   **Purpose**: Checks if a specific `position` (0-8) is available for a move.
    *   **Mechanism**: Checks if the position is within the valid range and if the spot is currently empty (not 'X' or 'O').
    *   **Returns**: `true` if available, `false` otherwise.
*   **`move(char* board, int position, char XorO)`**:
    *   **Purpose**: Places a marker (`XorO`) onto the board at the given `position`.
    *   **Mechanism**: Uses pointer arithmetic (`*(board + position) = XorO;`) for efficient assignment.
*   **`RESET(char board[9])`**:
    *   **Purpose**: Resets the game board to numbered dots (1-9) before a new game begins.
    *   **Mechanism**: Sets each board slot to the corresponding character '1' through '9'.
*   **`addToHistory(int position, char player)`**:
    *   **Purpose**: Records a move (position and player) into the game history array.
    *   **Mechanism**: Increments `moveCount` and stores the data, up to 9 moves.
*   **`showhistory()`**:
    *   **Purpose**: Prints the sequence of moves for the current game.
    *   **Mechanism**: Iterates through the `moveCount` and prints the move number, player, and spot number.

## 🎲 AI & Move Selection Functions

*   **`getrandom()`**:
    *   **Purpose**: Generates a random, available move (0-8).
    *   **Mechanism**: Uses `random_device` and `mt19937` to ensure randomness within the 0-8 range.
*   **`AIrandom(char board[9])`**:
    *   **Purpose**: Implements the Easy difficulty AI.
    *   **Mechanism**: Repeatedly calls `getrandom()` until an available spot is found.
*   **`AIbestmove(char board[9], char aiplayer, char player)`**:
    *   **Purpose**: Implements the Hard difficulty AI (one-move lookahead).
    *   **Mechanism**: First, checks if a winning move exists. Second, checks if a defensive move (blocking the opponent's win) exists. Third, chooses the center (4), then a corner, and finally a random open spot.
*   **`minimax(char board[9], int depth, bool isMax, char aiplayer)`**:
    *   **Purpose**: Recursive function for the Impossible difficulty AI. Evaluates moves based on win/loss/draw scores.
    *   **Mechanism**: Implements the core minimax algorithm, alternating between maximizing (AI) and minimizing (Opponent) scores.
*   **`bestMove(char board[9], char aiplayer)`**:
    *   **Purpose**: Uses `minimax` to determine the optimal move.
    *   **Mechanism**: Iterates through all available moves, calls `minimax` for each, and returns the move that yields the highest score.

## 💾 Statistics & I/O Functions

*   **`saveOneStats(ofstream& file, string prefix, const Stats& s)`**:
    *   **Purpose**: Writes individual stats (games, wins, losses, draws) for a specific mode (`prefix`) to the `stats.txt` file.
*   **`savestats()`**:
    *   **Purpose**: Coordinates the saving of *all* stats (total and per mode) to `stats.txt`.
    *   **Mechanism**: Opens `stats.txt` and calls `saveOneStats` for `all`, `pvp`, `pvai`, and `aivai`.
*   **`loadstats()`**:
    *   **Purpose**: Reads `stats.txt` and populates the global `stats` and `modeStats` arrays.
    *   **Mechanism**: Iterates through the file, reading `label` and `value` pairs, handling both modern and legacy file formats.
*   **`printOneStats(string label, const Stats* s)`**:
    *   **Purpose**: Prints a single stats record in a readable format, including a calculated win rate.
*   **`resetstats()`**:
    *   **Purpose**: Prompts the user and, upon confirmation, resets all global and mode statistics to zero, then saves the reset state.
*   **`showstats()`**:
    *   **Purpose**: Displays all collected stats (Total, PVP, PVAI, AIVAI) and prompts the user to reset.

## 🕹️ Game Mode Functions

*   **`playPvP(char board[9], string p1, string p2)`**:
    *   **Purpose**: Drives the Player vs Player game loop.
    *   **Mechanism**: Alternates turns between P1 and P2, calling `getValidMoveUser` and checking for win/draw conditions. Updates PVP stats upon completion.
*   **`playPvAI(char board[9], string name, char playerLetter, int difficulty)`**:
    *   **Purpose**: Drives the Player vs AI game loop.
    *   **Mechanism**: Alternates turns between the human player and the AI, using the specified `difficulty` (1, 2, or 3) to choose the AI move. Updates PVAI stats.
*   **`playAIvAI(char board[9], int difficulty1, int difficulty2)`**:
    *   **Purpose**: Drives the AI vs AI game loop.
    *   **Mechanism**: Alternates turns between 'X' (difficulty1) and 'O' (difficulty2), calling the appropriate AI function. Updates AIVAI stats.

## 🔢 Utility Functions

*   **`getValidMoveUser(char board[9])`**:
    *   **Purpose**: Prompts the user for a move (1-9) and validates input (digit, range, availability).
    *   **Mechanism**: Uses a `while(true)` loop with `try-catch` blocks to handle input errors (non-digit, out of range, occupied spot) until a valid spot is returned.
*   **`getValidNumber(string prompt, int minimum, int maximum)`**:
    *   **Purpose**: General utility to prompt the user for a number within a specified range.
    *   **Mechanism**: Reads input, checks if it's a whole number and if it falls within the required bounds.
*   **`main()`**:
    *   **Purpose**: The entry point of the application.
    *   **Mechanism**: Loads stats, enters a `do-while` menu loop, prompts the user for a game choice (1-5), and delegates execution to the respective `play*` functions.