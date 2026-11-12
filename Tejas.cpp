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

//TODO: MAKE SURE TO INCLUDE `PUBLIC`
//TODO: Make the movement into a class
//TODO: Use pointers
//TODO: Use Structures

// stats
int wins = 0;
int losses = 0;
int draws = 0;
int games = 0;

// keeps track of the moves in the current game

class Move {
    public:
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
        int choice = getValidMoveUser(board);
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
            int choice = getValidMoveUser(board);
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
//TODO: Add options for AI vs. AI function. 
void playAIvAI(char board[9]) {
    RESET(board);
    moveCount = 0;
    char current = 'X';
    while (true) {
        printboard(board);
        cout << "AI " << current << " is thinking..." << endl;
        char opponent;
        int choice1;
        int choice2;
        int difficulty1;
        int difficulty2;
        while (difficulty1 == 1 || difficulty1 == 2 || difficulty1 == 3) {
            cout << "for Player 1 what diffcultly do you want for them? (1 for easy, 2 for hard, 3 for impossible)";
            cin >> difficulty1;
            if (difficulty1 == 1)
                choice1 = AIrandom(board);
            if (difficulty1 == 2)
                choice1 = AIbestmove(board, current, opponent);
            if (difficulty1 == 3)
            choice1 = bestMove(board, current);
            else
                cout << "please input a 1, 2, or 3 for easy, hard or impossible";
        }
        while (difficulty2 == 1 || difficulty2 == 2 || difficulty2 == 3) {
            cout << "for Player 2 what diffcultly do you want for them? (1 for easy, 2 for hard, 3 for impossible)";
            cin >> difficulty2;
            if (difficulty2 == 1)
                choice1 = AIrandom(board);
            if (difficulty2 == 2)
                choice1 = AIbestmove(board, current, opponent);
            if (difficulty2 == 3)
            choice1 = bestMove(board, current);
            else
                cout << "please input a 1, 2, or 3 for easy, hard or impossible";
        }
        move(board, choice1, current);
        addToHistory(choice1, current);
        cout << "AI " << current << " picks spot " << 3 << choice1 + 1 << endl;
        if (checkwin(board, current)) {
            printboard(board);
            cout << "AI " << current << " wins!" << endl;
            if (current == 'X' || current == 'O') {
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
//TODO: Add exception handling
//TODO: Fix the menu options
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
			cout << endl;
            cout << "TIC TAC TOE" << endl;
            cout << "===========" << endl;
            cout << "Wins: " << wins << "  Losses: " << losses << "  Draws: " << draws << endl;
            cout << endl;
            cout << "1. Player vs Player" << endl;
            cout << "2. Player vs AI" << endl;
            cout << "3. AI vs AI" << endl;
            cout << "4. Stats" << endl;
            cout << "5. Quit" << endl;
			std::cin >> gamechoice;
		} while (!(gamechoice == 1) && !(gamechoice == 2) && !(gamechoice == 3) && !(gamechoice == 4) && !(gamechoice == 5));
		//do-while to pick your letter
		if(gamechoice == 1){
            // Player vs Player
        }
        if (gamechoice == 2){
            do {
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
        }
        if (gamechoice == 3){
            //AI vs AI
            playAIvAI(board);
        }
        if (gamechoice == 4){
            //Game Stats
        }
        if (gamechoice == 5){
            return 0;
        }
	return 0;
}