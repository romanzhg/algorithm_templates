/**
 * Greedy, swap the first unmatch.
 */
#include "common.h"

class Solution {
 public:
  int minSwapsCouples(vector<int>& row) {
    int swaps = 0;
    for (int i = 0; i < row.size() / 2; i++) {
      int first = i * 2, second = i * 2 + 1;
      int match = row[first] ^ 1;
      if (row[second] != match) {
        for (int j = second + 1; j < row.size(); j++) {
          if (row[j] == match) {
            swap(row[second], row[j]);
            break;
          }
        }
        swaps++;
      }
    }
    return swaps;
  }
};