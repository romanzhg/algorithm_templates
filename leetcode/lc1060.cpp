#include "common.h"

class Solution {
 public:
  int missingElement(vector<int>& nums, int k) {
    int cur = nums.front();
    for (int i = 1; i < nums.size(); i++) {
      int tmp = nums[i] - cur - 1;
      if (tmp >= k) {
        return cur + k;
      } else {
        k -= tmp;
      }
      cur = nums[i];
    }
    return cur + k;
  }
};