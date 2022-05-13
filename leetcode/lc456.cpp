/**
 * Solution is a little better then brute force. Need to look at other improvements.
 *
 * There is an O(nlog(n)) solution, basically sort the remaining array(index 2 to end),
 * do the prev_min, cur element thing, and every time the cur element advance, remove one
 * element from the sorted array.
 *
 */
#include "common.h"

class Solution {
 public:
  bool find132pattern(vector<int>& nums) {
    if (nums.size() < 3) {
      return false;
    }
    int prev_min = nums[0];
    for (int i = 1; i < nums.size() - 1; i++) {
      if (nums[i] <= prev_min) {
        prev_min = nums[i];
        continue;
      }
      // Now, nums[i] > prev_min.
      if (nums[i + 1] >= nums[i]) {
        continue;
      }
      // Now, nums[i] > prev_min && nums[i + 1] < nums[i].
      for (int j = i + 1; j < nums.size(); j++) {
        if (nums[j] > prev_min && nums[j] < nums[i]) {
          return true;
        }
      }
    }
    return false;
  }
};