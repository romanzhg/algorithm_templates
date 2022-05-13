#include "common.h"

const int kMod = 1e9 + 7;
class Solution {
 public:
  int kConcatenationMaxSum(vector<int>& arr, int k) {
    int arr_sum = 0;
    for (int v : arr) {
      arr_sum += v;
    }

    int max_sum_forward = 0;
    int tmp = 0;
    for (int v : arr) {
      tmp += v;
      max_sum_forward = max(max_sum_forward, tmp);
    }

    int max_sum_backward = 0;
    tmp = 0;
    for (int i = arr.size() - 1; i >= 0; i--) {
      tmp += arr[i];
      max_sum_backward = max(max_sum_backward, tmp);
    }

    int max_sum_two_arrs = (max_sum_forward + max_sum_backward) % kMod;

    int max_sum_inside = 0;
    tmp = 0;
    for (int v : arr) {
      if (tmp < 0) {
        tmp = 0;
      }
      tmp += v;
      max_sum_inside = max(max_sum_inside, tmp) % kMod;
    }

    if (k == 1) {
      return max_sum_inside;
    }
    if (k == 2) {
      return max(max_sum_inside, max_sum_two_arrs);
    }
    if (arr_sum >= 0) {
      int adder = 0;
      for (int i = 0; i < k - 2; i++) {
        adder = (adder + arr_sum) % kMod;
      }
      return max(max_sum_inside, (adder + max_sum_two_arrs) % kMod);
    } else {
      return max(max_sum_inside, max_sum_two_arrs);
    }
  }
};