#include <cfloat>
#include <climits>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
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
#include <utility>

using namespace std;
typedef long long ll;

#ifndef ONLINE_JUDGE
#endif

struct Node {
	int weight;
	Node* left;
	Node* right;
	Node() {
		weight = -1;
		left = NULL;
		right = NULL;
	}
};

double maxLen;
double r;
double leftDist, rightDist;

void expend(const Node& n, double c) {
	if (n.left == NULL && n.right == NULL) {
		return;
	}
	double leftWeight = n.left->weight;
	double rightWeight = n.right->weight;
	double leftLen = rightWeight / (leftWeight + rightWeight);
	double rightLen = leftWeight / (leftWeight + rightWeight);
	leftDist = min(leftDist, c - leftLen);
	rightDist = max(rightDist, c + rightLen);
	expend(*(n.left), c - leftLen);
	expend(*(n.right), c + rightLen);
}

// TODO: a possible improvement would be using Node* everywhere,
// this minimize the copy. This may save some time but is not likely
// to improve memory usage a lot, since this algorithm is essentially
// dfs, only one tree branch is saved in memory.
void visit(vector<Node> nodes) {
	if (nodes.size() == 1) {
		leftDist = 0;
		rightDist = 0;
		expend(nodes[0], 0);
		double len = rightDist - leftDist;
		if (len > maxLen && len < r) {
			maxLen = len;
		}
		return;
	}

	for (int i = 0; i < nodes.size(); i++) {
		for (int j = i + 1; j < nodes.size(); j++) {
			vector<Node> newNodes;
			Node n;
			n.left = &nodes[i];
			n.right = &nodes[j];
			n.weight = n.left->weight + n.right->weight;
			newNodes.push_back(n);
			for (int k = 0; k < nodes.size(); k++) {
				if (k != i && k != j) {
					newNodes.push_back(nodes[k]);
				}
			}
			// Order matters.
			visit(newNodes);
			swap(newNodes[0].left, newNodes[0].right);
			visit(newNodes);
		}
	}
}

int main() {
	int test_cases;
	scanf("%d", &test_cases);
	while (test_cases--) {
		int s;
		scanf("%lf %d", &r, &s);
		vector<Node> nodes;
		nodes.clear();
		maxLen = -1;

		for (int i = 0; i < s; i++) {
			Node n;
			scanf("%d", &n.weight);
			nodes.push_back(n);
		}

		visit(nodes);
		if (maxLen < 0) {
			printf("-1\n");
		} else {
			printf("%.8lf\n", maxLen);
		}
	}
	return 0;
}