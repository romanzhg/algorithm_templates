/**
 * Solution is an O(n ^ 2) algorithm, use prev to count the total occurance on the char to be added.
 *
 * Solution1 is an O(n) algorithm. For each char, check for how many substrings it will contribute to Unique.
 * Suppose the string is "...A.A..", then the left A will contribute 1 in 4 * 2 substrings.
 */
#include "common.h"

class Solution {
  const int kCharCount = 26;
  const int kMod = 1e9 + 7;

 public:
  int uniqueLetterString(string S) {
    map<char, int> char_to_prev;
    for (int i = 0; i < kCharCount; i++) {
      char_to_prev[i] = -1;
    }
    vector<int> prev(S.size());
    for (int i = 0; i < S.size(); i++) {
      prev[i] = char_to_prev[S[i] - 'A'];
      char_to_prev[S[i] - 'A'] = i;
    }
    int rtn = S.size();
    vector<int> result(S.size(), 1);
    for (int len = 2; len <= S.size(); len++) {
      for (int i = 0; i + len <= S.size(); i++) {
        int range_r = i + len;
        int index_to_check = range_r - 1;
        int p1 = prev[index_to_check];
        int p0 = p1 == -1 ? -1 : prev[p1];
        if (p0 < i && p1 >= i) {
          result[i]--;
        }
        if (p0 < i && p1 < i) {
          result[i]++;
        }
        rtn = (rtn + result[i]) % kMod;
      }
    }
    return rtn;
  }
};

class Solution1 {
  const int kCharCount = 26;
  const int kMod = 1e9 + 7;
 public:
  int uniqueLetterString(string S) {
    vector<vector<int>> char_to_index(kCharCount, {-1});
    for (int i = 0; i < S.size(); i++) {
      char_to_index[S[i] - 'A'].push_back(i);
    }
    for (vector<int>& v : char_to_index) {
      v.push_back(S.size());
    }
    int rtn = 0;
    for (const vector<int>& v : char_to_index) {
      for (int i = 1; i < v.size() - 1; i++) {
        int left = v[i - 1], mid = v[i], right = v[i + 1];
        rtn = (rtn + (mid - left) * (right - mid)) % kMod;
      }
    }
    return rtn;
  }
};