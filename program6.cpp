#include <iostream>
#include <stack>
#include <vector>
#include <chrono>
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

// Solve Tower of Hanoi puzzle with visualization
void towerOfHanoi(int n, char source, char destination, char auxiliary, bool visualize) {
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
    towerOfHanoi(n - 1, source, auxiliary, destination, visualize);
    
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
    towerOfHanoi(n - 1, auxiliary, destination, source, visualize);
    
    recursionDepth--;
}

int main() {
    int n;
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
    
    cout << "Show visualization? (y/n): ";
    cin >> choice;
    bool visualize = (choice == 'y' || choice == 'Y');
    
    // Initialize tower A with disks
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
    
    auto start = high_resolution_clock::now();
    towerOfHanoi(n, 'A', 'C', 'B', visualize);
    auto end = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(end - start);
    
    cout << "\n========================================" << endl;
    cout << "Performance Metrics:" << endl;
    cout << "  Total moves: " << moveCount << endl;
    cout << "  Expected moves: " << (1 << n) - 1 << endl;
    cout << "  Max recursion depth: " << maxRecursionDepth << endl;
    cout << "  Execution time: " << duration.count() << " microseconds" << endl;
    cout << "  Time per move: " << (double)duration.count() / moveCount << " μs" << endl;
    cout << "\nComplexity Analysis:" << endl;
    cout << "  Time Complexity: O(2^n)" << endl;
    cout << "  Space Complexity: O(n)" << endl;
    cout << "========================================" << endl;
    
    return 0;
}
