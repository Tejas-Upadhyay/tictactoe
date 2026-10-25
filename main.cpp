#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <ctime>

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
    //rows
    if (board[0] == XorO && board[1] == XorO && board[2] == XorO) {
        return true;
    }
    if (board[3] == XorO && board[4] == XorO && board[5] == XorO) {
        return true;
    }
    if (board[6] == XorO && board[7] == XorO && board[8] == XorO) {
        return true;
    }
    //columns
    if (board[0] == XorO && board[3] == XorO && board[6] == XorO) {
        return true;
    }
    if (board[1] == XorO && board[4] == XorO && board[7] == XorO) {
        return true;
    }
    if (board[2] == XorO && board[5] == XorO && board[8] == XorO) {
        return true;
    }
    //diagonals
    if (board[0] == XorO && board[4] == XorO && board[8] == XorO) {
        return true;
    }
    if (board[2] == XorO && board[4] == XorO && board[6] == XorO) {
        return true;
    }
    return false;
}

//checks for a draw
bool checkdraw(char board[9]) {
    for (int i = 0; i < 9; i++) {
        if (board[i] != 'X' && board[i] != 'O') {
            return false;
        }
    }
    return true;
}

//prints the board
void printboard(const char board[9]) {
    cout << endl;
    for (int j = 0; j < 3; j++) {
        for (int i = 0; i < 3; i++) {
            cout << board[j * 3 + i];
            if (i < 2) {
                cout << " | ";
            }
        }
        cout << endl;
        if (j < 2) {
            cout << "--+---+--" << endl;
        }
    }
    cout << endl;
}

//checks if a spot is free
bool available(char board[9], int position){
    if (position < 0 || position > 8) {
        return false;
    }
    if (board[position] == 'X' || board[position] == 'O') {
        return false;
    }
    return true;
}

void move(char board[9], int position, char XorO) {
    board[position] = XorO;
}

//function that resets the board CAREFUL WITH THIS ONE!!!
void RESET(char board[9]) {
    for (int i = 0; i < 9; i++) {
        board[i] = 1 + i + '0';
    }
}

void addToHistory(int position, char player) {
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

//gets a valid move from the user
int getValidMove(char board[9]) {
    int choice;
    while (true) {
        cout << "Pick a spot (1-9): ";
        cin >> choice;

        if (cin.eof()) {
            cout << endl << "No more input, exiting." << endl;
            exit(0);
        }
        if (cin.fail()) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Numbers only please." << endl;
            continue;
        }

        choice = choice - 1;
        if (choice < 0 || choice > 8) {
            cout << "Pick a number from 1 to 9." << endl;
        }
        else if (!available(board, choice)) {
            cout << "That spot is already taken." << endl;
        }
        else {
            return choice;
        }
    }
}

//gives a number from 0 to 8
int getrandom() {
    return rand() % 9;
}

//random ai, easy mode
int AIrandom(char board[9]) {
    int i;
    do {
        i = getrandom();
    } while (!available(board, i));
    return i;
}

//one move ahead ai, hard mode
int AIbestmove(char board[9], char aiplayer, char player) {
    for (int i = 0; i < 9; i++) {
        if (available(board, i)) {
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
        if (available(board, i)) {
            char original = board[i];
            move(board, i, player);
            if (checkwin(board, player)) {
                board[i] = original;
                return i;
            }
            board[i] = original;
        }
    }
    // nothing to win or block so go for the middle
    if (available(board, 4)) {
        return 4;
    }
    if (available(board, 0) || available(board, 2) ||
        available(board, 6) || available(board, 8)) {
        int i;
        do {
            i = getrandom();
        } while (!available(board, i) ||
                 (!(i == 0) && !(i == 2) && !(i == 6) && !(i == 8)));
        return i;
    }
    int i;
    do {
        i = getrandom();
    } while (!available(board, i));
    return i;
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
            if (available(board, i)) {
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
    }
    else {
        // now pretend the other player moves, they want the lowest score
        int best = 1000;
        for (int i = 0; i < 9; i++) {
            if (available(board, i)) {
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
        if (available(board, i)) {
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
    // TODO: maybe let the player choose who starts?
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
            }
            else if (difficulty == 2) {
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
        }
        else {
            opponent = 'X';
        }

        int choice;
        if (difficulty == 1) {
            choice = AIrandom(board);
        }
        else if (difficulty == 2) {
            choice = AIbestmove(board, current, opponent);
        }
        else {
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
        }
        else {
            current = 'X';
        }
    }
}

int main() {
    srand(time(0));
    loadstats();

    char board[9];
    char again;

    do {
        cout << endl;
        cout << "TIC TAC TOE" << endl;
        cout << "===========" << endl;
        cout << "Wins: " << wins << "  Losses: " << losses
             << "  Draws: " << draws << endl;
        cout << endl;
        cout << "1. Player vs Player" << endl;
        cout << "2. Player vs AI" << endl;
        cout << "3. AI vs AI" << endl;
        cout << "4. Stats" << endl;
        cout << "5. Quit" << endl;

        int mode;
        cout << "Choice: ";
        cin >> mode;
        while (cin.fail() || mode < 1 || mode > 5) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Enter a number from 1 to 5: ";
            cin >> mode;
        }

        if (mode == 4) {
            showstats();
            again = 'y';
            continue;
        }

        if (mode == 5) {
            break;
        }

        if (mode == 1) {
            string p1;
            string p2;
            cout << "Player 1 name: ";
            cin >> p1;
            cout << "Player 2 name: ";
            cin >> p2;
            playPvP(board, p1, p2);
        }
        else if (mode == 2) {
            string name;
            cout << "Your name: ";
            cin >> name;

            char letter;
            cout << "Play as X or O? ";
            cin >> letter;
            letter = toupper(letter);
            while (letter != 'X' && letter != 'O') {
                cout << "Just X or O: ";
                cin >> letter;
                letter = toupper(letter);
            }

            cout << endl;
            cout << "1. Easy" << endl;
            cout << "2. Hard" << endl;
            cout << "3. Impossible" << endl;
            int difficulty;
            cout << "Difficulty: ";
            cin >> difficulty;
            while (cin.fail() || difficulty < 1 || difficulty > 3) {
                cin.clear();
                cin.ignore(1000, '\n');
                cout << "Enter 1, 2 or 3: ";
                cin >> difficulty;
            }

            playPvAI(board, name, letter, difficulty);
        }
        else {
            string name;
            cout << "Your name: ";
            cin >> name;

            cout << endl;
            cout << "1. Easy" << endl;
            cout << "2. Hard" << endl;
            cout << "3. Impossible" << endl;
            int difficulty;
            cout << "Difficulty: ";
            cin >> difficulty;
            while (cin.fail() || difficulty < 1 || difficulty > 3) {
                cin.clear();
                cin.ignore(1000, '\n');
                cout << "Enter 1, 2 or 3: ";
                cin >> difficulty;
            }

            playAIvAI(board, difficulty);
        }

        cout << "Play again? (y/n): ";
        cin >> again;
        again = toupper(again);

    } while (again == 'Y');

    savestats();
    cout << endl << "Thanks for playing." << endl;

    return 0;
}
