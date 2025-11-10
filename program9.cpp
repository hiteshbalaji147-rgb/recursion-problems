#include <iostream>
#include <vector>
using namespace std;

int moveCount = 0;

// Solve Tower of Hanoi
void towerOfHanoi(int n, char source, char destination, char auxiliary) {
    // Base case: only one disk to move
    if (n == 1) {
        moveCount++;
        cout << "Move " << moveCount << ": Move disk 1 from " << source 
             << " to " << destination << endl;
        return;
    }
    
    // Move n-1 disks from source to auxiliary using destination
    towerOfHanoi(n - 1, source, auxiliary, destination);
    
    // Move the nth disk from source to destination
    moveCount++;
    cout << "Move " << moveCount << ": Move disk " << n << " from " << source 
         << " to " << destination << endl;
    
    // Move n-1 disks from auxiliary to destination using source
    towerOfHanoi(n - 1, auxiliary, destination, source);
}

int main() {
    int n;
    
    cout << "Tower of Hanoi Problem" << endl;
    cout << "======================" << endl;
    
    cout << "Enter number of disks: ";
    cin >> n;
    
    if (n <= 0) {
        cout << "Error: Number of disks must be positive!" << endl;
        return 1;
    }
    
    cout << "\nSolving Tower of Hanoi with " << n << " disks..." << endl;
    cout << "Source: A, Destination: C, Auxiliary: B" << endl;
    cout << "\nMoves:" << endl;
    cout << "------" << endl;
    
    moveCount = 0;
    towerOfHanoi(n, 'A', 'C', 'B');
    
    cout << "\n======================" << endl;
    cout << "Total moves: " << moveCount << endl;
    cout << "Expected moves: " << (1 << n) - 1 << " (2^" << n << " - 1)" << endl;
    cout << "======================" << endl;
    
    return 0;
}
