// 2-SAT.
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

const int MAX_FLIGHTS = 2020;

struct TwoSAT {
	int n;
	vector<int> graph[MAX_FLIGHTS * 2];
	bool mark[MAX_FLIGHTS * 2];
	int markedElem[MAX_FLIGHTS * 2], counter;

	bool dfs(int x) {
		if (mark[x ^ 1]) {
			return false;
		}
		if (mark[x]) {
			return true;
		}
		mark[x] = true;
		markedElem[counter++] = x;
		for (int i = 0; i < graph[x].size(); i++) {
			if (!dfs(graph[x][i])) {
				return false;
			}
		}
		return true;
	}

	// n: number of variables.
	void init(int n) {
		this->n = n;
		for (int i = 0; i < n * 2; i++) {
			graph[i].clear();
		}
		memset(mark, 0, sizeof(mark));
	}

	void addClause(int x, bool notX, int y, bool notY) {
		x = x * 2 + (notX ? 1 : 0);
		y = y * 2 + (notY ? 1 : 0);

		graph[x ^ 1].push_back(y);
		graph[y ^ 1].push_back(x);
	}

	bool solve() {
		for (int i = 0; i < n * 2; i += 2) {
			if (!mark[i] && !mark[i + 1]) {
				counter = 0;
				if (!dfs(i)) {
					while (counter > 0) {
						mark[markedElem[--counter]] = false;
					}
					if (!dfs(i + 1)) {
						return false;
					}
				}
			}
		}
		return true;
	}
};

int flights;
int early[MAX_FLIGHTS];
int late[MAX_FLIGHTS];
TwoSAT solver;

void helper(int gap, int i, int j, bool iTakeEarly, bool jTakeEarly) {
	int iLandTime = iTakeEarly ? early[i] : late[i];
	int jLandTime = jTakeEarly ? early[j] : late[j];
	if (abs(iLandTime - jLandTime) < gap) {
		// A conflit.
		solver.addClause(i, iTakeEarly, j, jTakeEarly);
	}
}
void addClausesHelper(int gap) {
	for (int i = 0; i < flights; i++) {
		for (int j = i + 1; j < flights; j++) {
			helper(gap, i, j, true, true);
			helper(gap, i, j, true, false);
			helper(gap, i, j, false, true);
			helper(gap, i, j, false, false);
		}
	}
}
int main() {
	while (scanf("%d", &flights) == 1) {
		for (int i = 0; i < flights; i++) {
			scanf("%d %d", &(early[i]), &(late[i]));
		}

		// Binary search on the max-min gap.
		int low = 0, high = 10000000;
		while (low < high) {
			int mid = low + (high - low + 1) / 2;

			solver.init(flights);
			addClausesHelper(mid);
			bool isValid = solver.solve();

			if (isValid) {
				low = mid;
			} else {
				high = mid - 1;
			}
		}
		printf("%d\n", low);	
	}
	return 0;
}