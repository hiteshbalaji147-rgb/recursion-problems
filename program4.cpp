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

// Iterative Fibonacci
int fibonacciIterative(int n) {
    if (n == 0) return 0;
    if (n == 1) return 1;

    int prev2 = 0, prev1 = 1, current;
    for (int i = 2; i <= n; i++) {
        current = prev1 + prev2;
        prev2 = prev1;
        prev1 = current;
    }
    return current;
}

int main() {
    int n;
    cout << "========================================" << endl;
    cout << "    Fibonacci Sequence Generator       " << endl;
    cout << "========================================" << endl;
    cout << "Enter the number of terms: ";
    cin >> n;

    // Input validation
    if (n <= 0) {
        cout << "Error: Please enter a positive integer!" << endl;
        return 1;
    }

    if (n > 40) {
        cout << "Warning: Basic recursion will be slow for n > 40!" << endl;
    }

    cout << "\n=== Fibonacci Sequence (Basic Recursion) ===" << endl;
    for (int i = 0; i < n; i++) {
        cout << fibonacci(i) << " ";
    }
    cout << endl;

    cout << "\n=== Fibonacci Sequence (Memoized) ===" << endl;
    vector<int> memo(n, -1);
    for (int i = 0; i < n; i++) {
        cout << fibonacciMemo(i, memo) << " ";
    }
    cout << endl;

    cout << "\n=== Fibonacci Sequence (Iterative) ===" << endl;
    for (int i = 0; i < n; i++) {
        cout << fibonacciIterative(i) << " ";
    }
    cout << endl;

    // Statistics
    cout << "\n========================================" << endl;
    cout << "Statistics:" << endl;
    cout << "  Total terms: " << n << endl;
    cout << "  Last term: " << fibonacciIterative(n - 1) << endl;
    
    int sum = 0;
    for (int i = 0; i < n; i++) {
        sum += fibonacciIterative(i);
    }
    cout << "  Sum of all terms: " << sum << endl;
    cout << "========================================" << endl;

    return 0;
}
