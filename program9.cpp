#include <iostream>
#include <vector>
#include <stack>
#include <cmath>
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

// Print graphical tower representation
void printGraphicalTowers(int maxDisks) {
    vector<int> disksA, disksB, disksC;
    
    // Extract disks from each tower
    stack<int> tempA = towerA.disks, tempB = towerB.disks, tempC = towerC.disks;
    while (!tempA.empty()) { disksA.push_back(tempA.top()); tempA.pop(); }
    while (!tempB.empty()) { disksB.push_back(tempB.top()); tempB.pop(); }
    while (!tempC.empty()) { disksC.push_back(tempC.top()); tempC.pop(); }
    
    cout << "\n";
    
    // Print from top to bottom
    for (int level = maxDisks - 1; level >= 0; level--) {
        // Tower A
        if (level < disksA.size()) {
            int disk = disksA[disksA.size() - 1 - level];
            int spaces = maxDisks - disk;
            cout << string(spaces, ' ');
            cout << string(disk * 2 - 1, '=');
            cout << string(spaces, ' ');
        } else {
            cout << string(maxDisks - 1, ' ') << "|" << string(maxDisks - 1, ' ');
        }
        
        cout << "   ";
        
        // Tower B
        if (level < disksB.size()) {
            int disk = disksB[disksB.size() - 1 - level];
            int spaces = maxDisks - disk;
            cout << string(spaces, ' ');
            cout << string(disk * 2 - 1, '=');
            cout << string(spaces, ' ');
        } else {
            cout << string(maxDisks - 1, ' ') << "|" << string(maxDisks - 1, ' ');
        }
        
        cout << "   ";
        
        // Tower C
        if (level < disksC.size()) {
            int disk = disksC[disksC.size() - 1 - level];
            int spaces = maxDisks - disk;
            cout << string(spaces, ' ');
            cout << string(disk * 2 - 1, '=');
            cout << string(spaces, ' ');
        } else {
            cout << string(maxDisks - 1, ' ') << "|" << string(maxDisks - 1, ' ');
        }
        
        cout << endl;
    }
    
    // Print base
    cout << string(maxDisks * 2 - 1, '-') << "   ";
    cout << string(maxDisks * 2 - 1, '-') << "   ";
    cout << string(maxDisks * 2 - 1, '-') << endl;
    
    // Print labels
    int labelSpace = maxDisks - 1;
    cout << string(labelSpace, ' ') << "A" << string(labelSpace, ' ') << "   ";
    cout << string(labelSpace, ' ') << "B" << string(labelSpace, ' ') << "   ";
    cout << string(labelSpace, ' ') << "C" << string(labelSpace, ' ') << endl;
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
void printAllTowers(int maxDisks, bool graphical) {
    if (graphical) {
        printGraphicalTowers(maxDisks);
    } else {
        cout << "\nCurrent State:" << endl;
        cout << "-------------" << endl;
        printTower(towerA, maxDisks);
        printTower(towerB, maxDisks);
        printTower(towerC, maxDisks);
        cout << "-------------" << endl;
    }
}

// Move disk between towers
void moveDisk(char from, char to, int diskNum, bool visualize, int maxDisks, bool graphical) {
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
            printAllTowers(maxDisks, graphical);
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
void towerOfHanoiVisualized(int n, char source, char destination, char auxiliary, 
                            bool visualize, int maxDisks, bool graphical) {
    recursionCalls++;
    
    // Base case: only one disk to move
    if (n == 1) {
        moveDisk(source, destination, 1, visualize, maxDisks, graphical);
        return;
    }
    
    // Move n-1 disks from source to auxiliary using destination
    towerOfHanoiVisualized(n - 1, source, auxiliary, destination, visualize, maxDisks, graphical);
    
    // Move the nth disk from source to destination
    moveDisk(source, destination, n, visualize, maxDisks, graphical);
    
    // Move n-1 disks from auxiliary to destination using source
    towerOfHanoiVisualized(n - 1, auxiliary, destination, source, visualize, maxDisks, graphical);
}

// Iterative solution for Tower of Hanoi
void towerOfHanoiIterative(int n) {
    // Calculate total moves
    int totalMoves = (1 << n) - 1;
    
    // Initialize towers
    Tower source('A'), dest('C'), aux('B');
    for (int i = n; i >= 1; i--) {
        source.disks.push(i);
    }
    
    // For even number of disks, swap destination and auxiliary
    if (n % 2 == 0) {
        swap(dest.name, aux.name);
    }
    
    cout << "\nIterative Solution:" << endl;
    cout << "-------------------" << endl;
    
    for (int i = 1; i <= totalMoves; i++) {
        if (i % 3 == 1) {
            // Move between source and destination
            if (!source.disks.empty() && (dest.disks.empty() || source.disks.top() < dest.disks.top())) {
                int disk = source.disks.top();
                source.disks.pop();
                dest.disks.push(disk);
                cout << "Move " << i << ": Move disk " << disk << " from " << source.name 
                     << " to " << dest.name << endl;
            } else {
                int disk = dest.disks.top();
                dest.disks.pop();
                source.disks.push(disk);
                cout << "Move " << i << ": Move disk " << disk << " from " << dest.name 
                     << " to " << source.name << endl;
            }
        } else if (i % 3 == 2) {
            // Move between source and auxiliary
            if (!source.disks.empty() && (aux.disks.empty() || source.disks.top() < aux.disks.top())) {
                int disk = source.disks.top();
                source.disks.pop();
                aux.disks.push(disk);
                cout << "Move " << i << ": Move disk " << disk << " from " << source.name 
                     << " to " << aux.name << endl;
            } else {
                int disk = aux.disks.top();
                aux.disks.pop();
                source.disks.push(disk);
                cout << "Move " << i << ": Move disk " << disk << " from " << aux.name 
                     << " to " << source.name << endl;
            }
        } else {
            // Move between auxiliary and destination
            if (!aux.disks.empty() && (dest.disks.empty() || aux.disks.top() < dest.disks.top())) {
                int disk = aux.disks.top();
                aux.disks.pop();
                dest.disks.push(disk);
                cout << "Move " << i << ": Move disk " << disk << " from " << aux.name 
                     << " to " << dest.name << endl;
            } else {
                int disk = dest.disks.top();
                dest.disks.pop();
                aux.disks.push(disk);
                cout << "Move " << i << ": Move disk " << disk << " from " << dest.name 
                     << " to " << aux.name << endl;
            }
        }
    }
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
    cout << "1. Show moves only (Recursive)" << endl;
    cout << "2. Show moves with simple visualization (Recursive)" << endl;
    cout << "3. Show moves with graphical visualization (Recursive)" << endl;
    cout << "4. Iterative solution" << endl;
    cout << "Enter choice (1-4): ";
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
    } else if (choice == 2 || choice == 3) {
        moveCount = 0;
        recursionCalls = 0;
        bool graphical = (choice == 3);
        
        // Initialize towers
        initializeTowers(n);
        
        cout << "\nInitial State:" << endl;
        printAllTowers(n, graphical);
        
        cout << "\nSolving..." << endl;
        towerOfHanoiVisualized(n, 'A', 'C', 'B', true, n, graphical);
        
        cout << "\n========================================" << endl;
        cout << "Final State:" << endl;
        printAllTowers(n, graphical);
        
        cout << "\n========================================" << endl;
        cout << "Statistics:" << endl;
        cout << "  Total moves: " << moveCount << endl;
        cout << "  Expected moves: " << (1 << n) - 1 << " (2^" << n << " - 1)" << endl;
        cout << "  Recursion calls: " << recursionCalls << endl;
        cout << "  Match: " << (moveCount == (1 << n) - 1 ? "✓" : "✗") << endl;
        cout << "========================================" << endl;
    } else if (choice == 4) {
        towerOfHanoiIterative(n);
        
        cout << "\n========================================" << endl;
        cout << "Statistics:" << endl;
        cout << "  Total moves: " << (1 << n) - 1 << endl;
        cout << "  Algorithm: Iterative (no recursion)" << endl;
        cout << "  Space Complexity: O(n)" << endl;
        cout << "========================================" << endl;
    }
    
    return 0;
}
