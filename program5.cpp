#include <iostream>
#include <queue>
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

// Calculate height of tree
int height(Node* root) {
    if (root == nullptr)
        return 0;
    
    int leftHeight = height(root->left);
    int rightHeight = height(root->right);
    
    return 1 + max(leftHeight, rightHeight);
}

// Count total nodes
int countNodes(Node* root) {
    if (root == nullptr)
        return 0;
    
    return 1 + countNodes(root->left) + countNodes(root->right);
}

// Count leaf nodes
int countLeaves(Node* root) {
    if (root == nullptr)
        return 0;
    
    if (root->left == nullptr && root->right == nullptr)
        return 1;
    
    return countLeaves(root->left) + countLeaves(root->right);
}

// Print tree structure
void printTree(Node* root, string indent = "", bool isRight = true) {
    if (root == nullptr)
        return;
    
    cout << indent;
    if (isRight) {
        cout << "R----";
        indent += "     ";
    } else {
        cout << "L----";
        indent += "|    ";
    }
    
    cout << root->data << endl;
    
    if (root->left != nullptr || root->right != nullptr) {
        if (root->left != nullptr)
            printTree(root->left, indent, false);
        else {
            cout << indent << "L----" << "NULL" << endl;
        }
        
        if (root->right != nullptr)
            printTree(root->right, indent, true);
        else {
            cout << indent << "R----" << "NULL" << endl;
        }
    }
}

// Inorder traversal (Left, Root, Right)
void inorder(Node* root) {
    if (root == nullptr)
        return;
    
    inorder(root->left);
    cout << root->data << " ";
    inorder(root->right);
}

// Preorder traversal (Root, Left, Right)
void preorder(Node* root) {
    if (root == nullptr)
        return;
    
    cout << root->data << " ";
    preorder(root->left);
    preorder(root->right);
}

// Postorder traversal (Left, Right, Root)
void postorder(Node* root) {
    if (root == nullptr)
        return;
    
    postorder(root->left);
    postorder(root->right);
    cout << root->data << " ";
}

// Level order traversal (BFS)
void levelOrder(Node* root) {
    if (root == nullptr)
        return;
    
    queue<Node*> q;
    q.push(root);
    
    while (!q.empty()) {
        Node* current = q.front();
        q.pop();
        
        cout << current->data << " ";
        
        if (current->left != nullptr)
            q.push(current->left);
        
        if (current->right != nullptr)
            q.push(current->right);
    }
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

// Display tree info
void displayTreeInfo(Node* root, string treeName) {
    cout << "\n========================================" << endl;
    cout << "  " << treeName << endl;
    cout << "========================================" << endl;
    
    cout << "\nTree Structure:" << endl;
    printTree(root);
    
    cout << "\nTree Statistics:" << endl;
    cout << "Height: " << height(root) << endl;
    cout << "Total nodes: " << countNodes(root) << endl;
    cout << "Leaf nodes: " << countLeaves(root) << endl;
    
    cout << "\nTree Traversals:" << endl;
    cout << "Inorder: ";
    inorder(root);
    cout << endl;
    
    cout << "Preorder: ";
    preorder(root);
    cout << endl;
    
    cout << "Postorder: ";
    postorder(root);
    cout << endl;
    
    cout << "Level Order: ";
    levelOrder(root);
    cout << endl;
    
    cout << "\nSymmetry Check: ";
    if (isSymmetric(root))
        cout << "✓ Tree is SYMMETRIC!" << endl;
    else
        cout << "✗ Tree is NOT symmetric!" << endl;
}

int main() {
    cout << "========================================" << endl;
    cout << "    Symmetric Binary Tree Program      " << endl;
    cout << "========================================" << endl;
    
    // Test Case 1: Symmetric tree
    Node* tree1 = createNode(1);
    tree1->left = createNode(2);
    tree1->right = createNode(2);
    tree1->left->left = createNode(3);
    tree1->left->right = createNode(4);
    tree1->right->left = createNode(4);
    tree1->right->right = createNode(3);
    
    displayTreeInfo(tree1, "Test Case 1: Symmetric Tree");
    
    // Test Case 2: Asymmetric tree
    Node* tree2 = createNode(1);
    tree2->left = createNode(2);
    tree2->right = createNode(2);
    tree2->left->right = createNode(3);
    tree2->right->right = createNode(3);
    
    displayTreeInfo(tree2, "Test Case 2: Asymmetric Tree");
    
    // Test Case 3: Single node (symmetric)
    Node* tree3 = createNode(1);
    
    displayTreeInfo(tree3, "Test Case 3: Single Node Tree");
    
    return 0;
}
