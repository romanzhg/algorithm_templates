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

// Split the input string by delimiter, return results in a vector.
// Delimiters are not returned.
void split(const string& str, char delim, vector<string>& result) {
  size_t cur = 0;
  size_t pos = str.find(delim);
  while (pos != string::npos) {
    if (pos != cur) {
      result.push_back(str.substr(cur, pos - cur));
    }
    pos++;
    cur = pos;
    pos = str.find(delim, pos);
  }
  if (cur != str.length()) {
    result.push_back(str.substr(cur));
  }
}

// Solution.
bool connected[26][26];
bool used[26];

// Min bandwidth for all permutations.
int globalMin;

// Min bandwidth for current permutation.
string minBandwidthStr;

// Get the bandwidth the the node located at index cur - 1.
// Distance is the diff on index. If there is only 1 element, dist would be 0.
int getCurrentBandwidth(int* result, int cur) {
	int maxDist = 0;
	for (int i = 0; i < cur; i++) {
		if(connected[result[i]][result[cur - 1]]) {
			maxDist = max(maxDist, cur - 1 - i);
		}
	}
	return maxDist;
}

string genString(int n, int* result) {
	string rtn = "";
	for (int i = 0; i < n; i++) {
		rtn += (char) (result[i] + 'A');
		rtn += ' ';
	}
	return rtn;
}

// localMax is an attribute associated with current state.
// This function can be renamed to dfs.
void visitPermutations(int n, int* result, int localMax, const int* elements, int cur) {
	// Current state processing, for intermediate/leaf node in the solution tree.
	int tmp = getCurrentBandwidth(result, cur);
	localMax = max(localMax, tmp);

	// Prune on the intermediate node.
	if (localMax > globalMin) {
		return;
	}

	// Process the leaf node.
	if (cur == n) {
		if (localMax < globalMin) {
			globalMin = localMax;
			minBandwidthStr = genString(n, result);
		} else if (localMax == globalMin) {
			minBandwidthStr = min(minBandwidthStr, genString(n, result));
		}
	}

	// State transition.
	for (int i = 0; i < n; i++) {
		bool isValid = true;
		for (int j = 0; j < cur; j++) {
			if (result[j] == elements[i]) {
				isValid = false;
				break;
			}
		}

		if (isValid) {
			result[cur] = elements[i];
			visitPermutations(n, result, localMax, elements, cur + 1);
		}
	}
}

void solve(const vector<int>& nodes) {
	globalMin = INT_MAX;
	minBandwidthStr.clear();
	int result[26];
	int localMax = 0;
	visitPermutations(nodes.size(), result, localMax, nodes.data(), 0);
	printf("%s-> %d\n", minBandwidthStr.c_str(), globalMin);
}

int main() {
	string line;
	while (cin >> line) {
		if (line == "#") {
			break;
		}
		memset(connected, 0, sizeof(connected));
		memset(used, 0, sizeof(used));
		vector<string> nodeConfigs;
		vector<int> nodes;
		split(line, ';', nodeConfigs);
		for (const string& nodeConfig: nodeConfigs) {
			vector<string> keyValue;
			split(nodeConfig, ':', keyValue);
			char a = keyValue[0][0];
			for (char b : keyValue[1]) {
				connected[a - 'A'][b - 'A'] = true;
				connected[b - 'A'][a - 'A'] = true;

				if (!used[a - 'A']) {
					used[a - 'A'] = true;
					nodes.push_back(a - 'A');
				}
				if (!used[b - 'A']) {
					used[b - 'A'] = true;
					nodes.push_back(b - 'A');
				}
			}
		}
		solve(nodes);
	}
	return 0;
}