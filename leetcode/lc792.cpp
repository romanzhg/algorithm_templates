/**
 * Check subsequence.
 *
 * The reference solution check all words together, by their unmatched part, should also consider.
 */
#include "common.h"

class Solution {
 public:
  int numMatchingSubseq(string S, vector<string>& words) {
    vector<set<int>> char_to_set(26);
    for (int i = 0; i < S.size(); i++) {
      char_to_set[S[i] - 'a'].insert(i);
    }

    int rtn = 0;
    for (string word : words) {
      int prev_index = -1;
      int char_index = 0;
      for (; char_index < word.size(); char_index++) {
        set<int>& to_search = char_to_set[word[char_index] - 'a'];
        auto it = to_search.upper_bound(prev_index);
        if (it == to_search.end()) {
          break;
        } else {
          prev_index = *it;
        }
      }
      if (char_index == word.size()) {
        rtn++;
      }
    }
    return rtn;
  }
};