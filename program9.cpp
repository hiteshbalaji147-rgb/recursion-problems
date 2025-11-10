#include <iostream>
#include <vector>
#include <stack>
using namespace std;

int moveCount = 0;
int recursionCalls = 0;

// Tower structure
struct Tower {
    stack<int> disks;
    char name;
    
    Tower(char n) : name(n) {}
};

// Global towers for visualization
Tower towerA('A'), towerB('B'), towerC('C');

// Get tower by name
Tower* getTower(char name) {
    if (name == 'A') return &towerA;
    if (name == 'B') return &towerB;
    if (name == 'C') return &towerC;
    return nullptr;
}

// Print a single tower
void printTower(Tower& tower, int maxDisks) {
    vector<int> disks;
    stack<int> temp = tower.disks;
    
    // Extract disks
    while (!temp.empty()) {
        disks.push_back(temp.top());
        temp.pop();
    }
    
    cout << "Tower " << tower.name << ": ";
    
    if (disks.empty()) {
        cout << "[ ]";
    } else {
        cout << "[ ";
        for (int i = disks.size() - 1; i >= 0; i--) {
            cout << disks[i];
            if (i > 0) cout << " ";
        }
        cout << " ]";
    }
    
    cout << " (" << disks.size() << " disks)";
    cout << endl;
}

// Print all towers
void printAllTowers(int maxDisks) {
    cout << "\nCurrent State:" << endl;
    cout << "-------------" << endl;
    printTower(towerA, maxDisks);
    printTower(towerB, maxDisks);
    printTower(towerC, maxDisks);
    cout << "-------------" << endl;
}

// Move disk between towers
void moveDisk(char from, char to, int diskNum, bool visualize) {
    Tower* source = getTower(from);
    Tower* dest = getTower(to);
    
    if (source && dest && !source->disks.empty()) {
        int disk = source->disks.top();
        source->disks.pop();
        dest->disks.push(disk);
        
        moveCount++;
        cout << "\nMove " << moveCount << ": Move disk " << diskNum 
             << " from Tower " << from << " to Tower " << to << endl;
        
        if (visualize) {
            printAllTowers(diskNum);
        }
    }
}

// Solve Tower of Hanoi (basic version)
void towerOfHanoiBasic(int n, char source, char destination, char auxiliary) {
    recursionCalls++;
    
    // Base case: only one disk to move
    if (n == 1) {
        moveCount++;
        cout << "Move " << moveCount << ": Move disk 1 from " << source 
             << " to " << destination << endl;
        return;
    }
    
    // Move n-1 disks from source to auxiliary using destination
    towerOfHanoiBasic(n - 1, source, auxiliary, destination);
    
    // Move the nth disk from source to destination
    moveCount++;
    cout << "Move " << moveCount << ": Move disk " << n << " from " << source 
         << " to " << destination << endl;
    
    // Move n-1 disks from auxiliary to destination using source
    towerOfHanoiBasic(n - 1, auxiliary, destination, source);
}

// Solve Tower of Hanoi with visualization
void towerOfHanoiVisualized(int n, char source, char destination, char auxiliary, bool visualize) {
    recursionCalls++;
    
    // Base case: only one disk to move
    if (n == 1) {
        moveDisk(source, destination, 1, visualize);
        return;
    }
    
    // Move n-1 disks from source to auxiliary using destination
    towerOfHanoiVisualized(n - 1, source, auxiliary, destination, visualize);
    
    // Move the nth disk from source to destination
    moveDisk(source, destination, n, visualize);
    
    // Move n-1 disks from auxiliary to destination using source
    towerOfHanoiVisualized(n - 1, auxiliary, destination, source, visualize);
}

// Initialize towers
void initializeTowers(int n) {
    // Clear all towers
    while (!towerA.disks.empty()) towerA.disks.pop();
    while (!towerB.disks.empty()) towerB.disks.pop();
    while (!towerC.disks.empty()) towerC.disks.pop();
    
    // Add disks to tower A (largest at bottom)
    for (int i = n; i >= 1; i--) {
        towerA.disks.push(i);
    }
}

int main() {
    int n, choice;
    
    cout << "========================================" << endl;
    cout << "       Tower of Hanoi Problem          " << endl;
    cout << "========================================" << endl;
    
    cout << "Enter number of disks: ";
    cin >> n;
    
    if (n <= 0) {
        cout << "Error: Number of disks must be positive!" << endl;
        return 1;
    }
    
    if (n > 10) {
        cout << "Warning: Large number of disks will generate many moves!" << endl;
        cout << "Expected moves: " << (1 << n) - 1 << endl;
    }
    
    cout << "\nChoose option:" << endl;
    cout << "1. Show moves only" << endl;
    cout << "2. Show moves with visualization" << endl;
    cout << "Enter choice (1-2): ";
    cin >> choice;
    
    cout << "\n========================================" << endl;
    cout << "Solving Tower of Hanoi with " << n << " disks" << endl;
    cout << "Source: A, Destination: C, Auxiliary: B" << endl;
    cout << "========================================" << endl;
    
    if (choice == 1) {
        moveCount = 0;
        recursionCalls = 0;
        
        cout << "\nMoves:" << endl;
        cout << "------" << endl;
        towerOfHanoiBasic(n, 'A', 'C', 'B');
        
        cout << "\n========================================" << endl;
        cout << "Statistics:" << endl;
        cout << "  Total moves: " << moveCount << endl;
        cout << "  Expected moves: " << (1 << n) - 1 << " (2^" << n << " - 1)" << endl;
        cout << "  Recursion calls: " << recursionCalls << endl;
        cout << "  Match: " << (moveCount == (1 << n) - 1 ? "✓" : "✗") << endl;
        cout << "========================================" << endl;
    } else if (choice == 2) {
        moveCount = 0;
        recursionCalls = 0;
        
        // Initialize towers
        initializeTowers(n);
        
        cout << "\nInitial State:" << endl;
        printAllTowers(n);
        
        cout << "\nSolving..." << endl;
        towerOfHanoiVisualized(n, 'A', 'C', 'B', true);
        
        cout << "\n========================================" << endl;
        cout << "Final State:" << endl;
        printAllTowers(n);
        
        cout << "\n========================================" << endl;
        cout << "Statistics:" << endl;
        cout << "  Total moves: " << moveCount << endl;
        cout << "  Expected moves: " << (1 << n) - 1 << " (2^" << n << " - 1)" << endl;
        cout << "  Recursion calls: " << recursionCalls << endl;
        cout << "  Match: " << (moveCount == (1 << n) - 1 ? "✓" : "✗") << endl;
        cout << "========================================" << endl;
    }
    
    return 0;
}
