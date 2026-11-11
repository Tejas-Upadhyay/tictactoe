// Names: Philip Walzack, Tejas Upadhyay
// Course: Programming 1
// Description: TicTacToe game that ultiizes MiniMax to become unbeatable.

#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <random>
using namespace std;

// tic tac toe

// stats
int wins = 0;
int losses = 0;
int draws = 0;
int games = 0;

// keeps track of the moves in the current game
struct Move {
    int position;
    char player;
};

Move history[9];
int moveCount = 0;

//function checks win state for either player or AI
bool checkwin(char board[9], char XorO) {
	//Rows
	if (board[0] == XorO && board[1] == XorO && board[2] == XorO) {
		return true;
	}
	if (board[3] == XorO && board[4] == XorO && board[5] == XorO) {
		return true;
	}
	if (board[6] == XorO && board[7] == XorO && board[8] == XorO) {
		return true;
	}
	//Columns
	if (board[0] == XorO && board[3] == XorO && board[6] == XorO) {
		return true;
	}
	if (board[1] == XorO && board[4] == XorO && board[7] == XorO) {
		return true;
	}
	if (board[2] == XorO && board[5] == XorO && board[8] == XorO) {
		return true;
	}
	//Diagonal
	if (board[0] == XorO && board[4] == XorO && board[8] == XorO) {
		return true;
	}
	if (board[2] == XorO && board[4] == XorO && board[6] == XorO) {
		return true;
	}
	return false;
}
//checks for draw
bool checkdraw(char board[9]) {
	for (int i = 0; i < 9; i++) {
		if (board[i] != 'O' && board[i] != 'X') {
			return false;
		}
	}
	return true;
}
//function prints the current state of the board
void printboard(const char board[9]) {
	for (int j = 0; j < 3; j++) {
		for (int i = 0; i < 3; i++) {
			std::cout << board[j * 3 + i] << " ";
		}
		std::cout << std::endl;
	}
}
//function to check if a spot is available
bool availability(char board[9], int position){
	if (board[position] == 'O' || board[position] == 'X') {
		return false;
		}
	else {
		return true;
		}
	}
//function to simplify movement of players
void move(char board[9], int position, char XorO) {
	board[position] = XorO;
}
//function that resets the board CAREFUL WITH THIS ONE!!!
void RESET(char board[9]) {
	for (int i = 0; i < 9; i++) {
		board[i] = 1 + i + '0';
	}
}
void addToHistory(int position, char player){
	if (moveCount < 9) {
        history[moveCount].position = position;
        history[moveCount].player = player;
        moveCount++;
    }
}
void showhistory() {
    cout << "Moves this game:" << endl;
    if (moveCount == 0) {
        cout << "  (none)" << endl;
    }
    for (int i = 0; i < moveCount; i++) {
        cout << "  " << i + 1 << ". " << history[i].player
             << " took spot " << history[i].position + 1 << endl;
    }
    cout << endl;
}
//function that gets a valid move from the user
int getValidMoveUser(char board[9]) {
	int choice;
	do {
		std::cout << "Pick a spot: ";
		std::cin >> choice;
		choice--;
	} while (!(choice >= 0) || !(choice <= 8) || !availability(board, choice));
	return choice;
}
//Random function
int getrandom() {
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> random(0, 8);
	return random(gen);
}
//Random based AI; Easy Mode
int AIrandom(char board[9]) {
	int i;
	do {
		i = getrandom();
	} while (!availability(board, i));
	return i;
}
//One move ahead AI: Hard Mode
int AIbestmove(char board[9], char aiplayer, char player) {
		for (int i = 0; i < 9; i++) {
			if (availability(board, i)) {
				char original = board[i];
				move(board, i, aiplayer);
				if (checkwin(board, aiplayer)) {
					board[i] = original;
					return i;
				}
				board[i] = original;
			}
		}
		for (int i = 0; i < 9; i++) {
			if (availability(board, i)) {
				char original = board[i];
				move(board, i, player);
				if (checkwin(board, player)) {
					board[i] = original;
					return i;
				}
				board[i] = original;
			}
		}
		if (availability(board, 4)) {
			return 4;
		}
		else if (availability(board, 0) || availability(board, 2) || availability(board, 6) || availability(board, 8)){
			int i;
			do {
				i = getrandom();
			} while (!availability(board, i) || (!(i == 0) && !(i == 2) && !(i == 6) && !(i == 8)));
			return i;
		}
		else {
			int i;
			do {
				i = getrandom();
			} while (!availability(board, i));
			return i;
		}
	}
    //minimax for impossible mode
int minimax(char board[9], int depth, bool isMax, char aiplayer) {
    char opponent = 'X';
    if (aiplayer == 'X') {
        opponent = 'O';
    }
    if (checkwin(board, aiplayer)) {
        return 10 - depth;
    }
    if (checkwin(board, opponent)) {
        return depth - 10;
    }
    if (checkdraw(board)) {
        return 0;
    }
    if (isMax) {
        int best = -1000;
        for (int i = 0; i < 9; i++) {
            if (availability(board, i)) {
                char original = board[i];
                board[i] = aiplayer;
                int score = minimax(board, depth + 1, false, aiplayer);
                board[i] = original;
                if (score > best) {
                    best = score;
                }
            }
        }
        return best;
    }else {
        // now pretend the other player moves, they want the lowest score
        int best = 1000;
        for (int i = 0; i < 9; i++) {
            if (availability(board, i)) {
                char original = board[i];
                board[i] = opponent;
                int score = minimax(board, depth + 1, true, aiplayer);
                board[i] = original;
                if (score < best) {
                    best = score;
                }
            }
        }
        return best;
    }
}
//finds the best move using minimax
int bestMove(char board[9], char aiplayer) {
    int bestVal = -1000;
    int best = -1;

    // try every empty square and see what score it gives
    for (int i = 0; i < 9; i++) {
        if (availability(board, i)) {
            char original = board[i];
            board[i] = aiplayer;
            int value = minimax(board, 0, false, aiplayer);
            board[i] = original;

            // cout << "move " << i + 1 << " scored " << value << endl;
            if (value > bestVal) {
                bestVal = value;
                best = i;
            }
        }
    }
    return best;
}
void savestats() {
    // just overwrite the file every time
    ofstream file("stats.txt");
    if (file.is_open()) {
        file << "games " << games << endl;
        file << "wins " << wins << endl;
        file << "losses " << losses << endl;
        file << "draws " << draws << endl;
        file.close();
    }
}
void loadstats() {
    ifstream file("stats.txt");
    if (!file.is_open()) {
        return; // first run, nothing saved yet
    }

    string label;
    int value;
    while (file >> label >> value) {
        if (label == "games") {
            games = value;
        }
        else if (label == "wins") {
            wins = value;
        }
        else if (label == "losses") {
            losses = value;
        }
        else if (label == "draws") {
            draws = value;
        }
    }
    file.close();
}
void showstats() {
    cout << endl;
    cout << "Games: " << games << endl;
    cout << "Wins: " << wins << endl;
    cout << "Losses: " << losses << endl;
    cout << "Draws: " << draws << endl;

    if (games > 0) {
        cout << "Win rate: " << (wins * 100 / games) << "%" << endl;
    }

    cout << endl << "Press enter to go back...";
    cin.ignore(1000, '\n');
    cin.get();
}
void playPvP(char board[9], string p1, string p2) {
    RESET(board);
    moveCount = 0;
    char current = 'X';
    string currentName = p1;
    while (true) {
        printboard(board);
        cout << currentName << " (" << current << ") your turn" << endl;
        int choice = getValidMove(board);
        move(board, choice, current);
        addToHistory(choice, current);
        if (checkwin(board, current)) {
            printboard(board);
            cout << currentName << " wins!" << endl;
            if (current == 'X') {
                wins++;
            }else {
                losses++;
            }
            games++;
            savestats();
            showhistory();
            return;
        }
        if (checkdraw(board)) {
            printboard(board);
            cout << "It's a draw." << endl;
            draws++;
            games++;
            savestats();
            showhistory();
            return;
        }
        if (current == 'X') {
            current = 'O';
            currentName = p2;
        }
        else {
            current = 'X';
            currentName = p1;
        }
    }
}
void playPvAI(char board[9], string name, char playerLetter, int difficulty) {
    RESET(board);
    moveCount = 0;
    char human = playerLetter;
    char aiplayer;
    if (human == 'X') {
        aiplayer = 'O';
    }
    else {
        aiplayer = 'X';
    }
	// X always goes first
    bool humanTurn = false;
    if (human == 'X') {
        humanTurn = true;
    }
	while (true) {
        printboard(board);

        if (humanTurn) {
            cout << name << " (" << human << ") your turn" << endl;
            int choice = getValidMove(board);
            move(board, choice, human);
            addToHistory(choice, human);
            if (checkwin(board, human)) {
                printboard(board);
                cout << "You win!" << endl;
                wins++;
                games++;
                savestats();
                showhistory();
                return;
            }
            if (checkdraw(board)) {
                printboard(board);
                cout << "It's a draw." << endl;
                draws++;
                games++;
                savestats();
                showhistory();
                return;
            }
            humanTurn = false;
        }
        else {
            int aichoice;
            if (difficulty == 1) {
                aichoice = AIrandom(board);
            }if (difficulty == 2) {
                aichoice = AIbestmove(board, aiplayer, human);
            }
            else {
                aichoice = bestMove(board, aiplayer);
            }

            move(board, aichoice, aiplayer);
            addToHistory(aichoice, aiplayer);
            cout << "AI picks spot " << aichoice + 1 << endl;

            if (checkwin(board, aiplayer)) {
                printboard(board);
                cout << "AI wins, you lost." << endl;
                losses++;
                games++;
                savestats();
                showhistory();
                return;
            }
            if (checkdraw(board)) {
                printboard(board);
                cout << "It's a draw." << endl;
                draws++;
                games++;
                savestats();
                showhistory();
                return;
            }
            humanTurn = true;
        }
    }
}
void playAIvAI(char board[9], int difficulty) {
    RESET(board);
    moveCount = 0;
    char current = 'X';
    while (true) {
        printboard(board);
        cout << "AI " << current << " is thinking..." << endl;
        char opponent;
        if (current == 'X') {
            opponent = 'O';
        }else {
            opponent = 'X';
        }
        int choice;
        if (difficulty == 1) {
            choice = AIrandom(board);
        }if (difficulty == 2) {
            choice = AIbestmove(board, current, opponent);
        }else {
            choice = bestMove(board, current);
        }
        move(board, choice, current);
        addToHistory(choice, current);
        cout << "AI " << current << " picks spot " << choice + 1 << endl;
        if (checkwin(board, current)) {
            printboard(board);
            cout << "AI " << current << " wins!" << endl;
            if (current == 'X') {
                wins++;
            }
            else {
                losses++;
            }
            games++;
            savestats();
            showhistory();
            return;
        }
        if (checkdraw(board)) {
            printboard(board);
            cout << "It's a draw." << endl;
            draws++;
            games++;
            savestats();
            showhistory();
            return;
        }
        if (current == 'X') {
            current = 'O';
        }else {
            current = 'X';
        }
    }
}
int main() {
	//do-while loop for the main game
	char again;
	do {
		//initializes board and player
		char board[9];
		RESET(board);
		char player;
		char aiplayer = ' ';
		int choice;
		int aichoice;
		int gamechoice = 0;
		//do-while to pick difficulty
		do {
			std::cout << "Pick your opponent: Easy (1), Hard (2), Impossible (3): ";
			std::cin >> gamechoice;
		} while (!(gamechoice == 1) && !(gamechoice == 2));
		//do-while to pick your letter
		do {
			std::cout << "Pick O or X: ";
			std::cin >> player;
			player = toupper(player);
		} while (!(player == 'O') && !(player == 'X'));
		if (player == 'O') {
			aiplayer = 'X';
		}
		else if (player == 'X') {
			aiplayer = 'O';
		}
		printboard(board);
		//EASY     while loop that is the main game function
		if (gamechoice == 1) {
			while (true) {
				//player move
				choice = getValidMoveUser(board);
				move(board, choice, player);
				printboard(board);
				if (checkwin(board, player) || checkwin(board, aiplayer) || checkdraw(board)) {
					break;
				}
				//Ai Best Move
				aichoice = AIrandom(board);
				std::cout << "Ai moved to: " << aichoice + 1 << std::endl;
				move(board, aichoice, aiplayer);
				printboard(board);
				if (checkwin(board, player) || checkwin(board, aiplayer) || checkdraw(board)) {
					break;
				}
			}
		}
		//HARD     while loop that is the main game function
		else if (gamechoice == 2) {
			while (true) {
				//player move
				choice = getValidMoveUser(board);
				move(board, choice, player);
				printboard(board);
				if (checkwin(board, player) || checkwin(board, aiplayer) || checkdraw(board)) {
					break;
				}
				//Ai Best Move
				aichoice = AIbestmove(board, aiplayer, player);
				std::cout << "Ai moved to: " << aichoice + 1 << std::endl;
				move(board, aichoice, aiplayer);
				printboard(board);
				if (checkwin(board, player) || checkwin(board, aiplayer) || checkdraw(board)) {
					break;
				}
			}
		}
		//HARD     while loop that is the main game function
		else if (gamechoice == 3) {
			while (true) {
				//player move
				choice = getValidMoveUser(board);
				move(board, choice, player);
				printboard(board);
				if (checkwin(board, player) || checkwin(board, aiplayer) || checkdraw(board)) {
					break;
				}
				//Ai Best Move
				aichoice = minimax(board, aiplayer, player);
				std::cout << "Ai moved to: " << aichoice + 1 << std::endl;
				move(board, aichoice, aiplayer);
				printboard(board);
				if (checkwin(board, player) || checkwin(board, aiplayer) || checkdraw(board)) {
					break;
				}
			}
		}
		//if statements checking for win condition to print
		if (checkwin(board, player)) {
			std::cout << "You win!" << std::endl;
			printboard(board);
		}
		else if (checkdraw(board)) {
			std::cout << "Draw..." << std::endl;
			printboard(board);
		}
		else {
			std::cout << "You lost!" << std::endl;
			printboard(board);
		}
		std::cout << "Go again? (Y/N): ";
		std::cin >> again;
		again = toupper(again);
	} while (again == 'Y');
	return 0;
}