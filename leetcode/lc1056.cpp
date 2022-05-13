#include "common.h"

class Solution {
  int GetRotatedDigit(int i) {
    if (i == 0) {
      return 0;
    } else if (i == 1) {
      return 1;
    } else if (i == 6) {
      return 9;
    } else if (i == 8) {
      return 8;
    } else {
      return 6;
    }
  }

 public:
  bool confusingNumber(int N) {
    if (N == 0) {
      return false;
    }
    vector<int> v;
    int tmp = N;
    while (tmp != 0) {
      int digit = tmp % 10;
      if (digit == 2 || digit == 3 || digit == 4 || digit == 5 || digit == 7) {
        return false;
      }
      v.insert(v.begin(), digit);
      tmp = tmp / 10;
    }

    int i = 0, j = v.size() - 1;
    vector<int> rv(v.size());
    while (i <= j) {
      if (i == j) {
        rv[i] = GetRotatedDigit(v[i]);
      } else {
        rv[i] = GetRotatedDigit(v[j]);
        rv[j] = GetRotatedDigit(v[i]);
      }
      i++;
      j--;
    }


    for (int i = 0; i < v.size(); i++) {
      if (rv[i] != v[i]) {
        return true;
      }
    }
    return false;
  }
};