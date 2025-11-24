#include <iostream>
#include <vector>
#include <chrono>
#include <unistd.h>
#include <set>
#include <iomanip>
using namespace std;
using namespace chrono;

#define N 9

// Statistics structure
struct Stats {
    int recursionCalls = 0;
    int backtrackCount = 0;
    int cellsFilled = 0;
    long long executionTime = 0;
};

Stats stats;
bool stepByStep = false;

// Print the Sudoku grid
void printGrid(int grid[N][N]) {
    cout << "\n";
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
    cout << endl;
}

// Print grid with colors
void printGridColored(int grid[N][N], int original[N][N]) {
    cout << "\n";
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
            } else if (original[row][col] != 0) {
                // Original numbers (given)
                cout << grid[row][col] << " ";
            } else {
                // Solved numbers (filled by algorithm)
                cout << "\033[1;32m" << grid[row][col] << "\033[0m ";
            }
        }
        cout << endl;
    }
    cout << endl;
}

// Highlight current cell
void printGridWithHighlight(int grid[N][N], int original[N][N], int hRow, int hCol) {
    cout << "\n";
    for (int row = 0; row < N; row++) {
        if (row % 3 == 0 && row != 0) {
            cout << "------+-------+------" << endl;
        }
        for (int col = 0; col < N; col++) {
            if (col % 3 == 0 && col != 0) {
                cout << "| ";
            }
            
            if (row == hRow && col == hCol) {
                // Highlight current cell
                cout << "\033[1;33m[" << (grid[row][col] == 0 ? '.' : char('0' + grid[row][col])) << "]\033[0m";
            } else if (grid[row][col] == 0) {
                cout << ". ";
            } else if (original[row][col] != 0) {
                cout << grid[row][col] << " ";
            } else {
                cout << "\033[1;32m" << grid[row][col] << "\033[0m ";
            }
        }
        cout << endl;
    }
    cout << endl;
}

// Check if it's safe to place num at grid[row][col]
bool isSafe(int grid[N][N], int row, int col, int num) {
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

// Find empty location in grid
bool findEmptyLocation(int grid[N][N], int &row, int &col) {
    for (row = 0; row < N; row++) {
        for (col = 0; col < N; col++) {
            if (grid[row][col] == 0) {
                return true;
            }
        }
    }
    return false;
}

// Find empty location with minimum possibilities (MRV heuristic)
bool findEmptyLocationOptimized(int grid[N][N], int &row, int &col) {
    int minPossibilities = 10;
    bool found = false;
    
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (grid[i][j] == 0) {
                int possibilities = 0;
                for (int num = 1; num <= 9; num++) {
                    if (isSafe(grid, i, j, num)) {
                        possibilities++;
                    }
                }
                
                if (possibilities < minPossibilities) {
                    minPossibilities = possibilities;
                    row = i;
                    col = j;
                    found = true;
                }
            }
        }
    }
    
    return found;
}

// Solve Sudoku using backtracking
bool solveSudoku(int grid[N][N], int original[N][N] = nullptr) {
    stats.recursionCalls++;
    
    int row, col;
    
    // If no empty location, puzzle is solved
    if (!findEmptyLocation(grid, row, col)) {
        return true;
    }
    
    if (stepByStep && original) {
        cout << "\nTrying cell (" << row << ", " << col << "):" << endl;
        printGridWithHighlight(grid, original, row, col);
        usleep(200000);
    }
    
    // Try digits 1 to 9
    for (int num = 1; num <= 9; num++) {
        if (isSafe(grid, row, col, num)) {
            if (stepByStep && original) {
                cout << "→ Placing " << num << " at (" << row << ", " << col << ")" << endl;
            }
            
            // Place number
            grid[row][col] = num;
            stats.cellsFilled++;
            
            // Recursively solve rest
            if (solveSudoku(grid, original)) {
                return true;
            }
            
            // Backtrack
            if (stepByStep && original) {
                cout << "← Backtracking from (" << row << ", " << col << ")" << endl;
            }
            
            grid[row][col] = 0;
            stats.backtrackCount++;
        }
    }
    
    return false;
}

// Solve Sudoku using optimized backtracking (MRV heuristic)
bool solveSudokuOptimized(int grid[N][N]) {
    stats.recursionCalls++;
    
    int row, col;
    
    // If no empty location, puzzle is solved
    if (!findEmptyLocationOptimized(grid, row, col)) {
        return true;
    }
    
    // Try digits 1 to 9
    for (int num = 1; num <= 9; num++) {
        if (isSafe(grid, row, col, num)) {
            // Place number
            grid[row][col] = num;
            stats.cellsFilled++;
            
            // Recursively solve rest
            if (solveSudokuOptimized(grid)) {
                return true;
            }
            
            // Backtrack
            grid[row][col] = 0;
            stats.backtrackCount++;
        }
    }
    
    return false;
}

// Validate Sudoku solution
bool validateSudoku(int grid[N][N]) {
    // Check all rows
    for (int row = 0; row < N; row++) {
        set<int> seen;
        for (int col = 0; col < N; col++) {
            if (grid[row][col] == 0) return false;
            if (seen.count(grid[row][col])) return false;
            seen.insert(grid[row][col]);
        }
    }
    
    // Check all columns
    for (int col = 0; col < N; col++) {
        set<int> seen;
        for (int row = 0; row < N; row++) {
            if (seen.count(grid[row][col])) return false;
            seen.insert(grid[row][col]);
        }
    }
    
    // Check all 3x3 boxes
    for (int boxRow = 0; boxRow < 3; boxRow++) {
        for (int boxCol = 0; boxCol < 3; boxCol++) {
            set<int> seen;
            for (int i = 0; i < 3; i++) {
                for (int j = 0; j < 3; j++) {
                    int row = boxRow * 3 + i;
                    int col = boxCol * 3 + j;
                    if (seen.count(grid[row][col])) return false;
                    seen.insert(grid[row][col]);
                }
            }
        }
    }
    
    return true;
}

// Count empty cells
int countEmptyCells(int grid[N][N]) {
    int count = 0;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (grid[i][j] == 0) {
                count++;
            }
        }
    }
    return count;
}

// Copy grid
void copyGrid(int source[N][N], int dest[N][N]) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            dest[i][j] = source[i][j];
        }
    }
}

// Print statistics
void printStats(string algorithm = "") {
    cout << "\n========================================" << endl;
    cout << "           Statistics";
    if (!algorithm.empty()) cout << " (" << algorithm << ")";
    cout << endl;
    cout << "========================================" << endl;
    cout << "Recursion calls: " << stats.recursionCalls << endl;
    cout << "Cells filled: " << stats.cellsFilled << endl;
    cout << "Backtrack operations: " << stats.backtrackCount << endl;
    cout << "Execution time: " << stats.executionTime << " μs" << endl;
    cout << "========================================" << endl;
}

// Print complexity analysis
void printComplexityAnalysis() {
    cout << "\n========================================" << endl;
    cout << "      Complexity Analysis              " << endl;
    cout << "========================================" << endl;
    cout << "\n1. Basic Backtracking:" << endl;
    cout << "   Time: O(9^m) where m = empty cells" << endl;
    cout << "   Space: O(m) for recursion stack" << endl;
    cout << "   Worst case: tries all possibilities" << endl;
    
    cout << "\n2. Optimized (MRV Heuristic):" << endl;
    cout << "   Time: O(9^m) but with better pruning" << endl;
    cout << "   Space: O(m) for recursion stack" << endl;
    cout << "   Improvement: Chooses cells with" << endl;
    cout << "   minimum remaining values first" << endl;
    
    cout << "\n3. Practical Performance:" << endl;
    cout << "   Easy puzzles: < 1ms" << endl;
    cout << "   Medium puzzles: 1-10ms" << endl;
    cout << "   Hard puzzles: 10-100ms" << endl;
    cout << "   Extreme puzzles: 100ms-1s" << endl;
    
    cout << "\nNote: m = number of empty cells" << endl;
    cout << "========================================" << endl;
}

// Input custom puzzle
void inputCustomPuzzle(int grid[N][N]) {
    cout << "\nEnter Sudoku puzzle (use 0 for empty cells):" << endl;
    cout << "Enter 9 rows of 9 numbers each:" << endl;
    
    for (int i = 0; i < N; i++) {
        cout << "Row " << (i + 1) << ": ";
        for (int j = 0; j < N; j++) {
            cin >> grid[i][j];
            if (grid[i][j] < 0 || grid[i][j] > 9) {
                cout << "Invalid input! Use numbers 0-9 only." << endl;
                j--;
            }
        }
    }
}

int main() {
    cout << "========================================" << endl;
    cout << "         Sudoku Solver                 " << endl;
    cout << "========================================" << endl;
    
    // Multiple test puzzles
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
    
    string difficulty[] = {"Easy", "Medium", "Hard"};
    
    int mode;
    cout << "\nSelect mode:" << endl;
    cout << "1. Solve preset puzzle" << endl;
    cout << "2. Input custom puzzle" << endl;
    cout << "3. Compare algorithms" << endl;
    cout << "Enter choice (1-3): ";
    cin >> mode;
    
    if (mode < 1 || mode > 3) {
        cout << "Invalid choice!" << endl;
        return 1;
    }
    
    int grid[N][N];
    int original[N][N];
    
    if (mode == 1 || mode == 3) {
        int choice;
        cout << "\nSelect puzzle difficulty:" << endl;
        cout << "1. Easy" << endl;
        cout << "2. Medium" << endl;
        cout << "3. Hard" << endl;
        cout << "Enter choice (1-3): ";
        cin >> choice;
        
        if (choice < 1 || choice > 3) {
            cout << "Invalid choice!" << endl;
            return 1;
        }
        
        copyGrid(puzzles[choice - 1], grid);
        copyGrid(puzzles[choice - 1], original);
        
        cout << "\n========================================" << endl;
        cout << "    " << difficulty[choice - 1] << " Puzzle" << endl;
        cout << "========================================" << endl;
    } else {
        inputCustomPuzzle(grid);
        copyGrid(grid, original);
        
        cout << "\n========================================" << endl;
        cout << "    Custom Puzzle" << endl;
        cout << "========================================" << endl;
    }
    
    int emptyCells = countEmptyCells(grid);
    cout << "Empty cells: " << emptyCells << endl;
    cout << "Given cells: " << (81 - emptyCells) << endl;
    
    cout << "\nOriginal Sudoku Puzzle:" << endl;
    printGrid(grid);
    
    if (mode == 3) {
        // Compare algorithms
        cout << "\n========================================" << endl;
        cout << "    Algorithm Comparison               " << endl;
        cout << "========================================" << endl;
        
        int grid1[N][N], grid2[N][N];
        copyGrid(original, grid1);
        copyGrid(original, grid2);
        
        // Basic backtracking
        stats = Stats();
        auto start1 = high_resolution_clock::now();
        bool solved1 = solveSudoku(grid1);
        auto end1 = high_resolution_clock::now();
        stats.executionTime = duration_cast<microseconds>(end1 - start1).count();
        Stats stats1 = stats;
        
        // Optimized backtracking
        stats = Stats();
        auto start2 = high_resolution_clock::now();
        bool solved2 = solveSudokuOptimized(grid2);
        auto end2 = high_resolution_clock::now();
        stats.executionTime = duration_cast<microseconds>(end2 - start2).count();
        Stats stats2 = stats;
        
        cout << "\nResults:" << endl;
        cout << left << setw(25) << "Algorithm" << setw(12) << "Solved" 
             << setw(15) << "Time (μs)" << setw(15) << "Recursions" 
             << setw(15) << "Backtracks" << endl;
        cout << string(82, '-') << endl;
        cout << setw(25) << "Basic Backtracking" << setw(12) << (solved1 ? "Yes" : "No")
             << setw(15) << stats1.executionTime << setw(15) << stats1.recursionCalls 
             << setw(15) << stats1.backtrackCount << endl;
        cout << setw(25) << "Optimized (MRV)" << setw(12) << (solved2 ? "Yes" : "No")
             << setw(15) << stats2.executionTime << setw(15) << stats2.recursionCalls 
             << setw(15) << stats2.backtrackCount << endl;
        
        if (stats2.executionTime > 0) {
            cout << "\nSpeedup: " << fixed << setprecision(2) 
                 << (double)stats1.executionTime / stats2.executionTime << "x" << endl;
        }
        
        cout << "\nSolved Sudoku:" << endl;
        printGridColored(grid2, original);
        
    } else {
        int algorithm;
        cout << "\nSelect algorithm:" << endl;
        cout << "1. Basic backtracking" << endl;
        cout << "2. Optimized backtracking (MRV heuristic)" << endl;
        cout << "Enter choice (1-2): ";
        cin >> algorithm;
        
        if (algorithm < 1 || algorithm > 2) {
            cout << "Invalid choice!" << endl;
            return 1;
        }
        
        if (algorithm == 1) {
            int vizMode;
            cout << "\nSelect visualization:" << endl;
            cout << "1. Instant solution" << endl;
            cout << "2. Step-by-step visualization" << endl;
            cout << "Enter choice (1-2): ";
            cin >> vizMode;
            
            stepByStep = (vizMode == 2);
        }
        
        stats = Stats();
        
        auto start = high_resolution_clock::now();
        bool solved;
        
        if (algorithm == 1) {
            solved = solveSudoku(grid, original);
        } else {
            solved = solveSudokuOptimized(grid);
        }
        
        auto end = high_resolution_clock::now();
        
        stats.executionTime = duration_cast<microseconds>(end - start).count();
        
        if (solved) {
            cout << "\n✓ Solved Successfully!" << endl;
            cout << "\nSolved Sudoku (green = filled by algorithm):" << endl;
            printGridColored(grid, original);
            
            bool valid = validateSudoku(grid);
            cout << "Validation: " << (valid ? "✓ Valid solution" : "✗ Invalid solution") << endl;
        } else {
            cout << "\n✗ No solution exists!" << endl;
        }
        
        printStats(algorithm == 1 ? "Basic" : "Optimized");
    }
    
    printComplexityAnalysis();
    
    return 0;
}
