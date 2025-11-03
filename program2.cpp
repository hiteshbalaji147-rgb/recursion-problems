#include <iostream>
using namespace std;

int factorialRecursive(int n, int depth = 0) {
    // Display recursion step
    for (int i = 0; i < depth; i++) cout << "  ";
    cout << "factorial(" << n << ")" << endl;

    // Base case
    if (n <= 1) {
        for (int i = 0; i < depth; i++) cout << "  ";
        cout << "-> Base case reached, returning 1" << endl;
        return 1;
    }

    // Recursive case
    int result = n * factorialRecursive(n - 1, depth + 1);
    for (int i = 0; i < depth; i++) cout << "  ";
    cout << "-> Returning " << n << " * factorial(" << n - 1 << ") = " << result << endl;
    return result;
}

int factorialIterative(int n) {
    int result = 1;
    for (int i = 2; i <= n; i++) {
        result *= i;
    }
    return result;
}

int main() {
    int n;
    cout << "========================================" << endl;
    cout << "     Factorial Calculator Program      " << endl;
    cout << "========================================" << endl;
    cout << "Enter a number: ";
    cin >> n;

    // Input validation
    if (n < 0) {
        cout << "Error: Factorial is not defined for negative numbers!" << endl;
        return 1;
    }

    // Overflow warning
    if (n > 12) {
        cout << "Warning: Result may overflow for n > 12 with int type!" << endl;
    }

    cout << "\n=== Recursive Approach ===" << endl;
    int recursiveResult = factorialRecursive(n);
    
    cout << "\n=== Iterative Approach ===" << endl;
    int iterativeResult = factorialIterative(n);
    cout << "Factorial of " << n << " = " << iterativeResult << endl;

    cout << "\n========================================" << endl;
    cout << "Final result: " << n << "! = " << recursiveResult << endl;
    cout << "========================================" << endl;
    return 0;
}
