
#include "common.h"

class Solution {
 public:
  int findJudge(int N, vector<vector<int>>& trust) {
    set<int> trust_others;
    vector<int> trust_count(N + 1, 0);

    for (const vector<int>& t : trust) {
      trust_others.insert(t[0]);
      trust_count[t[1]]++;
    }

    for (int i = 1 ; i <= N; i++) {
      if (trust_count[i] == (N - 1)
          && trust_others.find(i) == trust_others.end()) {
        return i;
      }
    }
    return -1;
  }
};