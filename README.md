# Tea Programming Language
Implementation <strong> a programming language </strong> named <strong>Tea</strong>. Exercise related to this topics:
<ul>
  <li><strong>Recursive Programming</strong></li>
  <li><strong>C++ (as language)</strong></li>
  <li><strong>Binary trees</strong></li>
  <li><strong>Tree Implementation</strong></li>
  <li><strong>Data Structures</strong></li>
</ul>

## Problem
There is an input file first line indicates values of nodes of the tree to be created. They are always placed like the graph given below:

<div style="margin:auto"><img src="https://raw.githubusercontent.com/urtuba/cpp_binary_tree/master/img/img1.PNG" alt="order" height="150px"></div>

Left to right, top to bottom. It is impossible to add a node on level 3 before fully filling level 2. Or you cannot add new node to right if left is blank.

Second line of input file indicates the target. We will try to find a path in the left sub-tree and another one in right sub-tree which makes the target number whey you sum their nodes. Example:

<div style="margin:auto"><img src="https://raw.githubusercontent.com/urtuba/cpp_binary_tree/master/img/img2.PNG" alt="target" height="150px"></div>

We need 1 path for each subtree. If program finds more than one, leftest path is valid in the subtree.

<div style="margin:auto"><img src="https://raw.githubusercontent.com/urtuba/cpp_binary_tree/master/img/img3.PNG" alt="more than 2 paths" height="150px"></div>

## Compile and Run

Compile it: ``g++ -std=c++11 -Wall -Werror b_tree.cpp -o b_tree ``

Run it: ``./b_tree input.txt``

Run on windows: ``b_tree input.txt``

## Notes

Remember the first image, there was a series of data like 1, 2, 3, 4, 5... As you can realize, this numbers are ordered with positive integers. Even tree is initialized with different data like 19, 25, 30... there is a secret 1, 2, 3 order in order to their position in the tree. I used this to distinguish whether an node is left or right subtree.

Whenever ``ptr -> nodeNum < 3*pow(2, floor(log2(ptr->nodeNum)))/2`` goes true, you can assume you are in left subtree.
