/*
IDA*, with repetition check. For the version without repetition check
see p11212_3.cc.

This implementation finished the attached test cases in 6.481726 seconds.

The heuristic is based on "every move can reduce the incorret positions
by at most 3".
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
unordered_map<int, int> visited;

int encode(const vector<int>& perm) {
	int tmp = 0;
	for (int i = 0; i < perm.size(); i++) {
		tmp = tmp * 10 + perm[i];
	}
	return tmp;
}

int getHeuristicDist(const vector<int>& perm) {
	int tmp = 0;
	for (int i = 1; i < numberCount; i++) {
		if ((perm[i - 1] + 1) != perm[i]) {
			tmp++;
		}
	}
	return (tmp + 2) / 3;
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

// This function is complex since it avoids breaking contiguous numbers.
void expand(const vector<int>& cur, const int curDist, vector<vector<int>>& result) {
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
						&& visited[tmpEncoded] > curDist + 1)) {
					visited[tmpEncoded] = curDist + 1;
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

bool search(const vector<int>& perm,
	int currentDist, int bound, int& nextBound) {
	int f = currentDist + getHeuristicDist(perm);
	if (f > bound) {
		nextBound = f;
		return false;
	}
	if (isTarget(perm)) {
		return true;
	}

	int minBound = INF;
	vector<vector<int>> successors;
	expand(perm, currentDist, successors);
	for (const vector<int>& successor : successors) {
		int encoded = encode(successor);

		int tmpBound;
		bool found = search(successor, currentDist + 1, bound, tmpBound);
		if (found) {
			return true;
		}
		minBound = min(minBound, tmpBound);
	}
	nextBound = minBound;
	return false;
}

int idastar(const vector<int>& perm) {
	int bound = getHeuristicDist(perm);

	while (true) {
		visited.clear();
		visited[encode(perm)] = 0;

		int nextBound;
		bool found = search(perm, 0, bound, nextBound);
		if (found) {
			return bound;
		}

		if (nextBound == INF) {
			return -1;
		} else {
			bound = nextBound;
		}
	}
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
		int rtn = idastar(perm);
		printf("Case %d: %d\n", ++caseNumber, rtn);
	}

	#ifndef ONLINE_JUDGE
	t = clock() - t;
	printf ("The program took %f seconds.\n", ((float)t) / CLOCKS_PER_SEC);
	#endif
	return 0;
}
/*
9
4 7 6 5 1 9 3 8 2
9
1 5 7 2 3 6 4 8 9
9
2 4 9 5 8 7 1 3 6
9
8 7 4 6 1 5 9 2 3
9
1 2 6 8 7 5 4 9 3
9
4 7 5 1 8 9 3 6 2
9
6 4 5 8 3 1 2 7 9
9
2 3 6 7 1 8 9 5 4
9
2 1 7 9 5 6 4 8 3
9
7 8 3 6 4 1 9 2 5
9
9 4 8 6 2 1 5 7 3
9
9 5 1 3 6 8 7 4 2
9
2 6 1 9 5 7 4 8 3
9
2 4 3 9 6 8 5 1 7
9
8 7 4 5 1 6 9 3 2
9
1 2 7 4 9 6 8 5 3
9
8 2 9 5 4 6 7 3 1
9
7 4 3 5 8 1 2 9 6
9
3 5 9 2 4 8 6 1 7
9
5 3 9 2 8 4 1 6 7
0
*/