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

    cout << x << "^" << n << " = " << power(x, n) << endl;
    return 0;
}
