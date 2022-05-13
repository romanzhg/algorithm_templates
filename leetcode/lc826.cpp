/**
 * 1. Sort job base on profit, if one job compare to the others has a lower profit and equal or
 * higher difficulty, it should not be considered. To remove such jobs, sort based on profit,
 * process from high to lower, and record current min difficulty -- the following jobs(profit lower)
 * should have a lower difficulty.
 *
 * 2. Use upper bound to find elements smaller or equal to a key.
 *
 * 3. Leetcode solution sort both profits and workers, which is faster than this.
 */
#include "common.h"

struct Elem {
  int d, p;
  Elem(int d, int p) : d(d), p(p) {};

  bool operator<(const Elem& o) const {
    return p > o.p;
  }
};

class Solution {
 public:
  int maxProfitAssignment(vector<int>& difficulty, vector<int>& profit, vector<int>& worker) {
    int job_size = difficulty.size();
    vector<Elem> elems;
    for (int i = 0; i < job_size; i++) {
      elems.push_back({difficulty[i], profit[i]});
    }
    sort(elems.begin(), elems.end());

    map<int, int> job_to_profit;
    int cur_difficulty = INT_MAX;
    for (const Elem& e : elems) {
      if (e.d < cur_difficulty) {
        cur_difficulty = e.d;
        job_to_profit[e.d] = e.p;
      }
    }

    int rtn = 0;
    for (int w : worker) {
      auto it = job_to_profit.upper_bound(w);
      if (it != job_to_profit.begin()) {
        it = prev(it);
        rtn += it->second;
      }
    }
    return rtn;
  }
};