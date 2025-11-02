#include <iostream>

using namespace std;

int main() {
    int N;
    cout << "========================================" << endl;
    cout << "   Number Sequence Printer Program     " << endl;
    cout << "========================================" << endl;
    cout << "Enter a positive integer N: ";
    cin >> N;
    
    // Input validation
    if (N <= 0) {
        cout << "Error: Please enter a positive integer!" << endl;
        return 1;
    }
    
    cout << "========================================" << endl;
    
    // Print from 1 to N
    cout << "Numbers from 1 to " << N << ": ";
    int sum = 0;
    int evenCount = 0;
    int oddCount = 0;
    
    for (int i = 1; i <= N; i++) {
        cout << i << " ";
        sum += i;
        if (i % 2 == 0) {
            evenCount++;
        } else {
            oddCount++;
        }
    }
    cout << endl;
    
    // Print from N to 1
    cout << "Numbers from " << N << " to 1: ";
    for (int i = N; i >= 1; i--) {
        cout << i << " ";
    }
    cout << endl;
    
    cout << "========================================" << endl;
    cout << "Statistics:" << endl;
    cout << "  Total numbers: " << N << endl;
    cout << "  Sum of numbers: " << sum << endl;
    cout << "  Average: " << (double)sum / N << endl;
    cout << "  Even numbers: " << evenCount << endl;
    cout << "  Odd numbers: " << oddCount << endl;
    cout << "========================================" << endl;
    
    return 0;
}
