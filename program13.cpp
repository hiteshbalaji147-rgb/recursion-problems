#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <iomanip>
using namespace std;
using namespace chrono;

// Statistics structure
struct Stats {
    int recursionCalls = 0;
    int solutionsFound = 0;
    int backtrackCount = 0;
    int dpCells = 0;
    long long executionTime = 0;
};

Stats stats;

// Print current subset
void printSubset(vector<int>& subset) {
    cout << "{ ";
    for (int i = 0; i < subset.size(); i++) {
        cout << subset[i];
        if (i < subset.size() - 1) cout << ", ";
    }
    cout << " }";
}

// Calculate sum of vector
int calculateSum(vector<int>& arr) {
    int sum = 0;
    for (int num : arr) sum += num;
    return sum;
}

// Backtracking approach to find all subsets with given sum
void subsetSumUtil(vector<int>& arr, int target, vector<int>& current, 
                   int index, vector<vector<int>>& solutions, bool findAll = true) {
    stats.recursionCalls++;
    
    int currentSum = calculateSum(current);
    
    // Base case: found a solution
    if (currentSum == target) {
        stats.solutionsFound++;
        solutions.push_back(current);
        if (!findAll) return; // Stop if only need one solution
        return;
    }
    
    // Base case: exceeded target or reached end
    if (currentSum > target || index >= arr.size()) {
        stats.backtrackCount++;
        return;
    }
    
    // Include current element
    current.push_back(arr[index]);
    subsetSumUtil(arr, target, current, index + 1, solutions, findAll);
    current.pop_back();
    
    // Exclude current element
    subsetSumUtil(arr, target, current, index + 1, solutions, findAll);
}

// Find all subsets with given sum (Backtracking)
vector<vector<int>> findSubsetSumBacktracking(vector<int>& arr, int target) {
    vector<vector<int>> solutions;
    vector<int> current;
    
    stats = Stats();
    
    auto start = high_resolution_clock::now();
    subsetSumUtil(arr, target, current, 0, solutions);
    auto end = high_resolution_clock::now();
    
    stats.executionTime = duration_cast<microseconds>(end - start).count();
    
    return solutions;
}

// Dynamic Programming approach - check if subset sum exists
bool subsetSumDP(vector<int>& arr, int target) {
    int n = arr.size();
    
    // Create DP table
    vector<vector<bool>> dp(n + 1, vector<bool>(target + 1, false));
    
    // Base case: sum 0 is always possible (empty subset)
    for (int i = 0; i <= n; i++) {
        dp[i][0] = true;
    }
    
    stats = Stats();
    auto start = high_resolution_clock::now();
    
    // Fill the DP table
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= target; j++) {
            stats.dpCells++;
            
            // Exclude current element
            dp[i][j] = dp[i - 1][j];
            
            // Include current element if possible
            if (j >= arr[i - 1]) {
                dp[i][j] = dp[i][j] || dp[i - 1][j - arr[i - 1]];
            }
        }
    }
    
    auto end = high_resolution_clock::now();
    stats.executionTime = duration_cast<microseconds>(end - start).count();
    
    return dp[n][target];
}

// DP approach to find one subset (reconstruct solution)
vector<int> findSubsetDP(vector<int>& arr, int target) {
    int n = arr.size();
    vector<int> result;
    
    // Create DP table
    vector<vector<bool>> dp(n + 1, vector<bool>(target + 1, false));
    
    for (int i = 0; i <= n; i++) {
        dp[i][0] = true;
    }
    
    stats = Stats();
    auto start = high_resolution_clock::now();
    
    // Fill the DP table
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= target; j++) {
            stats.dpCells++;
            dp[i][j] = dp[i - 1][j];
            if (j >= arr[i - 1]) {
                dp[i][j] = dp[i][j] || dp[i - 1][j - arr[i - 1]];
            }
        }
    }
    
    // Reconstruct the subset
    if (dp[n][target]) {
        int i = n, j = target;
        while (i > 0 && j > 0) {
            // If current element was included
            if (j >= arr[i - 1] && dp[i - 1][j - arr[i - 1]]) {
                result.push_back(arr[i - 1]);
                j -= arr[i - 1];
            }
            i--;
        }
    }
    
    auto end = high_resolution_clock::now();
    stats.executionTime = duration_cast<microseconds>(end - start).count();
    
    return result;
}

// Optimized backtracking with pruning
void subsetSumOptimized(vector<int>& arr, int target, vector<int>& current,
                        int index, int currentSum, vector<vector<int>>& solutions) {
    stats.recursionCalls++;
    
    // Found solution
    if (currentSum == target) {
        stats.solutionsFound++;
        solutions.push_back(current);
        return;
    }
    
    // Pruning: stop if exceeded or reached end
    if (currentSum > target || index >= arr.size()) {
        stats.backtrackCount++;
        return;
    }
    
    // Include current element
    current.push_back(arr[index]);
    subsetSumOptimized(arr, target, current, index + 1, currentSum + arr[index], solutions);
    current.pop_back();
    
    // Exclude current element
    subsetSumOptimized(arr, target, current, index + 1, currentSum, solutions);
}

// Find subsets with optimized backtracking
vector<vector<int>> findSubsetSumOptimized(vector<int>& arr, int target) {
    vector<vector<int>> solutions;
    vector<int> current;
    
    // Sort array for better pruning
    sort(arr.begin(), arr.end());
    
    stats = Stats();
    auto start = high_resolution_clock::now();
    subsetSumOptimized(arr, target, current, 0, 0, solutions);
    auto end = high_resolution_clock::now();
    
    stats.executionTime = duration_cast<microseconds>(end - start).count();
    
    return solutions;
}

// Print statistics
void printStats(string algorithm = "") {
    cout << "\n========================================" << endl;
    cout << "           Statistics";
    if (!algorithm.empty()) cout << " (" << algorithm << ")";
    cout << endl;
    cout << "========================================" << endl;
    
    if (stats.recursionCalls > 0) {
        cout << "Recursion calls: " << stats.recursionCalls << endl;
        cout << "Solutions found: " << stats.solutionsFound << endl;
        cout << "Backtrack operations: " << stats.backtrackCount << endl;
    }
    
    if (stats.dpCells > 0) {
        cout << "DP cells computed: " << stats.dpCells << endl;
    }
    
    cout << "Execution time: " << stats.executionTime << " μs" << endl;
    cout << "========================================" << endl;
}

// Print complexity analysis
void printComplexityAnalysis() {
    cout << "\n========================================" << endl;
    cout << "      Complexity Analysis              " << endl;
    cout << "========================================" << endl;
    cout << "\n1. Backtracking:" << endl;
    cout << "   Time: O(2^n) - explores all subsets" << endl;
    cout << "   Space: O(n) - recursion stack" << endl;
    cout << "   Best for: Finding all solutions" << endl;
    
    cout << "\n2. Dynamic Programming:" << endl;
    cout << "   Time: O(n × target)" << endl;
    cout << "   Space: O(n × target) - DP table" << endl;
    cout << "   Best for: Checking existence, large targets" << endl;
    
    cout << "\n3. Optimized Backtracking:" << endl;
    cout << "   Time: O(2^n) worst case, better with pruning" << endl;
    cout << "   Space: O(n) - recursion stack" << endl;
    cout << "   Best for: Sorted arrays, early pruning" << endl;
    
    cout << "\nNote: n = array size, target = target sum" << endl;
    cout << "========================================" << endl;
}

int main() {
    cout << "========================================" << endl;
    cout << "       Subset Sum Problem              " << endl;
    cout << "========================================" << endl;
    
    int n, target;
    cout << "\nEnter number of elements: ";
    cin >> n;
    
    vector<int> arr(n);
    cout << "Enter elements: ";
    for (int i = 0; i < n; i++) {
        cin >> arr[i];
    }
    
    cout << "Enter target sum: ";
    cin >> target;
    
    cout << "\nInput array: ";
    printSubset(arr);
    cout << endl;
    cout << "Target sum: " << target << endl;
    
    int choice;
    cout << "\nSelect algorithm:" << endl;
    cout << "1. Backtracking (find all solutions)" << endl;
    cout << "2. Dynamic Programming (check existence)" << endl;
    cout << "3. DP with reconstruction (find one solution)" << endl;
    cout << "4. Optimized Backtracking (sorted + pruning)" << endl;
    cout << "5. Compare all algorithms" << endl;
    cout << "Enter choice (1-5): ";
    cin >> choice;
    
    if (choice == 1) {
        vector<vector<int>> solutions = findSubsetSumBacktracking(arr, target);
        
        cout << "\n========================================" << endl;
        cout << "           Results                     " << endl;
        cout << "========================================" << endl;
        
        if (solutions.empty()) {
            cout << "No subset found with sum = " << target << endl;
        } else {
            cout << "Found " << solutions.size() << " subset(s) with sum = " << target << ":" << endl;
            cout << endl;
            
            for (int i = 0; i < solutions.size(); i++) {
                cout << "Solution " << (i + 1) << ": ";
                printSubset(solutions[i]);
                cout << " → Sum = " << calculateSum(solutions[i]) << endl;
            }
        }
        
        printStats("Backtracking");
        
    } else if (choice == 2) {
        bool exists = subsetSumDP(arr, target);
        
        cout << "\n========================================" << endl;
        cout << "           Results                     " << endl;
        cout << "========================================" << endl;
        
        if (exists) {
            cout << "✓ Subset with sum = " << target << " EXISTS" << endl;
        } else {
            cout << "✗ No subset with sum = " << target << " exists" << endl;
        }
        
        printStats("Dynamic Programming");
        
    } else if (choice == 3) {
        vector<int> solution = findSubsetDP(arr, target);
        
        cout << "\n========================================" << endl;
        cout << "           Results                     " << endl;
        cout << "========================================" << endl;
        
        if (solution.empty()) {
            cout << "No subset found with sum = " << target << endl;
        } else {
            cout << "Found subset: ";
            printSubset(solution);
            cout << " → Sum = " << calculateSum(solution) << endl;
        }
        
        printStats("DP with Reconstruction");
        
    } else if (choice == 4) {
        vector<vector<int>> solutions = findSubsetSumOptimized(arr, target);
        
        cout << "\n========================================" << endl;
        cout << "           Results                     " << endl;
        cout << "========================================" << endl;
        
        if (solutions.empty()) {
            cout << "No subset found with sum = " << target << endl;
        } else {
            cout << "Found " << solutions.size() << " subset(s) with sum = " << target << ":" << endl;
            cout << endl;
            
            for (int i = 0; i < solutions.size(); i++) {
                cout << "Solution " << (i + 1) << ": ";
                printSubset(solutions[i]);
                cout << " → Sum = " << calculateSum(solutions[i]) << endl;
            }
        }
        
        printStats("Optimized Backtracking");
        
    } else if (choice == 5) {
        cout << "\n========================================" << endl;
        cout << "    Algorithm Comparison               " << endl;
        cout << "========================================" << endl;
        
        // Backtracking
        vector<vector<int>> solutions1 = findSubsetSumBacktracking(arr, target);
        Stats stats1 = stats;
        
        // DP existence check
        bool exists = subsetSumDP(arr, target);
        Stats stats2 = stats;
        
        // DP with reconstruction
        vector<int> solution3 = findSubsetDP(arr, target);
        Stats stats3 = stats;
        
        // Optimized backtracking
        vector<vector<int>> solutions4 = findSubsetSumOptimized(arr, target);
        Stats stats4 = stats;
        
        cout << "\nResults:" << endl;
        cout << left << setw(25) << "Algorithm" << setw(15) << "Solutions" << setw(15) << "Time (μs)" << endl;
        cout << string(55, '-') << endl;
        cout << setw(25) << "Backtracking" << setw(15) << solutions1.size() << setw(15) << stats1.executionTime << endl;
        cout << setw(25) << "DP (existence)" << setw(15) << (exists ? "Yes" : "No") << setw(15) << stats2.executionTime << endl;
        cout << setw(25) << "DP (reconstruction)" << setw(15) << (solution3.empty() ? 0 : 1) << setw(15) << stats3.executionTime << endl;
        cout << setw(25) << "Optimized Backtracking" << setw(15) << solutions4.size() << setw(15) << stats4.executionTime << endl;
        
        cout << "\nFastest: ";
        long long minTime = min({stats1.executionTime, stats2.executionTime, stats3.executionTime, stats4.executionTime});
        if (minTime == stats2.executionTime) cout << "DP (existence check)";
        else if (minTime == stats3.executionTime) cout << "DP (reconstruction)";
        else if (minTime == stats4.executionTime) cout << "Optimized Backtracking";
        else cout << "Backtracking";
        cout << " (" << minTime << " μs)" << endl;
        
        cout << "========================================" << endl;
    }
    
    printComplexityAnalysis();
    
    return 0;
}
