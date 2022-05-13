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

// Notes: there are a few places in this program can be optimized.
// 1. Use libraries for the parsing. Refer to the book.
// 2. Use user managed memory pool.
#define LIMIT 1024

struct Node {
	bool specified;
	int value;
	Node* left;
	Node* right;

	Node() {
		specified = false;
		left = NULL;
		right = NULL;
	}
};

char tmp_buf[LIMIT];
char path[LIMIT];
int value;

// Return 0 on success, 1 on tree ends, 2 on input ends.
int get_element() {
	int c;
	int index = 0;
	memset(tmp_buf, 0, sizeof (tmp_buf));
	while(true) {
		c = getchar();
		if (c == EOF) {
			return 2;
		} else if (c == ')'){
			break;
		} else if (isspace(c)) {
			continue;
		} else if (c == '(') {
			continue;
		} else {
			tmp_buf[index] = c;
			index++;
		}
	}
	if (index == 0) {
		return 1;
	}

	int index_of_comma = strchr(tmp_buf, ',') - tmp_buf;
	char tmp_value[LIMIT];

	memset(tmp_value, 0, LIMIT);
	memset(path, 0, LIMIT);

	for (int i = 0; i < index_of_comma; i++) {
		tmp_value[i] = tmp_buf[i];
	}

	value = atoi(tmp_value);

	for (int i = index_of_comma + 1; i < strlen(tmp_buf); i++) {
		path[i - (index_of_comma + 1)] = tmp_buf[i];
	}
	return 0;
}

void remove_tree(Node* node) {
	if (node == NULL) {
		return;
	}
	remove_tree(node->left);
	remove_tree(node->right);
	delete node;
}

bool is_tree_valid(Node* node) {
	if (node == NULL) {
		return true;
	}
	if (node->specified == false) {
		return false;
	}
	return is_tree_valid(node->left) && is_tree_valid(node->right);
}

void level_order_print(bool is_first, Node* node) {
	queue<Node*> node_queue;
	node_queue.push(node);
	while(!node_queue.empty()) {
		Node* tmp = node_queue.front(); node_queue.pop();
		if (tmp == NULL) {
			continue;
		}
		if (is_first) {
			is_first = false;
			printf("%d", tmp->value);
		} else {
			printf(" %d", tmp->value);
		}
		node_queue.push(tmp->left);
		node_queue.push(tmp->right);
	}
}

int main() {
	int tmp;
	Node* root = new Node();
	bool isValid = true;
	while (true) {
		tmp = get_element();
		if (tmp == 1) {
			// Examine tree and print.
			if (isValid && is_tree_valid(root)) {
				level_order_print(true, root);
				printf("\n");
			} else {
				printf("not complete\n");
			}
			// Remove the existing tree.
			remove_tree(root);
			// Todo: remove root again outside the loop.
			root = new Node();
			isValid = true;
		} else if (tmp == 2) {
			break;
		} else {
			// Load the new element to tree.
			Node* current = root;
			for (int i = 0; i < strlen(path); i++) {
				if (path[i] == 'L') {
					if (current->left == NULL) {
						current->left = new Node();
					}
					current = current->left;
				} else {
					if (current->right == NULL) {
						current->right = new Node();
					}
					current = current->right;
				}
			}
			if (current->specified) {
				isValid = false;
			} else {
				current->specified = true;
				current->value = value;
			}
		}
	}
	remove_tree(root);
	return 0;
}