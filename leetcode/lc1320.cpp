/**
 * Full form: dp[i][p1][p2][0/1]: min cost of typed i chars, left hand at p1, right hand at p2, last used hand is
 * 0(left)/1(right). A sub problem with additional states.
 *
 * How to type optimally now depends on where the fingers are after typing last char(from optimal subproblem to
 * optimal current problem, the transition needs to be optimal). Need to record finger position.
 *
 * dp[i][p]: min cost of typed i chars, p for the position of the hand not used last time.
 *
 */

#include "common.h"

class Solution {
  vector<pair<int, int>> position = {
      {0, 0},
      {0, 1},
      {0, 2},
      {0, 3},
      {0, 4},
      {0, 5},

      {1, 0},
      {1, 1},
      {1, 2},
      {1, 3},
      {1, 4},
      {1, 5},

      {2, 0},
      {2, 1},
      {2, 2},
      {2, 3},
      {2, 4},
      {2, 5},

      {3, 0},
      {3, 1},
      {3, 2},
      {3, 3},
      {3, 4},
      {3, 5},

      {4, 0},
      {4, 1}
  };

  int GetDist(char a, char b) {
    return abs(position[a - 'A'].first - position[b - 'A'].first)
        + abs(position[a - 'A'].second - position[b - 'A'].second);
  }

 public:
  int minimumDistance(string word) {
    vector<vector<int>> dp(301, vector<int>(26, INT_MAX));
    for (int i = 0; i < 26; i++) {
      dp[1][i] = 0;
    }

    int rtn = INT_MAX;
    for (int i = 1; i < word.size(); i++) {
      for (int j = 0; j < 26; j++) {
        dp[i + 1][j] = min(dp[i + 1][j], dp[i][j] + GetDist(word[i], word[i - 1]));
        dp[i + 1][word[i - 1] - 'A'] = min(dp[i + 1][word[i - 1] - 'A'], dp[i][j] + GetDist(word[i], j + 'A'));
      }
    }

    for (int j = 0; j < 26; j++) {
      rtn = min(rtn, dp[word.size()][j]);
    }
    return rtn;
  }
};