#include <iostream>
#include <vector>
#include <iomanip>
using namespace std;

int solutionCount = 0;
int backtrackCount = 0;
int recursionCalls = 0;

// Check if placing queen at (row, col) is safe
bool isSafe(vector<vector<int>>& board, int row, int col, int n) {
    // Check column
    for (int i = 0; i < row; i++) {
        if (board[i][col] == 1)
            return false;
    }
    
    // Check upper left diagonal
    for (int i = row, j = col; i >= 0 && j >= 0; i--, j--) {
        if (board[i][j] == 1)
            return false;
    }
    
    // Check upper right diagonal
    for (int i = row, j = col; i >= 0 && j < n; i--, j++) {
        if (board[i][j] == 1)
            return false;
    }
    
    return true;
}

// Display board with simple ASCII
void displayBoardSimple(vector<vector<int>>& board, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cout << (board[i][j] ? "Q " : ". ");
        }
        cout << endl;
    }
}

// Display board with enhanced visualization
void displayBoardEnhanced(vector<vector<int>>& board, int n) {
    // Top border
    cout << "  ";
    for (int j = 0; j < n; j++) {
        cout << " " << j << " ";
    }
    cout << endl;
    
    cout << "  ";
    for (int j = 0; j < n; j++) {
        cout << "---";
    }
    cout << endl;
    
    // Board with row numbers
    for (int i = 0; i < n; i++) {
        cout << i << "|";
        for (int j = 0; j < n; j++) {
            if (board[i][j]) {
                cout << " Q ";
            } else {
                // Checkerboard pattern
                cout << ((i + j) % 2 == 0 ? " . " : " · ");
            }
        }
        cout << "|" << i << endl;
    }
    
    // Bottom border
    cout << "  ";
    for (int j = 0; j < n; j++) {
        cout << "---";
    }
    cout << endl;
    
    cout << "  ";
    for (int j = 0; j < n; j++) {
        cout << " " << j << " ";
    }
    cout << endl;
}

// Display board with attack visualization
void displayBoardWithAttacks(vector<vector<int>>& board, int n) {
    vector<vector<int>> attacks(n, vector<int>(n, 0));
    
    // Mark attacked squares
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (board[i][j] == 1) {
                // Mark row
                for (int k = 0; k < n; k++) attacks[i][k]++;
                // Mark column
                for (int k = 0; k < n; k++) attacks[k][j]++;
                // Mark diagonals
                for (int k = 0; k < n; k++) {
                    if (i + k < n && j + k < n) attacks[i + k][j + k]++;
                    if (i - k >= 0 && j - k >= 0) attacks[i - k][j - k]++;
                    if (i + k < n && j - k >= 0) attacks[i + k][j - k]++;
                    if (i - k >= 0 && j + k < n) attacks[i - k][j + k]++;
                }
            }
        }
    }
    
    // Display
    cout << "  ";
    for (int j = 0; j < n; j++) cout << " " << j << " ";
    cout << endl;
    
    for (int i = 0; i < n; i++) {
        cout << i << "|";
        for (int j = 0; j < n; j++) {
            if (board[i][j]) {
                cout << " Q ";
            } else if (attacks[i][j] > 0) {
                cout << " x ";
            } else {
                cout << " . ";
            }
        }
        cout << "|" << i << endl;
    }
}

// Find first solution
bool solveNQueensFirst(vector<vector<int>>& board, int row, int n) {
    recursionCalls++;
    
    // Base case: all queens placed
    if (row >= n)
        return true;
    
    // Try placing queen in each column
    for (int col = 0; col < n; col++) {
        if (isSafe(board, row, col, n)) {
            board[row][col] = 1;
            
            if (solveNQueensFirst(board, row + 1, n))
                return true;
            
            board[row][col] = 0; // Backtrack
            backtrackCount++;
        }
    }
    
    return false;
}

// Find all solutions
void solveNQueensAll(vector<vector<int>>& board, int row, int n, bool display, int displayStyle) {
    recursionCalls++;
    
    // Base case: all queens placed
    if (row >= n) {
        solutionCount++;
        if (display) {
            cout << "\n========================================" << endl;
            cout << "Solution " << solutionCount << ":" << endl;
            cout << "========================================" << endl;
            
            if (displayStyle == 1) {
                displayBoardSimple(board, n);
            } else if (displayStyle == 2) {
                displayBoardEnhanced(board, n);
            } else {
                displayBoardWithAttacks(board, n);
            }
        }
        return;
    }
    
    // Try placing queen in each column
    for (int col = 0; col < n; col++) {
        if (isSafe(board, row, col, n)) {
            board[row][col] = 1;
            solveNQueensAll(board, row + 1, n, display, displayStyle);
            board[row][col] = 0; // Backtrack
            backtrackCount++;
        }
    }
}

int main() {
    int n, choice, displayStyle;
    
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
        cout << "Note: No solution exists for N=" << n << endl;
        return 0;
    }
    
    cout << "\nChoose option:" << endl;
    cout << "1. Find first solution" << endl;
    cout << "2. Find all solutions (count only)" << endl;
    cout << "3. Find all solutions (display all)" << endl;
    cout << "Enter choice (1-3): ";
    cin >> choice;
    
    if (choice == 3) {
        cout << "\nDisplay style:" << endl;
        cout << "1. Simple (Q and .)" << endl;
        cout << "2. Enhanced (with coordinates)" << endl;
        cout << "3. Attack visualization" << endl;
        cout << "Enter style (1-3): ";
        cin >> displayStyle;
    }
    
    vector<vector<int>> board(n, vector<int>(n, 0));
    solutionCount = 0;
    backtrackCount = 0;
    recursionCalls = 0;
    
    if (choice == 1) {
        cout << "\nSearching for first solution..." << endl;
        if (solveNQueensFirst(board, 0, n)) {
            cout << "\n========================================" << endl;
            cout << "Solution found:" << endl;
            cout << "========================================" << endl;
            displayBoardEnhanced(board, n);
            cout << "\nStatistics:" << endl;
            cout << "  Recursion calls: " << recursionCalls << endl;
            cout << "  Backtrack operations: " << backtrackCount << endl;
        } else {
            cout << "\nNo solution exists!" << endl;
        }
    } else if (choice == 2) {
        cout << "\nCounting all solutions..." << endl;
        solveNQueensAll(board, 0, n, false, 1);
        cout << "\n========================================" << endl;
        cout << "Statistics:" << endl;
        cout << "  Total solutions: " << solutionCount << endl;
        cout << "  Recursion calls: " << recursionCalls << endl;
        cout << "  Backtrack operations: " << backtrackCount << endl;
        cout << "========================================" << endl;
    } else if (choice == 3) {
        if (n > 8) {
            cout << "\nWarning: Large board size may produce many solutions!" << endl;
        }
        cout << "\nFinding all solutions..." << endl;
        solveNQueensAll(board, 0, n, true, displayStyle);
        cout << "\n========================================" << endl;
        cout << "Statistics:" << endl;
        cout << "  Total solutions: " << solutionCount << endl;
        cout << "  Recursion calls: " << recursionCalls << endl;
        cout << "  Backtrack operations: " << backtrackCount << endl;
        cout << "========================================" << endl;
    }
    
    return 0;
}
