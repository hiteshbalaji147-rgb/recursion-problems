#include <iostream>
#include <vector>
using namespace std;

#define N 9

// Statistics structure
struct Stats {
    int recursionCalls = 0;
    int backtrackCount = 0;
    int validityChecks = 0;
    int numbersPlaced = 0;
    int numbersRemoved = 0;
};

Stats stats;

// Print the Sudoku grid
void printGrid(int grid[N][N]) {
    for (int row = 0; row < N; row++) {
        if (row % 3 == 0 && row != 0) {
            cout << "------+-------+------" << endl;
        }
        for (int col = 0; col < N; col++) {
            if (col % 3 == 0 && col != 0) {
                cout << "| ";
            }
            if (grid[row][col] == 0) {
                cout << ". ";
            } else {
                cout << grid[row][col] << " ";
            }
        }
        cout << endl;
    }
}

// Check if placing num at grid[row][col] is valid
bool isValid(int grid[N][N], int row, int col, int num) {
    stats.validityChecks++;
    
    // Check row
    for (int x = 0; x < N; x++) {
        if (grid[row][x] == num) {
            return false;
        }
    }
    
    // Check column
    for (int x = 0; x < N; x++) {
        if (grid[x][col] == num) {
            return false;
        }
    }
    
    // Check 3x3 box
    int startRow = row - row % 3;
    int startCol = col - col % 3;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (grid[i + startRow][j + startCol] == num) {
                return false;
            }
        }
    }
    
    return true;
}

// Find empty cell (returns true if found)
bool findEmptyCell(int grid[N][N], int &row, int &col) {
    for (row = 0; row < N; row++) {
        for (col = 0; col < N; col++) {
            if (grid[row][col] == 0) {
                return true;
            }
        }
    }
    return false;
}

// Validate initial puzzle
bool validatePuzzle(int grid[N][N]) {
    for (int row = 0; row < N; row++) {
        for (int col = 0; col < N; col++) {
            if (grid[row][col] != 0) {
                int num = grid[row][col];
                
                // Check if number is in valid range
                if (num < 1 || num > 9) {
                    cout << "Error: Invalid number " << num << " at (" << row << ", " << col << ")" << endl;
                    return false;
                }
                
                // Temporarily remove number to check validity
                grid[row][col] = 0;
                if (!isValid(grid, row, col, num)) {
                    cout << "Error: Duplicate " << num << " at (" << row << ", " << col << ")" << endl;
                    grid[row][col] = num;
                    return false;
                }
                grid[row][col] = num;
            }
        }
    }
    return true;
}

// Count empty cells
int countEmptyCells(int grid[N][N]) {
    int count = 0;
    for (int row = 0; row < N; row++) {
        for (int col = 0; col < N; col++) {
            if (grid[row][col] == 0) {
                count++;
            }
        }
    }
    return count;
}

// Solve Sudoku using backtracking
bool solveSudoku(int grid[N][N]) {
    stats.recursionCalls++;
    
    int row, col;
    
    // If no empty cell, puzzle is solved
    if (!findEmptyCell(grid, row, col)) {
        return true;
    }
    
    // Try digits 1-9
    for (int num = 1; num <= 9; num++) {
        if (isValid(grid, row, col, num)) {
            // Place number
            grid[row][col] = num;
            stats.numbersPlaced++;
            
            // Recursively solve
            if (solveSudoku(grid)) {
                return true;
            }
            
            // Backtrack
            grid[row][col] = 0;
            stats.numbersRemoved++;
            stats.backtrackCount++;
        }
    }
    
    return false;  // Trigger backtracking
}

// Print statistics
void printStats(int emptyCells) {
    cout << "\n========================================" << endl;
    cout << "           Statistics                  " << endl;
    cout << "========================================" << endl;
    cout << "Empty cells to fill: " << emptyCells << endl;
    cout << "Recursion calls: " << stats.recursionCalls << endl;
    cout << "Backtrack operations: " << stats.backtrackCount << endl;
    cout << "Validity checks: " << stats.validityChecks << endl;
    cout << "Numbers placed: " << stats.numbersPlaced << endl;
    cout << "Numbers removed: " << stats.numbersRemoved << endl;
    
    if (emptyCells > 0) {
        cout << "Avg attempts per cell: " << (double)stats.numbersPlaced / emptyCells << endl;
    }
    cout << "========================================" << endl;
}

int main() {
    cout << "========================================" << endl;
    cout << "         Sudoku Solver                 " << endl;
    cout << "========================================" << endl;
    
    // Example puzzle (0 represents empty cell)
    int grid[N][N] = {
        {5, 3, 0, 0, 7, 0, 0, 0, 0},
        {6, 0, 0, 1, 9, 5, 0, 0, 0},
        {0, 9, 8, 0, 0, 0, 0, 6, 0},
        {8, 0, 0, 0, 6, 0, 0, 0, 3},
        {4, 0, 0, 8, 0, 3, 0, 0, 1},
        {7, 0, 0, 0, 2, 0, 0, 0, 6},
        {0, 6, 0, 0, 0, 0, 2, 8, 0},
        {0, 0, 0, 4, 1, 9, 0, 0, 5},
        {0, 0, 0, 0, 8, 0, 0, 7, 9}
    };
    
    cout << "\nOriginal Puzzle:" << endl;
    printGrid(grid);
    
    // Validate puzzle
    cout << "\nValidating puzzle..." << endl;
    if (!validatePuzzle(grid)) {
        cout << "Invalid puzzle! Cannot solve." << endl;
        return 1;
    }
    cout << "Puzzle is valid!" << endl;
    
    int emptyCells = countEmptyCells(grid);
    cout << "Empty cells: " << emptyCells << endl;
    
    cout << "\n========================================" << endl;
    cout << "Solving..." << endl;
    cout << "========================================" << endl;
    
    stats = Stats();  // Reset statistics
    
    if (solveSudoku(grid)) {
        cout << "\nSolution Found:" << endl;
        printGrid(grid);
        printStats(emptyCells);
    } else {
        cout << "\nNo solution exists!" << endl;
        printStats(emptyCells);
    }
    
    return 0;
}
