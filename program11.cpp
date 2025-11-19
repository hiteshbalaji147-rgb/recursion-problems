#include <iostream>
#include <vector>
#include <unistd.h>
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
bool stepByStep = false;

// Print the Sudoku grid
void printGrid(int grid[N][N], int highlightRow = -1, int highlightCol = -1) {
    for (int row = 0; row < N; row++) {
        if (row % 3 == 0 && row != 0) {
            cout << "------+-------+------" << endl;
        }
        for (int col = 0; col < N; col++) {
            if (col % 3 == 0 && col != 0) {
                cout << "| ";
            }
            
            if (row == highlightRow && col == highlightCol) {
                cout << "[" << grid[row][col] << "]";
            } else if (grid[row][col] == 0) {
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
            
            if (stepByStep) {
                cout << "\nPlacing " << num << " at (" << row << ", " << col << "):" << endl;
                printGrid(grid, row, col);
                usleep(100000);  // 0.1 second delay
            }
            
            // Recursively solve
            if (solveSudoku(grid)) {
                return true;
            }
            
            // Backtrack
            if (stepByStep) {
                cout << "\nBacktracking from (" << row << ", " << col << "):" << endl;
            }
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
    
    double efficiency = (stats.validityChecks > 0) ? 
                       (double)stats.numbersPlaced / stats.validityChecks * 100 : 0;
    cout << "Efficiency: " << efficiency << "%" << endl;
    cout << "========================================" << endl;
}

// Predefined puzzles
void getPuzzle(int grid[N][N], int choice) {
    int puzzles[3][N][N] = {
        // Easy puzzle
        {
            {5, 3, 0, 0, 7, 0, 0, 0, 0},
            {6, 0, 0, 1, 9, 5, 0, 0, 0},
            {0, 9, 8, 0, 0, 0, 0, 6, 0},
            {8, 0, 0, 0, 6, 0, 0, 0, 3},
            {4, 0, 0, 8, 0, 3, 0, 0, 1},
            {7, 0, 0, 0, 2, 0, 0, 0, 6},
            {0, 6, 0, 0, 0, 0, 2, 8, 0},
            {0, 0, 0, 4, 1, 9, 0, 0, 5},
            {0, 0, 0, 0, 8, 0, 0, 7, 9}
        },
        // Medium puzzle
        {
            {0, 0, 0, 6, 0, 0, 4, 0, 0},
            {7, 0, 0, 0, 0, 3, 6, 0, 0},
            {0, 0, 0, 0, 9, 1, 0, 8, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 5, 0, 1, 8, 0, 0, 0, 3},
            {0, 0, 0, 3, 0, 6, 0, 4, 5},
            {0, 4, 0, 2, 0, 0, 0, 6, 0},
            {9, 0, 3, 0, 0, 0, 0, 0, 0},
            {0, 2, 0, 0, 0, 0, 1, 0, 0}
        },
        // Hard puzzle
        {
            {0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 3, 0, 8, 5},
            {0, 0, 1, 0, 2, 0, 0, 0, 0},
            {0, 0, 0, 5, 0, 7, 0, 0, 0},
            {0, 0, 4, 0, 0, 0, 1, 0, 0},
            {0, 9, 0, 0, 0, 0, 0, 0, 0},
            {5, 0, 0, 0, 0, 0, 0, 7, 3},
            {0, 0, 2, 0, 1, 0, 0, 0, 0},
            {0, 0, 0, 0, 4, 0, 0, 0, 9}
        }
    };
    
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            grid[i][j] = puzzles[choice][i][j];
        }
    }
}

int main() {
    cout << "========================================" << endl;
    cout << "         Sudoku Solver                 " << endl;
    cout << "========================================" << endl;
    
    int puzzleChoice, modeChoice;
    
    cout << "\nSelect puzzle difficulty:" << endl;
    cout << "1. Easy (51 empty cells)" << endl;
    cout << "2. Medium (56 empty cells)" << endl;
    cout << "3. Hard (60 empty cells)" << endl;
    cout << "Enter choice (1-3): ";
    cin >> puzzleChoice;
    
    if (puzzleChoice < 1 || puzzleChoice > 3) {
        cout << "Invalid choice!" << endl;
        return 1;
    }
    
    cout << "\nSelect mode:" << endl;
    cout << "1. Solve instantly" << endl;
    cout << "2. Step-by-step visualization" << endl;
    cout << "Enter choice (1-2): ";
    cin >> modeChoice;
    
    stepByStep = (modeChoice == 2);
    
    int grid[N][N];
    getPuzzle(grid, puzzleChoice - 1);
    
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
