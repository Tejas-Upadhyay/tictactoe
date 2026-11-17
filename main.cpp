//Names: Philip Walzack, Tejas Upadhyay
//Course: Programming 1
//Description: A Tic-Tac-Toe game with three modes. Impossible difficulty uses minimax AI to find the best possible move on any possible board.

#include <iostream>
#include <fstream>
#include <string>
#include <cctype>
#include <cstdlib>
#include <random>
#include <sstream>
#include <stdexcept>
using namespace std;

//tic tac toe

//stats
struct Stats {
    int wins = 0;
    int losses = 0;
    int draws = 0;
    int games = 0;
};

//which stats record goes with which mode
const int NUM_MODES = 3;
const int PVP = 0;
const int PVAI = 1;
const int AIVAI = 2;

//Game Stats
const int WIN = 0;
const int LOSS = 1;
const int DRAW = 2;

Stats stats; //all modes added together
Stats modeStats[NUM_MODES]; //one record for each mode

//keeps track of the moves in the current game
class Move {
public:
    int position;
    char player;
};
Move history[9];
int moveCount = 0;

//function checks win state for either player or AI
bool checkwin(char board[9], char XorO) { // Philip
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

//Checks if the board is full. We check for a win first.
bool checkdraw(char board[9]) { //philip
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
bool availability(char board[9], int position) {
    if (position < 0 || position > 8) {
        return false;
    }
    if (board[position] == 'O' || board[position] == 'X') {
        return false;
    }
    return true;
}

//function to simplify movement of players
void move(char* board, int position, char XorO) {
    //same as board[position] = XorO
    *(board + position) = XorO;
}

//Put the numbers 1 through 9 back on the board.
void RESET(char board[9]) {
    for (int i = 0; i < 9; i++) {
        board[i] = 1 + i + '0';
    }
}

//save each move so we can show them after the game
void addToHistory(int position, char player) {
    if (moveCount < 9) {
        history[moveCount].position = position;
        history[moveCount].player = player;
        moveCount++;
    }
}

//prints the moves from this game
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

//updates the total stats and the stats for this mode
void updateStats(int mode, int result) {
    stats.games++;
    modeStats[mode].games++;
    if (result == WIN) {
        stats.wins++;
        modeStats[mode].wins++;
    }
    else if (result == LOSS) {
        stats.losses++;
        modeStats[mode].losses++;
    }
    else {
        stats.draws++;
        modeStats[mode].draws++;
    }
}

//function that gets a valid move from the user
int getValidMoveUser(char board[9]) {
    int choice;
    string input;
    while (true) {
        cout << "Pick a spot (1-9): ";
        try {
            if (!(cin >> input)) {
                if (cin.eof()) {
                    cout << "\nInput ended. Exiting." << endl;
                    exit(0);
                }
                throw "Invalid input";
            }
            //Checks that every character is a digit
            //Range based loop that iterates and automatically inserts values into c to check for invalidity
            for (char c : input) {
                if (!isdigit(c)) {
                    throw "Invalid input";
                }
            }
            //converts the string input into integer for use
            choice = stoi(input);
            if (choice < 1 || choice > 9) {
                throw "Out of range";
            }
            if (!availability(board, choice - 1)) {
                throw "Position occupied";
            }
            return choice - 1;
        }
        //identifies error and prints appropriate response
        catch (const char* error) {
            if (strcmp(error, "Invalid input") == 0) {
                cout << "Please enter a whole number." << endl;
            }
            else if (strcmp(error, "Out of range") == 0) {
                cout << "Pick a number from 1 to 9." << endl;
            }
            else if (strcmp(error, "Position occupied") == 0) {
                cout << "That spot is already taken." << endl;
            }
        }
    }
}

//Random function
int getrandom() {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> random(0, 8);
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

//One move ahead AI; hard mode
int AIbestmove(char board[9], char aiplayer, char player) {
    // See if the AI can win on this turn.
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
    //If the other player can win next turn, block that spot.
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
    //Take the center if it's open. Otherwise, try a corner.
    if (availability(board, 4)) {
        return 4;
    }
    else if (availability(board, 0) || availability(board, 2) || availability(board, 6) || availability(board, 8)) {
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
    //A quicker win scores higher, and a later loss costs less.
    if (checkwin(board, aiplayer)) {
        return 10 - depth;
    }
    if (checkwin(board, opponent)) {
        return depth - 10;
    }
    if (checkdraw(board)) {
        return 0;
    }
    //The AI tries for the highest score. The other player tries for the lowest.
    if (isMax) {
        int best = -1000;
        for (int i = 0; i < 9; i++) {
            if (availability(board, i)) {
                char original = board[i];
                board[i] = aiplayer;
                int score = minimax(board, depth + 1, false, aiplayer);
                // Undo the move we just tried before checking the next spot.
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
        for (int i = 0; i < 9; i++) {
            if (availability(board, i)) {
                char original = board[i];
                board[i] = opponent;
                int score = minimax(board, depth + 1, true, aiplayer);
                // Undo the move we just tried before checking the next spot.
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

    //try every empty square and see what score it gets
    for (int i = 0; i < 9; i++) {
        if (availability(board, i)) {
            char original = board[i];
            board[i] = aiplayer;
            //We just tried the AI's move, so it's the other player's turn.
            int value = minimax(board, 0, false, aiplayer);
            board[i] = original;
            //cout << "move " << i + 1 << " scored " << value << endl;
            if (value > bestVal) {
                bestVal = value;
                best = i;
            }
        }
    }
    return best;
}

//saves one stats record with a prefix so load knows which mode it is
void saveOneStats(ofstream& file, string prefix, const Stats& s) {
    file << prefix << "_games " << s.games << endl;
    file << prefix << "_wins " << s.wins << endl;
    file << prefix << "_losses " << s.losses << endl;
    file << prefix << "_draws " << s.draws << endl;
}

void savestats() {
    try {
        ofstream file("stats.txt");
        if (!file.is_open()) {
            throw runtime_error("Couldn't open stats.txt.");
        }
        saveOneStats(file, "all", stats);
        saveOneStats(file, "pvp", modeStats[PVP]);
        saveOneStats(file, "pvai", modeStats[PVAI]);
        saveOneStats(file, "aivai", modeStats[AIVAI]);
        file.close();
        if (!file) {
            throw runtime_error("Couldn't finish saving stats.");
        }
    }
    catch (const runtime_error& error) {
        cerr << "Error: " << error.what() << endl;
    }
}

void loadstats() {
    ifstream file("stats.txt");
    if (!file.is_open()) {
        return; //first run, no stats saved yet
    }
    string label;
    int value;
    while (file >> label >> value) {
        if (label == "all_games") {
            stats.games = value;
        }
        else if (label == "all_wins") {
            stats.wins = value;
        }
        else if (label == "all_losses") {
            stats.losses = value;
        }
        else if (label == "all_draws") {
            stats.draws = value;
        }
        else if (label == "pvp_games") {
            modeStats[PVP].games = value;
        }
        else if (label == "pvp_wins") {
            modeStats[PVP].wins = value;
        }
        else if (label == "pvp_losses") {
            modeStats[PVP].losses = value;
        }
        else if (label == "pvp_draws") {
            modeStats[PVP].draws = value;
        }
        else if (label == "pvai_games") {
            modeStats[PVAI].games = value;
        }
        else if (label == "pvai_wins") {
            modeStats[PVAI].wins = value;
        }
        else if (label == "pvai_losses") {
            modeStats[PVAI].losses = value;
        }
        else if (label == "pvai_draws") {
            modeStats[PVAI].draws = value;
        }
        else if (label == "aivai_games") {
            modeStats[AIVAI].games = value;
        }
        else if (label == "aivai_wins") {
            modeStats[AIVAI].wins = value;
        }
        else if (label == "aivai_losses") {
            modeStats[AIVAI].losses = value;
        }
        else if (label == "aivai_draws") {
            modeStats[AIVAI].draws = value;
        }
        //old stats file only had these four lines
        else if (label == "games") {
            stats.games = value;
        }
        else if (label == "wins") {
            stats.wins = value;
        }
        else if (label == "losses") {
            stats.losses = value;
        }
        else if (label == "draws") {
            stats.draws = value;
        }
    }
    file.close();
}

//prints one stats record
void printOneStats(string label, const Stats* s) {
    cout << label << endl;
    cout << "  Games: " << s->games << endl;
    cout << "  Wins: " << s->wins << endl;
    cout << "  Losses: " << s->losses << endl;
    cout << "  Draws: " << s->draws << endl;
    if (s->games > 0) {
        cout << "  Win rate: " << (s->wins * 100.0 / s->games) << "%" << endl;
    }
    cout << endl;
}

//clears every stat after they say yes
void resetstats() {
    cout << "Reset all stats? This can't be undone. (Y/N): ";
    string answer;
    getline(cin, answer);
    if (!answer.empty() && toupper(answer[0]) == 'Y') {
        stats = Stats();
        for (int i = 0; i < NUM_MODES; i++) {
            modeStats[i] = Stats();
        }
        savestats();
        cout << "Stats reset." << endl;
    }
    else {
        cout << "Reset cancelled." << endl;
    }
}

//shows all the stats, press R to reset them
void showstats() {
    cout << endl;
    printOneStats("All Modes", &stats);
    printOneStats("Player vs Player", &modeStats[PVP]);
    printOneStats("Player vs AI", &modeStats[PVAI]);
    printOneStats("AI vs AI", &modeStats[AIVAI]);
    cout << "Press R to reset, or Enter to go back: ";
    string answer;
    getline(cin, answer);
    if (!answer.empty() && toupper(answer[0]) == 'R') {
        cout << endl;
        resetstats();
    }
}

void playPvP(char board[9], string p1, string p2) {
    RESET(board);
    moveCount = 0;
    char current = 'X';
    string currentName = p1;
    //X always starts
    while (true) {
        printboard(board);
        cout << currentName << " (" << current << ") your turn" << endl;
        int choice = getValidMoveUser(board);
        move(board, choice, current);
        addToHistory(choice, current);
        if (checkwin(board, current)) {
            printboard(board);
            cout << currentName << " wins!" << endl;
            // Count the result from X's side, so an O win is saved as a loss.
            if (current == 'X') {
                updateStats(PVP, WIN);
            }
            else {
                updateStats(PVP, LOSS);
            }
            savestats();
            showhistory();
            return;
        }
        if (checkdraw(board)) {
            printboard(board);
            cout << "It's a draw." << endl;
            updateStats(PVP, DRAW);
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
    //X always goes first, so the player starts if they picked X
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
                updateStats(PVAI, WIN);
                savestats();
                showhistory();
                return;
            }
            if (checkdraw(board)) {
                printboard(board);
                cout << "It's a draw." << endl;
                updateStats(PVAI, DRAW);
                savestats();
                showhistory();
                return;
            }
            humanTurn = false;
        }
        else {
            //AI turn, pick a move based on difficulty
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
                updateStats(PVAI, LOSS);
                savestats();
                showhistory();
                return;
            }
            if (checkdraw(board)) {
                printboard(board);
                cout << "It's a draw." << endl;
                updateStats(PVAI, DRAW);
                savestats();
                showhistory();
                return;
            }
            humanTurn = true;
        }
    }
}

void playAIvAI(char board[9], int difficulty1, int difficulty2) {
    RESET(board);
    moveCount = 0;
    char current = 'X';
    //X uses difficulty1 and O uses difficulty2
    while (true) {
        printboard(board);
        cout << "AI " << current << " is thinking..." << endl;
        char opponent;
        int difficulty;
        if (current == 'X') {
            opponent = 'O';
            difficulty = difficulty1;
        }
        else {
            opponent = 'X';
            difficulty = difficulty2;
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

            // Count the result from X's side, so an O win is saved as a loss.
            if (current == 'X') {
                updateStats(AIVAI, WIN);
            }
            else {
                updateStats(AIVAI, LOSS);
            }
            savestats();
            showhistory();
            return;
        }
        if (checkdraw(board)) {
            printboard(board);
            cout << "It's a draw." << endl;
            updateStats(AIVAI, DRAW);
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

//keeps asking until the user types a valid whole number
int getValidNumber(string prompt, int minimum, int maximum) {
    while (true) {
        cout << prompt;
        string line;
        if (!getline(cin, line)) {
            cout << "\nInput ended. Exiting." << endl;
            exit(0);
        }
        istringstream input(line);
        int number;
        char extra;
        if (!(input >> number) || (input >> extra)) {
            cout << "Please enter a whole number." << endl;
            continue;
        }
        if (number < minimum || number > maximum) {
            cout << "Enter a number from " << minimum
                << " to " << maximum << "." << endl;
            continue;
        }
        return number;
    }
}

int main() {
    loadstats();
    //do-while loop for the main game
    do {
        //initializes board and player
        char board[9];
        RESET(board);
        char player;
        int gamechoice = 0;
        //show menu
        cout << endl;
        cout << "TIC TAC TOE" << endl;
        cout << "===========" << endl;
        cout << "Wins: " << stats.wins << "  Losses: " << stats.losses << "  Draws: " << stats.draws << endl;
        cout << endl;

        cout << "1. Player vs Player" << endl;
        cout << "2. Player vs AI" << endl;
        cout << "3. AI vs AI" << endl;
        cout << "4. Stats" << endl;
        cout << "5. Quit" << endl;

        gamechoice = getValidNumber("Choice: ", 1, 5);
        //get both player names
        if (gamechoice == 1) {
            string player1;
            string player2;
            cout << "Player 1 name: ";
            cin >> player1;
            cout << "Player 2 name: ";
            cin >> player2;
            playPvP(board, player1, player2);
        }
        if (gamechoice == 2) {
            string name;
            cout << "Your name: ";
            cin >> name;
            cin.ignore(1000, '\n');
            //pick difficulty
            int diffculty = getValidNumber(
                "Pick your opponent: Easy (1), Hard (2), Impossible (3): ",
                1, 3
            );
            //pick O or X
            while (true) {
                cout << "Pick O or X: ";
                if (!(cin >> player)) {
                    if (cin.eof()) {
                        cout << "\nInput ended. Exiting." << endl;
                        exit(0);
                    }
                    cin.clear();
                    cin.ignore(1000, '\n');
                    cout << "Please enter O or X." << endl;
                    continue;
                }
                cin.ignore(1000, '\n');
                player = toupper(static_cast<unsigned char>(player));
                if (player == 'O' || player == 'X') {
                    break;
                }
                cout << "Please enter O or X." << endl;
            }
            playPvAI(board, name, player, diffculty);
        }
        if (gamechoice == 3) {
            //pick a difficulty for each AI
            int difficulty1 = getValidNumber(
                "AI X difficulty: Easy (1), Hard (2), Impossible (3): ",
                1, 3
            );

            int difficulty2 = getValidNumber(
                "AI O difficulty: Easy (1), Hard (2), Impossible (3): ",
                1, 3
            );
            playAIvAI(board, difficulty1, difficulty2);
        }
        if (gamechoice == 4) {
            showstats();
        }
        if (gamechoice == 5) {
            return 0;
        }
    } while (true);
}