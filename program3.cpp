#include <iostream>
using namespace std;

double power(double x, int n) {
    // Base case
    if (n == 0)
        return 1;

    // Handle negative exponents
    if (n < 0)
        return 1.0 / power(x, -n);

    // Recursive case
    return x * power(x, n - 1);
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

    cout << "\nBasic Recursion: " << x << "^" << n << " = " << power(x, n) << endl;
    cout << "Optimized Recursion: " << x << "^" << n << " = " << powerOptimized(x, n) << endl;
    return 0;
}
