/*
This program is a non standard implementation of IDA*, which allows the
heuristic function to be non-admissible. This is because the attribute
"level" is search from low to high.

With no heuristic, the program took 1296.115967 seconds.
This is still faster than plain iterative deepening based on dfs, since
all states are put into the queue, so internally they are searched in a
bfs way. In the case of
9
9 8 7 6 5 4 3 2 1
0
the answer is 5, the plain iterative deepening will reach level 8
on some branches, while this approach only search for nodes below and
on level 5.

With heuristic
int getHeuristicDist(const vector<int>& perm) {
	int tmp = 0;
	for (int i = 1; i < numberCount; i++) {
		if (perm[i - 1] > perm[i]) {
			tmp++;
		}
	}
	return tmp;
}
the program took 286.396301 seconds.

With heuristic 
int getHeuristicDist(const vector<int>& perm) {
	int tmp = 0;
	for (int i = 1; i < numberCount; i++) {
		if ((perm[i - 1] + 1) != perm[i]) {
			tmp++;
		}
	}
	return tmp / 3;
}
the program took 362.001434 seconds.

With heuristic 
int getHeuristicDist(const vector<int>& perm) {
	int tmp = 0;
	for (int i = 1; i < numberCount; i++) {
		if ((perm[i - 1] + 1) != perm[i]) {
			tmp++;
		}
	}
	return (tmp + 2) / 3;
}
the program took 304.229645 seconds.

In general this implementation is quite slow, it is suitable to
test other optimizations.

The non-admissible heuristic is faster than the admissible one.
A non-admissible heuristic is useful when the optimality is not required for a
problem(on achieved by other means), it reduces the count of states to be searched.
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

struct Element {
	vector<int> perm;
	// Priority is the sum of the distance from source and the distance
	// to target. Element with lower priority should be explored first.
	int priority;
	int level;

	Element(vector<int> perm = vector<int>(), int priority = INF, int level = 0) {
		this->perm = perm;
		this->priority = priority;
		this->level = level;
	}

	bool operator < (const Element& o) const {
		return priority > o.priority;
	}
};

// Get the Heuristic distance. This should be add with the current level
// (how many actions had already been taken.)
// Please note this heuristic distance is not admissible.
int getHeuristicDist(const vector<int>& perm) {
	int tmp = 0;
	for (int i = 1; i < numberCount; i++) {
		if ((perm[i - 1] + 1) != perm[i]) {
			tmp++;
		}
	}
	return (tmp + 2) / 3;
}

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

void printVector(const vector<int>& tmp) {
	for (int v : tmp) {
		printf("%d ", v);
	}
	printf("\n");
}

void joinPermutations(const vector<vector<int>>& cutted, vector<int>& result) {
	for (const vector<int>& a : cutted) {
		for (int b : a) {
			result.push_back(b);
		}
	}
}

void expand(const Element& cur, priority_queue<Element>& pq) {
	vector<vector<int>> cuttableUnits = breakPermutation(cur.perm);
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
						&& visited[tmpEncoded] > cur.level + 1)) {
					visited[tmpEncoded] = cur.level + 1;
					pq.emplace(tmp, getHeuristicDist(tmp) + cur.level, cur.level + 1);
				}

				// swap
				if (j < remainingElemSize) {
					iter_swap(remainingElems.begin() + j, remainingElems.begin() + j + 1);
				}
			}
		}
	}
}

// maxLevel: maximum moves to make.
bool idastar(int maxLevel, const vector<int>& perm) {
	// A* search 
	priority_queue<Element> pq;
	visited.clear();
	pq.emplace(perm, getHeuristicDist(perm) + 0, 0);
	visited.insert(make_pair(encode(perm), 0));

	while (!pq.empty()) {
		Element cur = pq.top();
		pq.pop();

		if (isTarget(cur.perm)) {
			return true;
		}
		if (cur.level < maxLevel) {
			expand(cur, pq);
		}
	}
	return false;
}

int solve(const vector<int>& perm) {
	for (int level = 0; level < numberCount; level++) {
		if (idastar(level, perm)) {
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