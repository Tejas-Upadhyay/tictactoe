#include <iostream>
#include <random>
#include <fstream>
#include <string>
#include <cstdlib>
#include <ctime>

using namespace std;

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
			cout << board[j * 3 + i] << " ";
		}
		cout << endl;
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
//function that gets a valid move from the user
int getValidMoveUser(char board[9]) {
	int choice;
	do {
		cout << "Pick a spot: ";
		cin >> choice;
		choice--;
	} while (!(choice >= 0) || !(choice <= 8) || !availability(board, choice));
	return choice;
}
//Random function
int getrandom() {
	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<> random(0, 8);
	return random(gen);
}
//Random based AI; easy mode
int AIrandom(char board[9]) {
	int i;
	do {
		i = getrandom();
	} while (!availability(board, i));
	return i;
}
//One move ahead AI; hard mode
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
        //block the player if they are about to win
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
        //if there is nothing to win or block, go for the middle
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
    //minimax; impossible mode
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
            for (int i = 0; i < 9; i++){
                if(availability(board, i)) {
                    char original = board[i];
                    move(board, i, aiplayer);
                    int score = minimax(board, depth + 1, false, aiplayer);
                    board[i] = original;
                    if (score > best) {
                        best = score;
                    }
                }
            }
            return best;
        }
        else {
            int best = 1000;
            for (int i = 0; i < 9; i++){
                if(availability(board, i)) {
                    char original = board[i];
                    move(board, i, aiplayer);
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
    //Stats
    int wins = 0;
    int losses = 0;
    int draws = 0;
    void saveStats(char board[9], char player, char aiplayer) {
        ofstream file("stats.txt");
        if (checkwin(board, player)){
            wins++;
        }
        else if (checkwin(board, aiplayer)){
            losses++;
        }
        else if (checkdraw(board)){
            draws++;
        }
        file << "Wins: " << wins << endl;
        file << "Losses: " << losses << endl;
        file << "Draws: " << draws << endl;
        file.close();
    }
    void loadStats(){
        ifstream file("stats.txt");
        if (!file.is_open()){
            cout << "No stats file found, starting fresh." << endl;
            return;
        }
        string line;
        int value;
        while (getline(file, line)) {
            if (line.find("Wins:") != string::npos) {
                value = stoi(line.substr(line.find(":") + 1));
                wins = value;
            }
            else if (line.find("Losses:") != string::npos) {
                value = stoi(line.substr(line.find(":") + 1));
                losses = value;
            }
            else if (line.find("Draws:") != string::npos) {
                value = stoi(line.substr(line.find(":") + 1));
                draws = value;
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
			cout << "Pick your opponent: Easy (1), Hard (2), Impossible (3) DOES NOT EXIST: ";
			cin >> gamechoice;
		} while (!(gamechoice == 1) && !(gamechoice == 2));
		//do-while to pick your letter
		do {
			cout << "Pick O or X: ";
			cin >> player;
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
				cout << "Ai moved to: " << aichoice + 1 << endl;
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
				cout << "Ai moved to: " << aichoice + 1 << endl;
				move(board, aichoice, aiplayer);
				printboard(board);
				if (checkwin(board, player) || checkwin(board, aiplayer) || checkdraw(board)) {
					break;
				}
			}
		}
		//if statements checking for win condition to print
		if (checkwin(board, player)) {
			cout << "You win!" << endl;
			printboard(board);
		}
		else if (checkdraw(board)) {
			cout << "Draw..." << endl;
			printboard(board);
		}
		else {
			cout << "You lost!" << endl;
			printboard(board);
		}
		saveStats(board, player, aiplayer);
		cout << "Go again? (Y/N): ";
		cin >> again;
		again = toupper(again);
	} while (again == 'Y');
	return 0;
}