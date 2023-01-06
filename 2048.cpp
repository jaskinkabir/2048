#include <iostream>
#include <string>
#include <vector>
#include <random>
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <Windows.h>
#include <utility>
#include <tuple>
#include <math.h>
#include <algorithm>
#include <fstream>
#include <unordered_map>
#include <tuplehash.h> //Hash function for tuple


int delay = 0;
bool debugging = true;
int score;
bool win;
bool gameEnd;

void clear() {
    system("cls");
}

std::vector<std::tuple<int, int>> updatedSpaces;

std::vector<std::vector<int>> board = { //Holds the values of each tile on the board
    {0,0,0,0},
    {0,0,0,0},
    {0,0,0,0},
    {0,0,0,0}
};

std::vector<std::vector<int>> boardUndo = {
    {0,0,0,0},
    {0,0,0,0},
    {0,0,0,0},
    {0,0,0,0}
};

std::vector<std::vector<int>> boardRedo = {
    {0,0,0,0},
    {0,0,0,0},
    {0,0,0,0},
    {0,0,0,0}
};

std::unordered_map<std::tuple<int, int>, char> boardChars; //Dictionary of each space in the board paired with the character that should border it 
std::vector<std::vector<std::string>> strBoard; //Copy of board vector but with appropriate spacing

bool checkWin() { //Return true as soon as a tile with value 2048 is found
    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++) {
            if (board[y][x] == 2048) return true;
        }
    }
    return false;
}

bool gameOver() {
    //Loop through each space in board. If any space has a valid move in any direction return false. Otherwise return true
    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++) {
            if (x < 3 && board[y][x + 1] == 0 || x < 3 && board[y][x + 1] == board[y][x]) return false;
            if (x > 0 && board[y][x - 1] == 0 || x > 0 && board[y][x - 1] == board[y][x]) return false;
            if (y < 3 && board[y + 1][x] == 0 || y < 3 && board[y + 1][x] == board[y][x]) return false;
            if (y > 0 && board[y - 1][x] == 0 || y > 0 && board[y - 1][x] == board[y][x]) return false;
        }
    }
    return true;
}

void displayBoard() {
    
    int numDigits; //Number of digits in current tile value 
    int x,y,f,g;

    for (auto& space : updatedSpaces) { 
        x = std::get<1>(space);
        y = std::get<0>(space);

        if (board[y][x] == 0) { //Set empty spaces to an empty cell 
            strBoard[y][x] = "    ";
            numDigits = -1;
        }
        else numDigits = int(std::log10(board[y][x])) + 1; //Expression finds number of digits in number
        switch (numDigits) {
        case 0: break; //Ignore empty spaces
        case 1: //Add a certain number of spaces before and after each number so that it is 'centered' within the cell
            strBoard[y][x] = "  " + std::to_string(board[y][x]) + ' ';
            break;
        case 2:
            strBoard[y][x] = ' ' + std::to_string(board[y][x]) + ' ';
            break;
        case 3:
            strBoard[y][x] = std::to_string(board[y][x]) + ' ';
            break;
        case 4:
            strBoard[y][x] = std::to_string(board[y][x]);

        }

         //Current tile's place in the sequence of characters
        if (board[y][x] == 0) {
            g = 0;
        }
        else g = int(log2(board[y][x])); //Log2 of the tile's value returns how many times it has been combined-1

        if (g < 7) f = g - 1;
        else f = (g % 6) - 1; //Modulo wraps the function back around to 0 once a value of 128 is reached. This assures that all values can be assigned a character

        switch (f) { //Depending on the value of each tile, assign a specific character to border that tile
        case -1: boardChars[std::make_tuple(y, x)] = ' '; break; //0
        case 0: boardChars[std::make_tuple(y, x)] = '+'; break; //2, 128
        case 1: boardChars[std::make_tuple(y, x)] = '?'; break; //4, 256
        case 2: boardChars[std::make_tuple(y, x)] = '$'; break; //8, 512
        case 3: boardChars[std::make_tuple(y, x)] = '%'; break; //16, 1024
        case 4: boardChars[std::make_tuple(y, x)] = '#'; break; //32, 2048
        case 5: boardChars[std::make_tuple(y, x)] = '*'; break; //64, 4096
        }

    }

    if (gameOver()) std::cout << "\nGAME OVER!!\n";

    for (y = 0; y < 4; y++) {
        std::cout << "---------------- ---------------- ---------------- ----------------\n";
        std::cout << "|              | |              | |              | |              |\n";
        std::cout << "|  " << boardChars[std::make_tuple(y, 0)] << boardChars[std::make_tuple(y, 0)] << boardChars[std::make_tuple(y, 0)] << boardChars[std::make_tuple(y, 0)] << boardChars[std::make_tuple(y, 0)] << boardChars[std::make_tuple(y, 0)] << boardChars[std::make_tuple(y, 0)] << boardChars[std::make_tuple(y, 0)] << boardChars[std::make_tuple(y, 0)] << boardChars[std::make_tuple(y, 0)] << "  | |  " << boardChars[std::make_tuple(y, 1)] << boardChars[std::make_tuple(y, 1)] << boardChars[std::make_tuple(y, 1)] << boardChars[std::make_tuple(y, 1)] << boardChars[std::make_tuple(y, 1)] << boardChars[std::make_tuple(y, 1)] << boardChars[std::make_tuple(y, 1)] << boardChars[std::make_tuple(y, 1)] << boardChars[std::make_tuple(y, 1)] << boardChars[std::make_tuple(y, 1)] << "  | |  " << boardChars[std::make_tuple(y, 2)] << boardChars[std::make_tuple(y, 2)] << boardChars[std::make_tuple(y, 2)] << boardChars[std::make_tuple(y, 2)] << boardChars[std::make_tuple(y, 2)] << boardChars[std::make_tuple(y, 2)] << boardChars[std::make_tuple(y, 2)] << boardChars[std::make_tuple(y, 2)] << boardChars[std::make_tuple(y, 2)] << boardChars[std::make_tuple(y, 2)] << "  | |  " << boardChars[std::make_tuple(y, 3)] << boardChars[std::make_tuple(y, 3)] << boardChars[std::make_tuple(y, 3)] << boardChars[std::make_tuple(y, 3)] << boardChars[std::make_tuple(y, 3)] << boardChars[std::make_tuple(y, 3)] << boardChars[std::make_tuple(y, 3)] << boardChars[std::make_tuple(y, 3)] << boardChars[std::make_tuple(y, 3)] << boardChars[std::make_tuple(y, 3)] << "  | \n";
        std::cout << "|  " << boardChars[std::make_tuple(y, 0)] << "        " << boardChars[std::make_tuple(y, 0)] << "  | |  " << boardChars[std::make_tuple(y, 1)] << "        " << boardChars[std::make_tuple(y, 1)] << "  | |  " << boardChars[std::make_tuple(y, 2)] << "        " << boardChars[std::make_tuple(y, 2)] << "  | |  " << boardChars[std::make_tuple(y, 3)] << "        " << boardChars[std::make_tuple(y, 3)] << "  |\n";
        std::cout << "|  " << boardChars[std::make_tuple(y, 0)] << "  " << strBoard[y][0] << "  " << boardChars[std::make_tuple(y, 0)] << "  | |  " << boardChars[std::make_tuple(y, 1)] << "  " << strBoard[y][1] << "  " << boardChars[std::make_tuple(y, 1)] << "  | |  " << boardChars[std::make_tuple(y, 2)] << "  " << strBoard[y][2] << "  " << boardChars[std::make_tuple(y, 2)] << "  | |  " << boardChars[std::make_tuple(y, 3)] << "  " << strBoard[y][3] << "  " << boardChars[std::make_tuple(y, 3)] << "  |\n";
        std::cout << "|  " << boardChars[std::make_tuple(y, 0)] << "        " << boardChars[std::make_tuple(y, 0)] << "  | |  " << boardChars[std::make_tuple(y, 1)] << "        " << boardChars[std::make_tuple(y, 1)] << "  | |  " << boardChars[std::make_tuple(y, 2)] << "        " << boardChars[std::make_tuple(y, 2)] << "  | |  " << boardChars[std::make_tuple(y, 3)] << "        " << boardChars[std::make_tuple(y, 3)] << "  |\n";
        std::cout << "|  " << boardChars[std::make_tuple(y, 0)] << boardChars[std::make_tuple(y, 0)] << boardChars[std::make_tuple(y, 0)] << boardChars[std::make_tuple(y, 0)] << boardChars[std::make_tuple(y, 0)] << boardChars[std::make_tuple(y, 0)] << boardChars[std::make_tuple(y, 0)] << boardChars[std::make_tuple(y, 0)] << boardChars[std::make_tuple(y, 0)] << boardChars[std::make_tuple(y, 0)] << "  | |  " << boardChars[std::make_tuple(y, 1)] << boardChars[std::make_tuple(y, 1)] << boardChars[std::make_tuple(y, 1)] << boardChars[std::make_tuple(y, 1)] << boardChars[std::make_tuple(y, 1)] << boardChars[std::make_tuple(y, 1)] << boardChars[std::make_tuple(y, 1)] << boardChars[std::make_tuple(y, 1)] << boardChars[std::make_tuple(y, 1)] << boardChars[std::make_tuple(y, 1)] << "  | |  " << boardChars[std::make_tuple(y, 2)] << boardChars[std::make_tuple(y, 2)] << boardChars[std::make_tuple(y, 2)] << boardChars[std::make_tuple(y, 2)] << boardChars[std::make_tuple(y, 2)] << boardChars[std::make_tuple(y, 2)] << boardChars[std::make_tuple(y, 2)] << boardChars[std::make_tuple(y, 2)] << boardChars[std::make_tuple(y, 2)] << boardChars[std::make_tuple(y, 2)] << "  | |  " << boardChars[std::make_tuple(y, 3)] << boardChars[std::make_tuple(y, 3)] << boardChars[std::make_tuple(y, 3)] << boardChars[std::make_tuple(y, 3)] << boardChars[std::make_tuple(y, 3)] << boardChars[std::make_tuple(y, 3)] << boardChars[std::make_tuple(y, 3)] << boardChars[std::make_tuple(y, 3)] << boardChars[std::make_tuple(y, 3)] << boardChars[std::make_tuple(y, 3)] << "  | \n";
        std::cout << "|              | |              | |              | |              |\n";
        std::cout << "---------------- ---------------- ---------------- ----------------\n";
    }

    std::cout << "Score: " << score << '\n';
    if (win) printf("You won!\nKeep playing to get a higher score.\n");
    
    updatedSpaces.clear();

}

void initializeGame() {
    int c,x,y;
    score = 0;
    win = false;
    gameEnd = false;

    for (int y = 0; y < 4; y++) { //Initialize vectors
        for (int x = 0; x < 4; x++) {
            boardChars[std::make_tuple(y, x)] = ' '; //Create blank borders around all spaces
            board[y][x] = 0; //Fill board with blank spaces
        }

    }

    strBoard = {
        {"    ","    ","    ","    ",},
        {"    ","    ","    ","    ",},
        {"    ","    ","    ","    ",},
        {"    ","    ","    ","    ",},
    };

    srand(time(0));
    for (int i = 0; i < 2; i++) { //Randomly generate 2 tiles, 4 is 3x less likely to be generated than 2
        c = rand() % 4;
        if (c == 3) c = 4;
        else c = 2;
        x = rand() % 4;
        y = rand() % 4;
        while (board[y][x] != 0) { //Make sure space is empty
            x = rand() % 4;
            y = rand() % 4;
        }
        board[y][x] = c;
        updatedSpaces.push_back(std::make_tuple(y, x));
    }
}

bool inCoSp(int x, int y, std::vector<std::tuple<int,int>> &vec) { //Return true if tile has already been combined
    std::tuple<int,int>tup = std::make_tuple(y, x);
    return std::count(vec.begin(), vec.end(), tup) != 0;
}

void move(char dir) { // Check for valid moves, move those tiles, then check again and repeat
    std::vector<std::tuple<int, int>> combinedSpaces={}; //List of spaces that have already been combined
    int moves = -1;
    bool success = false;
    boardUndo = board;


    while (moves != 0) { //Try to move tiles until no tiles are moved anymore
        moves = 0;

        switch (int(dir)) {
        case 100: //d
            for (int x = 0; x < 4; x++) {
                for (int y = 2; y > -1; y--) {
                    if (board[y][x] == 0) continue;
                    if (board[y + 1][x] == 0) {
                        board[y + 1][x] = board[y][x];
                        board[y][x] = 0;
                        moves++;
                        success = true;
                        updatedSpaces.push_back(std::make_tuple(y, x));
                        updatedSpaces.push_back(std::make_tuple(y + 1, x));
                    }

                    if (inCoSp(x, y, combinedSpaces) || inCoSp(x, y + 1, combinedSpaces)) continue;
                    if (board[y + 1][x] == board[y][x]) {
                        board[y + 1][x] += board[y][x];
                        board[y][x] = 0;
                        score += board[y + 1][x];
                        moves++;
                        success = true;
                        combinedSpaces.push_back(std::make_tuple(y + 1, x));
                        updatedSpaces.push_back(std::make_tuple(y, x));
                        updatedSpaces.push_back(std::make_tuple(y + 1, x));
                    }
                }
            }
            break;
        case 108: //l
            for (int y = 0; y < 4; y++) {
                for (int x = 1; x < 4; x++) {
                    if (board[y][x] == 0) continue;
                    if (board[y][x - 1] == 0 && board[y][x] != 0) {
                        board[y][x - 1] = board[y][x];
                        board[y][x] = 0;
                        moves++;
                        success = true;
                        updatedSpaces.push_back(std::make_tuple(y, x));
                        updatedSpaces.push_back(std::make_tuple(y, x - 1));
                    }

                    if (inCoSp(x, y, combinedSpaces) || inCoSp(x - 1, y, combinedSpaces)) continue;
                    if (board[y][x - 1] == board[y][x]) {
                        board[y][x - 1] += board[y][x];
                        board[y][x] = 0;
                        score += board[y][x - 1];
                        moves++;
                        success = true;
                        combinedSpaces.push_back(std::make_tuple(y, x - 1));
                        updatedSpaces.push_back(std::make_tuple(y, x));
                        updatedSpaces.push_back(std::make_tuple(y, x - 1));
                    }
                }
            }
            break;
        case 114: // r
            for (int y = 0; y < 4; y++) {
                for (int x = 2; x > -1; x--) {
                    if (board[y][x] == 0) continue;

                    if (board[y][x + 1] == 0) {
                        board[y][x + 1] = board[y][x];
                        board[y][x] = 0;
                        moves++;
                        success = true;
                        updatedSpaces.push_back(std::make_tuple(y, x));
                        updatedSpaces.push_back(std::make_tuple(y, x + 1));

                    }

                    if (inCoSp(x, y, combinedSpaces) || inCoSp(x + 1, y, combinedSpaces)) continue;
                    if (board[y][x + 1] == board[y][x]) {
                        board[y][x + 1] += board[y][x];
                        board[y][x] = 0;
                        score += board[y][x + 1];
                        moves++;
                        success = true;
                        combinedSpaces.push_back(std::make_tuple(y, x + 1));
                        updatedSpaces.push_back(std::make_tuple(y, x));
                        updatedSpaces.push_back(std::make_tuple(y, x + 1));
                    }
                }
            }
            break;
        case 117: //u
            for (int x = 0; x < 4; x++) {
                for (int y = 1; y < 4; y++) {
                    if (board[y][x] == 0) continue;//Ignore empty spaces

                    if (board[y - 1][x] == 0) { // If there is an empty space above the current tile, move value of current tile into the space above
                        board[y - 1][x] = board[y][x];
                        board[y][x] = 0;
                        moves++;
                        success = true;
                        updatedSpaces.push_back(std::make_tuple(y, x));//Make sure the current space and the space it should be moved into are updated
                        updatedSpaces.push_back(std::make_tuple(y - 1, x));
                    }

                    if (inCoSp(x, y, combinedSpaces) || inCoSp(x, y - 1, combinedSpaces)) continue; //Ignore tiles that have already been combined
                    if (board[y - 1][x] == board[y][x]) { //If there is a tile of the same value above the current tile, combine them
                        board[y - 1][x] += board[y][x];
                        board[y][x] = 0;
                        score += board[y - 1][x];
                        moves++;
                        success = true;
                        combinedSpaces.push_back(std::make_tuple(y - 1, x)); //Add the combined tile to the list of combined tiles
                        updatedSpaces.push_back(std::make_tuple(y, x));
                        updatedSpaces.push_back(std::make_tuple(y - 1, x));
                    }
                }
            }
        }

        if (moves != 0) { //If tiles were moved, display the movement
            Sleep(delay);
            clear();
            displayBoard();
            
        }
        

    }
    if (success) { //If the move was successful, reandomly generate a new tile
        
        srand(time(0));
        int c = rand() % 6;
        if (c == 5) c = 4;
        else c = 2;
        int x;
        int y;
        x = rand() % 4;
        y = rand() % 4;
        while (board[y][x] != 0) { //Make sure x and y point to an empty space
            x = rand() % 4;
            y = rand() % 4;
        }

        board[y][x] = c;
        updatedSpaces.push_back(std::make_tuple(y, x));
        
    }
}


int main()
{
    initializeGame();

    while (!gameEnd) {
        displayBoard();
        char a = _getch(); //Get user's keystroke

        if (a == 0 || a == 0xE0) a = _getch();

        std::string x;
        std::string y;

        switch (int(a)) {
        case 72: //Up arrow
            move('u');
            break;
        case 75: //Left arrow
            move('l');
            break;
        case 77: // Right arrow
            move('r');
            break;
        case 80: // Down arrow
            move('d');
            break;
        case 100: //D
            if (debugging) debugging = false; //Toggle debug (cheats) mode
            else debugging = true;
        }

        int c;
        if (debugging) {
            switch (int(a)) {
            case 101: //E
                //Assign a random value to each tile
                for (int x = 0; x < 4; x++) {
                    for (int y = 0; y < 4; y++) {
                        c = rand() % 2049;
                        while (log2(c) != int(log2(c)) || c==1) c = rand() % 2048; ; //Make sure c is a power of 2
                        board[y][x] = c;
                        updatedSpaces.push_back(std::make_tuple(y, x));
                    }
                }
                break;
            case 103: //G
                initializeGame(); 
                break;
            case 119: //W
                //Set given space to 2048
                std::cin >> x;
                std::cin >> y;
                board[std::stoi(y)][std::stoi(x)] = 2048;
                updatedSpaces.push_back(std::make_tuple(stoi(y), stoi(x)));
                break;
            case 121: //Y Redo function
                board = boardRedo;
                break;

            case 122: //Z
                boardRedo = board;
                board = boardUndo;
                break;
            }
        }

        
        if (gameOver()) { //Code to ask player to play again:
            Sleep(750);

            char a = ' ';//Variable surrounding Yes
            char b = ' ';//Variable surrounding No
            int res = 1;//User's selection, 1 if yes, 2 if no


            while (true)
            {
                switch (res) { //Sets whether yes or no should be highlighted based on what the user has selected
                case 1:
                    a = '*';
                    b = ' ';
                    break;
                case 2:
                    b = '*';
                    a = ' ';
                    break;
                }
                clear();
                displayBoard();
                std::cout << "Play again?\n";
                std::cout << "Use the arrow keys to select an answer, then press enter.\n\n";
                std::cout << a << a << a << a << a << " " << b << b << b << b << '\n';
                std::cout << a << "Yes" << a << " " << b << "No" << b << '\n';
                std::cout << a << a << a << a << a << " " << b << b << b << b << '\n';

                char i = _getch(); //Gets user's keystroke

                if (i == 0 || a == 0xE0) i = _getch(); //The arrow keys output a 0 and then the code for their key, so getch must be called twice

                if (i == 13) { //When enter is pressed, exit the loop
                    break;
                }
                else if (i == 75 && res == 2) {//left arrow key, checks if no is selected and moves to yes if it is
                    res = 1;
                }
                else if (i == 77 && res == 1) {//right arrow key, checks if yes is selected and moves to no if it is
                    res = 2;
                }
            }
            if (res == 1) {//reinitialize game if yes
                initializeGame();
            }
            else {//end program if no
                return 0;
            }

        }

        clear();
        win = checkWin();
        gameEnd = gameOver();
    }
    
}
