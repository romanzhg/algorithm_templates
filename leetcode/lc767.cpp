/**
 * Always put the most frequent char, with the restriction that it cannot be same as the previous one.
 */
#include "common.h"

class Solution {
 public:
  int GetNextCharWithRestriction(vector<int>& char_count, int res) {
    int rtn = -1;
    int max_occ = 0;
    for (int i = 0; i < 26; i++) {
      if (char_count[i] > max_occ && i != res) {
        max_occ = char_count[i];
        rtn = i;
      }
    }
    return rtn;
  }

  string reorganizeString(string S) {
    vector<int> char_count(26);
    for (char c : S) {
      char_count[c - 'a']++;
    }

    string rtn;
    int prev = -1;
    for (int i = 0; i < S.size(); i++) {
      int c = GetNextCharWithRestriction(char_count, prev);
      if (c == -1) {
        return "";
      }
      char_count[c]--;
      rtn += (char)(c + 'a');
      prev = c;
    }
    return rtn;
  }
};