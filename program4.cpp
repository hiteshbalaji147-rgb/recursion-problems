#include <iostream>
#include <vector>
using namespace std;

// Basic recursive Fibonacci
int fibonacci(int n) {
    // Base cases
    if (n == 0)
        return 0;
    if (n == 1)
        return 1;

    // Recursive case
    return fibonacci(n - 1) + fibonacci(n - 2);
}

// Optimized Fibonacci with memoization
int fibonacciMemo(int n, vector<int>& memo) {
    // Base cases
    if (n == 0)
        return 0;
    if (n == 1)
        return 1;

    // Check if already calculated
    if (memo[n] != -1)
        return memo[n];

    // Calculate and store in memo
    memo[n] = fibonacciMemo(n - 1, memo) + fibonacciMemo(n - 2, memo);
    return memo[n];
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

    cout << "Fibonacci sequence (Basic): ";
    for (int i = 0; i < n; i++) {
        cout << fibonacci(i) << " ";
    }
    cout << endl;

    cout << "Fibonacci sequence (Memoized): ";
    vector<int> memo(n, -1);
    for (int i = 0; i < n; i++) {
        cout << fibonacciMemo(i, memo) << " ";
    }
    cout << endl;

    return 0;
}
