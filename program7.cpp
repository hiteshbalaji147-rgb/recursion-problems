#include <iostream>
#include <vector>
#include <iomanip>
#include <chrono>
using namespace std;
using namespace chrono;

int solutionCount = 0;
int backtrackCount = 0;
int recursionCalls = 0;
int safetyChecks = 0;

// Check if placing queen at (row, col) is safe
bool isSafe(vector<vector<int>>& board, int row, int col, int n) {
    safetyChecks++;
    
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

// Optimized safety check using arrays
bool isSafeOptimized(vector<bool>& cols, vector<bool>& diag1, vector<bool>& diag2, 
                     int row, int col, int n) {
    safetyChecks++;
    return !cols[col] && !diag1[row - col + n - 1] && !diag2[row + col];
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

// Find first solution (basic)
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

// Find first solution (optimized)
bool solveNQueensOptimized(vector<vector<int>>& board, vector<bool>& cols, 
                          vector<bool>& diag1, vector<bool>& diag2, int row, int n) {
    recursionCalls++;
    
    if (row >= n)
        return true;
    
    for (int col = 0; col < n; col++) {
        if (isSafeOptimized(cols, diag1, diag2, row, col, n)) {
            board[row][col] = 1;
            cols[col] = diag1[row - col + n - 1] = diag2[row + col] = true;
            
            if (solveNQueensOptimized(board, cols, diag1, diag2, row + 1, n))
                return true;
            
            board[row][col] = 0;
            cols[col] = diag1[row - col + n - 1] = diag2[row + col] = false;
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

// Known solution counts for validation
int getKnownSolutionCount(int n) {
    int known[] = {0, 1, 0, 0, 2, 10, 4, 40, 92, 352, 724, 2680, 14200, 73712, 365596};
    if (n < 15) return known[n];
    return -1; // Unknown
}

int main() {
    int n, choice, displayStyle, algorithm;
    
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
    cout << "4. Compare algorithms" << endl;
    cout << "Enter choice (1-4): ";
    cin >> choice;
    
    if (choice == 1) {
        cout << "\nAlgorithm:" << endl;
        cout << "1. Basic backtracking" << endl;
        cout << "2. Optimized (with arrays)" << endl;
        cout << "Enter algorithm (1-2): ";
        cin >> algorithm;
    }
    
    if (choice == 3) {
        cout << "\nDisplay style:" << endl;
        cout << "1. Simple (Q and .)" << endl;
        cout << "2. Enhanced (with coordinates)" << endl;
        cout << "3. Attack visualization" << endl;
        cout << "Enter style (1-3): ";
        cin >> displayStyle;
    }
    
    vector<vector<int>> board(n, vector<int>(n, 0));
    
    if (choice == 1) {
        solutionCount = 0;
        backtrackCount = 0;
        recursionCalls = 0;
        safetyChecks = 0;
        
        cout << "\nSearching for first solution..." << endl;
        auto start = high_resolution_clock::now();
        
        bool found = false;
        if (algorithm == 1) {
            found = solveNQueensFirst(board, 0, n);
        } else {
            vector<bool> cols(n, false);
            vector<bool> diag1(2 * n - 1, false);
            vector<bool> diag2(2 * n - 1, false);
            found = solveNQueensOptimized(board, cols, diag1, diag2, 0, n);
        }
        
        auto end = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(end - start);
        
        if (found) {
            cout << "\n========================================" << endl;
            cout << "Solution found:" << endl;
            cout << "========================================" << endl;
            displayBoardEnhanced(board, n);
            cout << "\nPerformance Metrics:" << endl;
            cout << "  Execution time: " << duration.count() << " μs" << endl;
            cout << "  Recursion calls: " << recursionCalls << endl;
            cout << "  Safety checks: " << safetyChecks << endl;
            cout << "  Backtrack operations: " << backtrackCount << endl;
            cout << "  Avg time per call: " << fixed << setprecision(2) 
                 << (double)duration.count() / recursionCalls << " μs" << endl;
        } else {
            cout << "\nNo solution exists!" << endl;
        }
    } else if (choice == 2 || choice == 3) {
        solutionCount = 0;
        backtrackCount = 0;
        recursionCalls = 0;
        safetyChecks = 0;
        
        if (n > 8 && choice == 3) {
            cout << "\nWarning: Large board size may produce many solutions!" << endl;
        }
        
        cout << "\nFinding all solutions..." << endl;
        auto start = high_resolution_clock::now();
        solveNQueensAll(board, 0, n, choice == 3, displayStyle);
        auto end = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(end - start);
        
        cout << "\n========================================" << endl;
        cout << "Performance Metrics:" << endl;
        cout << "  Total solutions: " << solutionCount << endl;
        
        int known = getKnownSolutionCount(n);
        if (known != -1) {
            cout << "  Expected solutions: " << known;
            cout << (solutionCount == known ? " ✓" : " ✗") << endl;
        }
        
        cout << "  Execution time: " << duration.count() << " μs";
        if (duration.count() > 1000000) {
            cout << " (" << fixed << setprecision(2) << duration.count() / 1000000.0 << " s)";
        }
        cout << endl;
        cout << "  Recursion calls: " << recursionCalls << endl;
        cout << "  Safety checks: " << safetyChecks << endl;
        cout << "  Backtrack operations: " << backtrackCount << endl;
        cout << "  Solutions per second: " << fixed << setprecision(0) 
             << (solutionCount * 1000000.0) / duration.count() << endl;
        cout << "\nComplexity Analysis:" << endl;
        cout << "  Time Complexity: O(N!)" << endl;
        cout << "  Space Complexity: O(N²)" << endl;
        cout << "========================================" << endl;
    }
    
    return 0;
}
