#include <iostream>
using namespace std;

// Tree Node structure
struct Node {
    int data;
    Node* left;
    Node* right;
    
    Node(int val) {
        data = val;
        left = nullptr;
        right = nullptr;
    }
};

// Check if two trees are mirror images
bool isMirror(Node* left, Node* right) {
    // Both empty
    if (left == nullptr && right == nullptr)
        return true;
    
    // One empty, one not
    if (left == nullptr || right == nullptr)
        return false;
    
    // Check data and recursively check subtrees
    return (left->data == right->data) &&
           isMirror(left->left, right->right) &&
           isMirror(left->right, right->left);
}

int main() {
    cout << "Symmetric Binary Tree Program" << endl;
    return 0;
}
