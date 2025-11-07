#include <iostream>
using namespace std;

// Solve Tower of Hanoi puzzle
void towerOfHanoi(int n, char source, char destination, char auxiliary) {
    // Base case: only one disk to move
    if (n == 1) {
        cout << "Move disk 1 from " << source << " to " << destination << endl;
        return;
    }
    
    // Move n-1 disks from source to auxiliary using destination
    towerOfHanoi(n - 1, source, auxiliary, destination);
    
    // Move the nth disk from source to destination
    cout << "Move disk " << n << " from " << source << " to " << destination << endl;
    
    // Move n-1 disks from auxiliary to destination using source
    towerOfHanoi(n - 1, auxiliary, destination, source);
}

int main() {
    int n;
    cout << "Tower of Hanoi Problem" << endl;
    cout << "Enter number of disks: ";
    cin >> n;
    
    cout << "\nSolution:" << endl;
    towerOfHanoi(n, 'A', 'C', 'B');
    
    return 0;
}
