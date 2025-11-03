#include <iostream>
using namespace std;

int factorial(int n) {
    // Base case
    if (n <= 1)
        return 1;

    // Recursive case
    return n * factorial(n - 1);
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

    cout << "Factorial of " << n << " = " << factorial(n) << endl;
    return 0;
}
