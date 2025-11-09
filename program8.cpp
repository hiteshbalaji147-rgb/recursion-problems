#include <iostream>
#include <vector>
#include <iomanip>
#include <chrono>
#include <map>
using namespace std;
using namespace chrono;

int solutionCount = 0;
int recursionCalls = 0;
int memoHits = 0;

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

// Check if subset with given sum exists (Memoization)
bool isSubsetSumMemo(vector<int>& arr, int n, int sum, map<pair<int, int>, bool>& memo) {
    recursionCalls++;
    
    // Check if already computed
    pair<int, int> key = {n, sum};
    if (memo.find(key) != memo.end()) {
        memoHits++;
        return memo[key];
    }
    
    // Base cases
    if (sum == 0)
        return memo[key] = true;
    if (n == 0)
        return memo[key] = false;
    
    // If last element is greater than sum, ignore it
    if (arr[n - 1] > sum)
        return memo[key] = isSubsetSumMemo(arr, n - 1, sum, memo);
    
    // Check if sum can be obtained
    return memo[key] = isSubsetSumMemo(arr, n - 1, sum - arr[n - 1], memo) || 
                       isSubsetSumMemo(arr, n - 1, sum, memo);
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
void findAllSubsets(vector<int>& arr, int n, int sum, vector<int>& subset, bool display) {
    recursionCalls++;
    
    // Base case: if sum becomes 0, we found a subset
    if (sum == 0) {
        solutionCount++;
        if (display) {
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
        }
        return;
    }
    
    // Base case: no elements left
    if (n == 0)
        return;
    
    // If current element is greater than sum, skip it
    if (arr[n - 1] <= sum) {
        // Include current element
        subset.push_back(arr[n - 1]);
        findAllSubsets(arr, n - 1, sum - arr[n - 1], subset, display);
        subset.pop_back(); // Backtrack
    }
    
    // Exclude current element
    findAllSubsets(arr, n - 1, sum, subset, display);
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
    cout << "2. Check if subset exists (Memoization)" << endl;
    cout << "3. Check if subset exists (Dynamic Programming)" << endl;
    cout << "4. Find all subsets" << endl;
    cout << "5. Show DP table" << endl;
    cout << "6. Compare all algorithms" << endl;
    cout << "Enter choice (1-6): ";
    cin >> choice;
    
    cout << "\n========================================" << endl;
    cout << "Input:" << endl;
    cout << "  Array: ";
    printArray(arr);
    cout << "\n  Target sum: " << target << endl;
    cout << "========================================" << endl;
    
    if (choice == 1) {
        recursionCalls = 0;
        
        auto start = high_resolution_clock::now();
        bool result = isSubsetSumRecursive(arr, n, target);
        auto end = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(end - start);
        
        if (result) {
            cout << "\nResult: Subset with sum " << target << " EXISTS!" << endl;
        } else {
            cout << "\nResult: Subset with sum " << target << " DOES NOT EXIST!" << endl;
        }
        
        cout << "\nPerformance Metrics:" << endl;
        cout << "  Execution time: " << duration.count() << " μs" << endl;
        cout << "  Recursion calls: " << recursionCalls << endl;
        cout << "  Time Complexity: O(2^n)" << endl;
        cout << "  Space Complexity: O(n)" << endl;
    } else if (choice == 2) {
        recursionCalls = 0;
        memoHits = 0;
        map<pair<int, int>, bool> memo;
        
        auto start = high_resolution_clock::now();
        bool result = isSubsetSumMemo(arr, n, target, memo);
        auto end = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(end - start);
        
        if (result) {
            cout << "\nResult: Subset with sum " << target << " EXISTS!" << endl;
        } else {
            cout << "\nResult: Subset with sum " << target << " DOES NOT EXIST!" << endl;
        }
        
        cout << "\nPerformance Metrics:" << endl;
        cout << "  Execution time: " << duration.count() << " μs" << endl;
        cout << "  Recursion calls: " << recursionCalls << endl;
        cout << "  Memoization hits: " << memoHits << endl;
        cout << "  Cache hit rate: " << fixed << setprecision(2) 
             << (100.0 * memoHits / recursionCalls) << "%" << endl;
        cout << "  Memo table size: " << memo.size() << endl;
        cout << "  Time Complexity: O(n × sum)" << endl;
        cout << "  Space Complexity: O(n × sum)" << endl;
    } else if (choice == 3) {
        auto start = high_resolution_clock::now();
        bool result = isSubsetSumDP(arr, n, target);
        auto end = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(end - start);
        
        if (result) {
            cout << "\nResult: Subset with sum " << target << " EXISTS!" << endl;
        } else {
            cout << "\nResult: Subset with sum " << target << " DOES NOT EXIST!" << endl;
        }
        
        cout << "\nPerformance Metrics:" << endl;
        cout << "  Execution time: " << duration.count() << " μs" << endl;
        cout << "  DP table size: " << (n + 1) << " × " << (target + 1) << endl;
        cout << "  Time Complexity: O(n × sum)" << endl;
        cout << "  Space Complexity: O(n × sum)" << endl;
    } else if (choice == 4) {
        solutionCount = 0;
        recursionCalls = 0;
        vector<int> subset;
        
        char displayChoice;
        cout << "\nDisplay all subsets? (y/n): ";
        cin >> displayChoice;
        bool display = (displayChoice == 'y' || displayChoice == 'Y');
        
        cout << "\nFinding all subsets..." << endl;
        auto start = high_resolution_clock::now();
        findAllSubsets(arr, n, target, subset, display);
        auto end = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(end - start);
        
        cout << "\n\n========================================" << endl;
        cout << "Performance Metrics:" << endl;
        cout << "  Total subsets found: " << solutionCount << endl;
        cout << "  Execution time: " << duration.count() << " μs" << endl;
        cout << "  Recursion calls: " << recursionCalls << endl;
        
        if (solutionCount == 0) {
            cout << "\nNo subset with sum " << target << " exists!" << endl;
        }
        cout << "========================================" << endl;
    } else if (choice == 5) {
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
    } else if (choice == 6) {
        cout << "\n========================================" << endl;
        cout << "      Algorithm Comparison             " << endl;
        cout << "========================================" << endl;
        
        // Test 1: Recursive
        recursionCalls = 0;
        auto start1 = high_resolution_clock::now();
        bool result1 = isSubsetSumRecursive(arr, n, target);
        auto end1 = high_resolution_clock::now();
        auto duration1 = duration_cast<microseconds>(end1 - start1);
        int rec1 = recursionCalls;
        
        // Test 2: Memoization
        recursionCalls = 0;
        memoHits = 0;
        map<pair<int, int>, bool> memo;
        auto start2 = high_resolution_clock::now();
        bool result2 = isSubsetSumMemo(arr, n, target, memo);
        auto end2 = high_resolution_clock::now();
        auto duration2 = duration_cast<microseconds>(end2 - start2);
        int rec2 = recursionCalls;
        int hits2 = memoHits;
        
        // Test 3: DP
        auto start3 = high_resolution_clock::now();
        bool result3 = isSubsetSumDP(arr, n, target);
        auto end3 = high_resolution_clock::now();
        auto duration3 = duration_cast<microseconds>(end3 - start3);
        
        cout << "\nResults:" << endl;
        cout << "  Recursive: " << (result1 ? "EXISTS" : "DOES NOT EXIST") << endl;
        cout << "  Memoization: " << (result2 ? "EXISTS" : "DOES NOT EXIST") << endl;
        cout << "  DP: " << (result3 ? "EXISTS" : "DOES NOT EXIST") << endl;
        cout << "  All match: " << (result1 == result2 && result2 == result3 ? "✓" : "✗") << endl;
        
        cout << "\nPerformance Comparison:" << endl;
        cout << left << setw(25) << "Metric" << setw(15) << "Recursive" 
             << setw(15) << "Memoization" << "DP" << endl;
        cout << string(70, '-') << endl;
        
        cout << setw(25) << "Time (μs)" 
             << setw(15) << duration1.count() 
             << setw(15) << duration2.count() 
             << duration3.count() << endl;
        
        cout << setw(25) << "Recursion calls" 
             << setw(15) << rec1 
             << setw(15) << rec2 
             << "N/A" << endl;
        
        cout << setw(25) << "Memo hits" 
             << setw(15) << "N/A" 
             << setw(15) << hits2 
             << "N/A" << endl;
        
        cout << setw(25) << "Space used" 
             << setw(15) << "O(n)" 
             << setw(15) << memo.size() 
             << (n + 1) * (target + 1) << endl;
        
        // Find fastest
        long long fastest = min({duration1.count(), duration2.count(), duration3.count()});
        string winner = (fastest == duration1.count()) ? "Recursive" :
                       (fastest == duration2.count()) ? "Memoization" : "DP";
        
        cout << "\n========================================" << endl;
        cout << "Winner: " << winner << " algorithm" << endl;
        cout << "Speedup over recursive: " << fixed << setprecision(2) 
             << (double)duration1.count() / fastest << "x" << endl;
        cout << "========================================" << endl;
    }
    
    return 0;
}
