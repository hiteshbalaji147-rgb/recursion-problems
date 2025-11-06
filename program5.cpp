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

// Create a new node
Node* createNode(int data) {
    return new Node(data);
}

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

// Check if tree is symmetric
bool isSymmetric(Node* root) {
    if (root == nullptr)
        return true;
    
    return isMirror(root->left, root->right);
}

int main() {
    cout << "========================================" << endl;
    cout << "    Symmetric Binary Tree Program      " << endl;
    cout << "========================================" << endl;
    
    // Create a symmetric tree
    Node* root = createNode(1);
    root->left = createNode(2);
    root->right = createNode(2);
    root->left->left = createNode(3);
    root->left->right = createNode(4);
    root->right->left = createNode(4);
    root->right->right = createNode(3);
    
    if (isSymmetric(root))
        cout << "Tree is symmetric!" << endl;
    else
        cout << "Tree is not symmetric!" << endl;
    
    return 0;
}
