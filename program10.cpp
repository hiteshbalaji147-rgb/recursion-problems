#include <iostream>
#include <vector>
#include <bitset>
#include <chrono>
#include <iomanip>
#include <set>
using namespace std;
using namespace chrono;

int solutionCount = 0;
int backtrackCount = 0;
int recursionCalls = 0;

// Statistics structure
struct Stats {
    int totalChecks = 0;
    int safePositions = 0;
    int unsafePositions = 0;
    int queensPlaced = 0;
};

Stats stats;

// Store unique solutions
set<vector<int>> uniqueSolutions;

// Check if placing queen at (row, col) is safe
bool isSafe(vector<vector<int>>& board, int row, int col, int n) {
    stats.totalChecks++;
    
    // Check column
    for (int i = 0; i < row; i++) {
        if (board[i][col] == 1) {
            stats.unsafePositions++;
            return false;
        }
    }
    
    // Check upper left diagonal
    for (int i = row, j = col; i >= 0 && j >= 0; i--, j--) {
        if (board[i][j] == 1) {
            stats.unsafePositions++;
            return false;
        }
    }
    
    // Check upper right diagonal
    for (int i = row, j = col; i >= 0 && j < n; i--, j++) {
        if (board[i][j] == 1) {
            stats.unsafePositions++;
            return false;
        }
    }
    
    stats.safePositions++;
    return true;
}

// Convert board to solution vector (column position for each row)
vector<int> boardToSolution(vector<vector<int>>& board, int n) {
    vector<int> solution(n);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (board[i][j] == 1) {
                solution[i] = j;
                break;
            }
        }
    }
    return solution;
}

// Mark attacked positions
void markAttacks(vector<vector<int>>& attacks, int row, int col, int n) {
    // Mark column
    for (int i = 0; i < n; i++) {
        attacks[i][col]++;
    }
    
    // Mark row
    for (int j = 0; j < n; j++) {
        attacks[row][j]++;
    }
    
    // Mark diagonals
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (i - j == row - col || i + j == row + col) {
                attacks[i][j]++;
            }
        }
    }
}

// Print the board with attack visualization
void printBoardWithAttacks(vector<vector<int>>& board, int n) {
    vector<vector<int>> attacks(n, vector<int>(n, 0));
    
    // Calculate attacks
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (board[i][j] == 1) {
                markAttacks(attacks, i, j, n);
            }
        }
    }
    
    cout << "\nSolution " << solutionCount << ":" << endl;
    cout << "Board:" << endl;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (board[i][j] == 1) {
                cout << "Q ";
            } else {
                cout << ". ";
            }
        }
        cout << endl;
    }
    
    cout << "\nAttack Map (number of queens attacking each square):" << endl;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (board[i][j] == 1) {
                cout << "Q ";
            } else {
                cout << attacks[i][j] << " ";
            }
        }
        cout << endl;
    }
}

// Print the board (simple)
void printBoard(vector<vector<int>>& board, int n) {
    cout << "\nSolution " << solutionCount << ":" << endl;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (board[i][j] == 1) {
                cout << "Q ";
            } else {
                cout << ". ";
            }
        }
        cout << endl;
    }
    
    // Print solution vector
    vector<int> sol = boardToSolution(board, n);
    cout << "Position vector: [";
    for (int i = 0; i < n; i++) {
        cout << sol[i];
        if (i < n - 1) cout << ", ";
    }
    cout << "]" << endl;
}

// Print step-by-step board state
void printStep(vector<vector<int>>& board, int n, int currentRow, int currentCol, bool placing) {
    cout << "\n" << (placing ? "Placing" : "Removing") << " queen at (" << currentRow << ", " << currentCol << "):" << endl;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (i == currentRow && j == currentCol) {
                cout << (placing ? "Q*" : "X ");
            } else if (board[i][j] == 1) {
                cout << "Q ";
            } else {
                cout << ". ";
            }
        }
        cout << endl;
    }
}

// Solve N-Queens using backtracking
bool solveNQueens(vector<vector<int>>& board, int row, int n, bool findAll, bool showAttacks, bool stepByStep) {
    recursionCalls++;
    
    // Base case: all queens placed
    if (row == n) {
        solutionCount++;
        vector<int> sol = boardToSolution(board, n);
        uniqueSolutions.insert(sol);
        
        if (showAttacks) {
            printBoardWithAttacks(board, n);
        } else if (!stepByStep) {
            printBoard(board, n);
        }
        return true;
    }
    
    bool foundSolution = false;
    
    // Try placing queen in each column of current row
    for (int col = 0; col < n; col++) {
        if (isSafe(board, row, col, n)) {
            // Place queen
            board[row][col] = 1;
            stats.queensPlaced++;
            
            if (stepByStep) {
                printStep(board, n, row, col, true);
            }
            
            // Recurse to place rest of queens
            if (solveNQueens(board, row + 1, n, findAll, showAttacks, stepByStep)) {
                foundSolution = true;
                if (!findAll) {
                    return true;  // Stop after first solution
                }
            }
            
            // Backtrack: remove queen
            if (stepByStep && (findAll || !foundSolution)) {
                printStep(board, n, row, col, false);
            }
            board[row][col] = 0;
            backtrackCount++;
        }
    }
    
    return foundSolution;
}

// Optimized solution using bitmasking
int solveNQueensBitwise(int row, int n, int cols, int diag1, int diag2, bool countOnly) {
    if (row == n) {
        return 1;
    }
    
    int count = 0;
    int availablePositions = ((1 << n) - 1) & ~(cols | diag1 | diag2);
    
    while (availablePositions) {
        int position = availablePositions & -availablePositions;  // Get rightmost bit
        availablePositions -= position;  // Remove this bit
        
        count += solveNQueensBitwise(row + 1, n, cols | position, 
                                      (diag1 | position) << 1, 
                                      (diag2 | position) >> 1, countOnly);
    }
    
    return count;
}

// Print statistics
void printStats(int n) {
    cout << "\n========================================" << endl;
    cout << "           Statistics                  " << endl;
    cout << "========================================" << endl;
    cout << "Total solutions: " << solutionCount << endl;
    cout << "Unique solutions: " << uniqueSolutions.size() << endl;
    cout << "Recursion calls: " << recursionCalls << endl;
    cout << "Backtrack operations: " << backtrackCount << endl;
    cout << "Safety checks: " << stats.totalChecks << endl;
    cout << "  - Safe positions: " << stats.safePositions << endl;
    cout << "  - Unsafe positions: " << stats.unsafePositions << endl;
    cout << "Queens placed (total): " << stats.queensPlaced << endl;
    
    // Calculate efficiency
    double efficiency = (stats.totalChecks > 0) ? 
                       (double)stats.safePositions / stats.totalChecks * 100 : 0;
    cout << "Efficiency: " << fixed << setprecision(2) << efficiency << "% safe positions" << endl;
    
    // Average backtracks per solution
    if (solutionCount > 0) {
        cout << "Avg backtracks per solution: " << (double)backtrackCount / solutionCount << endl;
    }
    cout << "========================================" << endl;
}

// Known solution counts for N-Queens
int getKnownSolutionCount(int n) {
    int knownCounts[] = {0, 1, 0, 0, 2, 10, 4, 40, 92, 352, 724, 2680, 14200, 73712, 365596};
    if (n < 15) return knownCounts[n];
    return -1;  // Unknown
}

// Compare algorithms
void compareAlgorithms(int n) {
    cout << "\n========================================" << endl;
    cout << "    Algorithm Performance Comparison   " << endl;
    cout << "========================================" << endl;
    
    // Test standard backtracking
    vector<vector<int>> board(n, vector<int>(n, 0));
    solutionCount = 0;
    backtrackCount = 0;
    recursionCalls = 0;
    stats = Stats();
    uniqueSolutions.clear();
    
    auto start1 = high_resolution_clock::now();
    solveNQueens(board, 0, n, true, false, false);
    auto end1 = high_resolution_clock::now();
    auto duration1 = duration_cast<microseconds>(end1 - start1);
    
    int standardSolutions = solutionCount;
    int standardRecursions = recursionCalls;
    int standardBacktracks = backtrackCount;
    
    // Test bitwise solution
    auto start2 = high_resolution_clock::now();
    int bitwiseSolutions = solveNQueensBitwise(0, n, 0, 0, 0, true);
    auto end2 = high_resolution_clock::now();
    auto duration2 = duration_cast<microseconds>(end2 - start2);
    
    // Print comparison
    cout << "\nResults:" << endl;
    cout << left << setw(25) << "Metric" << setw(20) << "Standard" << "Bitwise" << endl;
    cout << string(65, '-') << endl;
    cout << setw(25) << "Execution time (μs)" << setw(20) << duration1.count() << duration2.count() << endl;
    cout << setw(25) << "Solutions found" << setw(20) << standardSolutions << bitwiseSolutions << endl;
    cout << setw(25) << "Recursion calls" << setw(20) << standardRecursions << "N/A" << endl;
    cout << setw(25) << "Backtrack operations" << setw(20) << standardBacktracks << "N/A" << endl;
    cout << setw(25) << "Space complexity" << setw(20) << "O(n²)" << "O(n)" << endl;
    cout << setw(25) << "Time complexity" << setw(20) << "O(n!)" << "O(n!)" << endl;
    
    long long faster = min(duration1.count(), duration2.count());
    string winner = (faster == duration1.count()) ? "Standard" : "Bitwise";
    
    cout << "\n========================================" << endl;
    cout << "Winner: " << winner << " algorithm" << endl;
    cout << "Speedup: " << fixed << setprecision(2) 
         << (double)max(duration1.count(), duration2.count()) / faster << "x faster" << endl;
    
    // Verify against known solution count
    int known = getKnownSolutionCount(n);
    if (known != -1) {
        cout << "Known solution count: " << known << endl;
        cout << "Verification: " << (standardSolutions == known ? "✓ PASS" : "✗ FAIL") << endl;
    }
    
    cout << "========================================" << endl;
}

int main() {
    int n, choice;
    
    cout << "========================================" << endl;
    cout << "         N-Queens Problem              " << endl;
    cout << "========================================" << endl;
    
    cout << "Enter board size (N): ";
    cin >> n;
    
    if (n <= 0) {
        cout << "Error: Board size must be positive!" << endl;
        return 1;
    }
    
    if (n == 2 || n == 3) {
        cout << "Note: No solution exists for N = " << n << endl;
        return 0;
    }
    
    cout << "\nChoose option:" << endl;
    cout << "1. Find first solution only" << endl;
    cout << "2. Find all solutions" << endl;
    cout << "3. Find all solutions with attack visualization" << endl;
    cout << "4. Step-by-step solution (first solution only)" << endl;
    cout << "5. Count solutions only (optimized bitwise)" << endl;
    cout << "6. Compare algorithms (Standard vs Bitwise)" << endl;
    cout << "Enter choice (1-6): ";
    cin >> choice;
    
    cout << "\n========================================" << endl;
    cout << "Solving " << n << "-Queens Problem" << endl;
    cout << "========================================" << endl;
    
    if (choice == 5) {
        // Optimized bitwise solution
        auto start = high_resolution_clock::now();
        int count = solveNQueensBitwise(0, n, 0, 0, 0, true);
        auto end = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(end - start);
        
        cout << "\nTotal solutions: " << count << endl;
        cout << "Execution time: " << duration.count() << " μs" << endl;
        cout << "Algorithm: Optimized bitwise (no board storage)" << endl;
        cout << "Space Complexity: O(n) - recursion stack only" << endl;
        cout << "Time Complexity: O(n!) - but faster due to bitwise ops" << endl;
        
        int known = getKnownSolutionCount(n);
        if (known != -1) {
            cout << "\nKnown solution count: " << known << endl;
            cout << "Verification: " << (count == known ? "✓ PASS" : "✗ FAIL") << endl;
        }
    } else if (choice == 6) {
        compareAlgorithms(n);
    } else {
        bool findAll = (choice == 2 || choice == 3);
        bool showAttacks = (choice == 3);
        bool stepByStep = (choice == 4);
        
        // Initialize board
        vector<vector<int>> board(n, vector<int>(n, 0));
        
        solutionCount = 0;
        backtrackCount = 0;
        recursionCalls = 0;
        stats = Stats();
        uniqueSolutions.clear();
        
        auto start = high_resolution_clock::now();
        if (solveNQueens(board, 0, n, findAll, showAttacks, stepByStep)) {
            auto end = high_resolution_clock::now();
            auto duration = duration_cast<microseconds>(end - start);
            
            printStats(n);
            cout << "\nExecution time: " << duration.count() << " μs" << endl;
            
            int known = getKnownSolutionCount(n);
            if (known != -1 && findAll) {
                cout << "Known solution count: " << known << endl;
                cout << "Verification: " << (solutionCount == known ? "✓ PASS" : "✗ FAIL") << endl;
            }
        } else {
            cout << "\nNo solution exists!" << endl;
        }
    }
    
    return 0;
}
