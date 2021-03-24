/*
 * Name: Jonathan Yang
 * ID: 100317635
 * Instructor: Bita Shadgar
 * Section: CPSC 2150 - 001
 * ChessKnight.cpp
 *      This program shows: In a chessboard (8x8), the knight can start from any
 *      position and land in any desired position.
 *      The program will give the shortest path from position A to position B.
 *
 *      In this .cpp file, I used Dijkstra's algorithm
*/

#include <iostream>
#include <stack>
#include <cstdio>

using namespace std;

const int BOARDSIZE = 8;
int chessBoard[BOARDSIZE][BOARDSIZE]; //[x,y]
int knightMove1[8] = {-1,-1,-2,-2,1, 1,2, 2};
int knightMove2[8] = {2 ,-2,1 ,-1,2,-2,1,-1};
stack<pair<int,int>> moves;

void initializeChessBoard();
void knight_moves(int& sx, int& sy, int& dx, int& dy);
int updateDijkstra(int& sx, int& sy, int& dx, int& dy);
int absValue(int n);
void printMoves(int& sx, int& sy, int& dx, int& dy);
bool canBeReachedFrom(int sx, int sy, int dx, int dy);
void printBoard();

/**
 * First initilize chessBoard, then ask for user input for starting point and ending point
 * Calls the function knight_moves()
 * @return
 */
int main() {
    initializeChessBoard();
    cout << "This program shows: In a chessboard (8x8), the knight can start from any "<< endl <<
    "position and land in any position you want." << endl;
    cout << "The program will give the shortest path from position A to position B." << endl << endl;
    int sx,sy,dx,dy;
    while(true){
        cout << "=> Enter the current Knight's location (e.g. 0 0 or 1 7): ";
        cin >> sx >> sy;
        cout << sx << " , " << sy << endl;
        if(sx >= BOARDSIZE || sy >= BOARDSIZE || sx < 0 || sy < 0){
            cout << "The size of board is " << BOARDSIZE << " x " << BOARDSIZE << endl;
            cout << "Input invalid!\n" << endl;
            continue;
        }
        break;
    }
    while(true){
        cout << "=> Enter the destination location: ";
        cin >> dx >> dy;
        cout << dx <<  " , " << dy << endl;
        if(dx >= BOARDSIZE || dy >= BOARDSIZE || dx < 0 || dy < 0){
            cout << "The size of board is " << BOARDSIZE << " x " << BOARDSIZE << endl;
            cout << "Input invalid!\n" << endl;
            continue;
        }
        break;
    }

    knight_moves(sx,sy,dx,dy);
    return 0;
}

/**
 * Initialize value for the chess board,
 * set all to INT_MAX
 * @param size
 * @param board
 */
void initializeChessBoard(){
    for(auto & i : chessBoard){
        for (int & j : i) {
            j = INT_MAX;
        }
    }
}

/**
 * Call updateDijkstra(), get how many steps for reaching the destination from the starting point
 * If only one step, push the starting point into the stack then print.
 * If more than one step, retrieve the steps by finding the number that is smaller than the [dx][dy]
 * one by one; and push each steps into the stack
 * @param sx start x
 * @param sy start y
 * @param dx destination x
 * @param dy destination y
 */
void knight_moves(int& sx, int& sy, int& dx, int& dy){
    int steps = updateDijkstra(sx,sy,dx,dy);
    if(steps > 1){
        int move = chessBoard[dx][dy];
        bool next;
        while(move > 1){
            next = false;
            for(int i = 0; i < BOARDSIZE && !next; i++){
                for(int j = 0; j < BOARDSIZE && !next; j++){
                    if(chessBoard[i][j] == move - 1 && canBeReachedFrom(moves.top().first,moves.top().second,i,j)){
                        moves.push(make_pair(i,j));
                        move--;
                        next = true;
                    }
                }
            }
        }
    }
    moves.push(make_pair(sx,sy));
    printMoves(sx,sy,dx,dy);
}

/**
 * Update the chessboard by using Dijkstra's algorithm,
 * The starting point is marked as 0, other numbers are
 * how many steps it takes to reach the point.
 * @param sx
 * @param sy
 * @param dx
 * @param dy
 * @return
 */
int updateDijkstra(int& sx, int& sy, int& dx, int& dy){
    moves.push(make_pair(dx,dy));
    if(sx == dx && sy == dy)
        return 0;
    if(canBeReachedFrom(sx,sy,dx,dy))
        return 1; // 1 move can reach the destination
    chessBoard[sx][sy] = 0;
    int mark = 1;
    for (int i = 0; i < 8; i++) {
        int goX = sx + knightMove1[i];
        int goY = sy + knightMove2[i];
        if(goX < BOARDSIZE && goX > -1 && goY < BOARDSIZE && goY > -1)
            chessBoard[goX][goY] = mark;
    }
    bool changed = true;
    while(changed){
        changed = false;
        for(int i = 0; i < BOARDSIZE ; i++){
            for(int j = 0; j < BOARDSIZE; j++){
                if(chessBoard[i][j] == mark){
                    for (int k = 0; k < 8; k++) {
                        if(i + knightMove1[k] < BOARDSIZE && j + knightMove2[k] < BOARDSIZE && j + knightMove2[k] >= 0 && i + knightMove1[k] >= 0) {
                            if (chessBoard[i + knightMove1[k]][j + knightMove2[k]] > mark + 1) {
                                chessBoard[i + knightMove1[k]][j + knightMove2[k]] = mark + 1;
                                changed = true;
                            }
                        }
                    }
                }
            }
        }
        mark++;
    }
    printBoard();
    return mark;
}

/**
 * Returns the absolute value of n
 * @param n
 * @return
 */
int absValue(int n){
    if(n < 0)
        return -n;
    else
        return n;
}

/**
 * Prints out the moves from the stack<pair<int,int>> moves.
 * @param sx
 * @param sy
 * @param dx
 * @param dy
 */
void printMoves(int& sx, int& sy, int& dx, int& dy){
    int num = (int)moves.size() - 1;
    cout << "\nYou made it in " << num << " moves from [" << sx << "," << sy <<  "] to [" << dx << "," << dy << "]! " << endl;
    cout << "Here is your path: " << endl;
    while(!moves.empty()){
        printf("\t[%i,%i]\n",moves.top().first,moves.top().second);
        moves.pop();
    }
}

/**
 * Conditions:
 * - the distance between sx and dx is 1, and the distance between sy and dy is 2
 * - the distance between sx and dx is 2, and the distance between sy and dy is 1
 * Returns true if fulfills one of the two conditions.
 * @param sx
 * @param sy
 * @param dx
 * @param dy
 * @return
 */
bool canBeReachedFrom(int sx, int sy, int dx, int dy){
    return (absValue(dx - sx) == 1 && absValue(dy - sy) == 2)||(absValue(dx - sx) == 2 && absValue(dy - sy) == 1) ;
}

/**
 * Prints out the chessboard
 */
void printBoard(){
    cout <<" |\t";
    for(int i = 0; i < BOARDSIZE; i++){
        cout << i << "\t";
    }
    cout << "\n---------------------------------";
    cout << "\n";
    for(int i = 0; i < BOARDSIZE; i++){
        cout << i << "|\t";
        for (auto & j : chessBoard) {
            cout << j[i] << "\t";
        }
        cout << "\n";
    }

    cout << "This chessboard shows the starting point (displayed as 0) \nand number of steps the knight can land onto the certain position." << endl;
}