#include <algorithm>
#include <cfloat>
#include <climits>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
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
#include <chrono>

using namespace std;

// n in [0, 31]
uint32_t setBit(uint32_t value, int32_t n) {
  return value | (1U << n);
}

int bug_count, patch_count;

struct Patch {
  Patch(char* from, char* to, int cost) : cost(cost) {
    mask_need_to_present = 0;
    mask_cannot_present = 0;
    mask_unset = 0;
    mask_set = 0;

    for (int i = 0; i < bug_count; i++) {
      if (from[i] == '+') {
        mask_need_to_present = setBit(mask_need_to_present, i);
      }
      if (from[i] == '-') {
        mask_cannot_present = setBit(mask_cannot_present, i);
      }
    }

    for (int i = 0; i < bug_count; i++) {
      if (to[i] == '+') {
        mask_set = setBit(mask_set, i);
      }
      if (to[i] == '-') {
        mask_unset = setBit(mask_unset, i);
      }
    }
  }

  // vertex is a bit map. 1 for the bug presents, 0 for the bug doesn't present.
  bool CanApplyOn(uint32_t vertex) const {
    uint32_t tmp;
    tmp = vertex & mask_need_to_present;
    if (tmp != mask_need_to_present) {
      return false;
    }

    tmp = ~vertex;
    tmp = tmp & mask_cannot_present;
    return tmp == mask_cannot_present;
  }

  uint32_t Apply(uint32_t vertex) const {
    vertex = vertex & (~mask_unset);
    vertex = vertex | mask_set;
    return vertex;
  }

  uint32_t mask_need_to_present;
  uint32_t mask_cannot_present;
  uint32_t mask_unset;
  uint32_t mask_set;
  int cost;
};

struct HeapNode {
  int dist;
  uint32_t vertex;
  HeapNode(int dist, uint32_t vertex) : dist(dist), vertex(vertex) {}
  bool operator< (const HeapNode& o) const {
    return dist > o.dist;
  }
};

// Global data.
set<int> done;
vector<Patch> patches;

int Dijkstra(uint32_t src, uint32_t target) {
  priority_queue<HeapNode> pq;
  pq.push((HeapNode){0, src});

  while(!pq.empty()) {
    HeapNode x = pq.top(); pq.pop();
    uint32_t cur_vertex = x.vertex;
    if (cur_vertex == target) {
      return x.dist;
    }
    // A node may be pushed multiple times, but with the bool array done here, we only process
    // each node once. The first time one node popped corresponds to its shortest distance.
    if (done.find(cur_vertex) != done.end()) {
      continue;
    }
    done.insert(cur_vertex);

    for (const Patch& p : patches) {
      if (!p.CanApplyOn(cur_vertex)) {
        continue;
      }
      uint32_t new_vertex = p.Apply(cur_vertex);
      if (done.find(new_vertex) != done.end()) {
        continue;
      }
      int new_dist = x.dist + p.cost;
      pq.emplace(new_dist, new_vertex);
    }
  }
  return -1;
}


int main() {
  int test_case_counter = 0;
  while (true) {

    scanf("%d %d", &bug_count, &patch_count);
    if (bug_count == 0 && patch_count == 0) {
      break;
    }

//    auto cycle_start = std::chrono::system_clock::now();

    patches.clear();
    done.clear();
    for (int i = 0; i < patch_count; i++) {
      int cost;
      char from[30], to[30];
      scanf("%d %s %s", &cost, from, to);
      Patch p(from, to, cost);
      patches.push_back(p);
    }

    uint32_t src = 1;
    src = (src << bug_count) - 1;
    int shortest_dist = Dijkstra(src, 0);

    cout << "Product " << ++test_case_counter << endl;
    if (shortest_dist == -1) {
      cout << "Bugs cannot be fixed." << endl;
    } else {
      cout << "Fastest sequence takes " << shortest_dist << " seconds." << endl;
    }
    cout << endl;

//    auto cycle_end = std::chrono::system_clock::now();
//    std::chrono::duration<double> elapsed_seconds = cycle_end - cycle_start;
//    std::cout << "Elapsed time: " << elapsed_seconds.count() << "s\n";

  }
  return 0;
}