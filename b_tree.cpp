// Author: Samed Kahyaoglu
// Github: urtuba
// Restored: 2026

#include <fstream>
#include <iostream>
#include <istream>
#include <stdexcept>
#include <string>
#include <vector>

// Thrown when the input is wrong. The message names the line.
class InputError : public std::runtime_error {
public:
    InputError(int line, const std::string& what)
        : std::runtime_error("line " + std::to_string(line) + ": " + what) {}
};

enum class NumberStatus { Ok, NotANumber, OutOfRange };

// Reads a whole number (optional sign, then digits) that fits in an int.
NumberStatus parse_int(const std::string& word, int& value) {
    size_t i = 0;
    bool negative = false;
    if (!word.empty() && (word[0] == '+' || word[0] == '-')) {
        negative = (word[0] == '-');
        i = 1;
    }
    if (i == word.size()) {
        return NumberStatus::NotANumber;
    }

    // The magnitude stops growing once it is clearly too big for an int.
    unsigned long long magnitude = 0;
    for (; i < word.size(); i++) {
        if (word[i] < '0' || word[i] > '9') {
            return NumberStatus::NotANumber;
        }
        if (magnitude <= 2147483648ULL) {
            magnitude = magnitude * 10 + static_cast<unsigned>(word[i] - '0');
        }
    }
    if (magnitude > (negative ? 2147483648ULL : 2147483647ULL)) {
        return NumberStatus::OutOfRange;
    }
    value = negative ? static_cast<int>(-static_cast<long long>(magnitude))
                     : static_cast<int>(magnitude);
    return NumberStatus::Ok;
}

// Splits a line into words. Spaces, tabs and a carriage return (from a
// Windows line ending) separate words.
std::vector<std::string> split_words(const std::string& line) {
    std::vector<std::string> words;
    std::string word;
    for (char c : line) {
        if (c == ' ' || c == '\t' || c == '\r') {
            if (!word.empty()) {
                words.push_back(word);
                word.clear();
            }
        } else {
            word += c;
        }
    }
    if (!word.empty()) {
        words.push_back(word);
    }
    return words;
}

// A word for an error message. Long words are cut.
std::string shown(const std::string& word) {
    if (word.size() > 20) {
        return "'" + word.substr(0, 20) + "...'";
    }
    return "'" + word + "'";
}

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

    // Line 1 holds the node values, line 2 holds the target. Nothing but
    // whitespace may follow. Throws InputError if the input breaks a rule.
    void read(std::istream& input) {
        std::string line;
        if (!std::getline(input, line)) {
            throw InputError(1, "input is empty");
        }
        std::vector<std::string> words = split_words(line);
        if (words.empty()) {
            throw InputError(1, "no values (expected whole numbers separated by spaces)");
        }
        for (const std::string& word : words) {
            int value = 0;
            NumberStatus status = parse_int(word, value);
            if (status == NumberStatus::NotANumber) {
                throw InputError(1, "value " + shown(word) + " is not a whole number");
            }
            if (status == NumberStatus::OutOfRange) {
                throw InputError(1, "value " + shown(word) + " is out of int range");
            }
            add_node(value);
        }

        if (!std::getline(input, line)) {
            throw InputError(2, "target is missing");
        }
        words = split_words(line);
        if (words.empty()) {
            throw InputError(2, "target is missing");
        }
        int target = 0;
        NumberStatus status = parse_int(words[0], target);
        if (status == NumberStatus::NotANumber) {
            throw InputError(2, "target must be a whole number, got " + shown(words[0]));
        }
        if (status == NumberStatus::OutOfRange) {
            throw InputError(2, "target " + shown(words[0]) + " is out of int range");
        }
        if (words.size() > 1) {
            throw InputError(2, "target must be one number, got a second: " + shown(words[1]));
        }
        target_ = target;

        int line_number = 2;
        while (std::getline(input, line)) {
            line_number++;
            words = split_words(line);
            if (!words.empty()) {
                throw InputError(line_number,
                                 "unexpected data after the target: " + shown(words[0]));
            }
        }
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
    long long target_ = 0;
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
    void search(const Node* node, long long sum, std::string text, Side side) {
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

// Reads the tree from the stream and prints the two paths.
// Returns the exit status.
int run(std::istream& input) {
    try {
        Tree tree;
        tree.read(input);
        tree.print_paths();
    } catch (const InputError& e) {
        std::cerr << "error: " << e.what() << '\n';
        return 1;
    }
    return 0;
}

int main(int argc, char* argv[]) {
    if (argc > 2) {
        std::cerr << "usage: b_tree [INPUT_FILE]\n";
        return 2;
    }
    if (argc == 1) {
        return run(std::cin);
    }

    std::ifstream file(argv[1]);
    if (!file) {
        std::cerr << "error: cannot open " << argv[1] << '\n';
        return 1;
    }
    return run(file);
}
