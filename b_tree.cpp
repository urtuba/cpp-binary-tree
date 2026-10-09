// Author: Samed Kahyaoglu
// Github: urtuba
// Restored: 2026

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

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
    ~Tree() { free_nodes(root_); }

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

    // Search the whole tree and print the left path, then the right path.
    void print_paths() {
        if (root_ != nullptr) {
            // The root belongs to neither subtree, but it is counted as left
            // here. So a target equal to the root's value prints the root
            // alone as the left path. This is a known bug.
            search(root_, 0, "Path Found:", Side::Left);
        }
        std::cout << left_.text << '\n';
        std::cout << right_.text << '\n';
    }

private:
    Node* root_ = nullptr;
    int count_ = 0;
    int target_ = 0;
    Result left_;
    Result right_;

    static void free_nodes(Node* node) {
        if (node == nullptr) {
            return;
        }
        free_nodes(node->left);
        free_nodes(node->right);
        delete node;
    }

    // Put the value in the first free place, so that the tree stays complete.
    void add_node(int value) {
        Node* node = new Node{value, count_ + 1, nullptr, nullptr};
        if (root_ == nullptr) {
            root_ = node;
            count_++;
            return;
        }

        // Walk from the new node's number up to the root. Each step tells
        // whether the node is a left child (even) or a right child (odd).
        // The steps are collected bottom-up, so the last one is next to the root.
        int steps[10];
        int step_count = 0;
        for (int n = node->number; n != 1; n /= 2) {
            steps[step_count++] = (n % 2 == 0) ? 1 : 2;
        }

        // Follow the steps from the root down to the parent.
        Node* parent = root_;
        for (int i = step_count - 1; i > 0; i--) {
            parent = (steps[i] == 1) ? parent->left : parent->right;
        }

        if (steps[0] == 1) {
            parent->left = node;
        } else {
            parent->right = node;
        }
        count_++;
    }

    // Preorder search. It carries the sum and the text of the path from the
    // root to this node. The first path found for a side is kept.
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

        // The root's two children start the two sides. Below them, the side
        // does not change.
        bool is_root = (node == root_);
        if (node->left != nullptr) {
            search(node->left, sum, text, is_root ? Side::Left : side);
        }
        if (node->right != nullptr) {
            search(node->right, sum, text, is_root ? Side::Right : side);
        }
    }
};

int main(int, char* argv[]) {
    Tree tree;
    tree.read_file(argv[1]);
    tree.print_paths();
    return 0;
}
