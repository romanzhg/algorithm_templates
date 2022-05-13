/**
 * cur_window corresponds to the current state in the i loop, so when the loop is finished,
 * cur_window gets updated.
 *
 *
 */
#include "common.h"

class Solution {
 public:
  string removeDuplicates(string s, int k) {
    if ((int)s.size() < k) {
      return s;
    }

    map<char, int> cur_window;
    for (int i = 0; i < k; i++) {
      cur_window[s[i]]++;
    }
    for (int i = 0; i <= s.size() - k; i++) {
      if (cur_window[s[i]] == k) {
        string s1(s.begin(), s.begin() + i);
        string s2(s.begin() + i + k, s.end());
        return removeDuplicates(s1 + s2, k);
      }
      cur_window[s[i]]--;
      cur_window[s[i + k]]++;
    }
    return s;
  }
};