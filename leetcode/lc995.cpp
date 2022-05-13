/**
 * Always flip the first.
 * Use a window to maintain the flip count.
 */
#include "common.h"

class Solution {
 public:
  int minKBitFlips(vector<int>& A, int K) {
    int rtn = 0;
    queue<int> window;
    for (int i = 0; i < A.size(); i++) {
      if (!window.empty()) {
        if (window.front() <= (i - K)) {
          window.pop();
        }
      }
      if (A[i] ^ (window.size() % 2) == 0) {
        if (i > A.size() - K) {
          return -1;
        }
        window.push(i);
        rtn++;
      }
    }
    return rtn;
  }
};