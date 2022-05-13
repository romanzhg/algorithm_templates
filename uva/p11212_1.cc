/*
Iterative deepening(with depth limitting dfs).
5 (test case 5 5 4 3 2 1 0)
The program took 0.006875 seconds.
6
The program took 0.056515 seconds.
7
The program took 0.829754 seconds.
8
The program took 7.473376 seconds.
9
The program took 159.598434 seconds.
*/
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
#define INF 1000000000

int numberCount;
int level;
// In this problem, the state may be revisited with a lower level, so
// we have to record the level here, and update the level when the new
// visit is better.
// Record only visited or not would result in wrong answer.
//
// In general,
// If the states are in the shape of a tree, we don't need
// to check visited.
// (For DFS and BFS)If the states are in the shape of a well connected graph, but we
// only care about the reachability, we only need a set to check visited.
// (For DFS and BFS)If the states are in the shape of a well connected graph, we care
// about distance, then we need to use a map to check visited(which can maintain
// the distance of a visit).
// (For Iterative Deepening) visited(implemented as a map) is not needed, since we are
// searching for the lowest move count, and we always increase the count by one. But
// checking visited can speedup the problem by reduce duplications.
unordered_map<int, int> visited;

int encode(const vector<int>& perm) {
	int tmp = 0;
	for (int i = 0; i < perm.size(); i++) {
		tmp = tmp * 10 + perm[i];
	}
	return tmp;
}

bool isTarget(const vector<int>& perm) {
	int tmp = 0;
	for (int i = 1; i < numberCount; i++) {
		if (perm[i - 1] > perm[i]) {
			tmp++;
		}
	}
	return tmp == 0;
}

vector<vector<int>> breakPermutation(vector<int> permutation) {
  vector<vector<int>> rtn;
  int i = 0;
  while (i < numberCount) {
    int j = i + 1;
    while (j < numberCount) {
      if (permutation[j-1] + 1 != permutation[j]) {
        break;
      } else {
        j++;
      }
    }
    rtn.push_back(vector<int>(permutation.begin() + i, permutation.begin() + j));
    i = j;
  }
  return rtn;
}

void joinPermutations(const vector<vector<int>>& cutted, vector<int>& result) {
	for (const vector<int>& a : cutted) {
		for (int b : a) {
			result.push_back(b);
		}
	}
}

void expand(const vector<int>& cur, const int curLevel, vector<vector<int>>& result) {
	vector<vector<int>> cuttableUnits = breakPermutation(cur);
	int size = cuttableUnits.size();
	for (int i = 0; i < size; i++) {
		for (int len = 1; len < size; len++) {
			/* i + len refers to an end condition */
			if (i + len  > size) {
				break;
			}
			// merged cut out elem.
			vector<int> cutOutElems;
			vector<vector<int>> remainingElems;
			for (int j = 0; j < size; j++) {
				if (j >= i && j < i + len) {
					cutOutElems.insert(cutOutElems.end(),
						cuttableUnits[j].begin(),
						cuttableUnits[j].end());
				} else {
					remainingElems.push_back(cuttableUnits[j]);
				}
			}

			int remainingElemSize = remainingElems.size();
			remainingElems.insert(remainingElems.begin(), cutOutElems);
			for (int j = 0; j < remainingElemSize + 1; j++) {
				// process
				vector<int> tmp;
				joinPermutations(remainingElems, tmp);
				int tmpEncoded = encode(tmp);
				if (visited.find(tmpEncoded) == visited.end()
					|| (visited.find(tmpEncoded) != visited.end()
						&& visited[tmpEncoded] > curLevel + 1)) {
					visited[tmpEncoded] = curLevel + 1;
					result.push_back(tmp);
				}

				// swap
				if (j < remainingElemSize) {
					iter_swap(remainingElems.begin() + j, remainingElems.begin() + j + 1);
				}
			}
		}
	}
}

// Depth limitted dfs.
bool dls(int depth, const vector<int>& perm) {
	if (depth == 0) {
		return isTarget(perm);
	}
	vector<vector<int>> children;
	expand(perm, level - depth, children);
	for (const vector<int>& child : children) {
		if (dls(depth - 1, child)) {
			return true;
		}
	}
	return false;
}

int solve(const vector<int>& perm) {
	for (level = 0; level < numberCount; level++) {
		visited.clear();
		visited.insert(make_pair(encode(perm), 0));
		if (dls(level, perm)) {
			return level;
		}
	}
	return -1;
}

int main() {
	#ifndef ONLINE_JUDGE
	clock_t t;
	t = clock();
	#endif

	int caseNumber = 0;
	while (scanf("%d", &numberCount) == 1 && numberCount != 0) {
		vector<int> perm(numberCount);
		for (int i = 0; i < numberCount; i++) {
			scanf("%d", &perm[i]);
		}
		int rtn = solve(perm);
		printf("Case %d: %d\n", ++caseNumber, rtn);
	}
	#ifndef ONLINE_JUDGE
	t = clock() - t;
	printf ("The program took %f seconds.\n", ((float)t) / CLOCKS_PER_SEC);
	#endif
	return 0;
}
/*
5
5 4 3 2 1
0

6
6 5 4 3 2 1
0
*/