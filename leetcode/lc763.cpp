/**
 * Coding problem. Find the first and last occurance of each char, get the first unprocessed
 * one, get the range(extend the range with any other char in between).
 */
#include "common.h"
class Solution {
  int GetMinOcc(const vector<int>& first_index) {
    int rtn = -1;
    int occ = INT_MAX;
    for (int i = 0; i < 26; i++) {
      if (first_index[i] != -1 && first_index[i] < occ) {
        occ = first_index[i];
        rtn = i;
      }
    }
    return rtn;
  }

  int GetElemInBetween(const vector<int>& first_index, int start, int end) {
    for (int i = 0; i < 26; i++) {
      if (first_index[i] <= end && first_index[i] >= start) {
        return i;
      }
    }
    return -1;
  }

 public:
  vector<int> partitionLabels(string S) {
    vector<int> last_index(26, -1);
    vector<int> first_index(26, -1);
    for (int i = 0; i < S.size(); i++) {
      if (first_index[S[i] - 'a'] == -1) {
        first_index[S[i] - 'a'] = i;
        last_index[S[i] - 'a'] = i;
      } else {
        last_index[S[i] - 'a'] = i;
      }
    }
    vector<int> rtn;
    while (GetMinOcc(first_index) != -1) {
      int i = GetMinOcc(first_index);
      int start = first_index[i];
      first_index[i] = -1;
      int end = last_index[i];
      while (GetElemInBetween(first_index, start, end) != -1) {
        int e = GetElemInBetween(first_index, start, end);
        first_index[e] = -1;
        end = max(end, last_index[e]);
      }
      rtn.push_back(end - start + 1);
    }
    return rtn;
  }
};