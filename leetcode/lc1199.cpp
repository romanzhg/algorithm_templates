/**
 * Solution should be correct but TLE.
 *
 * Solution1 models the problem as building a binary tree, and uses Huffman's algorithm.
 */

#include "common.h"

class Solution1 {
 public:
  int minBuildTime(vector<int>& blocks, int split) {
    priority_queue<int, std::vector<int>, std::greater<int>> pq;
    for (int v : blocks) {
      pq.push(v);
    }
    while (pq.size() > 1) {
      int a = pq.top(); pq.pop();
      int b = pq.top(); pq.pop();
      pq.push(split + max(a, b));
    }
    return pq.top();
  }
};

class Solution {
  int split_cost;
  vector<int> blocks_sorted;
  int blocks_size;
  map<int, map<int, int>> saved_search;
 public:

  int GetMinTime(int start_index, int workers) {
    int todo_count = blocks_size - start_index;
    if (workers >= todo_count) {
      return blocks_sorted[start_index];
    }

    if (saved_search.find(start_index) != saved_search.end()) {
      if (saved_search[start_index].find(workers) != saved_search[start_index].end()) {
        return saved_search[start_index][workers];
      }
    }

    int tmp = INT_MAX;
    for (int to_split = 1; to_split <= min(workers, todo_count - workers); to_split++) {
      int cur_workers = workers - to_split;

      if (cur_workers == 0) {
        tmp = min(tmp, GetMinTime(start_index, to_split * 2) + split_cost);
      } else {
        tmp = min(tmp,
                  max(GetMinTime(start_index + cur_workers, to_split * 2) + split_cost,
                      blocks_sorted[start_index]));
      }
    }
    saved_search[start_index][workers] = tmp;
    return tmp;
  }

  int minBuildTime(vector<int>& blocks, int split) {
    saved_search.clear();
    split_cost = split;
    blocks_sorted = blocks;
    blocks_size = blocks.size();
    sort(blocks_sorted.begin(), blocks_sorted.end());
    reverse(blocks_sorted.begin(), blocks_sorted.end());

    return GetMinTime(0, 1);
  }
};