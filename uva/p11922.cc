// Splay tree. Note the comparison is based on rank.
#include <cfloat>
#include <climits>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <algorithm>
#include <deque>
#include <functional>
#include <iostream>
#include <limits>
#include <list>
#include <map>
#include <queue>
#include <set>
#include <stack>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <utility>

using namespace std;
typedef long long ll;

int numbers, instructions;
vector<int> result;

// Splay tree.
struct Node {
	// 0 for left child, 1 for right child.
	Node* ch[2];
	int value;
	int size;
	bool flip;

	Node() {size = 0; flip = false;};

	void pushdown() {
		if (flip) {
			flip = false;
			swap(ch[0], ch[1]);
			ch[0]->flip = !ch[0]->flip;
			ch[1]->flip = !ch[1]->flip;
		}
	}

	// For splay the comparison is based on rank.
	int cmp(int rank) const {
		int d = rank - ch[0]->size;
		if (d == 1) {
			return -1;
		}
		return d <= 0 ? 0 : 1;
	}

	void maintain() {
		size = 1 + ch[0]->size + ch[1]->size;
	}
};

Node* sentinal = new Node();

// d <- 0 for left rotation, d <- 1 for right rotation.
void rotate(Node* &o, int d) {
	Node* k = o->ch[d^1];
	o->ch[d^1] = k->ch[d];
	k->ch[d] = o;
	o->maintain();
	k->maintain();
	o = k;
}

// @param k: k-th, in [1..n].
// Rotate the k-th element to top of the tree.
void splay(Node* &o, int k) {
	o->pushdown();
	int d = o->cmp(k);
	if (d == 1) {
		k -= o->ch[0]->size + 1;
	}
	if (d != -1) {
		Node* p = o->ch[d];
		p->pushdown();
		int d2 = p->cmp(k);
		int k2 = (d2 == 0 ? k : k - p->ch[0]->size - 1);
		if (d2 != -1) {
			splay(p->ch[d2], k2);
			if (d == d2) {
				rotate(o, d^1);
			} else {
				rotate(o->ch[d], d);
			}
		}
		rotate(o, d^1);
	}
}

// Merge two sequences.
Node* merge(Node* left, Node* right) {
	splay(left, left->size);
	left->ch[1] = right;
	left->maintain();
	return left;
}

// Split the original sequence at the kth element, so [1..k]-th
// elements are on left, [k+1..n]-th element are on right.
void split(Node* o, int k, Node* &left, Node* &right) {
	splay(o, k);
	left = o;
	right = o->ch[1];
	o->ch[1] = sentinal;
	left->maintain();
}

Node* buildTree(int l, int r) {
	if (l > r) {
		return sentinal;
	}
	int m = l + (r - l) / 2;
	Node* rtn = new Node();
	rtn->value = m;
	rtn->flip = false;
	rtn->ch[0] = buildTree(l, m - 1);
	rtn->ch[1] = buildTree(m + 1, r);
	rtn->maintain();
	return rtn;
}

void updateTree(Node* &root, int a, int b) {
	Node *left, *right, *mid, *tmp;
	split(root, a - 1, left, right);
	split(right, b - a + 1, mid, tmp);
	right = tmp;
	mid->flip = !mid->flip;
	root = merge(merge(left, right), mid);
}

void printTree(Node* o) {
	if (o == sentinal) {
		return;
	}
	o->pushdown();
	printTree(o->ch[0]);
	result.push_back(o->value);
	printTree(o->ch[1]);
}

int main() {
	scanf("%d %d", &numbers, &instructions);

	Node* root = buildTree(1, numbers + 1);

	for (int i = 0; i < instructions; i++) {
		int a, b;
		scanf("%d %d", &a, &b);
		a++;
		b++;
		updateTree(root, a, b);
	}

	// Print the tree.
	printTree(root);
	for (int i = 1; i <= numbers; i++) {
		printf("%d\n", result[i] - 1);
	}
	return 0;
}