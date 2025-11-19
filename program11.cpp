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
    long long executionTime = 0;
};

Stats stats;
bool stepByStep = false;
int solutionCount = 0;

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
    
    for (int x = 0; x < N; x++) {
        if (grid[row][x] == num) return false;
    }
    
    for (int x = 0; x < N; x++) {
        if (grid[x][col] == num) return false;
    }
    
    int startRow = row - row % 3;
    int startCol = col - col % 3;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (grid[i + startRow][j + startCol] == num) return false;
        }
    }
    
    return true;
}

// Find empty cell
bool findEmptyCell(int grid[N][N], int &row, int &col) {
    for (row = 0; row < N; row++) {
        for (col = 0; col < N; col++) {
            if (grid[row][col] == 0) return true;
        }
    }
    return false;
}

// Find empty cell with MRV heuristic
bool findEmptyCellMRV(int grid[N][N], int &row, int &col) {
    int minOptions = 10;
    bool found = false;
    
    for (int r = 0; r < N; r++) {
        for (int c = 0; c < N; c++) {
            if (grid[r][c] == 0) {
                int options = 0;
                for (int num = 1; num <= 9; num++) {
                    if (isValid(grid, r, c, num)) options++;
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

// Validate puzzle
bool validatePuzzle(int grid[N][N]) {
    for (int row = 0; row < N; row++) {
        for (int col = 0; col < N; col++) {
            if (grid[row][col] != 0) {
                int num = grid[row][col];
                if (num < 1 || num > 9) return false;
                
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
            if (grid[row][col] == 0) count++;
        }
    }
    return count;
}

// Basic solver
bool solveSudoku(int grid[N][N]) {
    stats.recursionCalls++;
    
    int row, col;
    if (!findEmptyCell(grid, row, col)) return true;
    
    for (int num = 1; num <= 9; num++) {
        if (isValid(grid, row, col, num)) {
            grid[row][col] = num;
            stats.numbersPlaced++;
            
            if (stepByStep) {
                cout << "\nPlacing " << num << " at (" << row << ", " << col << "):" << endl;
                printGrid(grid, row, col);
                usleep(100000);
            }
            
            if (solveSudoku(grid)) return true;
            
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

// Optimized solver
bool solveSudokuOptimized(int grid[N][N]) {
    stats.recursionCalls++;
    
    int row, col;
    if (!findEmptyCellMRV(grid, row, col)) return true;
    
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
        
        if (solveSudokuOptimized(grid)) return true;
        
        if (stepByStep) {
            cout << "\nBacktracking from (" << row << ", " << col << "):" << endl;
        }
        grid[row][col] = 0;
        stats.numbersRemoved++;
        stats.backtrackCount++;
    }
    
    return false;
}

// Count all solutions (for uniqueness check)
bool countSolutions(int grid[N][N], int &count, int maxCount = 2) {
    int row, col;
    if (!findEmptyCell(grid, row, col)) {
        count++;
        return count < maxCount;  // Continue if we haven't reached maxCount
    }
    
    for (int num = 1; num <= 9; num++) {
        if (isValid(grid, row, col, num)) {
            grid[row][col] = num;
            
            if (!countSolutions(grid, count, maxCount)) {
                grid[row][col] = 0;
                return false;  // Stop early if we found enough solutions
            }
            
            grid[row][col] = 0;
        }
    }
    
    return true;
}

// Check if puzzle has unique solution
bool hasUniqueSolution(int grid[N][N]) {
    int testGrid[N][N];
    copyGrid(grid, testGrid);
    
    int count = 0;
    countSolutions(testGrid, count, 2);
    
    return count == 1;
}

// Get hint (find easiest cell to fill)
void getHint(int grid[N][N]) {
    int bestRow = -1, bestCol = -1;
    int minOptions = 10;
    vector<int> bestPossible;
    
    for (int r = 0; r < N; r++) {
        for (int c = 0; c < N; c++) {
            if (grid[r][c] == 0) {
                vector<int> possible = getPossibleValues(grid, r, c);
                
                if (possible.size() > 0 && possible.size() < minOptions) {
                    minOptions = possible.size();
                    bestRow = r;
                    bestCol = c;
                    bestPossible = possible;
                }
            }
        }
    }
    
    if (bestRow != -1) {
        cout << "\n========================================" << endl;
        cout << "              Hint                     " << endl;
        cout << "========================================" << endl;
        cout << "Easiest cell: (" << bestRow << ", " << bestCol << ")" << endl;
        cout << "Possible values: ";
        for (int val : bestPossible) {
            cout << val << " ";
        }
        cout << endl;
        
        if (bestPossible.size() == 1) {
            cout << "This cell has only ONE possible value!" << endl;
            cout << "You should place: " << bestPossible[0] << endl;
        } else {
            cout << "This cell has " << bestPossible.size() << " possible values." << endl;
        }
        cout << "========================================" << endl;
    } else {
        cout << "\nNo empty cells found!" << endl;
    }
}

// Fill grid with valid solution
bool fillGrid(int grid[N][N]) {
    int row, col;
    if (!findEmptyCell(grid, row, col)) return true;
    
    vector<int> numbers = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    random_shuffle(numbers.begin(), numbers.end());
    
    for (int num : numbers) {
        if (isValid(grid, row, col, num)) {
            grid[row][col] = num;
            if (fillGrid(grid)) return true;
            grid[row][col] = 0;
        }
    }
    
    return false;
}

// Generate puzzle
void generatePuzzle(int grid[N][N], int difficulty) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            grid[i][j] = 0;
        }
    }
    
    fillGrid(grid);
    
    int cellsToRemove;
    if (difficulty == 1) cellsToRemove = 40;
    else if (difficulty == 2) cellsToRemove = 50;
    else cellsToRemove = 55;
    
    int removed = 0;
    while (removed < cellsToRemove) {
        int row = rand() % N;
        int col = rand() % N;
        
        if (grid[row][col] != 0) {
            int backup = grid[row][col];
            grid[row][col] = 0;
            
            if (hasUniqueSolution(grid)) {
                removed++;
            } else {
                grid[row][col] = backup;
            }
        }
    }
}

// Analyze difficulty
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
    solveSudoku(grid1);
    auto end1 = high_resolution_clock::now();
    auto duration1 = duration_cast<microseconds>(end1 - start1);
    
    Stats basicStats = stats;
    basicStats.executionTime = duration1.count();
    
    stats = Stats();
    auto start2 = high_resolution_clock::now();
    solveSudokuOptimized(grid2);
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
    
    cout << "\n========================================" << endl;
    cout << "Performance Improvements:" << endl;
    cout << "========================================" << endl;
    
    double timeImprovement = (double)basicStats.executionTime / optimizedStats.executionTime;
    double recursionImprovement = (double)basicStats.recursionCalls / optimizedStats.recursionCalls;
    
    cout << "Time speedup: " << fixed << setprecision(2) << timeImprovement << "x faster" << endl;
    cout << "Recursion reduction: " << recursionImprovement << "x fewer calls" << endl;
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
    cout << "4. Get hint for puzzle" << endl;
    cout << "5. Check solution uniqueness" << endl;
    cout << "Enter choice (1-5): ";
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
        
        cout << "\nGenerating puzzle with unique solution..." << endl;
        generatePuzzle(grid, difficulty);
        
        cout << "\nGenerated Puzzle:" << endl;
        printGrid(grid);
        
        string analyzedDiff = analyzeDifficulty(grid);
        cout << "  Analyzed difficulty: " << analyzedDiff << endl;
        
        cout << "\nVerifying uniqueness..." << endl;
        if (hasUniqueSolution(grid)) {
            cout << "✓ Puzzle has a unique solution!" << endl;
        } else {
            cout << "✗ Puzzle has multiple solutions!" << endl;
        }
        
        return 0;
    }
    
    int puzzleChoice;
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
    
    cout << "\nOriginal Puzzle:" << endl;
    printGrid(grid);
    
    if (mainChoice == 3) {
        string difficulty = analyzeDifficulty(grid);
        cout << "  Overall difficulty: " << difficulty << endl;
        return 0;
    }
    
    if (mainChoice == 4) {
        getHint(grid);
        return 0;
    }
    
    if (mainChoice == 5) {
        cout << "\nChecking solution uniqueness..." << endl;
        if (hasUniqueSolution(grid)) {
            cout << "✓ Puzzle has a unique solution!" << endl;
        } else {
            cout << "✗ Puzzle has multiple solutions!" << endl;
        }
        return 0;
    }
    
    int algoChoice, modeChoice;
    
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
