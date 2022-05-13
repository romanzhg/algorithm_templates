// Treap.
// Note the value in the treap is <node weight, node id>, since treap requires
// its value to be unique. Node id makes sure it is unique, while node weight
// is mainly used in the comparison. Since this problem is related to rank,
// there will always be an id, if for some other problem there is no id, one
// should be given.
// Another solution would be modify the treap to all nodes with same value, and
// this won't be a problem here since the unions will only merge, not splite, so
// update any node in the union/treap would be ok.
//
// Note the union find structure doesn't have the rank array, and when two nodes
// joins, which one would be the parent is specified, this is non-standard.
// This should not be a problem since there is path compression: parent is updated
// when being checked.
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

const int MAX_VERTEXES = 20020;
const int MAX_EDGES = 60060;
enum OpType {DeleteEdge, Change, Query};
struct Op {
	OpType type;
	// DeleteEdge uses only arg1: the edge id.
	// Query uses arg1: the vertex id and arg2: the k-th maximum.
	// Change uses arg1: the vertex id, arg2: the new value,
	// arg3: the old value. the new value and old value is to be
	// flipped as we update the ops.
	int arg1, arg2, arg3;
	Op(OpType t, int arg1 = 0, int arg2 = 0, int arg3 = 0) :
		type(t), arg1(arg1), arg2(arg2), arg3(arg3) {};
};

// Data for this problem.
int vertexCount, edgeCount, queryCount;
double accQueryResult;
int weights[MAX_VERTEXES];
pair<int, int> edges[MAX_EDGES];
bool edgesDeleted[MAX_EDGES];
vector<Op> operations;

// Data structures for union-find.
int parent[MAX_VERTEXES];
// Note: the rank array is not really useful since we compress the parent
// array in getParentUF.
// int rank[MAX_VERTEXES];

int getParentUF(int vertexId) {
	if (parent[vertexId] != vertexId) {
		int p = getParentUF(parent[vertexId]);
		parent[vertexId] = p;
		return p;
	} else {
		return vertexId;
	}
}

void joinWithParentUF(int p, int other) {
	int p1 = getParentUF(p);
	int p2 = getParentUF(other);
	if (p1 != p2) {
		parent[p2] = p1;
	}
}

// Data structures for treap.
struct Node {
	// 0 for left child, 1 for right child.
	Node* ch[2];
	int priority;
	pair<int, int> value;
	int size;

	int cmp(const pair<int, int>& otherValue) const {
		if (this->value == otherValue) {
			return -1;
		}
		return otherValue < this->value ? 0 : 1;
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
void rotateTreap(Node* &o, int d) {
	Node* k = o->ch[d^1];
	o->ch[d^1] = k->ch[d];
	k->ch[d] = o;
	o->maintain();
	k->maintain();
	o = k;
}

void insertTreap(Node* &o, const pair<int, int>& x) {
	if (o == NULL) {
		o = new Node();
		o->ch[0] = NULL;
		o->ch[1] = NULL;
		o->value = x;
		o->priority = rand();
		o->size = 1;
	} else {
		int d = o->cmp(x);
		insertTreap(o->ch[d], x);
		o->maintain();
		if (o->ch[d]->priority > o->priority) {
			rotateTreap(o, d^1);
		}
	}
}

void removeTreap(Node* &o, const pair<int, int>& x) {
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
			rotateTreap(o, d2);
			removeTreap(o->ch[d2], x);
			o->maintain();
		}
	} else {
		o->size = o->size - 1;
		removeTreap(o->ch[d], x);
	}
}

int findKthTreap(Node* o, int k) {
	if (k == 1) {
		if (o->ch[0] == NULL) {
			return o->value.first;
		} else {
			return findKthTreap(o->ch[0], k);
		}
	}
	if (o->ch[0] == NULL) {
		return findKthTreap(o->ch[1], k - 1);
	} else if (k - 1 > o->ch[0]->size) {
		return findKthTreap(o->ch[1], k - 1 - o->ch[0]->size);
	} else if (k - 1 == o->ch[0]->size) {
		return o->value.first;
	} else {
		return findKthTreap(o->ch[0], k);
	}
}

// Find the k-th largest element.
// For a treap with n elements, the valid k belongs to [1...N].
int findKthLargestTreap(Node* o, int k) {
	if (k < 1 || k > o->size) {
		return 0;
	}

	return findKthTreap(o, o->size + 1 - k);
}

// Join two treaps by insert every nodes of on tree to the other.
// TODO: The other tree should be removed.
// But the oj seems doesn't case about memory.
void joinTreap(Node* &root, Node* other) {
	if (other == NULL) {
		return;
	}
	insertTreap(root, other->value);
	joinTreap(root, other->ch[0]);
	joinTreap(root, other->ch[1]);
}

Node* treaps[MAX_VERTEXES];

int main() {
	int caseId = 0;
	while (true) {
		scanf("%d %d", &vertexCount, &edgeCount);
		if (vertexCount == 0 && edgeCount == 0) {
			break;
		}

		for (int i = 1; i <= vertexCount; i++) {
			scanf("%d", &(weights[i]));
		}

		for (int i = 1; i <= edgeCount; i++) {
			scanf("%d %d", &(edges[i].first), &(edges[i].second));
		}

		// Initialization.
		queryCount = 0;
		accQueryResult = 0;
		operations.clear();
		memset(edgesDeleted, 0, sizeof(edgesDeleted));
		for (int i = 1; i <= vertexCount; i++) {
			parent[i] = i;
		}
		memset(treaps, 0, sizeof(treaps));

		// Run queries.
		while (true) {
			char tmpStr[10];
			scanf("%s", tmpStr);
			int tmp1, tmp2;
			if (tmpStr[0] == 'E') {
				break;
			} else if (tmpStr[0] == 'Q') {
				scanf("%d %d", &tmp1, &tmp2);
				operations.push_back(Op(OpType::Query, tmp1, tmp2));
				queryCount++;
			} else if (tmpStr[0] == 'D') {
				scanf("%d", &tmp1);
				operations.push_back(Op(OpType::DeleteEdge, tmp1));
			} else if (tmpStr[0] == 'C') {
				scanf("%d %d", &tmp1, &tmp2);
				operations.push_back(Op(OpType::Change, tmp1, tmp2));
			}
		}

		// Run the operations first, update them if needed.
		for (Op& op : operations) {
			if (op.type == OpType::Query) {
				continue;
			} else if (op.type == OpType::DeleteEdge) {
				edgesDeleted[op.arg1] = true;
			} else if (op.type == OpType::Change) {
				op.arg3 = weights[op.arg1];
				weights[op.arg1] = op.arg2;
			}
		}

		// Build the union find structure for vertexes.
		for (int i = 1; i <= edgeCount; i++) {
			if (edgesDeleted[i]) {
				continue;
			}
			joinWithParentUF(edges[i].first, edges[i].second);
		}

		// Build a treap for each group.
		for (int i = 1; i <= vertexCount; i++) {
			insertTreap(treaps[getParentUF(i)], make_pair(weights[i], i));
		}

		// Run all the ops in reversed order.
		reverse(operations.begin(), operations.end());
		for (const Op& op : operations) {
			if (op.type == OpType::Query) {
				Node* tmpRoot = treaps[getParentUF(op.arg1)];
				accQueryResult += findKthLargestTreap(tmpRoot, op.arg2);
			} else if (op.type == OpType::DeleteEdge) {
				// Now it is add edge, should join two treaps.
				int treapIndex1 = getParentUF(edges[op.arg1].first);
				int treapIndex2 = getParentUF(edges[op.arg1].second);
				if (treapIndex1 == treapIndex2) {
					continue;
				}

				int treap1Size = treaps[treapIndex1]->size;
				int treap2Size = treaps[treapIndex2]->size;
				int newParent = treap1Size >= treap2Size ? treapIndex1 : treapIndex2;
				int newChild = treapIndex1 ^ treapIndex2 ^ newParent;

				joinWithParentUF(newParent, newChild);
				joinTreap(treaps[newParent], treaps[newChild]);
			} else if (op.type == OpType::Change) {
				// Remove the node, and add a new node back.
				int curParent = getParentUF(op.arg1);
				removeTreap(treaps[curParent], make_pair(op.arg2, op.arg1));
				insertTreap(treaps[curParent], make_pair(op.arg3, op.arg1));
			}
		}

		printf("Case %d: %.6lf\n", ++caseId, accQueryResult / queryCount);
	}
	return 0;
}