//SHOULD BE COMPILED WITH -std=c++11 ARGUMENT

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cstdlib>
#include <cmath>
using namespace std;

struct Node{
  int data; // holds input value
  int nodeNum; // holds order of node
  Node* left;
  Node* right;
};

struct Tree {
  Node* root;
  bool left; // true: left path found
  bool right; // true: right path found
  string leftPath; // text will be written for left path
  string rightPath; // same for right
  int nodeCount; // how many nodes in this tree.
  int target; // value to reach
  int dir[10]; // I use it to find position of a node
  void create() {
    root = NULL;
    nodeCount = 0;
    for (int i = 0; i < 10; i++) {
      dir[i]=0;
    }
    target = 0;
    left = false;
    right = false;
    leftPath = "No Path Found";
    rightPath = "No Path Found";
  } // initializer
  Node* newnode(int data) {
    Node* newNode = new Node;
    newNode -> data = data;
    newNode -> left = NULL;
    newNode -> right = NULL;
    newNode -> nodeNum = nodeCount + 1;
    return newNode;
  } // creates a node with given integer
  void add_node(int data) { // calls newnode() and places it to required position
    if (nodeCount == 0) {
      root = newnode(data);
      nodeCount++;
      return;
    }
    if (nodeCount != 0) {
      int x = nodeCount + 1;
      int dirCount = 0; // stands for branch number to arrive target place
      while (x != 1) { // this math algorithm gives me route
          if (x % 2 == 0) {
            x = x / 2;
            dir[dirCount++] = 1; //stands for left
          } else {
            x = (x - 1) / 2;
            dir[dirCount++] = 2; //stands for right
          }
        }
        Node* node_ptr = root;
        while (true) { // this loop goes over route which was founded
          if (dirCount == 1) break;
          if (dir[dirCount - 1] == 1) {
            node_ptr = node_ptr -> left;
            dirCount--;
          } else {
            node_ptr = node_ptr -> right;
            dirCount--;
          }
        }
        if (dir[0] == 1) { // last step on route
          node_ptr -> left = newnode(data);
          nodeCount++;
          return;
        } else {
          node_ptr -> right = newnode(data);
          nodeCount++;
          return;
        }
      }
    }
  void read_file(char** argv) {
    ifstream inputFile(argv[1]);
    string line;
    getline(inputFile, line);
    stringstream myLine(line); // i get values from first line
    int a;
    while(myLine >> a) add_node(a);
    inputFile >> target; // value in the second line
  }
  void path_finder(int sum = 0, Node* ptr = NULL, string text = "Path Found:") {
    if(ptr == NULL) ptr = root;

    sum = sum + ptr -> data;
    text = text + " " + to_string(ptr -> data);
    if(target == sum)
    {
      if ((ptr -> nodeNum < 3*pow(2, floor(log2(ptr->nodeNum)))/2) && (left == false)) { // first condition of if is a math formula which recognizes which subtree (L/R) are we in.
        left = true;
          leftPath = text;
      }
      if (!(ptr->nodeNum < 3*pow(2, floor(log2(ptr->nodeNum)))/2) && (right == false)){
        right = true;
        rightPath = text;
      }
    }
    if (ptr -> left != NULL) path_finder(sum, ptr->left, text);
    if (ptr -> right != NULL) path_finder(sum, ptr-> right, text);
    return;
  }
};

int main(int argc, char *argv[]) {
  Tree myt;
  myt.create();
  myt.read_file(argv);
  myt.path_finder();
  cout << myt.leftPath << endl;
  cout << myt.rightPath << endl;
  return 0;
}
