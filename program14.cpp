#include <iostream>
#include <vector>
#include <chrono>
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

// Print the chessboard
void printBoard(vector<vector<int>>& board, int n) {
    cout << "\n";
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (board[i][j] == 1) {
                cout << " Q ";
            } else {
                cout << " . ";
            }
        }
        cout << endl;
    }
    cout << endl;
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

// Solve N-Queens using backtracking
bool solveNQueensUtil(vector<vector<int>>& board, int row, int n, bool findAll, 
                      vector<vector<vector<int>>>& solutions) {
    stats.recursionCalls++;
    
    // Base case: all queens placed
    if (row >= n) {
        stats.solutionsFound++;
        solutions.push_back(board);
        return true;
    }
    
    bool foundSolution = false;
    
    // Try placing queen in each column of current row
    for (int col = 0; col < n; col++) {
        if (isSafe(board, row, col, n)) {
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
            board[row][col] = 0;
            stats.backtrackCount++;
        }
    }
    
    return foundSolution;
}

// Main function to solve N-Queens
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

// Print statistics
void printStats() {
    cout << "\n========================================" << endl;
    cout << "           Statistics                  " << endl;
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
    cout << "\nSelect mode:" << endl;
    cout << "1. Find first solution only" << endl;
    cout << "2. Find all solutions" << endl;
    cout << "Enter choice (1-2): ";
    cin >> choice;
    
    bool findAll = (choice == 2);
    
    cout << "\nSolving " << n << "-Queens problem..." << endl;
    
    vector<vector<vector<int>>> solutions = solveNQueens(n, findAll);
    
    cout << "\n========================================" << endl;
    cout << "           Results                     " << endl;
    cout << "========================================" << endl;
    
    if (solutions.empty()) {
        cout << "No solution found!" << endl;
    } else {
        cout << "Found " << solutions.size() << " solution(s):" << endl;
        
        for (int i = 0; i < solutions.size(); i++) {
            cout << "\n--- Solution " << (i + 1) << " ---";
            printBoard(solutions[i], n);
        }
    }
    
    printStats();
    
    return 0;
}
