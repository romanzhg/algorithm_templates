// BFS.
// This program took 13.788684 seconds.
// Compare to p1343_2, this program is optimized by reusing visited between 1, 2 and 3.
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

struct Solution {
  int number;
  string ops;
  Solution() {
    number = -1;
    ops.clear();
  };

  bool operator < (const Solution& o) const {
    return ops > o.ops;
  };
};

int targetIndex[] = {6, 7, 8, 11, 12, 15, 16, 17};
vector<vector<int>> operations;

priority_queue<Solution> solutions;
unordered_map<uint32_t, pair<string, int>> visited;


void printVector(const vector<int>& v) {
  for (int tmp : v) {
    printf("%d ", tmp);
  }
  printf("\n");
}

uint32_t encode(const vector<int>& state) {
  uint32_t rtn = 0;
  for (int i = 0; i < 24; i++) {
    if (state[i] == 1) {
      rtn = rtn | (1U << i);
    }
  }
  return rtn;
}

bool isTarget(const vector<int>& state) {
  for (int i : targetIndex) {
    if (state[i] != 1) {
      return false;
    }
  }
  return true;
}

// Implements the < operator.
bool isStringSmaller(const string& a, const string& b) {
  if (a.size() != b.size()) {
    return a.size() < b.size();
  }
  for (int i = 0; i < a.size(); i++) {
    if (a[i] != b[i]) {
      return a[i] < b[i];
    }
  }
  return false;
}

void expand(queue<vector<int>>& level, const vector<int>& elem, int number) {
  uint32_t curEncoded = encode(elem);
  string curOps = visited[curEncoded].first;
  for (int i = 0; i < operations.size(); i++) {
    const vector<int>& operation = operations[i];
    vector<int> tmp = elem;
    for (int i = 0; i < operation.size() - 1; i++) {
      swap(tmp[operation[i]], tmp[operation[i + 1]]);
    }
    uint32_t encoded = encode(tmp);
    if (visited.find(encoded) == visited.end()
      || isStringSmaller(curOps + (char)('A' + i), visited.at(encoded).first)) {
      visited[encoded] = make_pair(curOps + (char)('A' + i), number);
      level.push(tmp);
    }
  }
  return;
}

void searchOneLevel(queue<vector<int>>& level, int number) {
  int levelSize = level.size();
  while (levelSize--) {
    vector<int> elem = level.front();
    level.pop();
    if (isTarget(elem)) {
      Solution solution;
      solution.ops = visited[encode(elem)].first;
      solution.number = visited[encode(elem)].second;
      solutions.push(solution);
    }
    expand(level, elem, number);
  }
}

void solve(int* input) {
  vector<int> input1(24);
  vector<int> input2(24);
  vector<int> input3(24);
  queue<vector<int>> level1;
  queue<vector<int>> level2;
  queue<vector<int>> level3;

  for (int i = 0; i < 24; i++) {
    if (input[i] == 1) {
      input1[i] = 1;
    } else {
      input1[i] = 0;
    }
  }
  for (int i = 0; i < 24; i++) {
    if (input[i] == 2) {
      input2[i] = 1;
    } else {
      input2[i] = 0;
    }
  }
  for (int i = 0; i < 24; i++) {
    if (input[i] == 3) {
      input3[i] = 1;
    } else {
      input3[i] = 0;
    }
  }

  visited.clear();
  while (!solutions.empty()) {
    solutions.pop();
  }

  level1.push(input1);
  level2.push(input2);
  level3.push(input3);

  visited[encode(input1)] = make_pair("", 1);
  visited[encode(input2)] = make_pair("", 2);
  visited[encode(input3)] = make_pair("", 3);

  while (solutions.empty()) {
    searchOneLevel(level1, 1);
    searchOneLevel(level2, 2);
    searchOneLevel(level3, 3);
  }

  Solution solution = solutions.top();
  if (solution.ops.size() == 0) {
    printf("No moves needed\n");
  } else {
    printf("%s\n", solution.ops.c_str());
  }
  printf("%d\n", solution.number);
}

int main() {
  #ifndef ONLINE_JUDGE
  clock_t t;
  t = clock();
  #endif

  // A.
  operations.push_back({0, 2, 6, 11, 15, 20, 22});
  operations.push_back({1, 3, 8, 12, 17, 21, 23});
  operations.push_back({10, 9, 8, 7, 6, 5, 4});
  operations.push_back({19, 18, 17, 16, 15, 14, 13});
  // E.
  operations.push_back({23, 21, 17, 12, 8, 3, 1});
  operations.push_back({22, 20, 15, 11, 6, 2, 0});
  operations.push_back({13, 14, 15, 16, 17, 18, 19});
  operations.push_back({4, 5, 6, 7, 8, 9, 10});

  int input[24];
  while (scanf("%d", &input[0]) == 1 && input[0] != 0) {
    for (int i = 1; i < 24; i++) {
      scanf("%d", &input[i]);
    }
    solve(input);
  }

  #ifndef ONLINE_JUDGE
  t = clock() - t;
  printf ("This program took %f seconds.\n", ((float)t) / CLOCKS_PER_SEC);
  #endif
  return 0;
}

/*
1 1 1 1 3 2 3 2 3 1 3 2 2 3 1 2 2 2 3 1 2 1 3 3
1 1 1 1 1 1 1 1 2 2 2 2 2 2 2 2 3 3 3 3 3 3 3 3
2 2 2 2 2 2 1 1 1 2 2  1  1  3  3  1  1  1  3  3  3  3  3  3
1 3 2 1 3 1 3 2 1 1 2 3 2 3 1 2 1 2 3 2 2 3 1 3
1 2 3 1 2 3 1 2 3 1 2 3 1 2 3 1 2 3 1 2 3 1 2 3
1 2 1 3 2 2 3 1 2 2 3 2 3 3 3 1 1 3 1 3 1 1 2 2
0

AC
2
DDHH
2
No moves needed
1
BCGGABBCG
3
ACBDDBFHF
3
EFHBBDDDH
3
*/