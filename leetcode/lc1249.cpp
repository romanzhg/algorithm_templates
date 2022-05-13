/**
 * This program is actually not well written. Should use a struct instead of positive/negative integer
 * to denote '(' or ')', should check if start_index equals to the end.
 *
 */
#include "common.h"

class Solution {
 public:
  string minRemoveToMakeValid(string s) {
    vector<int> p_indexes;
    for (int i = 0; i < s.size(); i++) {
      if (s[i] == '(') {
        p_indexes.push_back(i + 1);
      } else if (s[i] == ')') {
        if (p_indexes.empty() || p_indexes.back() < 0) {
          p_indexes.push_back(-(i + 1));
        } else {
          p_indexes.pop_back();
        }
      }
    }

    // Note: in the first version there is no + 1 here, so it was a problem.
    // If one part of the program changed, the other part should also be changed.
    // It is better not hide logic inside values.
    p_indexes.push_back((int)s.size() + 1);

    string rtn = "";
    int start_index = 0;
    for (int v : p_indexes) {
      v = abs(v) - 1;
      if (start_index == v) {
        start_index = v + 1;
        continue;
      } else {
        rtn += s.substr(start_index, v - start_index);
        start_index = v + 1;
      }
    }
    return rtn;
  }
};