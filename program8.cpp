#include <iostream>
#include <vector>
#include <iomanip>
using namespace std;

int solutionCount = 0;
int recursionCalls = 0;

// Check if subset with given sum exists (Recursive)
bool isSubsetSumRecursive(vector<int>& arr, int n, int sum) {
    recursionCalls++;
    
    // Base cases
    if (sum == 0)
        return true;
    if (n == 0)
        return false;
    
    // If last element is greater than sum, ignore it
    if (arr[n - 1] > sum)
        return isSubsetSumRecursive(arr, n - 1, sum);
    
    // Check if sum can be obtained by:
    // 1. Including the last element
    // 2. Excluding the last element
    return isSubsetSumRecursive(arr, n - 1, sum - arr[n - 1]) || 
           isSubsetSumRecursive(arr, n - 1, sum);
}

// Check if subset with given sum exists (Dynamic Programming)
bool isSubsetSumDP(vector<int>& arr, int n, int sum) {
    // Create DP table
    vector<vector<bool>> dp(n + 1, vector<bool>(sum + 1, false));
    
    // If sum is 0, answer is true (empty subset)
    for (int i = 0; i <= n; i++)
        dp[i][0] = true;
    
    // Fill the DP table
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= sum; j++) {
            if (arr[i - 1] > j) {
                // Exclude current element
                dp[i][j] = dp[i - 1][j];
            } else {
                // Include or exclude current element
                dp[i][j] = dp[i - 1][j] || dp[i - 1][j - arr[i - 1]];
            }
        }
    }
    
    return dp[n][sum];
}

// Print DP table
void printDPTable(vector<int>& arr, int n, int sum) {
    vector<vector<bool>> dp(n + 1, vector<bool>(sum + 1, false));
    
    for (int i = 0; i <= n; i++)
        dp[i][0] = true;
    
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= sum; j++) {
            if (arr[i - 1] > j) {
                dp[i][j] = dp[i - 1][j];
            } else {
                dp[i][j] = dp[i - 1][j] || dp[i - 1][j - arr[i - 1]];
            }
        }
    }
    
    // Print table
    cout << "\nDP Table (1 = possible, 0 = not possible):" << endl;
    cout << "========================================" << endl;
    
    // Header
    cout << setw(6) << "Sum→";
    for (int j = 0; j <= sum; j++) {
        cout << setw(3) << j;
    }
    cout << endl;
    
    cout << "Elem↓" << endl;
    
    // Rows
    for (int i = 0; i <= n; i++) {
        if (i == 0)
            cout << setw(6) << "{}";
        else
            cout << setw(6) << arr[i - 1];
        
        for (int j = 0; j <= sum; j++) {
            cout << setw(3) << (dp[i][j] ? "1" : "0");
        }
        cout << endl;
    }
    cout << "========================================" << endl;
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
    
    if (target < 0) {
        cout << "Error: Target sum must be non-negative!" << endl;
        return 1;
    }
    
    cout << "\nChoose option:" << endl;
    cout << "1. Check if subset exists (Recursive)" << endl;
    cout << "2. Check if subset exists (Dynamic Programming)" << endl;
    cout << "3. Find all subsets" << endl;
    cout << "4. Show DP table" << endl;
    cout << "5. Compare Recursive vs DP" << endl;
    cout << "Enter choice (1-5): ";
    cin >> choice;
    
    cout << "\n========================================" << endl;
    cout << "Input:" << endl;
    cout << "  Array: ";
    printArray(arr);
    cout << "\n  Target sum: " << target << endl;
    cout << "========================================" << endl;
    
    if (choice == 1) {
        recursionCalls = 0;
        
        if (isSubsetSumRecursive(arr, n, target)) {
            cout << "\nResult: Subset with sum " << target << " EXISTS!" << endl;
        } else {
            cout << "\nResult: Subset with sum " << target << " DOES NOT EXIST!" << endl;
        }
        
        cout << "\nStatistics:" << endl;
        cout << "  Recursion calls: " << recursionCalls << endl;
        cout << "  Time Complexity: O(2^n)" << endl;
        cout << "  Space Complexity: O(n)" << endl;
    } else if (choice == 2) {
        if (isSubsetSumDP(arr, n, target)) {
            cout << "\nResult: Subset with sum " << target << " EXISTS!" << endl;
        } else {
            cout << "\nResult: Subset with sum " << target << " DOES NOT EXIST!" << endl;
        }
        
        cout << "\nStatistics:" << endl;
        cout << "  Time Complexity: O(n × sum)" << endl;
        cout << "  Space Complexity: O(n × sum)" << endl;
    } else if (choice == 3) {
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
    } else if (choice == 4) {
        if (target > 50) {
            cout << "\nWarning: Large target sum will create a big table!" << endl;
            cout << "Table size: " << (n + 1) << " × " << (target + 1) << endl;
        }
        
        printDPTable(arr, n, target);
        
        if (isSubsetSumDP(arr, n, target)) {
            cout << "\nResult: Subset with sum " << target << " EXISTS!" << endl;
        } else {
            cout << "\nResult: Subset with sum " << target << " DOES NOT EXIST!" << endl;
        }
    } else if (choice == 5) {
        cout << "\n========================================" << endl;
        cout << "    Recursive vs Dynamic Programming   " << endl;
        cout << "========================================" << endl;
        
        // Test recursive
        recursionCalls = 0;
        bool result1 = isSubsetSumRecursive(arr, n, target);
        int recCalls = recursionCalls;
        
        // Test DP
        bool result2 = isSubsetSumDP(arr, n, target);
        
        cout << "\nResults:" << endl;
        cout << "  Recursive: " << (result1 ? "EXISTS" : "DOES NOT EXIST") << endl;
        cout << "  DP: " << (result2 ? "EXISTS" : "DOES NOT EXIST") << endl;
        cout << "  Match: " << (result1 == result2 ? "✓" : "✗") << endl;
        
        cout << "\nComparison:" << endl;
        cout << left << setw(25) << "Metric" << setw(20) << "Recursive" << "DP" << endl;
        cout << string(60, '-') << endl;
        cout << setw(25) << "Recursion calls" << setw(20) << recCalls << "N/A" << endl;
        cout << setw(25) << "Time Complexity" << setw(20) << "O(2^n)" << "O(n × sum)" << endl;
        cout << setw(25) << "Space Complexity" << setw(20) << "O(n)" << "O(n × sum)" << endl;
        cout << setw(25) << "Best for" << setw(20) << "Small n" << "Large n" << endl;
        
        cout << "\n========================================" << endl;
    }
    
    return 0;
}
