#include <iostream>
using namespace std;

int fibonacci(int n) {
    // Base cases
    if (n == 0)
        return 0;
    if (n == 1)
        return 1;

    // Recursive case
    return fibonacci(n - 1) + fibonacci(n - 2);
}

int main() {
    int n;
    cout << "Enter the number of terms: ";
    cin >> n;

    // Input validation
    if (n <= 0) {
        cout << "Error: Please enter a positive integer!" << endl;
        return 1;
    }

    cout << "Fibonacci sequence: ";
    for (int i = 0; i < n; i++) {
        cout << fibonacci(i) << " ";
    }
    cout << endl;
    return 0;
}
