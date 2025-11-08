#include <iostream>
#include <vector>
using namespace std;

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

// Solve N-Queens using backtracking
bool solveNQueens(vector<vector<int>>& board, int row, int n) {
    // Base case: all queens placed
    if (row >= n)
        return true;
    
    // Try placing queen in each column
    for (int col = 0; col < n; col++) {
        if (isSafe(board, row, col, n)) {
            board[row][col] = 1;
            
            if (solveNQueens(board, row + 1, n))
                return true;
            
            board[row][col] = 0; // Backtrack
        }
    }
    
    return false;
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

int main() {
    int n;
    cout << "N-Queens Problem" << endl;
    cout << "Enter board size (N): ";
    cin >> n;
    
    vector<vector<int>> board(n, vector<int>(n, 0));
    
    if (solveNQueens(board, 0, n)) {
        cout << "\nSolution found:" << endl;
        displayBoard(board, n);
    } else {
        cout << "\nNo solution exists!" << endl;
    }
    
    return 0;
}
