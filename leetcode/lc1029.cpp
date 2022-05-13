/**
 * Greedy normally involves a sort, so needs to think about what to sort.
 * In this problem the value sorted is "loss if the person is scheduled to city A".
 * Greedy algorithms normally proved by contradiction.
 */
#include "common.h"
struct Elem {
  int id, value;
  Elem(int id, int value) : id(id), value(value) {};
  bool operator<(const Elem& o) const {
    return value < o.value;
  }
};

class Solution {
 public:
  int twoCitySchedCost(vector<vector<int>>& costs) {
    vector<Elem> elems;
    for (int i = 0; i < costs.size(); i++) {
      elems.push_back({i, costs[i][0] - costs[i][1]});
    }
    sort(elems.begin(), elems.end());

    set<int> a_set;
    for (int i = 0; i < costs.size() / 2; i++) {
      a_set.insert(elems[i].id);
    }

    int rtn = 0;
    for (int i = 0; i < costs.size(); i++) {
      rtn += a_set.find(i) == a_set.end() ? costs[i][1] : costs[i][0];
    }
    return rtn;
  }
};