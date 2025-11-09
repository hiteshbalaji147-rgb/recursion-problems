#include <iostream>
#include <vector>
using namespace std;

// Check if subset with given sum exists
bool isSubsetSum(vector<int>& arr, int n, int sum) {
    // Base cases
    if (sum == 0)
        return true;
    if (n == 0)
        return false;
    
    // If last element is greater than sum, ignore it
    if (arr[n - 1] > sum)
        return isSubsetSum(arr, n - 1, sum);
    
    // Check if sum can be obtained by:
    // 1. Including the last element
    // 2. Excluding the last element
    return isSubsetSum(arr, n - 1, sum - arr[n - 1]) || 
           isSubsetSum(arr, n - 1, sum);
}

// Print array
void printArray(vector<int>& arr) {
    cout << "{ ";
    for (int i = 0; i < arr.size(); i++) {
        cout << arr[i];
        if (i < arr.size() - 1) cout << ", ";
    }
    cout << " }";
}

int main() {
    int n, target;
    
    cout << "Subset Sum Problem" << endl;
    cout << "==================" << endl;
    
    cout << "Enter number of elements: ";
    cin >> n;
    
    vector<int> arr(n);
    cout << "Enter elements: ";
    for (int i = 0; i < n; i++) {
        cin >> arr[i];
    }
    
    cout << "Enter target sum: ";
    cin >> target;
    
    cout << "\nArray: ";
    printArray(arr);
    cout << "\nTarget sum: " << target << endl;
    
    if (isSubsetSum(arr, n, target)) {
        cout << "\nResult: Subset with sum " << target << " EXISTS!" << endl;
    } else {
        cout << "\nResult: Subset with sum " << target << " DOES NOT EXIST!" << endl;
    }
    
    return 0;
}
