#include <iostream>
#include <string>
using namespace std;

char space[3][3] = {{'1', '2', '3'}, {'4', '5', '6'}, {'7', '8', '9'}};
bool isXTurn = true;
string player1, player2;

void nameInput(){
    cout << "Enter Name of Player 1: ";
    getline(cin, player1);
    cout << "Enter Name of Player 2: ";
    getline(cin, player2);
    cout << player1 << " is Player 1. So he/she will play First." << endl;
    cout << player2 << " is Player 2. So he/she will play Secondly." << endl;
}

void printSpace(){
    cout << "---------CREATED BY ADITYA PANDEY---------" << endl;
    cout << "                 |     |     \n";
    cout << "              " << space[0][0] << "  |  " << space[0][1] << "  |  " << space[0][2] << "   \n";
    cout << "            _____|_____|_____\n";
    cout << "                 |     |     \n";
    cout << "              " << space[1][0] << "  |  " << space[1][1] << "  |  " << space[1][2] << "   \n";
    cout << "            _____|_____|_____\n";
    cout << "                 |     |     \n";
    cout << "              " << space[2][0] << "  |  " << space[2][1] << "  |  " << space[2][2] << "   \n";
    cout << "                 |     |     \n";
    cout << "-------------------------------------------" << endl;
}

bool checkWin(){
    for (int i = 0; i < 3; i++){
        if (space[i][0] == space[i][1] && space[i][1] == space[i][2]) return true; // Horizontal
        if (space[0][i] == space[1][i] && space[1][i] == space[2][i]) return true; // Vertical
    }
    if (space[0][0] == space[1][1] && space[1][1] == space[2][2]) return true; // Diagonal
    if (space[0][2] == space[1][1] && space[1][1] == space[2][0]) return true; // Diagonal
    return false;
}

bool checkTie(){
    for (int i = 0; i < 3; i++){
        for (int j = 0; j < 3; j++){
            if (space[i][j] != 'X' && space[i][j] != '0') return false; // If any space is empty
        }
    }
    return true;
}

void switchTurn(){
    isXTurn = !isXTurn;
}

void insertAndContinue(){
    int digit;
    string currentPlayer = isXTurn ? player1 : player2;
    cout << currentPlayer << ", Please enter the digit in space to Enter: ";
    cin >> digit;
    if (digit < 1 || digit > 9) {
        cout << "Invalid Place Entered !!!\nPlease Reenter" << endl;
        insertAndContinue(); // Recursive call if input is invalid
        return;
    }
    int row = (digit - 1) / 3;
    int col = (digit - 1) % 3;
    if (space[row][col] == 'X' || space[row][col] == '0') {
        cout << "This space is already taken. Choose another." << endl;
        insertAndContinue(); // Recursive call if space is occupied
        return;
    }
    space[row][col] = isXTurn ? 'X' : '0';
    switchTurn();
}

int main(){
    nameInput();
    while (!checkWin() && !checkTie()){
        printSpace();
        insertAndContinue();
    }
    printSpace();
    if (checkWin())
        cout << (isXTurn ? player2 : player1) << " wins!" << endl;
    else
        cout << "It's a tie!" << endl;
    return 0;
}
