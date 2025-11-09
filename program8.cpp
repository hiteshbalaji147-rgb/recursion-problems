#include <iostream>
#include <vector>
using namespace std;

int solutionCount = 0;
int recursionCalls = 0;

// Check if subset with given sum exists
bool isSubsetSum(vector<int>& arr, int n, int sum) {
    recursionCalls++;
    
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

// Find and print all subsets with given sum
void findAllSubsets(vector<int>& arr, int n, int sum, vector<int>& subset) {
    recursionCalls++;
    
    // Base case: if sum becomes 0, we found a subset
    if (sum == 0) {
        solutionCount++;
        cout << "\nSubset " << solutionCount << ": { ";
        for (int i = 0; i < subset.size(); i++) {
            cout << subset[i];
            if (i < subset.size() - 1) cout << ", ";
        }
        cout << " }";
        
        // Calculate sum for verification
        int total = 0;
        for (int x : subset) total += x;
        cout << " (Sum = " << total << ")";
        return;
    }
    
    // Base case: no elements left
    if (n == 0)
        return;
    
    // If current element is greater than sum, skip it
    if (arr[n - 1] <= sum) {
        // Include current element
        subset.push_back(arr[n - 1]);
        findAllSubsets(arr, n - 1, sum - arr[n - 1], subset);
        subset.pop_back(); // Backtrack
    }
    
    // Exclude current element
    findAllSubsets(arr, n - 1, sum, subset);
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
    int n, target, choice;
    
    cout << "========================================" << endl;
    cout << "        Subset Sum Problem             " << endl;
    cout << "========================================" << endl;
    
    cout << "Enter number of elements: ";
    cin >> n;
    
    if (n <= 0) {
        cout << "Error: Number of elements must be positive!" << endl;
        return 1;
    }
    
    vector<int> arr(n);
    cout << "Enter elements: ";
    for (int i = 0; i < n; i++) {
        cin >> arr[i];
    }
    
    cout << "Enter target sum: ";
    cin >> target;
    
    cout << "\nChoose option:" << endl;
    cout << "1. Check if subset exists" << endl;
    cout << "2. Find all subsets" << endl;
    cout << "Enter choice (1-2): ";
    cin >> choice;
    
    cout << "\n========================================" << endl;
    cout << "Input:" << endl;
    cout << "  Array: ";
    printArray(arr);
    cout << "\n  Target sum: " << target << endl;
    cout << "========================================" << endl;
    
    if (choice == 1) {
        recursionCalls = 0;
        
        if (isSubsetSum(arr, n, target)) {
            cout << "\nResult: Subset with sum " << target << " EXISTS!" << endl;
        } else {
            cout << "\nResult: Subset with sum " << target << " DOES NOT EXIST!" << endl;
        }
        
        cout << "\nStatistics:" << endl;
        cout << "  Recursion calls: " << recursionCalls << endl;
    } else if (choice == 2) {
        solutionCount = 0;
        recursionCalls = 0;
        vector<int> subset;
        
        cout << "\nFinding all subsets..." << endl;
        findAllSubsets(arr, n, target, subset);
        
        cout << "\n\n========================================" << endl;
        cout << "Statistics:" << endl;
        cout << "  Total subsets found: " << solutionCount << endl;
        cout << "  Recursion calls: " << recursionCalls << endl;
        
        if (solutionCount == 0) {
            cout << "\nNo subset with sum " << target << " exists!" << endl;
        }
        cout << "========================================" << endl;
    }
    
    return 0;
}
