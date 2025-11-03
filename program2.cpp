#include <iostream>
using namespace std;

int factorial(int n, int depth = 0) {
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
    int result = n * factorial(n - 1, depth + 1);
    for (int i = 0; i < depth; i++) cout << "  ";
    cout << "-> Returning " << n << " * factorial(" << n - 1 << ") = " << result << endl;
    return result;
}

int main() {
    int n;
    cout << "Enter a number: ";
    cin >> n;

    // Input validation
    if (n < 0) {
        cout << "Error: Factorial is not defined for negative numbers!" << endl;
        return 1;
    }

    cout << "\nRecursion steps:\n";
    int result = factorial(n);
    cout << "\nFinal result: Factorial of " << n << " = " << result << endl;
    return 0;
}
