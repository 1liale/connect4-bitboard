#include <cstdint>
#include <iostream>
#include <string>
#include <sstream>

using namespace std;

/*
 * Simple program implementing connect4 with bitboard from scratch
 * Purpose: Training bit manipulation skills
 * Author: Alex Li
 * */

/*
 * Board organized as:
 * 48 47 46 45 44 43 42 * empty row all 0 bits
 * 41 40 39 38 37 36 35
 * 34 33 32 31 30 29 28
 * 27 26 25 24 23 22 21
 * 20 19 18 17 16 15 14
 * 13 12 11 10 9  8  7
 * 6  5  4  3  2  1  0
 * */

class Board {
    uint64_t curPlayer;
    uint64_t state;
    int turn;

    const int HEIGHT = 6;
    const int WIDTH = 7;

    // checks if top cell of column is unoccupied
    bool canPlace(int column) {
        if (column < 0 || column >= WIDTH) return false;
        return (state & ((UINT64_C(1) << column) << WIDTH*(HEIGHT - 1))) == 0;
    }
public:
    Board(): curPlayer{0}, state{0}, turn{0} {}
    
    void place(int column) {
        if (!canPlace(column)) { cout << "Cannot place at column " << column << "!" << endl; return; }
        else cout << "Placed a stone at column " << column << endl;

        curPlayer ^= state;
        int row = 0;
        while ((state & ((UINT64_C(1) << column) << WIDTH * row)) != 0) {
            row++;
        }
        state |= ((UINT64_C(1) << column) << WIDTH * row);
        print();
        turn++;
    }

    bool checkWin() {
        uint64_t pos = curPlayer ^ state;
        // horizontal
        uint64_t mask = pos & (pos >> 1);
        if (mask & (mask >> 2)) return true;
        // vertical
        mask = pos & (pos >> WIDTH);
        if (mask & (mask >> (2 * WIDTH))) return true;
        // diag 1
        mask = pos & (pos >> (WIDTH - 1));
        if (mask & (mask >> (2*(WIDTH - 1)))) return true;
        // diag 2
        mask = pos & (pos >> (WIDTH + 1));
        if (mask & (mask >> (2*(WIDTH + 1)))) return true;
        return false;
    }

    // prints bitboard to readable form
    void print() {
        int i = 48; 
        char token1 = turn % 2 ? 'x' : 'o';
        char token2 = turn % 2 ? 'o' : 'x';
        while (i >= 0) {
            if (curPlayer & state & (UINT64_C(1) << i)) {
                cout << token1 << " ";
            } else if (state & (UINT64_C(1) << i)) {
               cout << token2 << " ";
            } else  {
                cout << ". ";
            }
            if (i % 7 == 0) {
                cout << endl;
            }
            --i;
        }        
    }
};

// driver code
int main() {
    string input;
    Board *connect4 = nullptr;
    cout << endl << "Enter Command: ";
    while (getline(cin, input)) {
        stringstream ss{input};
        string command;
        ss >> command;
        
        if (command == "game") {
            delete connect4;
            connect4 = new Board(); 
            cout << endl << "Welcome to connect4" << endl;
            cout << "Starting a new game..." << endl;
            connect4->print();
        } else if (command == "place" && connect4 != nullptr) {
            int column;
            ss >> column;
            connect4->place(column);
            if (connect4->checkWin()) {
                delete connect4;
                connect4 = new Board();
                cout << "Congrats, you have won!" << endl;
            }
        } else if (command == "exit") {
            break;
        } else {
            cout << endl << "Command not recognized, try again" << endl;
        }
        
        cout << endl << "Enter Command: ";
    }
    delete connect4;
}
