#include <iostream>
#include <vector>
#include <chrono>
using namespace std;
using namespace chrono;

int recursionCalls = 0;

// Basic recursive Fibonacci
int fibonacci(int n) {
    recursionCalls++;
    
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
    recursionCalls = 0;
    auto start1 = high_resolution_clock::now();
    for (int i = 0; i < n; i++) {
        cout << fibonacci(i) << " ";
    }
    auto end1 = high_resolution_clock::now();
    auto duration1 = duration_cast<milliseconds>(end1 - start1);
    cout << endl;
    cout << "Time: " << duration1.count() << " ms" << endl;
    cout << "Total recursion calls: " << recursionCalls << endl;
    cout << "Time Complexity: O(2^n)" << endl;
    cout << "Space Complexity: O(n)" << endl;

    cout << "\n=== Fibonacci Sequence (Memoized) ===" << endl;
    vector<int> memo(n, -1);
    auto start2 = high_resolution_clock::now();
    for (int i = 0; i < n; i++) {
        cout << fibonacciMemo(i, memo) << " ";
    }
    auto end2 = high_resolution_clock::now();
    auto duration2 = duration_cast<milliseconds>(end2 - start2);
    cout << endl;
    cout << "Time: " << duration2.count() << " ms" << endl;
    cout << "Time Complexity: O(n)" << endl;
    cout << "Space Complexity: O(n)" << endl;

    cout << "\n=== Fibonacci Sequence (Iterative) ===" << endl;
    auto start3 = high_resolution_clock::now();
    for (int i = 0; i < n; i++) {
        cout << fibonacciIterative(i) << " ";
    }
    auto end3 = high_resolution_clock::now();
    auto duration3 = duration_cast<milliseconds>(end3 - start3);
    cout << endl;
    cout << "Time: " << duration3.count() << " ms" << endl;
    cout << "Time Complexity: O(n^2) for loop" << endl;
    cout << "Space Complexity: O(1)" << endl;

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
    
    cout << "\nPerformance Comparison:" << endl;
    cout << "  Basic Recursion: " << duration1.count() << " ms" << endl;
    cout << "  Memoized: " << duration2.count() << " ms" << endl;
    cout << "  Iterative: " << duration3.count() << " ms" << endl;
    cout << "========================================" << endl;

    return 0;
}
