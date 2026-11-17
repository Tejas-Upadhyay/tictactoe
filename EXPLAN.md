# Main.cpp Explanation (line by line)
The `struct Stats` groups the wins, losses, draws, and total games into one record. Each counter starts at 0 and can change as games are played.

We use `const` for values that should stay the same while the program runs, such as the number of game modes and the numbers that identify each mode and result.

`NUM_MODES`, `PVP`, `PVAI`, and `AIVAI` define the number of game modes and identify each mode's statistics record, allowing us to create the statistics array and access the correct record when updating, saving, loading, and displaying statistics.

`WIN`, `LOSS`, and `DRAW` identify the possible game results so `updateStats` knows which counter to increase.

The function `checkwin` goes through all possible ways of winning the game TicTacToe and if even one of `if` statments happen to be true then the game is over

`checkdraw` checks if there is any more empty postions for 