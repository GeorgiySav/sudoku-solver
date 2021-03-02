#include <iostream>
#include <vector>
#include <windows.h>
#include <ctime>

using namespace std;

void StoreUntouch();
void DisplayGrid();
void FindValidPos();
void BackTrack();
int FindValidNum();

//array for the sudoku puzzle
int grid[9][9] = { { 0, 0, 0, 0, 7, 0, 0, 0, 0 },
                   { 3, 6, 0, 0, 0, 9, 1, 8, 0 },
                   { 0, 2, 0, 5, 0, 0, 3, 0, 0 },
                   { 0, 0, 0, 0, 0, 0, 0, 0, 0 },
                   { 0, 0, 8, 2, 0, 0, 0, 4, 0 },
                   { 0, 4, 0, 6, 0, 3, 0, 0, 0 },
                   { 0, 5, 0, 0, 0, 0, 0, 0, 6 },
                   { 6, 0, 3, 0, 0, 0, 2, 0, 0 },
                   { 0, 0, 0, 0, 9, 0, 7, 0, 0 }, };

//array for which positions in the puzzle cannot be changed
bool notouch[9][9] = { {false, false, false, false, false, false, false, false, false},
                       {false, false, false, false, false, false, false, false, false},
                       {false, false, false, false, false, false, false, false, false},
                       {false, false, false, false, false, false, false, false, false},
                       {false, false, false, false, false, false, false, false, false},
                       {false, false, false, false, false, false, false, false, false},
                       {false, false, false, false, false, false, false, false, false},
                       {false, false, false, false, false, false, false, false, false},
                       {false, false, false, false, false, false, false, false, false} };

//array to store the position of the cell the program is checking
int pos[2] = { 0, 0 };
//stores the last value that can be changed
int lastpos[2] = { 0, 0 };
//stores the number returned from findvalidnum
int returnednum = 0;

//tells the program when to stop
bool stop = false;
//tells the program if its backtracking
bool backtracking = false;
//says if the puzzle is unsolvable
bool unsolvable = false;

//stores the amount of time it took the program to solve the puzzle
double elapsed_secs = 0;                       

int main()
{
    /// <summary>
    /// 
    /// Backtracking method of solving sudoku puzzles
    /// 
    /// How it works:
    /// The program will cycle through all changeable values in the puzzle and will ignore all values that have already been put into the sudoku
    /// When it reaches a changeable square, it will cycle from the number in its cell +1 , to 9
    /// For example, if the current cell contains 2, then it will cycle from 3 to 9
    /// The program will stop cycling until a valid number is found, or it reaches 9
    /// 
    /// The program checks if a number is valid by checking if that number appears in its row, column and 3x3 square
    /// If it passes all checks, meaning that the number doesn't appear, then the new number will replace the current number in that cell
    /// 
    /// If the cell doesn't have any valid numbers that can be put into that cell, so if 9 isn't a valid number in the cell, it will set that cells value to 0, then the program will start to backtrack
    /// It will visit the previous cell, and check if a different number (that is higher than the current number in the cell) can be put into that cell
    /// If it can, then it will change that value and continue cycling until it has to backtrack again
    /// If there is no other value that can be put in, then it will continue to backtrack until it finds a cell that can be changed and then continue with its solving
    /// 
    /// The program will stop once a valid value is put into the last cell that can be changed is changed
    /// 
    /// </summary>

    //starts the timer to see how long it takes for the computer to solve the puzzle
    clock_t begin = clock();

    //stores all unchangeable values in the puzzle, so that those values can't be changed when solving
    StoreUntouch();

    cout << "unsolved puzzle: " << endl;
    DisplayGrid();

    //will solve the puzzle until its solved
    while (!stop)
    {
        //system("CLS");
        //DisplayGrid();

        //will only find a valid pos if the program isn't backtracking
        if (!backtracking) { FindValidPos(); }

        //gets a number from findvalidnum that will intruct the program what to do
        returnednum = FindValidNum();

        //if the program recieves 10, then the program found no valid numbers for that cell, so it will start to backtrack
        if (returnednum == 10)
        {
            BackTrack();
        }
        //checks if the position being changed is the last cell so it can check whether the sudoku puzzle has been solved
        else if (pos[0] == lastpos[0] && pos[1] == lastpos[1])
        {
            //sets the cell to the returened number from findvalidnum
            grid[pos[1]][pos[0]] = returnednum;
            //ends the timer for the clock
            clock_t end = clock();
            //calculates the amount of time it took for the program to solve the sudoku
            elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
            //sets stop to true to tell the program that the puzzle is solved
            stop = true;
        }
        //changes the cell to the returned number
        else
            grid[pos[1]][pos[0]] = returnednum;
        
        if (pos[1] < 0 || pos[0] < 0) 
        { 
            stop = true; 
            unsolvable = true; 

            //ends the timer for the clock
            clock_t end = clock();
            //calculates the amount of time it took for the program to find it was unsolvable
            elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
        }
    }
    //displays the solved puzzle with the amount of time it took
    cout << "Time taken(seconds): " << elapsed_secs << endl;
    if (!unsolvable)
    {
        cout << "solved puzzle: " << endl;
        DisplayGrid();
    }
    else
        cout << "Puzzle is unsolvable";

}

//stores which numbers cannot be changed
void StoreUntouch()
{
    //cycles through all cells in the grid before solving
    for (int i = 0; i < size(grid); i++)
    {
        for (int j = 0; j < size(grid); j++)
        {
            //if the cell doesn't contain a 0, then it cannot be touched
            if (grid[i][j] != 0) { notouch[i][j] = true; }
            else { lastpos[0] = j; lastpos[1] = i; }
        }
    }
}

//displays the grid
void DisplayGrid()
{
    //cycles through all cells in the puzzle
    for (int i = 0; i < size(grid); i++)
    {
        for (int j = 0; j < size(grid); j++)
        {
            if (j == 2 || j == 5)
                cout << grid[i][j] << " | ";
            else
                cout << grid[i][j] << " "; // displays the number in the cell
        }
        //starts a new line each row
        if (i == 2 || i == 5)
            cout << endl << "---------------------" << endl;
        else
            cout << endl;
    }
}

//finds a position that can be changed when not backtracking
void FindValidPos()
{
    //cycles through all cells in the puzzle
    for (int i = 0; i < size(grid); i++)
        for (int j = 0; j < size(grid); j++)
            if (grid[i][j] == 0) { pos[0] = j; pos[1] = i; j = 100; i = 100; } //finds a valid cell when the cells value is equal to 0, the position of the valid cell is stored in pos
}

int FindValidNum()
{
    //bool to tell the program when to stop checking if the number is valid
    bool stop = false;

    //checks through the numbers available to check if they are valid
    for (int i = grid[pos[1]][pos[0]] + 1; i < 10; i++)
    {   
        // checks if that value is in that column //
        for (int y = 0; y < 9; y++)
            if (grid[y][pos[0]] == i) { stop = true; y = 9; }

        if (!stop)
        {
            // checks if that value is in that row //
            for (int x = 0; x < 9; x++)
                if (grid[pos[1]][x] == i) { stop = true; x = 9; }

        }
        if (!stop)
        {
            // checks if that value is in 3x3 that its in //

            if (pos[0] >= 0 && pos[0] <= 2 && pos[1] >= 0 && pos[1] <= 2)
            {
                //cycles through all values in that 3x3
                for (int y = 0; y <= 2; y++)
                    for (int x = 0; x <= 2; x++)
                        if (grid[y][x] == i) { stop = true; x = 9; y = 9; }
            }
            else if (pos[0] >= 3 && pos[0] <= 5 && pos[1] >= 0 && pos[1] <= 2)
            {
                for (int y = 0; y <= 2; y++)
                    for (int x = 3; x <= 5; x++)
                        if (grid[y][x] == i) { stop = true; x = 9; y = 9; }
            }
            else if (pos[0] >= 6 && pos[0] <= 8 && pos[1] >= 0 && pos[1] <= 2)
            {
                for (int y = 0; y <= 2; y++)
                    for (int x = 6; x <= 8; x++)
                        if (grid[y][x] == i) { stop = true; x = 9; y = 9; }
                
            }
            else if (pos[0] >= 0 && pos[0] <= 2 && pos[1] >= 3 && pos[1] <= 5)
            {
                for (int y = 3; y <= 5; y++)
                    for (int x = 0; x <= 2; x++)
                        if (grid[y][x] == i) { stop = true; x = 9; y = 9; }
            }
            else if (pos[0] >= 3 && pos[0] <= 5 && pos[1] >= 3 && pos[1] <= 5)
            {
                for (int y = 3; y <= 5; y++)
                    for (int x = 3; x <= 5; x++)
                        if (grid[y][x] == i) { stop = true; x = 9; y = 9; }
            }
            else if (pos[0] >= 6 && pos[0] <= 8 && pos[1] >= 3 && pos[1] <= 5)
            {
                for (int y = 3; y <= 5; y++)
                    for (int x = 6; x <= 8; x++)
                        if (grid[y][x] == i) { stop = true; x = 9; y = 9; }
            }
            else if (pos[0] >= 0 && pos[0] <= 2 && pos[1] >= 6 && pos[1] <= 8)
            {
                for (int y = 6; y <= 8; y++)
                    for (int x = 0; x <= 2; x++)
                        if (grid[y][x] == i) { stop = true; x = 9; y = 9; }
            }
            else if (pos[0] >= 3 && pos[0] <= 5 && pos[1] >= 6 && pos[1] <= 8)
            {
                for (int y = 6; y <= 8; y++)
                    for (int x = 3; x <= 5; x++)
                        if (grid[y][x] == i) { stop = true; x = 9; y = 9; }
            }
            else if (pos[0] >= 6 && pos[0] <= 8 && pos[1] >= 6 && pos[1] <= 8)
            {
                for (int y = 6; y <= 8; y++)
                    for (int x = 6; x <= 8; x++)
                        if (grid[y][x] == i) { stop = true; x = 9; y = 9; }
            }

        }

        //if the program didn't find any contradictions, then backtracking can be set to false to stop backtracking and returns the valid number
        if (!stop) { backtracking = false; return i; }
        //if the program found contradictions and it cycled through all numbers, then backtracking will be set to true
        else if (stop && i == 9) { grid[pos[1]][pos[0]] = 0; backtracking = true; return 10; }
        //if there were contradictions, but not all numbers have been checked, then it will continue to check
        stop = false;
    }
}

//the back tracking
void BackTrack()
{
    //x and y contains the positions of the cell its checking
    int x = pos[0];
    int y = pos[1];
    //tells the program if a number has been found
    bool numfound = false;

    //will backtrack until a number can be changed
    do 
    {
        //if the backtracking reaches the first value of the row, then it will decrease y by 1 to move to the column ontop and x is set to the last value in that row
        if (x == 0 && y > 0) { x = 8; y--; }
        else //otherwise x is decreased
            x--;

        //since 9 cannot be changed to anyother values, then the value is set to 0 and the program continues to backtrack
        if (grid[y][x] == 9 && !notouch[y][x]) grid[y][x] = 0;
        //if the number can be changed and isn't 9, then the cell the program will be checking is set to the new cell and the program stops backtracking here
        else if (!notouch[y][x]) {  numfound = true; pos[0] = x; pos[1] = y; }
    
    } while (!numfound);
}