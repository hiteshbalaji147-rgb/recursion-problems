#include <iostream>
#include <vector>
#include <stack>
#include <cmath>
#include <chrono>
#include <iomanip>
using namespace std;
using namespace chrono;

int moveCount = 0;
int recursionCalls = 0;

// Move statistics
struct MoveStats {
    int disk1Moves = 0;
    int disk2Moves = 0;
    int disk3Moves = 0;
    int otherMoves = 0;
    int AtoB = 0, AtoC = 0;
    int BtoA = 0, BtoC = 0;
    int CtoA = 0, CtoB = 0;
};

MoveStats stats;

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

// Update move statistics
void updateStats(char from, char to, int disk) {
    // Track disk moves
    if (disk == 1) stats.disk1Moves++;
    else if (disk == 2) stats.disk2Moves++;
    else if (disk == 3) stats.disk3Moves++;
    else stats.otherMoves++;
    
    // Track direction
    if (from == 'A' && to == 'B') stats.AtoB++;
    else if (from == 'A' && to == 'C') stats.AtoC++;
    else if (from == 'B' && to == 'A') stats.BtoA++;
    else if (from == 'B' && to == 'C') stats.BtoC++;
    else if (from == 'C' && to == 'A') stats.CtoA++;
    else if (from == 'C' && to == 'B') stats.CtoB++;
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
        updateStats(from, to, diskNum);
        
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
        updateStats(source, destination, 1);
        cout << "Move " << moveCount << ": Move disk 1 from " << source 
             << " to " << destination << endl;
        return;
    }
    
    // Move n-1 disks from source to auxiliary using destination
    towerOfHanoiBasic(n - 1, source, auxiliary, destination);
    
    // Move the nth disk from source to destination
    moveCount++;
    updateStats(source, destination, n);
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
void towerOfHanoiIterative(int n, bool showMoves = true) {
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
    
    if (showMoves) {
        cout << "\nIterative Solution:" << endl;
        cout << "-------------------" << endl;
    }
    
    for (int i = 1; i <= totalMoves; i++) {
        if (i % 3 == 1) {
            // Move between source and destination
            if (!source.disks.empty() && (dest.disks.empty() || source.disks.top() < dest.disks.top())) {
                int disk = source.disks.top();
                source.disks.pop();
                dest.disks.push(disk);
                updateStats(source.name, dest.name, disk);
                if (showMoves) {
                    cout << "Move " << i << ": Move disk " << disk << " from " << source.name 
                         << " to " << dest.name << endl;
                }
            } else {
                int disk = dest.disks.top();
                dest.disks.pop();
                source.disks.push(disk);
                updateStats(dest.name, source.name, disk);
                if (showMoves) {
                    cout << "Move " << i << ": Move disk " << disk << " from " << dest.name 
                         << " to " << source.name << endl;
                }
            }
        } else if (i % 3 == 2) {
            // Move between source and auxiliary
            if (!source.disks.empty() && (aux.disks.empty() || source.disks.top() < aux.disks.top())) {
                int disk = source.disks.top();
                source.disks.pop();
                aux.disks.push(disk);
                updateStats(source.name, aux.name, disk);
                if (showMoves) {
                    cout << "Move " << i << ": Move disk " << disk << " from " << source.name 
                         << " to " << aux.name << endl;
                }
            } else {
                int disk = aux.disks.top();
                aux.disks.pop();
                source.disks.push(disk);
                updateStats(aux.name, source.name, disk);
                if (showMoves) {
                    cout << "Move " << i << ": Move disk " << disk << " from " << aux.name 
                         << " to " << source.name << endl;
                }
            }
        } else {
            // Move between auxiliary and destination
            if (!aux.disks.empty() && (dest.disks.empty() || aux.disks.top() < dest.disks.top())) {
                int disk = aux.disks.top();
                aux.disks.pop();
                dest.disks.push(disk);
                updateStats(aux.name, dest.name, disk);
                if (showMoves) {
                    cout << "Move " << i << ": Move disk " << disk << " from " << aux.name 
                         << " to " << dest.name << endl;
                }
            } else {
                int disk = dest.disks.top();
                dest.disks.pop();
                aux.disks.push(disk);
                updateStats(dest.name, aux.name, disk);
                if (showMoves) {
                    cout << "Move " << i << ": Move disk " << disk << " from " << dest.name 
                         << " to " << aux.name << endl;
                }
            }
        }
    }
}

// Print move pattern analysis
void printMoveAnalysis(int n) {
    cout << "\n========================================" << endl;
    cout << "       Move Pattern Analysis           " << endl;
    cout << "========================================" << endl;
    
    cout << "\nDisk Movement Frequency:" << endl;
    if (n >= 1) cout << "  Disk 1: " << stats.disk1Moves << " moves (2^" << (n-1) << " = " << (1 << (n-1)) << ")" << endl;
    if (n >= 2) cout << "  Disk 2: " << stats.disk2Moves << " moves (2^" << (n-2) << " = " << (1 << (n-2)) << ")" << endl;
    if (n >= 3) cout << "  Disk 3: " << stats.disk3Moves << " moves (2^" << (n-3) << " = " << (1 << (n-3)) << ")" << endl;
    if (n > 3) cout << "  Other disks: " << stats.otherMoves << " moves" << endl;
    
    cout << "\nDirection Statistics:" << endl;
    cout << "  A → B: " << stats.AtoB << " moves" << endl;
    cout << "  A → C: " << stats.AtoC << " moves" << endl;
    cout << "  B → A: " << stats.BtoA << " moves" << endl;
    cout << "  B → C: " << stats.BtoC << " moves" << endl;
    cout << "  C → A: " << stats.CtoA << " moves" << endl;
    cout << "  C → B: " << stats.CtoB << " moves" << endl;
    
    cout << "\nPattern Observations:" << endl;
    cout << "  • Disk k moves exactly 2^(n-k) times" << endl;
    cout << "  • Smallest disk (1) moves most: " << stats.disk1Moves << " times" << endl;
    if (n >= 2) {
        cout << "  • Each disk moves half as often as the one above it" << endl;
    }
    cout << "  • Total moves follow formula: 2^n - 1" << endl;
    cout << "========================================" << endl;
}

// Print complexity analysis
void printComplexityAnalysis(int n) {
    cout << "\n========================================" << endl;
    cout << "       Complexity Analysis             " << endl;
    cout << "========================================" << endl;
    
    cout << "\nTime Complexity:" << endl;
    cout << "  • Recurrence: T(n) = 2T(n-1) + 1" << endl;
    cout << "  • Solution: T(n) = 2^n - 1" << endl;
    cout << "  • For n=" << n << ": " << (1 << n) - 1 << " operations" << endl;
    
    cout << "\nSpace Complexity:" << endl;
    cout << "  • Recursive: O(n) - call stack depth" << endl;
    cout << "  • Iterative: O(n) - tower storage" << endl;
    cout << "  • For n=" << n << ": " << n << " stack frames max" << endl;
    
    cout << "\nGrowth Rate:" << endl;
    cout << "  n  |  Moves  |  Time (approx)" << endl;
    cout << "  ---|---------|----------------" << endl;
    for (int i = 1; i <= min(n + 3, 20); i++) {
        long long moves = (1LL << i) - 1;
        cout << "  " << setw(2) << i << " | " << setw(7) << moves << " | ";
        if (moves < 60) {
            cout << moves << " seconds" << endl;
        } else if (moves < 3600) {
            cout << (moves / 60) << " minutes" << endl;
        } else if (moves < 86400) {
            cout << (moves / 3600) << " hours" << endl;
        } else if (moves < 31536000) {
            cout << (moves / 86400) << " days" << endl;
        } else {
            cout << (moves / 31536000) << " years" << endl;
        }
    }
    
    cout << "\nReal-world Impact:" << endl;
    cout << "  • 64 disks: 2^64 - 1 = 18,446,744,073,709,551,615 moves" << endl;
    cout << "  • At 1 move/second: ~585 billion years!" << endl;
    cout << "  • This demonstrates exponential growth" << endl;
    cout << "========================================" << endl;
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

// Reset statistics
void resetStats() {
    stats = MoveStats();
    moveCount = 0;
    recursionCalls = 0;
}

int main() {
    int n, choice;
    
    cout << "========================================" << endl;
    cout << "       Tower of Hanoi Problem          " << endl;
    cout << "     Complete DSA Implementation       " << endl;
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
    cout << "5. Compare Recursive vs Iterative" << endl;
    cout << "6. Complexity analysis" << endl;
    cout << "Enter choice (1-6): ";
    cin >> choice;
    
    cout << "\n========================================" << endl;
    cout << "Solving Tower of Hanoi with " << n << " disks" << endl;
    cout << "Source: A, Destination: C, Auxiliary: B" << endl;
    cout << "========================================" << endl;
    
    if (choice == 1) {
        resetStats();
        
        cout << "\nMoves:" << endl;
        cout << "------" << endl;
        
        auto start = high_resolution_clock::now();
        towerOfHanoiBasic(n, 'A', 'C', 'B');
        auto end = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(end - start);
        
        cout << "\n========================================" << endl;
        cout << "Performance Metrics:" << endl;
        cout << "  Execution time: " << duration.count() << " μs" << endl;
        cout << "  Total moves: " << moveCount << endl;
        cout << "  Expected moves: " << (1 << n) - 1 << " (2^" << n << " - 1)" << endl;
        cout << "  Recursion calls: " << recursionCalls << endl;
        cout << "  Match: " << (moveCount == (1 << n) - 1 ? "✓" : "✗") << endl;
        cout << "  Time Complexity: O(2^n)" << endl;
        cout << "  Space Complexity: O(n)" << endl;
        cout << "========================================" << endl;
        
        printMoveAnalysis(n);
    } else if (choice == 2 || choice == 3) {
        resetStats();
        bool graphical = (choice == 3);
        
        // Initialize towers
        initializeTowers(n);
        
        cout << "\nInitial State:" << endl;
        printAllTowers(n, graphical);
        
        cout << "\nSolving..." << endl;
        
        auto start = high_resolution_clock::now();
        towerOfHanoiVisualized(n, 'A', 'C', 'B', true, n, graphical);
        auto end = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(end - start);
        
        cout << "\n========================================" << endl;
        cout << "Final State:" << endl;
        printAllTowers(n, graphical);
        
        cout << "\n========================================" << endl;
        cout << "Performance Metrics:" << endl;
        cout << "  Execution time: " << duration.count() << " μs" << endl;
        cout << "  Total moves: " << moveCount << endl;
        cout << "  Expected moves: " << (1 << n) - 1 << " (2^" << n << " - 1)" << endl;
        cout << "  Recursion calls: " << recursionCalls << endl;
        cout << "  Match: " << (moveCount == (1 << n) - 1 ? "✓" : "✗") << endl;
        cout << "========================================" << endl;
        
        printMoveAnalysis(n);
    } else if (choice == 4) {
        resetStats();
        
        auto start = high_resolution_clock::now();
        towerOfHanoiIterative(n, true);
        auto end = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(end - start);
        
        cout << "\n========================================" << endl;
        cout << "Performance Metrics:" << endl;
        cout << "  Execution time: " << duration.count() << " μs" << endl;
        cout << "  Total moves: " << (1 << n) - 1 << endl;
        cout << "  Algorithm: Iterative (no recursion)" << endl;
        cout << "  Time Complexity: O(2^n)" << endl;
        cout << "  Space Complexity: O(n)" << endl;
        cout << "========================================" << endl;
        
        printMoveAnalysis(n);
    } else if (choice == 5) {
        cout << "\n========================================" << endl;
        cout << "   Recursive vs Iterative Comparison   " << endl;
        cout << "========================================" << endl;
        
        // Test Recursive
        resetStats();
        auto start1 = high_resolution_clock::now();
        towerOfHanoiBasic(n, 'A', 'C', 'B');
        auto end1 = high_resolution_clock::now();
        auto duration1 = duration_cast<microseconds>(end1 - start1);
        int recMoves = moveCount;
        int recCalls = recursionCalls;
        
        // Test Iterative
        resetStats();
        auto start2 = high_resolution_clock::now();
        towerOfHanoiIterative(n, false);
        auto end2 = high_resolution_clock::now();
        auto duration2 = duration_cast<microseconds>(end2 - start2);
        
        cout << "\nComparison Results:" << endl;
        cout << left << setw(25) << "Metric" << setw(20) << "Recursive" << "Iterative" << endl;
        cout << string(65, '-') << endl;
        cout << setw(25) << "Execution time (μs)" << setw(20) << duration1.count() << duration2.count() << endl;
        cout << setw(25) << "Total moves" << setw(20) << recMoves << (1 << n) - 1 << endl;
        cout << setw(25) << "Recursion calls" << setw(20) << recCalls << "0" << endl;
        cout << setw(25) << "Space complexity" << setw(20) << "O(n)" << "O(n)" << endl;
        cout << setw(25) << "Time complexity" << setw(20) << "O(2^n)" << "O(2^n)" << endl;
        cout << setw(25) << "Readability" << setw(20) << "High" << "Medium" << endl;
        
        long long faster = min(duration1.count(), duration2.count());
        string winner = (faster == duration1.count()) ? "Recursive" : "Iterative";
        
        cout << "\n========================================" << endl;
        cout << "Winner: " << winner << " algorithm" << endl;
        cout << "Speedup: " << fixed << setprecision(2) 
             << (double)max(duration1.count(), duration2.count()) / faster << "x faster" << endl;
        
        cout << "\nRecommendations:" << endl;
        cout << "  • Recursive: More intuitive and elegant" << endl;
        cout << "  • Iterative: Avoids stack overflow for large n" << endl;
        cout << "  • Both have same time/space complexity" << endl;
        cout << "========================================" << endl;
    } else if (choice == 6) {
        printComplexityAnalysis(n);
        
        cout << "\nMathematical Properties:" << endl;
        cout << "  • Minimum moves: 2^n - 1 (proven optimal)" << endl;
        cout << "  • Disk k moves: 2^(n-k) times" << endl;
        cout << "  • Recurrence: T(n) = 2T(n-1) + 1, T(1) = 1" << endl;
        cout << "  • Closed form: T(n) = 2^n - 1" << endl;
        
        cout << "\nHistorical Context:" << endl;
        cout << "  • Invented by Édouard Lucas in 1883" << endl;
        cout << "  • Legend: 64 golden disks in temple" << endl;
        cout << "  • World ends when puzzle solved!" << endl;
        cout << "  • Classic example of recursion" << endl;
        cout << "========================================" << endl;
    }
    
    return 0;
}
