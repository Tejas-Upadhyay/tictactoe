# Tic-Tac-Toe

A terminal Tic-Tac-Toe game written in C++ for Programming 1 by Philip Walzack and Tejas Upadhyay. Play against a friend, challenge the computer, or watch two AIs compete.

## Features

- **Player vs Player:** Two people take turns on the same terminal.
- **Player vs AI:** Choose X or O and one of three difficulty levels.
- **AI vs AI:** Select a difficulty for each AI and watch the game play out.
- Move history displayed after each game.
- Saved overall and per-mode statistics, including games, wins, losses, draws, and win rate.

## Build and run

You need a C++ compiler with C++11 support or newer, such as GCC or Clang. The game uses only the C++ standard library.

From the project directory on macOS or Linux:

```sh
g++ -std=c++11 -Wall -Wextra Main.cpp -o tictactoe
./tictactoe
```

On Windows with MinGW installed, build and run from PowerShell:

```powershell
g++ -std=c++11 -Wall -Wextra Main.cpp -o tictactoe.exe
.\tictactoe.exe
```

## How to play

Choose an option from the main menu:

```text
1. Player vs Player
2. Player vs AI
3. AI vs AI
4. Stats
5. Quit
```

Enter player names as single words. In Player vs Player, the first player is X. In Player vs AI, you choose your symbol. **X always moves first.**

On your turn, enter the number of an available square:

```text
1 2 3
4 5 6
7 8 9
```

Get three of your marks in a row, column, or diagonal to win. A full board with no winner is a draw. After each game, the move history is shown and you return to the main menu.

## AI difficulty

| Level | Behavior |
| --- | --- |
| Easy (1) | Chooses a random available square. |
| Hard (2) | Takes an immediate win or blocks an immediate loss, then favors the center and corners. |
| Impossible (3) | Uses minimax to search possible outcomes and choose an optimal move. |

## Statistics

Results are saved automatically after each completed game to `stats.txt` in the directory where you run the program, and loaded the next time it starts there.

- **Player vs AI:** Wins and losses are recorded from the human player's perspective.
- **Player vs Player and AI vs AI:** Wins and losses are recorded from X's perspective.
- **All Modes:** Combines the results from all three modes using those conventions.

Choose **Stats** to view the records. Press **R**, then confirm with **Y**, to reset all saved statistics.

## Source

All game logic, AI strategies, input handling, and statistics storage are in [Main.cpp](Main.cpp).
