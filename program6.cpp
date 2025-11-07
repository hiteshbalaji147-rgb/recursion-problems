#include <iostream>
#include <stack>
#include <vector>
#include <chrono>
#include <cmath>
using namespace std;
using namespace chrono;

int moveCount = 0;
int recursionDepth = 0;
int maxRecursionDepth = 0;
stack<int> towerA, towerB, towerC;

// Display current state of towers
void displayTowers(int n) {
    cout << "\nCurrent State:" << endl;
    cout << "----------------------------------------" << endl;
    
    // Create temporary stacks to preserve original
    stack<int> tempA = towerA, tempB = towerB, tempC = towerC;
    vector<int> a, b, c;
    
    while (!tempA.empty()) { a.push_back(tempA.top()); tempA.pop(); }
    while (!tempB.empty()) { b.push_back(tempB.top()); tempB.pop(); }
    while (!tempC.empty()) { c.push_back(tempC.top()); tempC.pop(); }
    
    // Display from top to bottom
    for (int i = n - 1; i >= 0; i--) {
        cout << "  ";
        if (i < a.size()) cout << "[" << a[i] << "]"; else cout << " | ";
        cout << "    ";
        if (i < b.size()) cout << "[" << b[i] << "]"; else cout << " | ";
        cout << "    ";
        if (i < c.size()) cout << "[" << c[i] << "]"; else cout << " | ";
        cout << endl;
    }
    
    cout << " ---    ---    ---" << endl;
    cout << "  A      B      C" << endl;
    cout << "----------------------------------------" << endl;
}

// Get tower reference by character
stack<int>& getTower(char tower) {
    if (tower == 'A') return towerA;
    if (tower == 'B') return towerB;
    return towerC;
}

// Solve Tower of Hanoi puzzle with visualization (Recursive)
void towerOfHanoiRecursive(int n, char source, char destination, char auxiliary, bool visualize) {
    recursionDepth++;
    if (recursionDepth > maxRecursionDepth) {
        maxRecursionDepth = recursionDepth;
    }
    
    // Base case: only one disk to move
    if (n == 1) {
        moveCount++;
        
        // Move disk
        stack<int>& src = getTower(source);
        stack<int>& dest = getTower(destination);
        int disk = src.top();
        src.pop();
        dest.push(disk);
        
        cout << "\nMove " << moveCount << ": Move disk " << disk << " from " << source << " to " << destination;
        cout << " [Depth: " << recursionDepth << "]" << endl;
        
        if (visualize) {
            displayTowers(towerA.size() + towerB.size() + towerC.size());
        }
        
        recursionDepth--;
        return;
    }
    
    // Move n-1 disks from source to auxiliary using destination
    towerOfHanoiRecursive(n - 1, source, auxiliary, destination, visualize);
    
    // Move the nth disk from source to destination
    moveCount++;
    stack<int>& src = getTower(source);
    stack<int>& dest = getTower(destination);
    int disk = src.top();
    src.pop();
    dest.push(disk);
    
    cout << "\nMove " << moveCount << ": Move disk " << disk << " from " << source << " to " << destination;
    cout << " [Depth: " << recursionDepth << "]" << endl;
    
    if (visualize) {
        displayTowers(towerA.size() + towerB.size() + towerC.size());
    }
    
    // Move n-1 disks from auxiliary to destination using source
    towerOfHanoiRecursive(n - 1, auxiliary, destination, source, visualize);
    
    recursionDepth--;
}

// Iterative solution using bit manipulation
void towerOfHanoiIterative(int n, bool visualize) {
    int totalMoves = (1 << n) - 1; // 2^n - 1
    char pegs[] = {'A', 'B', 'C'};
    
    // For even number of disks, swap destination and auxiliary
    if (n % 2 == 0) {
        swap(pegs[1], pegs[2]);
    }
    
    for (int i = 1; i <= totalMoves; i++) {
        moveCount++;
        
        // Find which disk to move using bit manipulation
        int from = (i & i - 1) % 3;
        int to = ((i | i - 1) + 1) % 3;
        
        cout << "\nMove " << moveCount << ": Move from " << pegs[from] << " to " << pegs[to] << endl;
        
        if (visualize && moveCount <= 20) {
            displayTowers(n);
        }
    }
}

int main() {
    int n, method;
    char choice;
    
    cout << "========================================" << endl;
    cout << "       Tower of Hanoi Problem          " << endl;
    cout << "========================================" << endl;
    cout << "Enter number of disks: ";
    cin >> n;
    
    // Input validation
    if (n <= 0) {
        cout << "Error: Number of disks must be positive!" << endl;
        return 1;
    }
    
    if (n > 10) {
        cout << "Warning: Large number of disks will generate many moves!" << endl;
        cout << "Expected moves: " << (1 << n) - 1 << endl;
    }
    
    cout << "\nChoose method:" << endl;
    cout << "1. Recursive solution" << endl;
    cout << "2. Iterative solution" << endl;
    cout << "3. Compare both" << endl;
    cout << "Enter choice (1-3): ";
    cin >> method;
    
    cout << "Show visualization? (y/n): ";
    cin >> choice;
    bool visualize = (choice == 'y' || choice == 'Y');
    
    if (method == 1 || method == 3) {
        // Recursive solution
        cout << "\n========================================" << endl;
        cout << "       RECURSIVE SOLUTION              " << endl;
        cout << "========================================" << endl;
        
        // Initialize tower A with disks
        while (!towerA.empty()) towerA.pop();
        while (!towerB.empty()) towerB.pop();
        while (!towerC.empty()) towerC.pop();
        
        for (int i = n; i >= 1; i--) {
            towerA.push(i);
        }
        
        if (visualize) {
            cout << "\nInitial State:" << endl;
            displayTowers(n);
        }
        
        cout << "\nSolution:" << endl;
        moveCount = 0;
        recursionDepth = 0;
        maxRecursionDepth = 0;
        
        auto start1 = high_resolution_clock::now();
        towerOfHanoiRecursive(n, 'A', 'C', 'B', visualize);
        auto end1 = high_resolution_clock::now();
        auto duration1 = duration_cast<microseconds>(end1 - start1);
        
        cout << "\n========================================" << endl;
        cout << "Recursive Performance Metrics:" << endl;
        cout << "  Total moves: " << moveCount << endl;
        cout << "  Expected moves: " << (1 << n) - 1 << endl;
        cout << "  Max recursion depth: " << maxRecursionDepth << endl;
        cout << "  Execution time: " << duration1.count() << " microseconds" << endl;
        cout << "  Time per move: " << (double)duration1.count() / moveCount << " μs" << endl;
        cout << "========================================" << endl;
    }
    
    if (method == 2 || method == 3) {
        // Iterative solution
        cout << "\n========================================" << endl;
        cout << "       ITERATIVE SOLUTION              " << endl;
        cout << "========================================" << endl;
        
        // Reset towers
        while (!towerA.empty()) towerA.pop();
        while (!towerB.empty()) towerB.pop();
        while (!towerC.empty()) towerC.pop();
        
        for (int i = n; i >= 1; i--) {
            towerA.push(i);
        }
        
        cout << "\nSolution:" << endl;
        moveCount = 0;
        
        auto start2 = high_resolution_clock::now();
        towerOfHanoiIterative(n, visualize);
        auto end2 = high_resolution_clock::now();
        auto duration2 = duration_cast<microseconds>(end2 - start2);
        
        cout << "\n========================================" << endl;
        cout << "Iterative Performance Metrics:" << endl;
        cout << "  Total moves: " << moveCount << endl;
        cout << "  Expected moves: " << (1 << n) - 1 << endl;
        cout << "  Execution time: " << duration2.count() << " microseconds" << endl;
        cout << "  Time per move: " << (double)duration2.count() / moveCount << " μs" << endl;
        cout << "========================================" << endl;
    }
    
    cout << "\n========================================" << endl;
    cout << "Complexity Analysis:" << endl;
    cout << "  Time Complexity: O(2^n)" << endl;
    cout << "  Space Complexity:" << endl;
    cout << "    - Recursive: O(n) [call stack]" << endl;
    cout << "    - Iterative: O(1) [constant space]" << endl;
    cout << "  Formula: Moves = 2^n - 1" << endl;
    cout << "  For n=" << n << ": " << (1 << n) - 1 << " moves" << endl;
    cout << "========================================" << endl;
    
    return 0;
}
