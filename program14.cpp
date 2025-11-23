#include <iostream>
#include <vector>
#include <chrono>
#include <unistd.h>
#include <set>
#include <iomanip>
using namespace std;
using namespace chrono;

// Statistics structure
struct Stats {
    int recursionCalls = 0;
    int solutionsFound = 0;
    int backtrackCount = 0;
    long long executionTime = 0;
};

Stats stats;
bool stepByStep = false;

// Print the chessboard with colors
void printBoard(vector<vector<int>>& board, int n) {
    cout << "\n  ";
    for (int i = 0; i < n; i++) {
        cout << " " << i << " ";
    }
    cout << endl;
    
    for (int i = 0; i < n; i++) {
        cout << i << " ";
        for (int j = 0; j < n; j++) {
            if (board[i][j] == 1) {
                cout << " Q ";
            } else if (board[i][j] == -1) {
                cout << " X "; // Attacked position
            } else {
                cout << " . ";
            }
        }
        cout << endl;
    }
    cout << endl;
}

// Print board with Unicode chess pieces
void printBoardUnicode(vector<vector<int>>& board, int n) {
    cout << "\n  ";
    for (int i = 0; i < n; i++) {
        cout << " " << i << " ";
    }
    cout << endl;
    
    for (int i = 0; i < n; i++) {
        cout << i << " ";
        for (int j = 0; j < n; j++) {
            if (board[i][j] == 1) {
                cout << " ♛ ";
            } else {
                // Checkerboard pattern
                if ((i + j) % 2 == 0) {
                    cout << " ░ ";
                } else {
                    cout << " ▓ ";
                }
            }
        }
        cout << endl;
    }
    cout << endl;
}

// Mark all attacked positions
void markAttackedPositions(vector<vector<int>>& board, int n) {
    vector<vector<int>> attacked(n, vector<int>(n, 0));
    
    // Copy queen positions
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (board[i][j] == 1) {
                attacked[i][j] = 1;
            }
        }
    }
    
    // Mark attacked positions
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (board[i][j] == 1) {
                // Mark row
                for (int k = 0; k < n; k++) {
                    if (attacked[i][k] != 1) attacked[i][k] = -1;
                }
                
                // Mark column
                for (int k = 0; k < n; k++) {
                    if (attacked[k][j] != 1) attacked[k][j] = -1;
                }
                
                // Mark diagonals
                for (int k = 0; k < n; k++) {
                    for (int l = 0; l < n; l++) {
                        if (abs(i - k) == abs(j - l) && attacked[k][l] != 1) {
                            attacked[k][l] = -1;
                        }
                    }
                }
            }
        }
    }
    
    board = attacked;
}

// Print board with attacked positions
void printBoardWithAttacks(vector<vector<int>>& board, int n) {
    vector<vector<int>> attacked = board;
    markAttackedPositions(attacked, n);
    
    cout << "\n  ";
    for (int i = 0; i < n; i++) {
        cout << " " << i << " ";
    }
    cout << endl;
    
    for (int i = 0; i < n; i++) {
        cout << i << " ";
        for (int j = 0; j < n; j++) {
            if (attacked[i][j] == 1) {
                cout << " ♛ ";
            } else if (attacked[i][j] == -1) {
                cout << " × ";
            } else {
                if ((i + j) % 2 == 0) {
                    cout << " ░ ";
                } else {
                    cout << " ▓ ";
                }
            }
        }
        cout << endl;
    }
    cout << endl;
}

// Validate if solution is correct
bool validateSolution(vector<vector<int>>& board, int n) {
    int queenCount = 0;
    
    // Count queens
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (board[i][j] == 1) {
                queenCount++;
            }
        }
    }
    
    if (queenCount != n) {
        return false;
    }
    
    // Check if any two queens attack each other
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (board[i][j] == 1) {
                // Check row
                for (int k = j + 1; k < n; k++) {
                    if (board[i][k] == 1) return false;
                }
                
                // Check column
                for (int k = i + 1; k < n; k++) {
                    if (board[k][j] == 1) return false;
                }
                
                // Check diagonals
                for (int k = 1; k < n; k++) {
                    if (i + k < n && j + k < n && board[i + k][j + k] == 1) return false;
                    if (i + k < n && j - k >= 0 && board[i + k][j - k] == 1) return false;
                }
            }
        }
    }
    
    return true;
}

// Get queen positions as string
string getQueenPositions(vector<vector<int>>& board, int n) {
    string result = "";
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (board[i][j] == 1) {
                result += "(" + to_string(i) + "," + to_string(j) + ") ";
            }
        }
    }
    return result;
}

// Check if placing queen at (row, col) is safe
bool isSafe(vector<vector<int>>& board, int row, int col, int n) {
    // Check column
    for (int i = 0; i < row; i++) {
        if (board[i][col] == 1) {
            return false;
        }
    }
    
    // Check upper left diagonal
    for (int i = row, j = col; i >= 0 && j >= 0; i--, j--) {
        if (board[i][j] == 1) {
            return false;
        }
    }
    
    // Check upper right diagonal
    for (int i = row, j = col; i >= 0 && j < n; i--, j++) {
        if (board[i][j] == 1) {
            return false;
        }
    }
    
    return true;
}

// Optimized safety check using sets
bool isSafeOptimized(int row, int col, set<int>& cols, set<int>& diag1, set<int>& diag2) {
    return cols.find(col) == cols.end() &&
           diag1.find(row - col) == diag1.end() &&
           diag2.find(row + col) == diag2.end();
}

// Solve N-Queens using backtracking
bool solveNQueensUtil(vector<vector<int>>& board, int row, int n, bool findAll, 
                      vector<vector<vector<int>>>& solutions) {
    stats.recursionCalls++;
    
    if (stepByStep) {
        cout << "\nTrying to place queen in row " << row << endl;
        printBoard(board, n);
        usleep(300000);
    }
    
    // Base case: all queens placed
    if (row >= n) {
        stats.solutionsFound++;
        solutions.push_back(board);
        
        if (stepByStep) {
            cout << "\n✓ SOLUTION FOUND!" << endl;
            printBoardUnicode(board, n);
            usleep(1000000);
        }
        
        return true;
    }
    
    bool foundSolution = false;
    
    // Try placing queen in each column of current row
    for (int col = 0; col < n; col++) {
        if (isSafe(board, row, col, n)) {
            if (stepByStep) {
                cout << "→ Placing queen at (" << row << ", " << col << ")" << endl;
            }
            
            // Place queen
            board[row][col] = 1;
            
            // Recurse to place rest of queens
            if (solveNQueensUtil(board, row + 1, n, findAll, solutions)) {
                foundSolution = true;
                if (!findAll) {
                    return true; // Stop after first solution
                }
            }
            
            // Backtrack
            if (stepByStep) {
                cout << "← Backtracking from (" << row << ", " << col << ")" << endl;
            }
            
            board[row][col] = 0;
            stats.backtrackCount++;
        }
    }
    
    return foundSolution;
}

// Optimized N-Queens using sets for O(1) conflict checking
bool solveNQueensOptimizedUtil(vector<vector<int>>& board, int row, int n,
                                set<int>& cols, set<int>& diag1, set<int>& diag2,
                                bool findAll, vector<vector<vector<int>>>& solutions) {
    stats.recursionCalls++;
    
    // Base case: all queens placed
    if (row >= n) {
        stats.solutionsFound++;
        solutions.push_back(board);
        return true;
    }
    
    bool foundSolution = false;
    
    for (int col = 0; col < n; col++) {
        if (isSafeOptimized(row, col, cols, diag1, diag2)) {
            // Place queen
            board[row][col] = 1;
            cols.insert(col);
            diag1.insert(row - col);
            diag2.insert(row + col);
            
            // Recurse
            if (solveNQueensOptimizedUtil(board, row + 1, n, cols, diag1, diag2, findAll, solutions)) {
                foundSolution = true;
                if (!findAll) {
                    return true;
                }
            }
            
            // Backtrack
            board[row][col] = 0;
            cols.erase(col);
            diag1.erase(row - col);
            diag2.erase(row + col);
            stats.backtrackCount++;
        }
    }
    
    return foundSolution;
}

// Main function to solve N-Queens (basic)
vector<vector<vector<int>>> solveNQueens(int n, bool findAll = true) {
    vector<vector<int>> board(n, vector<int>(n, 0));
    vector<vector<vector<int>>> solutions;
    
    stats = Stats();
    
    auto start = high_resolution_clock::now();
    solveNQueensUtil(board, 0, n, findAll, solutions);
    auto end = high_resolution_clock::now();
    
    stats.executionTime = duration_cast<microseconds>(end - start).count();
    
    return solutions;
}

// Main function to solve N-Queens (optimized)
vector<vector<vector<int>>> solveNQueensOptimized(int n, bool findAll = true) {
    vector<vector<int>> board(n, vector<int>(n, 0));
    vector<vector<vector<int>>> solutions;
    set<int> cols, diag1, diag2;
    
    stats = Stats();
    
    auto start = high_resolution_clock::now();
    solveNQueensOptimizedUtil(board, 0, n, cols, diag1, diag2, findAll, solutions);
    auto end = high_resolution_clock::now();
    
    stats.executionTime = duration_cast<microseconds>(end - start).count();
    
    return solutions;
}

// Count total solutions for N-Queens
int countSolutions(int n) {
    vector<vector<int>> board(n, vector<int>(n, 0));
    vector<vector<vector<int>>> solutions;
    set<int> cols, diag1, diag2;
    
    stats = Stats();
    
    auto start = high_resolution_clock::now();
    solveNQueensOptimizedUtil(board, 0, n, cols, diag1, diag2, true, solutions);
    auto end = high_resolution_clock::now();
    
    stats.executionTime = duration_cast<microseconds>(end - start).count();
    
    return solutions.size();
}

// Print statistics
void printStats(string algorithm = "") {
    cout << "\n========================================" << endl;
    cout << "           Statistics";
    if (!algorithm.empty()) cout << " (" << algorithm << ")";
    cout << endl;
    cout << "========================================" << endl;
    cout << "Recursion calls: " << stats.recursionCalls << endl;
    cout << "Solutions found: " << stats.solutionsFound << endl;
    cout << "Backtrack operations: " << stats.backtrackCount << endl;
    cout << "Execution time: " << stats.executionTime << " μs" << endl;
    cout << "========================================" << endl;
}

int main() {
    cout << "========================================" << endl;
    cout << "         N-Queens Problem              " << endl;
    cout << "========================================" << endl;
    
    int n;
    cout << "\nEnter the value of N (board size): ";
    cin >> n;
    
    if (n < 1) {
        cout << "Invalid input! N must be at least 1." << endl;
        return 1;
    }
    
    if (n == 2 || n == 3) {
        cout << "\nNo solution exists for N = " << n << endl;
        return 0;
    }
    
    int choice;
    cout << "\nSelect algorithm:" << endl;
    cout << "1. Basic backtracking" << endl;
    cout << "2. Optimized backtracking (with sets)" << endl;
    cout << "3. Count all solutions" << endl;
    cout << "4. Validate custom solution" << endl;
    cout << "5. Compare algorithms" << endl;
    cout << "Enter choice (1-5): ";
    cin >> choice;
    
    if (choice == 1 || choice == 2) {
        int mode;
        cout << "\nSelect mode:" << endl;
        cout << "1. Find first solution only" << endl;
        cout << "2. Find all solutions" << endl;
        cout << "Enter choice (1-2): ";
        cin >> mode;
        
        bool findAll = (mode == 2);
        
        if (choice == 1) {
            int vizMode;
            cout << "\nSelect visualization:" << endl;
            cout << "1. Instant solution" << endl;
            cout << "2. Step-by-step visualization" << endl;
            cout << "Enter choice (1-2): ";
            cin >> vizMode;
            
            stepByStep = (vizMode == 2);
        }
        
        cout << "\nSolving " << n << "-Queens problem..." << endl;
        
        vector<vector<vector<int>>> solutions;
        
        if (choice == 1) {
            solutions = solveNQueens(n, findAll);
        } else {
            solutions = solveNQueensOptimized(n, findAll);
        }
        
        cout << "\n========================================" << endl;
        cout << "           Results                     " << endl;
        cout << "========================================" << endl;
        
        if (solutions.empty()) {
            cout << "No solution found!" << endl;
        } else {
            cout << "Found " << solutions.size() << " solution(s):" << endl;
            
            int displayLimit = min((int)solutions.size(), 3);
            for (int i = 0; i < displayLimit; i++) {
                cout << "\n--- Solution " << (i + 1) << " ---";
                cout << "\nQueen positions: " << getQueenPositions(solutions[i], n);
                printBoardUnicode(solutions[i], n);
                
                cout << "With attacked positions:";
                printBoardWithAttacks(solutions[i], n);
                
                cout << "Valid: " << (validateSolution(solutions[i], n) ? "✓ Yes" : "✗ No") << endl;
            }
            
            if (solutions.size() > 3) {
                cout << "\n... and " << (solutions.size() - 3) << " more solutions" << endl;
            }
        }
        
        printStats(choice == 1 ? "Basic" : "Optimized");
        
    } else if (choice == 3) {
        cout << "\nCounting all solutions for " << n << "-Queens..." << endl;
        
        int count = countSolutions(n);
        
        cout << "\n========================================" << endl;
        cout << "           Results                     " << endl;
        cout << "========================================" << endl;
        cout << "Total number of solutions: " << count << endl;
        
        printStats("Optimized");
        
    } else if (choice == 4) {
        cout << "\nEnter queen positions (row col) for " << n << " queens:" << endl;
        vector<vector<int>> board(n, vector<int>(n, 0));
        
        for (int i = 0; i < n; i++) {
            int row, col;
            cout << "Queen " << (i + 1) << ": ";
            cin >> row >> col;
            
            if (row >= 0 && row < n && col >= 0 && col < n) {
                board[row][col] = 1;
            } else {
                cout << "Invalid position!" << endl;
                i--;
            }
        }
        
        cout << "\n========================================" << endl;
        cout << "           Validation                  " << endl;
        cout << "========================================" << endl;
        
        cout << "\nYour solution:";
        printBoardUnicode(board, n);
        
        cout << "With attacked positions:";
        printBoardWithAttacks(board, n);
        
        bool valid = validateSolution(board, n);
        cout << "\nValidation result: " << (valid ? "✓ VALID SOLUTION!" : "✗ INVALID - Queens attack each other") << endl;
        
    } else if (choice == 5) {
        cout << "\n========================================" << endl;
        cout << "    Algorithm Comparison               " << endl;
        cout << "========================================" << endl;
        
        // Basic backtracking
        auto solutions1 = solveNQueens(n, true);
        Stats stats1 = stats;
        
        // Optimized backtracking
        auto solutions2 = solveNQueensOptimized(n, true);
        Stats stats2 = stats;
        
        cout << "\nResults:" << endl;
        cout << left << setw(25) << "Algorithm" << setw(12) << "Solutions" 
             << setw(15) << "Time (μs)" << setw(15) << "Recursions" 
             << setw(15) << "Backtracks" << endl;
        cout << string(82, '-') << endl;
        cout << setw(25) << "Basic Backtracking" << setw(12) << solutions1.size()
             << setw(15) << stats1.executionTime << setw(15) << stats1.recursionCalls 
             << setw(15) << stats1.backtrackCount << endl;
        cout << setw(25) << "Optimized (Sets)" << setw(12) << solutions2.size()
             << setw(15) << stats2.executionTime << setw(15) << stats2.recursionCalls 
             << setw(15) << stats2.backtrackCount << endl;
        
        cout << "\nSpeedup: " << fixed << setprecision(2) 
             << (double)stats1.executionTime / stats2.executionTime << "x" << endl;
        cout << "========================================" << endl;
    }
    
    return 0;
}
