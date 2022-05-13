#include "common.h"

class Solution {
  // Return if a is a divisor of b.
  bool IsDivisor(string a, string b) {
    if (a.size() > b.size() || (b.size() % a.size() != 0)) {
      return false;
    }

    for (int i = 0; i < b.size(); i++) {
      if (b[i] != a[i % a.size()]) {
        return false;
      }
    }
    return true;
  }

 public:
  string gcdOfStrings(string str1, string str2) {
    if (str1.size() > str2.size()) {
      swap(str1, str2);
    }
    for (int l = str1.size(); l >= 1; l--) {
      string substr = str1.substr(0, l);
      if (IsDivisor(substr, str1) && IsDivisor(substr, str2)) {
        return substr;
      }
    }
    return "";
  }
};