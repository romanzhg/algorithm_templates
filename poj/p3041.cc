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

#define MAX_K 10010
#define MAX_V 1010

// Global data.
// The row index and column index 
int R[MAX_K], C[MAX_K];
// Size of the matrix, number of starts;
int N, K;
// Number of vertexes.
int V;

// Data for solve.
vector<int> G[MAX_V];

// Data for existMatch, DFS to see if a node has a match.
bool used[MAX_V];
int matchesWith[MAX_V];

// There exist a match for vertex n with an unused vertex.
bool existMatch(int n) {
	used[n] = true;
	for (int i = 0; i < G[n].size(); i++) {
		int m = G[n][i];
		int k = matchesWith[m];
		if (k == -1 || ((used[k] == false) && (existMatch(k)))) {
			matchesWith[n] = m;
			matchesWith[m] = n;
			return true;
		}
	}
	return false;
}


void addEdge(int from, int to) {
	G[from].push_back(to);
	G[to].push_back(from);
}

void solve() {
	V = N * 2;
	// Draw the graph.
	for (int i = 0; i < K; i++) {
		addEdge(R[i] - 1, N + C[i] - 1);
	}

	int rtn = 0;
	memset(matchesWith, -1, sizeof(matchesWith));
	// For each node, find if there is a match.
	for (int i = 0; i < V; i++) {
		if (matchesWith[i] != -1) {
			continue;
		}
		memset(used, false, sizeof(used));
		if (existMatch(i)) {
			rtn++;
		}
	}
	printf("%d\n", rtn);
}

int main() {
	while(scanf("%d %d", &N, &K) != EOF) {
		for (int i = 0; i < K; i++) {
			scanf("%d %d", &R[i], &C[i]);
		}
		solve();
	}

  return 0;
}