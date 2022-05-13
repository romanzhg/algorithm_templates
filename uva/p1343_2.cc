// BFS.
// The program took 14.436517 seconds.
// With BFS, the visited record does not need to record distance, it just
// needs to store some parent data to rebuild the path.
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

struct VisitRecord {
  // The encoded previous state.
  uint32_t prev;
  char op;

  VisitRecord(uint32_t prev = 0, char op = 0) :
    prev(prev), op(op) {};
};

int targetIndex[] = {6, 7, 8, 11, 12, 15, 16, 17};
int processedStates;
priority_queue<Solution> solutions;
vector<vector<int>> operations;

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

void expand(queue<vector<int>>& level,
            vector<int>& elem, uint32_t curEncoded,
            unordered_map<uint32_t, VisitRecord>& visited) {
  for (int i = 0; i < operations.size(); i++) {
    const vector<int>& operation = operations[i];
    vector<int> tmp = elem;
    for (int i = 0; i < operation.size() - 1; i++) {
      swap(tmp[operation[i]], tmp[operation[i + 1]]);
    }
    uint32_t encoded = encode(tmp);
    if (visited.find(encoded) == visited.end()) {
      visited[encoded] = VisitRecord(curEncoded, 'A' + i);
      level.push(tmp);
    }
  }
  return;
}

string getPrev(const vector<int>& elem,
              unordered_map<uint32_t, VisitRecord>& visited) {
  VisitRecord vr = visited[encode(elem)];
  string rtn;
  while (vr.prev != 0) {
    rtn.push_back(vr.op);
    vr = visited[vr.prev];
  }
  reverse(rtn.begin(), rtn.end());
  return rtn;
}

void searchOneLevel(queue<vector<int>>& level, int currentLevel,
                    unordered_map<uint32_t, VisitRecord>& visited, int number) {
  int levelSize = level.size();
  while (levelSize--) {
    #ifndef ONLINE_JUDGE
    ++processedStates;
    #endif

    vector<int> elem = level.front();
    level.pop();
    uint32_t curEncoded = encode(elem);
    if (isTarget(elem)) {
      Solution tmp;
      tmp.number = number;
      tmp.ops = getPrev(elem, visited);
      solutions.push(tmp);
      return;
    }
    expand(level, elem, curEncoded, visited);
  }
}

void solve(int* input) {
  vector<int> input1(24);
  vector<int> input2(24);
  vector<int> input3(24);
  queue<vector<int>> level1;
  queue<vector<int>> level2;
  queue<vector<int>> level3;

  // Key: encoded state.
  // Value: pair<distance, the previous state encoded>.
  unordered_map<uint32_t, VisitRecord> visited1;
  unordered_map<uint32_t, VisitRecord> visited2;
  unordered_map<uint32_t, VisitRecord> visited3;

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

  processedStates = 0;
  visited1.clear();
  visited2.clear();
  visited3.clear();
  while (!solutions.empty()) {
    solutions.pop();
  }

  level1.push(input1);
  level2.push(input2);
  level3.push(input3);

  int currentLevel = 0;
  visited1[encode(input1)] = VisitRecord(0, 0);
  visited2[encode(input2)] = VisitRecord(0, 0);
  visited3[encode(input3)] = VisitRecord(0, 0);

  while (solutions.empty()) {
    searchOneLevel(level1, currentLevel, visited1, 1);
    searchOneLevel(level2, currentLevel, visited2, 2);
    searchOneLevel(level3, currentLevel, visited3, 3);
    ++currentLevel;
  }

  Solution solution = solutions.top();
  if (solution.ops.size() == 0) {
    printf("No moves needed\n");
  } else {
    printf("%s\n", solution.ops.c_str());
  }
  printf("%d\n", solution.number);

  #ifndef ONLINE_JUDGE
  printf("processedStates: %d\n", processedStates);
  #endif
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
  printf ("The program took %f seconds.\n", ((float)t) / CLOCKS_PER_SEC);
  #endif
  return 0;
}