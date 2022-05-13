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

const int VALUE_RANGE = 2 * 1000000;
enum OpType {DeleteEdge, Change, Query};
struct Op {
  OpType type;
  // DeleteEdge uses only arg1: the edge id.
  // Query uses arg1: the vertex id and arg2: the k-th maximum.
  // Change uses arg1: the vertex id, arg2: the new value,
  // arg3: the old value. the new value and old value is to be
  // flipped as we update the ops.
  int arg1, arg2, arg3;
  Op(OpType t, int arg1 = 0, int arg2 = 0, int arg3 = 0) :
    type(t), arg1(arg1), arg2(arg2), arg3(arg3) {};
};

// int nodes = 10;
// int edges = 10;
// int queries = 5;
// int changes = 5;
// int deletes = edges / 5 * 4;

int nodes = 2 * 10000;
int edges = 6 * 10000;
int queries = 2 * 100000;
int changes = 0; //2 * 100000;
int deletes = edges / 5 * 4;

int randomNode() {
  return rand() % nodes + 1;
}

int randomWeight() {
  return rand() % VALUE_RANGE - VALUE_RANGE / 2;
}

int randomEdge() {
  return rand() % edges + 1;
}

int main() {
  srand(time(NULL));
 
  printf("%d %d\n", nodes, edges);
  for (int i = 0; i < nodes; i++) {
    printf("%d\n", randomWeight());
  }

  set<pair<int, int>> existingEdges;
  for (int i = 0; i < edges; i++) {
    while (true) {
      int a = randomNode();
      int b = randomNode();
      if (a != b && existingEdges.find(make_pair(a, b)) == existingEdges.end()) {
        printf("%d %d\n", a, b);
        existingEdges.insert(make_pair(a, b));
        break;
      } 
    }
  }

  vector<Op> operations;
  for (int i = 0; i < queries; i++) {
    operations.push_back(Op(OpType::Query, randomNode(), rand() % 1000 + 1));
  }
  for (int i = 0; i < changes; i++) {
    operations.push_back(Op(OpType::Change, randomNode(), randomWeight()));
  }

  set<int> deletedEdges;
  for (int i = 0; i < deletes; i++) {
    int tmpEdge;
    while (true) {
      tmpEdge = randomEdge();
      if (deletedEdges.find(tmpEdge) == deletedEdges.end()) {
        deletedEdges.insert(tmpEdge);
        break;
      }
    }
    operations.push_back(Op(OpType::DeleteEdge, tmpEdge));
  }
  random_shuffle(operations.begin(), operations.end());

  for (const Op& op : operations) {
    if (op.type == OpType::Query) {
      printf("Q %d %d\n", op.arg1, op.arg2);
    } else if (op.type == OpType::Change) {
      printf("C %d %d\n", op.arg1, op.arg2);
    } else if (op.type == OpType::DeleteEdge) {
      printf("D %d\n", op.arg1);
    }
  }
  printf("E\n");
  printf("0 0\n");
  return 0;
}