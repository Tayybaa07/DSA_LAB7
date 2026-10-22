#include <iostream>
#include <string>
#include <stack>
#include <vector>
#include <algorithm>
#include <cctype>

using namespace std;

// Node structure for the character binary tree
struct TreeNode {
    char data;
    TreeNode* left;
    TreeNode* right;

    TreeNode(char val) : data(val), left(nullptr), right(nullptr) {}
};

// Node structure for the integer BST
struct BSTNode {
    int data;
    BSTNode* left;
    BSTNode* right;

    BSTNode(int val) : data(val), left(nullptr), right(nullptr) {}
};

//  Build binary tree from string
TreeNode* parseTreeString(const string& str, int& index) {
    if (index >= (int)str.length() || str[index] == ')' || str[index] == ',') {
        return nullptr;
    }

    // Read node character
    TreeNode* root = new TreeNode(str[index++]);

    // Check for child nodes
    if (index < (int)str.length() && str[index] == '(') {
        index++; // Skip '('

        // Left child (empty if next char is ',')
        if (index < (int)str.length() && str[index] != ',') {
            root->left = parseTreeString(str, index);
        }

        // Comma separating left and right children
        if (index < (int)str.length() && str[index] == ',') {
            index++;
        }

        // Right child (empty if next char is ')')
        if (index < (int)str.length() && str[index] != ')') {
            root->right = parseTreeString(str, index);
        }

        if (index < (int)str.length() && str[index] == ')') {
            index++; // Skip ')'
        }
    }

    return root;
}

// Preorder 
void generatePreorder(TreeNode* root) {
    if (!root) return;
    cout << root->data << " ";
    generatePreorder(root->left);
    generatePreorder(root->right);
}

//  Preorder 
void generatePreorderString(TreeNode* root) {
    if (!root) return;
    cout << root->data;
    if (root->left || root->right) {
        cout << "(";
        generatePreorderString(root->left);
        cout << ",";
        generatePreorderString(root->right);
        cout << ")";
    }
}

//  Inorder 
void generateInorderIterative(TreeNode* root) {
    stack<TreeNode*> s;
    TreeNode* curr = root;

    while (curr != nullptr || !s.empty()) {
        while (curr != nullptr) {
            s.push(curr);
            curr = curr->left;
        }
        curr = s.top();
        s.pop();
        cout << curr->data << " ";
        curr = curr->right;
    }
}

// Postorder 
void generatePostorderIterative(TreeNode* root) {
    if (!root) return;

    stack<TreeNode*> s1, s2;
    s1.push(root);

    while (!s1.empty()) {
        TreeNode* curr = s1.top();
        s1.pop();
        s2.push(curr);

        if (curr->left) s1.push(curr->left);
        if (curr->right) s1.push(curr->right);
    }

    while (!s2.empty()) {
        cout << s2.top()->data << " ";
        s2.pop();
    }
}

// 5. Validate tree 

bool validateNode(const string& s, size_t& i) {
    // Every node must start with an alphanumeric label
    if (i >= s.length() || !isalnum((unsigned char)s[i])) return false;
    i++;

    // Optional children group
    if (i < s.length() && s[i] == '(') {
        i++; // skip '('

        // Optional left child
        if (i < s.length() && s[i] != ',') {
            if (!validateNode(s, i)) return false;
        }

        // Mandatory comma
        if (i >= s.length() || s[i] != ',') return false;
        i++;

        // Optional right child
        if (i < s.length() && s[i] != ')') {
            if (!validateNode(s, i)) return false;
        }

        // Mandatory closing bracket
        if (i >= s.length() || s[i] != ')') return false;
        i++;
    }
    return true;
}

bool isValidTreeString(const string& str) {
    size_t i = 0;
    if (!validateNode(str, i)) return false;
    return i == str.length(); // nothing left over (e.g. "a(b)c" fails)
}

//Height of tree 
int getTreeHeight(TreeNode* root) {
    if (!root) return -1;
    return 1 + max(getTreeHeight(root->left), getTreeHeight(root->right));
}

//  Depth of each node
void printNodeDepths(TreeNode* root, int currentDepth = 0) {
    if (!root) return;
    cout << "Node '" << root->data << "' Depth: " << currentDepth << "\n";
    printNodeDepths(root->left, currentDepth + 1);
    printNodeDepths(root->right, currentDepth + 1);
}

//  BST insert + inorder
BSTNode* insertBST(BSTNode* root, int val) {
    if (!root) return new BSTNode(val);
    if (val < root->data)
        root->left = insertBST(root->left, val);
    else if (val > root->data)
        root->right = insertBST(root->right, val);
    return root;
}

void inorderBST(BSTNode* root) {
    if (!root) return;
    inorderBST(root->left);
    cout << root->data << " ";
    inorderBST(root->right);
}

// Memory cleanup 
void freeTree(TreeNode* root) {
    if (!root) return;
    freeTree(root->left);
    freeTree(root->right);
    delete root;
}

void freeBST(BSTNode* root) {
    if (!root) return;
    freeBST(root->left);
    freeBST(root->right);
    delete root;
}

int main() {
    string treeStr = "a(b(d,e),c(,f(g,)))";
    cout << "BINARY TREE OPERATIONS\n";
    cout << "Tree String: " << treeStr << "\n\n";

    // Validate the given string
    cout << (isValidTreeString(treeStr)
                 ? "[Validation] Tree string is VALID.\n"
                 : "[Validation] Tree string is INVALID.\n");

    // Extra validation tests
    vector<string> tests = {"a(b)c", "(a)", "()", "a(b(d,e)", "a(b,c,d)", "a(b,c))", "a", "a(,)"};
    cout << "\nValidation tests:\n";
    for (const string& t : tests) {
        cout << "  " << t << " -> " << (isValidTreeString(t) ? "VALID" : "INVALID") << "\n";
    }

    // Build tree
    int index = 0;
    TreeNode* root = parseTreeString(treeStr, index);

    // Traversals
    cout << "\nPreorder Traversal (Recursive): ";
    generatePreorder(root);
    cout << "\n";

    cout << "Preorder String Form (Recursive): ";
    generatePreorderString(root);
    cout << "\n";

    cout << "Inorder Traversal (Iterative Stack): ";
    generateInorderIterative(root);
    cout << "\n";

    cout << "Postorder Traversal (Iterative Stack): ";
    generatePostorderIterative(root);
    cout << "\n";

    // Height and depth
    cout << "\nTree Height: " << getTreeHeight(root) << "\n";
    cout << "\nNode Depths:\n";
    printNodeDepths(root);

    // BST generation
    cout << "\nBST Generation from Sequence\n";
    vector<int> sequence = {50, 30, 70, 20, 40, 60, 80};
    BSTNode* bstRoot = nullptr;

    for (int val : sequence) {
        bstRoot = insertBST(bstRoot, val);
    }

    cout << "Sequence inserted: [50, 30, 70, 20, 40, 60, 80]\n";
    cout << "BST Inorder Traversal (Sorted): ";
    inorderBST(bstRoot);
    cout << "\n";

    // Free memory
    freeTree(root);
    freeBST(bstRoot);

    return 0;
}