#include <iostream>
#include <vector>
using namespace std;

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
}

// Solve N-Queens using backtracking
bool solveNQueens(vector<vector<int>>& board, int row, int n, bool findAll, bool showAttacks) {
    recursionCalls++;
    
    // Base case: all queens placed
    if (row == n) {
        solutionCount++;
        if (showAttacks) {
            printBoardWithAttacks(board, n);
        } else {
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
            
            // Recurse to place rest of queens
            if (solveNQueens(board, row + 1, n, findAll, showAttacks)) {
                foundSolution = true;
                if (!findAll) {
                    return true;  // Stop after first solution
                }
            }
            
            // Backtrack: remove queen
            board[row][col] = 0;
            backtrackCount++;
        }
    }
    
    return foundSolution;
}

// Print statistics
void printStats(int n) {
    cout << "\n========================================" << endl;
    cout << "           Statistics                  " << endl;
    cout << "========================================" << endl;
    cout << "Total solutions: " << solutionCount << endl;
    cout << "Recursion calls: " << recursionCalls << endl;
    cout << "Backtrack operations: " << backtrackCount << endl;
    cout << "Safety checks: " << stats.totalChecks << endl;
    cout << "  - Safe positions: " << stats.safePositions << endl;
    cout << "  - Unsafe positions: " << stats.unsafePositions << endl;
    cout << "Queens placed (total): " << stats.queensPlaced << endl;
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
    cout << "Enter choice (1-3): ";
    cin >> choice;
    
    bool findAll = (choice == 2 || choice == 3);
    bool showAttacks = (choice == 3);
    
    // Initialize board
    vector<vector<int>> board(n, vector<int>(n, 0));
    
    cout << "\n========================================" << endl;
    cout << "Solving " << n << "-Queens Problem" << endl;
    cout << "========================================" << endl;
    
    solutionCount = 0;
    backtrackCount = 0;
    recursionCalls = 0;
    stats = Stats();
    
    if (solveNQueens(board, 0, n, findAll, showAttacks)) {
        printStats(n);
    } else {
        cout << "\nNo solution exists!" << endl;
    }
    
    return 0;
}
