/**
 * The code can be more concise by explicitly code the window.
 *
 * In Solution1, i refers to interval start, j refers to interval end.
 */
#include "common.h"

class Solution {
 public:
  int numKLenSubstrNoRepeats(string S, int K) {
    map<char, int> count;
    if (S.size() < K) {
      return 0;
    }
    for (int i = 0; i < K; i++) {
      count[S[i]]++;
    }

    int rtn = 0;
    if (count.size() == K) {
      rtn++;
    }

    for (int i = K ; i < S.size(); i++) {
      count[S[i-K]]--;
      if (count[S[i-K]] == 0) {
        count.erase(S[i-K]);
      }
      count[S[i]]++;
      if (count.size() == K) {
        rtn++;
      }
    }
    return rtn;
  }
};

class Solution1 {
 public:
  int numKLenSubstrNoRepeats(string S, int K) {
    set<char> char_set;

    int rtn = 0, j = 0;
    for (int i = 0; i < S.size(); i++) {
      while (char_set.find(S[i]) != char_set.end()) {
        char_set.erase(S[j]);
        j++;
      }
      char_set.insert(S[i]);
      if (i - j + 1 >= K) {
        rtn++;
      }

    }
    return rtn;
  }
};