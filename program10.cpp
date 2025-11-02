#include <iostream>

using namespace std;

int main() {
    int N;
    cout << "Enter a positive integer N: ";
    cin >> N;
    
    // Print from 1 to N
    cout << "Numbers from 1 to " << N << ": ";
    for (int i = 1; i <= N; i++) {
        cout << i << " ";
    }
    cout << endl;
    
    // Print from N to 1
    cout << "Numbers from " << N << " to 1: ";
    for (int i = N; i >= 1; i--) {
        cout << i << " ";
    }
    cout << endl;
    
    return 0;
}
