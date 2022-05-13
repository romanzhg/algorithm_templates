// A* search.
// Each move is at best one step closer to the target, the heuristic function
// is constructed based on this observation.
// Notice the two version of string comparison.
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

  // This comparison is not lexicographical comparison, shorter string are smaller.
  bool operator < (const Solution& o) const {
    if (ops.size() == o.ops.size()) {
      return ops > o.ops;
    } else {
      return ops.size() > o.ops.size();
    }
  };
};

struct Element {
  string ops;
  vector<int> state;
  int distHeuristic;
  Element(string ops = "", vector<int> state = {}, int distHeuristic = 0) :
    ops(ops), state(state),
    distHeuristic(distHeuristic) {};

  bool operator < (const Element& o) const {
    int distSelf = distHeuristic + ops.size();
    int distOther = o.distHeuristic + o.ops.size();
    if (distSelf != distOther) {
      return distSelf > distOther;
    } else {
      // Lexicographical comparison.
      return ops > o.ops;
    }
  };
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

int getHeuristicDist(const vector<int>& state) {
  int ctn = 0;
  for (int i : targetIndex) {
    if (state[i] != 1) {
      ctn++;
    }
  }
  return ctn;
}

void expand(const Element& elem, priority_queue<Element>& elements,
  unordered_set<uint32_t>& visited) {
  for (int i = 0; i < operations.size(); i++) {
    const vector<int>& operation = operations[i];
    vector<int> tmp = elem.state;
    for (int i = 0; i < operation.size() - 1; i++) {
      swap(tmp[operation[i]], tmp[operation[i + 1]]);
    }
    uint32_t encoded = encode(tmp);
    if (visited.find(encoded) != visited.end()) {
      continue;
    }
    Element tmpElem;
    tmpElem.state = tmp;
    tmpElem.ops = elem.ops + (char)('A' + i);
    tmpElem.distHeuristic = getHeuristicDist(tmp);
    elements.push(tmpElem);
  }
}

void solveInternal(int* input, int curNumber) {
  vector<int> tmpInput(24);
  priority_queue<Element> elements;
  unordered_set<uint32_t> visited;

  for (int i = 0; i < 24; i++) {
    if (input[i] == curNumber) {
      tmpInput[i] = 1;
    } else {
      tmpInput[i] = 0;
    }
  }

  elements.push(Element("", tmpInput, getHeuristicDist(tmpInput)));

  while (!elements.empty()) {
    Element elem = elements.top();
    elements.pop();
    uint32_t encoded = encode(elem.state);
    if (visited.find(encoded) != visited.end()) {
      continue;
    }
    visited.insert(encoded);

    #ifndef ONLINE_JUDGE
    ++processedStates;
    #endif

    if (isTarget(elem.state)) {
      Solution tmp;
      tmp.number = curNumber;
      tmp.ops = elem.ops;
      solutions.push(tmp);
      return;
    }
    expand(elem, elements, visited);
  }
}

void solve(int* input) {
  processedStates = 0;
  while (!solutions.empty()) {
    solutions.pop();
  }

  // TODO: prune between the three searchs.
  solveInternal(input, 1);
  solveInternal(input, 2);
  solveInternal(input, 3);
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