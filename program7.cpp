#include <iostream>
#include <vector>
using namespace std;

int solutionCount = 0;
int backtrackCount = 0;

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

// Display board
void displayBoard(vector<vector<int>>& board, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cout << (board[i][j] ? "Q " : ". ");
        }
        cout << endl;
    }
}

// Find first solution
bool solveNQueensFirst(vector<vector<int>>& board, int row, int n) {
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
void solveNQueensAll(vector<vector<int>>& board, int row, int n, bool display) {
    // Base case: all queens placed
    if (row >= n) {
        solutionCount++;
        if (display) {
            cout << "\nSolution " << solutionCount << ":" << endl;
            displayBoard(board, n);
        }
        return;
    }
    
    // Try placing queen in each column
    for (int col = 0; col < n; col++) {
        if (isSafe(board, row, col, n)) {
            board[row][col] = 1;
            solveNQueensAll(board, row + 1, n, display);
            board[row][col] = 0; // Backtrack
            backtrackCount++;
        }
    }
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
        cout << "Note: No solution exists for N=" << n << endl;
        return 0;
    }
    
    cout << "\nChoose option:" << endl;
    cout << "1. Find first solution" << endl;
    cout << "2. Find all solutions (count only)" << endl;
    cout << "3. Find all solutions (display all)" << endl;
    cout << "Enter choice (1-3): ";
    cin >> choice;
    
    vector<vector<int>> board(n, vector<int>(n, 0));
    solutionCount = 0;
    backtrackCount = 0;
    
    if (choice == 1) {
        cout << "\nSearching for first solution..." << endl;
        if (solveNQueensFirst(board, 0, n)) {
            cout << "\nSolution found:" << endl;
            displayBoard(board, n);
            cout << "\nBacktrack operations: " << backtrackCount << endl;
        } else {
            cout << "\nNo solution exists!" << endl;
        }
    } else if (choice == 2) {
        cout << "\nCounting all solutions..." << endl;
        solveNQueensAll(board, 0, n, false);
        cout << "\nTotal solutions found: " << solutionCount << endl;
        cout << "Backtrack operations: " << backtrackCount << endl;
    } else if (choice == 3) {
        if (n > 8) {
            cout << "\nWarning: Large board size may produce many solutions!" << endl;
        }
        cout << "\nFinding all solutions..." << endl;
        solveNQueensAll(board, 0, n, true);
        cout << "\n========================================" << endl;
        cout << "Total solutions found: " << solutionCount << endl;
        cout << "Backtrack operations: " << backtrackCount << endl;
        cout << "========================================" << endl;
    }
    
    return 0;
}
