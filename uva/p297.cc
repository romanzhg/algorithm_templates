#include <cstdio>
#include <cstdlib>
#include <climits>
#include <iostream>
#include <queue>
#include <deque>
#include <vector>
#include <utility>
#include <algorithm>
#include <functional>
#include <cstring>
#include <string>
#include <map>
#include <set>
#include <stack>
using namespace std;

#define TOTAL_PIXELS 1024

struct Node {
  char value;
  Node* c1;
  Node* c2;
  Node* c3;
  Node* c4;
  Node() {
  	value = 'e';
  	c1 = NULL;
  	c2 = NULL;
  	c3 = NULL;
  	c4 = NULL;
  };
};

int black_count;

// Recursively traverse the tree, update black count.
void helper(int level, Node* tree1, Node* tree2) {
	char tree1_value = tree1 == NULL ? 'e' : tree1->value;
	char tree2_value = tree2 == NULL ? 'e' : tree2->value;

	if (tree1_value == 'f' || tree2_value == 'f') {
		black_count += (TOTAL_PIXELS >> level);
		return;
	}
	if (tree1_value == 'p' || tree2_value == 'p') {
		helper(level + 2,
			tree1 == NULL ? NULL : tree1->c1,
			tree2 == NULL ? NULL : tree2->c1);
		helper(level + 2,
			tree1 == NULL ? NULL : tree1->c2,
			tree2 == NULL ? NULL : tree2->c2);
		helper(level + 2,
			tree1 == NULL ? NULL : tree1->c3,
			tree2 == NULL ? NULL : tree2->c3);
		helper(level + 2,
			tree1 == NULL ? NULL : tree1->c4,
			tree2 == NULL ? NULL : tree2->c4);
	}
}

void delete_tree(Node* node) {
	if (node == NULL) {
		return;
	}
	delete_tree(node->c1);
	delete_tree(node->c2);
	delete_tree(node->c3);
	delete_tree(node->c4);
	delete node;
}

Node* build_tree(const string& tree, int& index) {
	Node* rtn = new Node();
	char tmp = tree[index];
	rtn->value = tmp;

  if (tmp == 'p') {
  	index++;
  	rtn->c1 = build_tree(tree, index);
  	index++;
  	rtn->c2 = build_tree(tree, index);
  	index++;
  	rtn->c3 = build_tree(tree, index);
  	index++;
  	rtn->c4 = build_tree(tree, index);
  }
  return rtn;
}

int main() {
	int case_count;
	cin >> case_count;
	while (case_count--) {
		string tree1;
		string tree2;
		cin >> tree1;
		cin >> tree2;

		int index;
		index = 0;
		Node* root1 = build_tree(tree1, index);
		index = 0;
		Node* root2 = build_tree(tree2, index);

		black_count = 0;
		helper(0, root1, root2);
		printf("There are %d black pixels.\n", black_count);

		delete_tree(root1);
		delete_tree(root2);
	}
	return 0;
}