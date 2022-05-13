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

// Structures for Treap.
struct Node {
	// 0 for left child, 1 for right child.
	Node* ch[2];
	int priority;
	int value;
	int cmp(int value) const {
		if (this->value == value) {
			return -1;
		}
		return value < this->value ? 0 : 1;
	}
	Node() {
	}

	// d <- 0 for left rotation, d <- 1 for right rotation.
	void rotate(Node* &o, int d) {
		Node* k = o->ch[d^1];
		o->ch[d^1] = k->ch[d];
		k->ch[d] = o;
		o = k;
	}
};

void insert(Node* &o, int x) {
	if (o == NULL) {
		o = new Node();
		o->ch[0] = NULL;
		o->ch[1] = NULL;
		o->value = x;
		o->priority = rand();
	} else {
		int d = o->cmp(x);
		// Note this insert then rotate, like a tail rotation?
		insert(o->ch[d], x);
		if (o->ch[d]->priority > o->priority) {
			rotate(o, d^1);
		}
	}
}

void remove(Node* &o, int x) {
	int d = o->cmp(x);
	if (d == -1) {
		if (o->ch[0] == NULL) {
			Node* toDelete = o;
			o = o->ch[1];
			delete toDelete;
		} else if (o->ch[1] == NULL) {
			Node* toDelete = o;
			o = o->ch[0];
			delete toDelete;
		} else {
			int d2 = o->ch[0]->priority > o->ch[1]->priority ? 1 : 0;
			rotate(o, d2);
			remove(o->ch[d2], x);
		}
	} else {
		remove(o->ch[d], x);
	}
}

// Returns 1 for exist, 0 for not exist.
int find(Node* o, int x) {
	while (o != NULL) {
		int d = o->cmp(x);
		if (d == -1) {
			return 1;
		} else {
			o = ch[d];
		}
		return 0;
	}
}

// Structures for Treap with rank.
struct Node {
	// 0 for left child, 1 for right child.
	Node* ch[2];
	int priority;
	int value;
	int size;
	int cmp(int value) const {
		if (this->value == value) {
			return -1;
		}
		return value < this->value ? 0 : 1;
	}
	Node() {
	}

	void maintain() {
		size = 1;
		if (ch[0] != NULL) {
			size += ch[0]->size;
		}
		if (ch[1] != NULL) {
			size += ch[1]->size;
		}
	}

};

// d <- 0 for left rotation, d <- 1 for right rotation.
void rotate(Node* &o, int d) {
	Node* k = o->ch[d^1];
	o->ch[d^1] = k->ch[d];
	k->ch[d] = o;
	o->maintain();
	k->maintain();
	o = k;
}

void insertTreap(Node* &o, int x) {
	if (o == NULL) {
		o = new Node();
		o->ch[0] = NULL;
		o->ch[1] = NULL;
		o->value = x;
		o->priority = rand();
	} else {
		int d = o->cmp(x);
		// Note this insert then rotate, like a tail rotation?
		insert(o->ch[d], x);
		o->maintain();
		if (o->ch[d]->priority > o->priority) {
			rotate(o, d^1);
		}
	}
}

void removeTreap(Node* &o, int x) {
	int d = o->cmp(x);
	if (d == -1) {
		if (o->ch[0] == NULL) {
			Node* toDelete = o;
			o = o->ch[1];
			delete toDelete;
		} else if (o->ch[1] == NULL) {
			Node* toDelete = o;
			o = o->ch[0];
			delete toDelete;
		} else {
			int d2 = o->ch[0]->priority > o->ch[1]->priority ? 1 : 0;
			rotate(o, d2);
			remove(o->ch[d2], x);
			o->maintain();
		}
	} else {
		o->size--;
		remove(o->ch[d], x);
	}
}

// Returns 1 for exist, 0 for not exist.
int findTreap(Node* o, int x) {
	while (o != NULL) {
		int d = o->cmp(x);
		if (d == -1) {
			return 1;
		} else {
			o = ch[d];
		}
		return 0;
	}
}

int main() {
	return 0;
}