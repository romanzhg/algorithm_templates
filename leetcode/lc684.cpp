/**
 * Union-Find.
 */
#include "common.h"

class Solution {
 public:
  vector<int> parent;

  void Union(int a, int b) {
    int pa = Find(a);
    int pb = Find(b);
    parent[pa] = pb;
  }

  int Find(int a) {
    if (parent[a] == a) {
      return a;
    }
    int p = Find(parent[a]);
    parent[a] = p;
    return p;
  }

  vector<int> findRedundantConnection(vector<vector<int>>& edges) {
    parent.resize(edges.size() + 1);
    for (int i = 0; i < edges.size() + 1; i++) {
      parent[i] = i;
    }
    for (vector<int>& edge : edges) {
      if (Find(edge[0]) == Find(edge[1])) {
        return edge;
      }
      Union(edge[0], edge[1]);
    }
    return {};
  }
};