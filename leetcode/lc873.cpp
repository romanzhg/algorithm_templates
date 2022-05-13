/**
 * First two numbers of a fib sequence defines the whole sequence.
 */
#include "common.h"

class Solution {
 public:
  int lenLongestFibSubseq(vector<int>& A) {
    vector<int> f;
    f.push_back(1);
    f.push_back(1);
    int a = 1, b = 1;
    while (f.back() <= 1e9) {
      f.push_back(a + b);
      a = b;
      b = f.back();
    }

    unordered_set<int> numbers;
    for (int v : A) {
      numbers.insert(v);
    }

    int rtn = 0;
    for (int l = 0; l < A.size() - 1; l++) {
      for (int r = l + 1; r < A.size(); r++) {
        int max_len = 0;
        int cur_num = A[l] * f[max_len] + A[r] * f[max_len + 1];
        while (numbers.find(cur_num) != numbers.end()) {
          max_len++;
          rtn = max(rtn, max_len);
          cur_num = A[l] * f[max_len] + A[r] * f[max_len + 1];
        }
      }
    }
    if (rtn == 0) {
      return 0;
    } else {
      return rtn + 2;
    }
  }
};