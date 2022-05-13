/**
 * A modified minimum spanning tree.
 *
 * Assume every village has a well at the beginning.
 * Connect two village if the connecting cost is smaller than the more costly well,
 * then update the well cost(union find may choose anyone as the parent).
 * Finally count the well cost for each connected components.
 */
#include "common.h"

struct Pipe {
  int a, b, cost;
  Pipe(int a, int b, int cost) : a(a), b(b), cost(cost) {};
  bool operator<(const Pipe& o) const {
    return cost > o.cost;
  }
};

class Solution {
 public:
  vector<int> parent;
  void Union(int a, int b) {
    int pa = Find(a);
    int pb = Find(b);
    parent[pa] = pb;
  }

  int Find(int a) {
    if (a == parent[a]) {
      return a;
    }
    parent[a] = Find(parent[a]);
    return parent[a];
  }

  int minCostToSupplyWater(int n, vector<int>& wells, vector<vector<int>>& pipes) {
    parent.resize(n + 1);
    for (int i = 1; i <= n; i++) {
      parent[i] = i;
    }

    priority_queue<Pipe> pq;
    for (const vector<int>& p : pipes) {
      pq.push(Pipe(p[0], p[1], p[2]));
    }

    int total_cost = 0;
    while (!pq.empty()) {
      Pipe p = pq.top(); pq.pop();
      int parent_a = Find(p.a);
      int parent_b = Find(p.b);
      if (parent_a == parent_b) {
        continue;
      }
      int cost = max(wells[parent_a - 1], wells[parent_b - 1]);
      int new_well_cost = min(wells[parent_a - 1], wells[parent_b - 1]);
      if (p.cost < cost) {
        total_cost += p.cost;
        Union(parent_a, parent_b);
        wells[Find(parent_a) - 1] = new_well_cost;
      }
    }

    set<int> components;
    for (int i = 1; i <= n; i++) {
      components.insert(Find(i));
    }
    for (int v : components) {
      total_cost += wells[v - 1];
    }
    return total_cost;
  }
};