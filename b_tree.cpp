// Author: Samed Kahyaoglu
// Github: urtuba
// Restored: 2026

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

// One node of the tree. Nodes are numbered level by level, left to right,
// starting with 1 at the root. Node k has the children 2k and 2k+1.
struct Node {
    int value;
    int number;
    Node* left;
    Node* right;
};

// Which subtree of the root a path ends in.
enum class Side { Left, Right };

// The best path found for one side.
struct Result {
    bool found = false;
    std::string text = "No Path Found";
};

// A complete binary tree that owns its nodes and frees them when destroyed.
class Tree {
public:
    Tree() = default;
    Tree(const Tree&) = delete;
    Tree& operator=(const Tree&) = delete;
    ~Tree() {
        for (Node* node : nodes_) {
            delete node;
        }
    }

    // Line 1 of the file holds the node values, line 2 holds the target.
    void read_file(const char* file_name) {
        std::ifstream input(file_name);
        std::string line;
        std::getline(input, line);
        std::istringstream values(line);
        int value;
        while (values >> value) {
            add_node(value);
        }
        input >> target_;
    }

    // Search both subtrees of the root and print the left path, then the
    // right path. A path has at least one node below the root, because the
    // root alone is in neither subtree.
    void print_paths() {
        if (!nodes_.empty()) {
            const Node* root = nodes_[0];
            std::string text = "Path Found: " + std::to_string(root->value);
            if (root->left != nullptr) {
                search(root->left, root->value, text, Side::Left);
            }
            if (root->right != nullptr) {
                search(root->right, root->value, text, Side::Right);
            }
        }
        std::cout << left_.text << '\n';
        std::cout << right_.text << '\n';
    }

private:
    // nodes_[k - 1] is node number k, so the parent of node k is node k / 2.
    std::vector<Node*> nodes_;
    int target_ = 0;
    Result left_;
    Result right_;

    // Put the value in the first free place, so that the tree stays complete.
    void add_node(int value) {
        int number = static_cast<int>(nodes_.size()) + 1;
        Node* node = new Node{value, number, nullptr, nullptr};
        nodes_.push_back(node);
        if (number == 1) {
            return;
        }
        Node* parent = nodes_[number / 2 - 1];
        if (number % 2 == 0) {
            parent->left = node;
        } else {
            parent->right = node;
        }
    }

    // Preorder search of one subtree. It carries the sum and the text of the
    // path from the root to this node. The first path found for a side is kept.
    void search(const Node* node, int sum, std::string text, Side side) {
        sum += node->value;
        text += " " + std::to_string(node->value);

        if (sum == target_) {
            Result& result = (side == Side::Left) ? left_ : right_;
            if (!result.found) {
                result.found = true;
                result.text = text;
            }
        }

        if (node->left != nullptr) {
            search(node->left, sum, text, side);
        }
        if (node->right != nullptr) {
            search(node->right, sum, text, side);
        }
    }
};

int main(int, char* argv[]) {
    Tree tree;
    tree.read_file(argv[1]);
    tree.print_paths();
    return 0;
}
