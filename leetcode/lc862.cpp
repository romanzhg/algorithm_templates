/**
 * Use prefix sum to accelerate.
 *
 * Solution1 is a low efficient implementation, note it actually modifies the array by doing
 * the prefix sum. Should modify it to actully use prefix sum.
 */
#include "common.h"

struct Elem {
  // value and index.
  int v, i;
};

class Solution {
 public:
  int shortestSubarray(vector<int>& A, int K) {
    deque<Elem> elems;
    elems.push_back({0, 0});

    int prev = 0;
    int rtn = INT_MAX;
    for (int i = 1; i < A.size() + 1; i++) {
      prev += A[i - 1];
      while (!elems.empty()) {
        if (prev - elems.front().v >= K) {
          rtn = min(rtn, i - elems.front().i);
          elems.pop_front();
        } else {
          break;
        }
      }
      while (!elems.empty()) {
        if (elems.back().v >= prev) {
          elems.pop_back();
        } else {
          break;
        }
      }
      elems.push_back({prev, i});
    }

    if (rtn == INT_MAX) {
      return -1;
    } else {
      return rtn;
    }
  }
};


class Solution1 {
 public:
  int shortestSubarray(vector<int>& A, int K) {
    int rtn = INT_MAX;
    vector<Elem> elems;
    elems.push_back({A[0], 0});
    if (A[0] >= K) {
      return 1;
    }
    for (int i = 1; i < A.size(); i++) {
      if (A[i] >= K) {
        return 1;
      }
      // 1. Remove any element that is smaller or equal to 0.
      for (int j = elems.size() - 1; j >=0; j--) {
        if (elems[j].v < 0) {
          elems.erase(elems.begin() + j);
        } else {
          elems[j].v += A[i];
          if (elems[j].v >= K) {
            rtn = min(rtn, i - elems[j].i + 1);
          }
        }
      }
      // 2. Add A[i] to the set, update rtn.
      elems.push_back({A[i], i});
    }

    if (rtn == INT_MAX) {
      return -1;
    } else {
      return rtn;
    }
  }
};