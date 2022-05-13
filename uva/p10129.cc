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
#include <iostream>
using namespace std;

#define INF 100000000
#define NUM_OF_CHARS 26

// Undirected graph.
int graph[NUM_OF_CHARS][NUM_OF_CHARS];
int out_degree[NUM_OF_CHARS];
int in_degree[NUM_OF_CHARS];

char word[1010];

// Data structure for solve.
bool visited[NUM_OF_CHARS];
bool node_exists[NUM_OF_CHARS];

void dfs(int node) {
	if (visited[node]) {
		return;
	}
	visited[node] = true;
	for (int i = 0; i < NUM_OF_CHARS; i++) {
		if (graph[node][i] > 0) {
			dfs(i);
		}
	}
}

bool solve() {
	memset(visited, false, sizeof(visited));
	int components = 0;
	for (int i = 0; i < NUM_OF_CHARS; i++) {
		if (node_exists[i] && !visited[i]) {
			dfs(i);
			components++;
		}
	}
	if (components > 1) {
		// printf("components: %d\n", components);
		return false;
	}
	int out_larger = 0, in_larger = 0;

	// for (int i = 0; i < NUM_OF_CHARS; i++) {
	// 	printf("%c out degree: %d in degree %d\n", i + 'a', out_degree[i], in_degree[i]);
	// }

	for (int i = 0; i < NUM_OF_CHARS; i++) {
		if (out_degree[i] == in_degree[i]) {
			continue;
		} else if (out_degree[i] == in_degree[i] + 1) {
			out_larger++;
		} else if (out_degree[i] + 1 == in_degree[i]) {
			in_larger++;
		} else {
			return false;
		}
	}
	if (out_larger == 0 && in_larger == 0) {
		return true;
	} else if (out_larger == 1 && in_larger == 1) {
		return true;
	} else {
		return false;
	}
}

int main() {
	int test_cases;
	scanf("%d", &test_cases);
	for (int i = 0; i < test_cases; i++) {
		int word_count;
		//printf("in a new test case\n");
		scanf("%d", &word_count);

		memset(out_degree, 0, sizeof(out_degree));
		memset(in_degree, 0, sizeof(in_degree));
		memset(graph, 0, NUM_OF_CHARS * NUM_OF_CHARS);
		memset(node_exists, false, sizeof(node_exists));

		// Read the delimiter.
		cin.getline(word, sizeof(word) - 1);
		for (int j = 0; j < word_count; j++) {
			cin.getline(word, sizeof(word) - 1);
			int size = strlen(word);
			char first = word[0];
			char last = word[size - 1];

			node_exists[first - 'a'] = true;
			node_exists[last - 'a'] = true;

			graph[first - 'a'][last - 'a']++;
			graph[last - 'a'][first - 'a']++;
			out_degree[last - 'a']++;
			in_degree[first - 'a']++;
		}

		bool rtn = solve();
		if (rtn) {
			printf("Ordering is possible.\n");
		} else {
			printf("The door cannot be opened.\n");
		}
	}
	return 0;
}