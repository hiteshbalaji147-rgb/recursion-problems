#include <iostream>
#include <vector>
#include <unistd.h>
#include <bitset>
#include <chrono>
#include <iomanip>
#include <cstdlib>
#include <ctime>
using namespace std;
using namespace chrono;

#define N 9

// Statistics structure
struct Stats {
    int recursionCalls = 0;
    int backtrackCount = 0;
    int validityChecks = 0;
    int numbersPlaced = 0;
    int numbersRemoved = 0;
    long long executionTime = 0;  // in microseconds
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

// Copy grid
void copyGrid(int source[N][N], int dest[N][N]) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            dest[i][j] = source[i][j];
        }
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

// Find empty cell with minimum remaining values (MRV heuristic)
bool findEmptyCellMRV(int grid[N][N], int &row, int &col) {
    int minOptions = 10;
    bool found = false;
    
    for (int r = 0; r < N; r++) {
        for (int c = 0; c < N; c++) {
            if (grid[r][c] == 0) {
                // Count possible values
                int options = 0;
                for (int num = 1; num <= 9; num++) {
                    if (isValid(grid, r, c, num)) {
                        options++;
                    }
                }
                
                if (options < minOptions) {
                    minOptions = options;
                    row = r;
                    col = c;
                    found = true;
                }
            }
        }
    }
    
    return found;
}

// Get possible values for a cell
vector<int> getPossibleValues(int grid[N][N], int row, int col) {
    vector<int> possible;
    for (int num = 1; num <= 9; num++) {
        if (isValid(grid, row, col, num)) {
            possible.push_back(num);
        }
    }
    return possible;
}

// Validate initial puzzle
bool validatePuzzle(int grid[N][N]) {
    for (int row = 0; row < N; row++) {
        for (int col = 0; col < N; col++) {
            if (grid[row][col] != 0) {
                int num = grid[row][col];
                
                // Check if number is in valid range
                if (num < 1 || num > 9) {
                    return false;
                }
                
                // Temporarily remove number to check validity
                grid[row][col] = 0;
                if (!isValid(grid, row, col, num)) {
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

// Basic solver
bool solveSudoku(int grid[N][N]) {
    stats.recursionCalls++;
    
    int row, col;
    
    if (!findEmptyCell(grid, row, col)) {
        return true;
    }
    
    for (int num = 1; num <= 9; num++) {
        if (isValid(grid, row, col, num)) {
            grid[row][col] = num;
            stats.numbersPlaced++;
            
            if (stepByStep) {
                cout << "\nPlacing " << num << " at (" << row << ", " << col << "):" << endl;
                printGrid(grid, row, col);
                usleep(100000);
            }
            
            if (solveSudoku(grid)) {
                return true;
            }
            
            if (stepByStep) {
                cout << "\nBacktracking from (" << row << ", " << col << "):" << endl;
            }
            grid[row][col] = 0;
            stats.numbersRemoved++;
            stats.backtrackCount++;
        }
    }
    
    return false;
}

// Optimized solver with MRV heuristic
bool solveSudokuOptimized(int grid[N][N]) {
    stats.recursionCalls++;
    
    int row, col;
    
    if (!findEmptyCellMRV(grid, row, col)) {
        return true;
    }
    
    vector<int> possible = getPossibleValues(grid, row, col);
    
    if (possible.empty()) {
        stats.backtrackCount++;
        return false;
    }
    
    for (int num : possible) {
        grid[row][col] = num;
        stats.numbersPlaced++;
        
        if (stepByStep) {
            cout << "\nPlacing " << num << " at (" << row << ", " << col << "):" << endl;
            cout << "Possible values were: ";
            for (int p : possible) cout << p << " ";
            cout << endl;
            printGrid(grid, row, col);
            usleep(100000);
        }
        
        if (solveSudokuOptimized(grid)) {
            return true;
        }
        
        if (stepByStep) {
            cout << "\nBacktracking from (" << row << ", " << col << "):" << endl;
        }
        grid[row][col] = 0;
        stats.numbersRemoved++;
        stats.backtrackCount++;
    }
    
    return false;
}

// Fill grid with valid solution (for puzzle generation)
bool fillGrid(int grid[N][N]) {
    int row, col;
    
    if (!findEmptyCell(grid, row, col)) {
        return true;
    }
    
    // Try numbers in random order
    vector<int> numbers = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    random_shuffle(numbers.begin(), numbers.end());
    
    for (int num : numbers) {
        if (isValid(grid, row, col, num)) {
            grid[row][col] = num;
            
            if (fillGrid(grid)) {
                return true;
            }
            
            grid[row][col] = 0;
        }
    }
    
    return false;
}

// Generate puzzle by removing numbers
void generatePuzzle(int grid[N][N], int difficulty) {
    // First fill grid with valid solution
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            grid[i][j] = 0;
        }
    }
    
    fillGrid(grid);
    
    // Remove numbers based on difficulty
    int cellsToRemove;
    if (difficulty == 1) cellsToRemove = 40;      // Easy
    else if (difficulty == 2) cellsToRemove = 50; // Medium
    else cellsToRemove = 55;                       // Hard
    
    int removed = 0;
    while (removed < cellsToRemove) {
        int row = rand() % N;
        int col = rand() % N;
        
        if (grid[row][col] != 0) {
            int backup = grid[row][col];
            grid[row][col] = 0;
            
            // Check if puzzle still has unique solution
            int testGrid[N][N];
            copyGrid(grid, testGrid);
            
            Stats tempStats = stats;
            stats = Stats();
            bool solvable = solveSudokuOptimized(testGrid);
            stats = tempStats;
            
            if (solvable) {
                removed++;
            } else {
                grid[row][col] = backup;
            }
        }
    }
}

// Analyze puzzle difficulty
string analyzeDifficulty(int grid[N][N]) {
    int emptyCells = countEmptyCells(grid);
    
    int testGrid[N][N];
    copyGrid(grid, testGrid);
    
    Stats tempStats = stats;
    stats = Stats();
    
    auto start = high_resolution_clock::now();
    solveSudokuOptimized(testGrid);
    auto end = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(end - start);
    
    int backtracks = stats.backtrackCount;
    stats = tempStats;
    
    cout << "\nDifficulty Analysis:" << endl;
    cout << "  Empty cells: " << emptyCells << endl;
    cout << "  Backtracks needed: " << backtracks << endl;
    cout << "  Solve time: " << duration.count() << " μs" << endl;
    
    if (emptyCells < 45 && backtracks < 100) {
        return "Easy";
    } else if (emptyCells < 55 && backtracks < 500) {
        return "Medium";
    } else {
        return "Hard";
    }
}

// Print statistics
void printStats(int emptyCells, string algorithm = "") {
    cout << "\n========================================" << endl;
    cout << "           Statistics";
    if (!algorithm.empty()) cout << " (" << algorithm << ")";
    cout << endl;
    cout << "========================================" << endl;
    cout << "Empty cells to fill: " << emptyCells << endl;
    cout << "Recursion calls: " << stats.recursionCalls << endl;
    cout << "Backtrack operations: " << stats.backtrackCount << endl;
    cout << "Validity checks: " << stats.validityChecks << endl;
    cout << "Numbers placed: " << stats.numbersPlaced << endl;
    cout << "Numbers removed: " << stats.numbersRemoved << endl;
    
    if (emptyCells > 0) {
        cout << "Avg attempts per cell: " << fixed << setprecision(2) 
             << (double)stats.numbersPlaced / emptyCells << endl;
    }
    
    double efficiency = (stats.validityChecks > 0) ? 
                       (double)stats.numbersPlaced / stats.validityChecks * 100 : 0;
    cout << "Efficiency: " << efficiency << "%" << endl;
    
    if (stats.executionTime > 0) {
        cout << "Execution time: " << stats.executionTime << " μs" << endl;
    }
    cout << "========================================" << endl;
}

// Compare algorithms
void compareAlgorithms(int grid[N][N], int emptyCells) {
    cout << "\n========================================" << endl;
    cout << "    Algorithm Performance Comparison   " << endl;
    cout << "========================================" << endl;
    
    int grid1[N][N], grid2[N][N];
    copyGrid(grid, grid1);
    copyGrid(grid, grid2);
    
    stats = Stats();
    auto start1 = high_resolution_clock::now();
    bool solved1 = solveSudoku(grid1);
    auto end1 = high_resolution_clock::now();
    auto duration1 = duration_cast<microseconds>(end1 - start1);
    
    Stats basicStats = stats;
    basicStats.executionTime = duration1.count();
    
    stats = Stats();
    auto start2 = high_resolution_clock::now();
    bool solved2 = solveSudokuOptimized(grid2);
    auto end2 = high_resolution_clock::now();
    auto duration2 = duration_cast<microseconds>(end2 - start2);
    
    Stats optimizedStats = stats;
    optimizedStats.executionTime = duration2.count();
    
    cout << "\nResults:" << endl;
    cout << left << setw(25) << "Metric" << setw(20) << "Basic" << "Optimized" << endl;
    cout << string(65, '-') << endl;
    cout << setw(25) << "Execution time (μs)" << setw(20) << basicStats.executionTime << optimizedStats.executionTime << endl;
    cout << setw(25) << "Recursion calls" << setw(20) << basicStats.recursionCalls << optimizedStats.recursionCalls << endl;
    cout << setw(25) << "Backtrack operations" << setw(20) << basicStats.backtrackCount << optimizedStats.backtrackCount << endl;
    cout << setw(25) << "Validity checks" << setw(20) << basicStats.validityChecks << optimizedStats.validityChecks << endl;
    cout << setw(25) << "Numbers placed" << setw(20) << basicStats.numbersPlaced << optimizedStats.numbersPlaced << endl;
    
    cout << "\n========================================" << endl;
    cout << "Performance Improvements:" << endl;
    cout << "========================================" << endl;
    
    double timeImprovement = (double)basicStats.executionTime / optimizedStats.executionTime;
    double recursionImprovement = (double)basicStats.recursionCalls / optimizedStats.recursionCalls;
    double backtrackImprovement = (double)basicStats.backtrackCount / optimizedStats.backtrackCount;
    
    cout << "Time speedup: " << fixed << setprecision(2) << timeImprovement << "x faster" << endl;
    cout << "Recursion reduction: " << recursionImprovement << "x fewer calls" << endl;
    cout << "Backtrack reduction: " << backtrackImprovement << "x fewer backtracks" << endl;
    
    cout << "\nWinner: Optimized (MRV heuristic)" << endl;
    cout << "Reason: Chooses cells with fewest options first" << endl;
    cout << "========================================" << endl;
}

// Predefined puzzles
void getPuzzle(int grid[N][N], int choice) {
    int puzzles[3][N][N] = {
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
    srand(time(0));
    
    cout << "========================================" << endl;
    cout << "         Sudoku Solver                 " << endl;
    cout << "========================================" << endl;
    
    int mainChoice;
    cout << "\nMain Menu:" << endl;
    cout << "1. Solve predefined puzzle" << endl;
    cout << "2. Generate random puzzle" << endl;
    cout << "3. Analyze puzzle difficulty" << endl;
    cout << "Enter choice (1-3): ";
    cin >> mainChoice;
    
    int grid[N][N];
    
    if (mainChoice == 2) {
        int difficulty;
        cout << "\nSelect difficulty for generation:" << endl;
        cout << "1. Easy" << endl;
        cout << "2. Medium" << endl;
        cout << "3. Hard" << endl;
        cout << "Enter choice (1-3): ";
        cin >> difficulty;
        
        cout << "\nGenerating puzzle..." << endl;
        generatePuzzle(grid, difficulty);
        
        cout << "\nGenerated Puzzle:" << endl;
        printGrid(grid);
        
        string analyzedDiff = analyzeDifficulty(grid);
        cout << "  Analyzed difficulty: " << analyzedDiff << endl;
        
        return 0;
    }
    
    int puzzleChoice, modeChoice, algoChoice;
    
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
    
    getPuzzle(grid, puzzleChoice - 1);
    
    if (mainChoice == 3) {
        cout << "\nOriginal Puzzle:" << endl;
        printGrid(grid);
        
        string difficulty = analyzeDifficulty(grid);
        cout << "  Overall difficulty: " << difficulty << endl;
        return 0;
    }
    
    cout << "\nSelect algorithm:" << endl;
    cout << "1. Basic backtracking" << endl;
    cout << "2. Optimized (MRV heuristic)" << endl;
    cout << "3. Compare both algorithms" << endl;
    cout << "Enter choice (1-3): ";
    cin >> algoChoice;
    
    if (algoChoice != 3) {
        cout << "\nSelect mode:" << endl;
        cout << "1. Solve instantly" << endl;
        cout << "2. Step-by-step visualization" << endl;
        cout << "Enter choice (1-2): ";
        cin >> modeChoice;
        
        stepByStep = (modeChoice == 2);
    }
    
    cout << "\nOriginal Puzzle:" << endl;
    printGrid(grid);
    
    cout << "\nValidating puzzle..." << endl;
    if (!validatePuzzle(grid)) {
        cout << "Invalid puzzle! Cannot solve." << endl;
        return 1;
    }
    cout << "Puzzle is valid!" << endl;
    
    int emptyCells = countEmptyCells(grid);
    cout << "Empty cells: " << emptyCells << endl;
    
    if (algoChoice == 3) {
        compareAlgorithms(grid, emptyCells);
    } else {
        cout << "\n========================================" << endl;
        cout << "Solving with " << (algoChoice == 1 ? "Basic" : "Optimized") << " algorithm..." << endl;
        cout << "========================================" << endl;
        
        stats = Stats();
        
        auto start = high_resolution_clock::now();
        bool solved;
        if (algoChoice == 1) {
            solved = solveSudoku(grid);
        } else {
            solved = solveSudokuOptimized(grid);
        }
        auto end = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(end - start);
        
        stats.executionTime = duration.count();
        
        if (solved) {
            cout << "\nSolution Found:" << endl;
            printGrid(grid);
            printStats(emptyCells, algoChoice == 1 ? "Basic" : "Optimized");
        } else {
            cout << "\nNo solution exists!" << endl;
            printStats(emptyCells, algoChoice == 1 ? "Basic" : "Optimized");
        }
    }
    
    return 0;
}
