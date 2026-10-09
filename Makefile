CXX ?= c++
CXXFLAGS = -std=c++17 -Wall -Wextra -Wpedantic -Werror -O2

all: b_tree

b_tree: b_tree.cpp
	$(CXX) $(CXXFLAGS) -o $@ b_tree.cpp

test: b_tree
	sh tests/run.sh ./b_tree

clean:
	rm -f b_tree

.PHONY: all test clean
