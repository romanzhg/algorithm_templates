/**
 * Simple greedy, similar to Huffman's algorithm.
 *
 */
#include "common.h"

class Solution {
 public:
  int connectSticks(vector<int>& sticks) {
    priority_queue<int, vector<int>, std::greater<int>> pq;
    int rtn = 0;
    for (int v : sticks) {
      pq.push(v);
    }

    while(pq.size() > 1) {
      int a = pq.top(); pq.pop();
      int b = pq.top(); pq.pop();
      rtn += a + b;
      pq.push(a + b);
    }
    return rtn;
  }
};