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
                   int index, vector<vector<int>>& solutions) {
    stats.recursionCalls++;
    
    int currentSum = calculateSum(current);
    
    // Base case: found a solution
    if (currentSum == target) {
        stats.solutionsFound++;
        solutions.push_back(current);
        return;
    }
    
    // Base case: exceeded target or reached end
    if (currentSum > target || index >= arr.size()) {
        stats.backtrackCount++;
        return;
    }
    
    // Include current element
    current.push_back(arr[index]);
    subsetSumUtil(arr, target, current, index + 1, solutions);
    current.pop_back();
    
    // Exclude current element
    subsetSumUtil(arr, target, current, index + 1, solutions);
}

// Find all subsets with given sum
vector<vector<int>> findSubsetSum(vector<int>& arr, int target) {
    vector<vector<int>> solutions;
    vector<int> current;
    
    stats = Stats();
    
    auto start = high_resolution_clock::now();
    subsetSumUtil(arr, target, current, 0, solutions);
    auto end = high_resolution_clock::now();
    
    stats.executionTime = duration_cast<microseconds>(end - start).count();
    
    return solutions;
}

// Check if subset sum exists (optimized - returns on first solution)
bool subsetSumExists(vector<int>& arr, int target, vector<int>& current, int index) {
    int currentSum = calculateSum(current);
    
    if (currentSum == target) return true;
    if (currentSum > target || index >= arr.size()) return false;
    
    // Include current element
    current.push_back(arr[index]);
    if (subsetSumExists(arr, target, current, index + 1)) return true;
    current.pop_back();
    
    // Exclude current element
    return subsetSumExists(arr, target, current, index + 1);
}

// Print statistics
void printStats() {
    cout << "\n========================================" << endl;
    cout << "           Statistics                  " << endl;
    cout << "========================================" << endl;
    cout << "Recursion calls: " << stats.recursionCalls << endl;
    cout << "Solutions found: " << stats.solutionsFound << endl;
    cout << "Backtrack operations: " << stats.backtrackCount << endl;
    cout << "Execution time: " << stats.executionTime << " μs" << endl;
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
    
    // Find all solutions
    vector<vector<int>> solutions = findSubsetSum(arr, target);
    
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
    
    printStats();
    
    return 0;
}
