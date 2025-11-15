#include <iostream>
#include <vector>
using namespace std;

int solutionCount = 0;

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

// Print the board
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
bool solveNQueens(vector<vector<int>>& board, int row, int n, bool findAll) {
    // Base case: all queens placed
    if (row == n) {
        solutionCount++;
        printBoard(board, n);
        return true;
    }
    
    bool foundSolution = false;
    
    // Try placing queen in each column of current row
    for (int col = 0; col < n; col++) {
        if (isSafe(board, row, col, n)) {
            // Place queen
            board[row][col] = 1;
            
            // Recurse to place rest of queens
            if (solveNQueens(board, row + 1, n, findAll)) {
                foundSolution = true;
                if (!findAll) {
                    return true;  // Stop after first solution
                }
            }
            
            // Backtrack: remove queen
            board[row][col] = 0;
        }
    }
    
    return foundSolution;
}

int main() {
    int n;
    char choice;
    
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
    
    cout << "Find all solutions? (y/n): ";
    cin >> choice;
    bool findAll = (choice == 'y' || choice == 'Y');
    
    // Initialize board
    vector<vector<int>> board(n, vector<int>(n, 0));
    
    cout << "\n========================================" << endl;
    cout << "Solving " << n << "-Queens Problem" << endl;
    cout << "========================================" << endl;
    
    solutionCount = 0;
    
    if (solveNQueens(board, 0, n, findAll)) {
        cout << "\n========================================" << endl;
        cout << "Total solutions found: " << solutionCount << endl;
        cout << "========================================" << endl;
    } else {
        cout << "\nNo solution exists!" << endl;
    }
    
    return 0;
}
