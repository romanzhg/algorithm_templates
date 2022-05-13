#include <cstdio>
#include <cstdlib>
#include <climits>
#include <queue>
#include <vector>
#include <utility>
#include <algorithm>
#include <functional>
#include <cstring>
#include <string>
#include <map>
using namespace std;

#define INF 100000000

#define MAX_N 110
// Number of tasks.
int n;
// 0 means no edge, 1 means edge exists.
int graph[MAX_N][MAX_N];

// Data for solve.
// tmp counter.
int t;
int visited[MAX_N];
int sorted[MAX_N];

// Run topology sort, return false if there is a cycle.
bool dfs(int node) {
	visited[node] = -1;
	for (int i = 0; i < n; i++) {
		if (graph[node][i] == 1) {
			if (visited[i] < 0) {
				return false;
			}
			if (visited[i] == 0) {
				bool rtn = dfs(i);
				if (!rtn) {
					return false;
				}
			}
		}
	}
	visited[node] = 1;
	sorted[t] = node;
	--t;
	return true;
}

bool solve() {
  // Run topology sort on nodes. then print the result.
	t = n - 1;
	memset(visited, 0, sizeof(visited));

	for (int i = 0; i < n; i++) {
		if (!visited[i]) {
			if (!dfs(i)) {
				return false;
			}
		}
	}

	for (int i = 0; i < n; i++) {
		printf("%d ", sorted[i] + 1);
	}
	printf("\n");
	return true;
}

int main() {
	int m;
	while(scanf("%d %d", &n, &m)) {
		if (n == 0 && m == 0) {
			break;
		}
		// Initialize the graph.
		for (int i = 0; i < MAX_N; i++) {
			for (int j = 0; j < MAX_N; j++) {
				graph[i][j] = 0;
			}
		}
		for (int i = 0; i < m; i++) {
			int a, b;
			scanf("%d %d", &a, &b);
			graph[a - 1][b - 1] = 1;
		}
		solve();
	}
  return 0;
}