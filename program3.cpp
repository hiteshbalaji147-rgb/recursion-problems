#include <iostream>
#include <cmath>
using namespace std;

double power(double x, int n, int depth = 0) {
    // Display recursion step
    for (int i = 0; i < depth; i++) cout << "  ";
    cout << "power(" << x << ", " << n << ")" << endl;

    // Base case
    if (n == 0) {
        for (int i = 0; i < depth; i++) cout << "  ";
        cout << "-> Base case: returning 1" << endl;
        return 1;
    }

    // Handle negative exponents
    if (n < 0) {
        for (int i = 0; i < depth; i++) cout << "  ";
        cout << "-> Negative exponent: 1 / power(" << x << ", " << -n << ")" << endl;
        return 1.0 / power(x, -n, depth + 1);
    }

    // Recursive case
    double result = x * power(x, n - 1, depth + 1);
    for (int i = 0; i < depth; i++) cout << "  ";
    cout << "-> Returning " << x << " * power(" << x << ", " << n - 1 << ") = " << result << endl;
    return result;
}

// Optimized power using divide and conquer
double powerOptimized(double x, int n) {
    // Base case
    if (n == 0)
        return 1;

    // Handle negative exponents
    if (n < 0)
        return 1.0 / powerOptimized(x, -n);

    // Divide and conquer
    double half = powerOptimized(x, n / 2);
    if (n % 2 == 0)
        return half * half;
    else
        return x * half * half;
}

// Iterative power function
double powerIterative(double x, int n) {
    if (n == 0) return 1;
    
    bool negative = n < 0;
    n = abs(n);
    
    double result = 1;
    for (int i = 0; i < n; i++) {
        result *= x;
    }
    
    return negative ? 1.0 / result : result;
}

int main() {
    double x;
    int n;
    cout << "Enter base (x): ";
    cin >> x;
    cout << "Enter exponent (n): ";
    cin >> n;

    // Input validation
    if (x == 0 && n < 0) {
        cout << "Error: 0 raised to negative power is undefined!" << endl;
        return 1;
    }

    cout << "\n=== Basic Recursion Steps ===" << endl;
    double result1 = power(x, n);
    
    cout << "\n=== Optimized Recursion ===" << endl;
    double result2 = powerOptimized(x, n);
    cout << "Result: " << x << "^" << n << " = " << result2 << endl;
    
    cout << "\n=== Iterative Approach ===" << endl;
    double result3 = powerIterative(x, n);
    cout << "Result: " << x << "^" << n << " = " << result3 << endl;
    
    return 0;
}
