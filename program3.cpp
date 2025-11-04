#include <iostream>
#include <cmath>
#include <chrono>
using namespace std;
using namespace chrono;

int recursionCount = 0;

double power(double x, int n, int depth = 0) {
    recursionCount++;
    
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
    cout << "========================================" << endl;
    cout << "     Power Function Calculator         " << endl;
    cout << "========================================" << endl;
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
    recursionCount = 0;
    auto start1 = high_resolution_clock::now();
    double result1 = power(x, n);
    auto end1 = high_resolution_clock::now();
    auto duration1 = duration_cast<microseconds>(end1 - start1);
    cout << "Recursion calls: " << recursionCount << endl;
    cout << "Time: " << duration1.count() << " microseconds" << endl;
    
    cout << "\n=== Optimized Recursion ===" << endl;
    auto start2 = high_resolution_clock::now();
    double result2 = powerOptimized(x, n);
    auto end2 = high_resolution_clock::now();
    auto duration2 = duration_cast<microseconds>(end2 - start2);
    cout << "Result: " << x << "^" << n << " = " << result2 << endl;
    cout << "Time: " << duration2.count() << " microseconds" << endl;
    
    cout << "\n=== Iterative Approach ===" << endl;
    auto start3 = high_resolution_clock::now();
    double result3 = powerIterative(x, n);
    auto end3 = high_resolution_clock::now();
    auto duration3 = duration_cast<microseconds>(end3 - start3);
    cout << "Result: " << x << "^" << n << " = " << result3 << endl;
    cout << "Time: " << duration3.count() << " microseconds" << endl;
    
    cout << "\n========================================" << endl;
    
    return 0;
}
