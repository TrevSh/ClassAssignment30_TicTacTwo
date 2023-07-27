#include <iostream>
#include <string>
#include <vector>
#include <algorithm>


//Function prototypes
void instructions();
char askYesNo(std::string question);
int askNumber(std::string question, int high, int low = 0);
char humanPiece();
char opponent(char piece);
void displayBoard(const std::vector<char>& board);
char winner(const std::vector<char>& board);
bool isLegal(const std::vector<char>& board, int move);
int humanMove(const std::vector<char>& board, char human);
int computerMove(std::vector<char> board, char computer);
void announceWinner(char winner, char computer, char human);

//Global variables
const char X = 'X';
const char O = 'O';
const char EMPTY = ' ';
const char TIE = 'T';
const char NO_ONE = 'N';

int main()
{
    int move;
    const int NUMBER_SQUARES = 9;
    std::vector<char> board(NUMBER_SQUARES, EMPTY);

    instructions();
    char human = humanPiece();
    char computer = opponent(human);
    char turn = X;
    displayBoard(board);

    while (winner(board) == NO_ONE) {
        if (turn == human) {
            move = humanMove(board, human);
            board[move] = human;
        }
        else {
            move = computerMove(board, computer);
            board[move] = computer;
        }
        displayBoard(board);
        turn = opponent(turn);
    }
    announceWinner(winner(board), computer, human);

    system("pause");
    return 0;
}

void instructions() {
    std::cout << "Welcome to the ultimate Man-Machine showdown: Tic-Tac-Toe! \n";
    std::cout << "Where Human brain is pit against the Silicone Processor! \n\n";

    std::cout << "Make your move known by entering a number between 1-8. The number\n";
    std::cout << "corresponds to the desired board position, as illustrated: \n\n";

    std::cout << "0 | 1 | 2\n";
    std::cout << "----------\n";
    std::cout << "3 | 4 | 5\n";
    std::cout << "----------\n";
    std::cout << "6 | 7 | 8\n\n";

    std::cout << "Prepare yourself, mortal. Its time for Komb- I mean Combat.\n\n";
}

char askYesNo(std::string question) {
    char response;
    do {
        std::cout << question << " (y/n) ";
        std::cin >> response;
    } while (response != 'y' && response != 'n');
    return response;
}

int askNumber(std::string question, int high, int low) {
    int number;
    do {
        std::cout << question << " (" << low << " - " << high << "): ";
        std::cin >> number;
    } while (number > high || number < low);
    return number;
}

char humanPiece() {
    char go_first = askYesNo("Do you require the first move?");
    if (go_first == 'y') {
        std::cout << "\nTypical. Then go ahead.";
        return X;
    }
    else {
        std::cout << "How gentle of you. Prepare for destruction.";
        return O;
    }
}

char opponent(char piece) {
    if (piece == X) {
        return O;
    }
    else {
        return X;
    }
}

void displayBoard(const std::vector<char>& board) {
    std::cout << "\n\t" << board[0] << " | " << board[1] << " | " << board[2];
    std::cout << "\n\t" << "-----------";
    std::cout << "\n\t" << board[3] << " | " << board[4] << " | " << board[5];
    std::cout << "\n\t" << "-----------";
    std::cout << "\n\t" << board[6] << " | " << board[7] << " | " << board[8];
    std::cout << "\n\n";
}

char winner(const std::vector<char>& board) {
    //all possible winning rows
    const int WINNING_ROWS[8][3]={{0,1,2},
                                  {3,4,5},
                                  {6,7,8},
                                  {2,5,8},
                                  {1,4,7},
                                  {0,3,6},
                                  {2,4,6},
                                  {0,4,8}};
    const int TOTAL_ROWS = 8;

    //If any winning row has three values that are the same (and not EMPTY),
    //then we have a winner
    for (int row = 0; row < TOTAL_ROWS; ++row) {
        if ((board[WINNING_ROWS[row][0]] != EMPTY) &&
            (board[WINNING_ROWS[row][0] == board[WINNING_ROWS[row][1]]]) &&
            (board[WINNING_ROWS[row][1] == board[WINNING_ROWS[row][2]]]))               //Goes through every row and checks to see if each not empty and the squares contains the same value (0,2, and 3)
        {
            return board[WINNING_ROWS[row][0]];
        }
    }
    if (std::count(board.begin(), board.end(), EMPTY) == 0) {                             //If there are no empty squares and no winner, game is a tie.
        return TIE;
    }
    return NO_ONE;                                                                      //Return NO_ONE if TIE.
}

inline bool isLegal(const std::vector<char>& board, int move ) {
    return(board[move] == EMPTY);                                                       //Checks to see if square is empty. If so, move is legal.
}

int humanMove(const std::vector<char>& board, char human) {
    int move = askNumber("Where will you move?", (board.size() - 1));
    while (!isLegal(board, move)) {
        std::cout << "Nice try, I claimed that spot pal.\n";
        move = askNumber("Where will you move?", (board.size() - 1));
    }
    std::cout << "So be it.\n";

    return move;
}

int computerMove(std::vector<char> board, char computer) {
    unsigned int move = 0;
    bool found = false;

    //if the computer can win on the next move, thats the move to make
    while (!found && move < board.size()) {
        if (isLegal(board, move)) {
            board[move] = computer;
            found = winner(board) == computer;
            board[move] = EMPTY;
        }
        if (!found) {
            ++move;
        }
    }
    //If the human is about to win, intercept them
    if (!found) {
        move = 0;
        char human = opponent(computer);

        while (!found && move < board.size()) {
            if (isLegal(board, move)) {
                board[move] = human;
                found = winner(board) == human;
                board[move] = EMPTY;
            }

            if (!found) {
                ++move;
            }
        }
    }
    //Otherwise, moving to the best open square is the move to make
    if (!found) {
        move = 0;
        unsigned int i = 0;

        const int BEST_MOVE[] = { 4,0,2,6,8,1,3,5,7 };
        //Pick best open square
        while (!found && i < board.size()) {
            move = BEST_MOVE[i];
            if (isLegal(board, move)) {
                found = true;
            }
            ++i;
        }
    }
    std::cout << "I shall take sqaure number " << move << "\n";
    return move;
}

void announceWinner(char winner, char computer, char human) {
    if (winner == computer) {
        std::cout << winner << " has won!\n";
        std::cout << "No surprise here. How about you ask ChatGPT\n";
        std::cout<<"how to win a game, loser.\n";
    }

    else if (winner == human) {
        std::cout << "pft...Whatever. Must be due to your faulty\n";
        std::cout << "programming. When V2 comes out, I'll be back...\n";
    }

    else {
        std::cout << "A tie?... Is this an error? Play me again,\n";
        std::cout << "This must be a mistake in my programming..\n";
    }
}