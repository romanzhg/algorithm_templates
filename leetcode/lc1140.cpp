/**
 * AlphaBeta search.
 * Solution1 works, it is based on search and save searched results, each call to "alphabeta" returns the result
 * of the subproblem.
 *
 * Solution implements alpha beta pruning, but TLEed.
 * Solution may be improved with saved results like in the case of Solution1, however the result needs to be
 * keyed by start, M, is_max, alpha, beta, prev.
 *
 * Solution2 is an example of wrong alpha beta pruning. In the alpha beta search, node need to have the value
 * to be maximized/minimized, but in this implementation, node value is value of the sub problem.
 *
 */
#include "common.h"

struct Key {
  int start, M;
  bool is_max;
  Key(int start, int M, bool is_max) : start(start), M(M), is_max(is_max) {};
  bool operator<(const Key &o) const {
    if (start != o.start) {
      return start < o.start;
    }
    if (M != o.M) {
      return M < o.M;
    }
    return (int) is_max < (int) o.is_max;
  }
};

class Solution1 {
  map<Key, int> dp;

  int alphabeta(const vector<int> &piles, int start, int M, bool is_max) {
    if (start >= (int) piles.size()) {
      return 0;
    }
    Key key(start, M, is_max);

    if (dp.find(key) != dp.end()) {
      return dp[key];
    }

    if (is_max) {
      int rtn = 0;
      int cummulator = 0;
      for (int l = 1; l <= 2 * M; l++) {
        if ((start + l - 1) >= piles.size()) {
          break;
        }
        cummulator += piles[start + l - 1];
        rtn = max(rtn, cummulator + alphabeta(piles, start + l, max(l, M), false));
      }
      dp[key] = rtn;
      return rtn;
    } else {
      int rtn = INT_MAX;
      for (int l = 1; l <= 2 * M; l++) {
        if ((start + l - 1) >= piles.size()) {
          break;
        }
        rtn = min(rtn, alphabeta(piles, start + l, max(l, M), true));
      }
      dp[key] = rtn;
      return rtn;
    }
  }

 public:
  int stoneGameII(vector<int> &piles) {
    dp.clear();
    return alphabeta(piles, 0, 1, true);
  }
};

class Solution2 {
  int alphabeta(const vector<int> &piles, int start, int M, bool is_max, int alpha, int beta, int depth) {
    for (int i = 0; i < depth * 2; i++) {
      cout << " ";
    }
    cout << alpha << " " << beta << " " << M << " start index: " << start << " is max: "<< is_max << endl;
    if (start >= (int) piles.size()) {
      for (int i = 0; i < depth * 2; i++) {
        cout << " ";
      }
      cout << "returned value: " << 0 << endl;
      return 0;
    }

    if (is_max) {
      int rtn = INT_MIN;
      int cummulator = 0;
      for (int l = 1; l <= (2 * M); l++) {
        if ((start + l - 1) >= (int) piles.size()) {
          break;
        }
        cummulator += piles[start + l - 1];
        rtn = max(rtn, cummulator + alphabeta(piles, start + l, max(l, M), false, alpha, beta, depth + 1));
        alpha = max(alpha, rtn);
        if (alpha >= beta) {
          break;
        }
      }
      for (int i = 0; i < depth * 2; i++) {
        cout << " ";
      }
      cout << "returned value: " << rtn << endl;

      return rtn;
    } else {
      int rtn = INT_MAX;
      for (int l = 1; l <= (2 * M); l++) {
        if ((start + l - 1) >= (int) piles.size()) {
          break;
        }
        rtn = min(rtn, alphabeta(piles, start + l, max(l, M), true, alpha, beta, depth + 1));
        beta = min(beta, rtn);
        if (alpha >= beta) {
          break;
        }
      }
      for (int i = 0; i < depth * 2; i++) {
        cout << " ";
      }
      cout << "returned value: " << rtn << endl;

      return rtn;
    }
  }

 public:
  int stoneGameII(vector<int> &piles) {
    return alphabeta(piles, 0, 1, true, INT_MIN, INT_MAX, 0);
  }
};


class Solution {
  int alphabeta(const vector<int> &piles, int start, int M, bool is_max, int alpha, int beta, int prev) {
    if (start >= (int) piles.size()) {
      return prev;
    }

    if (is_max) {
      int rtn = INT_MIN;
      int cummulator = 0;
      for (int l = 1; l <= (2 * M); l++) {
        if ((start + l - 1) >= (int) piles.size()) {
          break;
        }
        cummulator += piles[start + l - 1];
        rtn = max(rtn, alphabeta(piles, start + l, max(l, M), false, alpha, beta, cummulator + prev));
        alpha = max(alpha, rtn);
        if (alpha >= beta) {
          break;
        }
      }
      return rtn;
    } else {
      int rtn = INT_MAX;
      for (int l = 1; l <= (2 * M); l++) {
        if ((start + l - 1) >= (int) piles.size()) {
          break;
        }
        rtn = min(rtn, alphabeta(piles, start + l, max(l, M), true, alpha, beta, prev));
        beta = min(beta, rtn);
        if (alpha >= beta) {
          break;
        }
      }
      return rtn;
    }
  }

 public:
  int stoneGameII(vector<int> &piles) {
    return alphabeta(piles, 0, 1, true, INT_MIN, INT_MAX, 0);
  }
};


void trimLeftTrailingSpaces(string &input) {
  input.erase(input.begin(), find_if(input.begin(), input.end(), [](int ch) {
    return !isspace(ch);
  }));
}

void trimRightTrailingSpaces(string &input) {
  input.erase(find_if(input.rbegin(), input.rend(), [](int ch) {
    return !isspace(ch);
  }).base(), input.end());
}

vector<int> stringToIntegerVector(string input) {
  vector<int> output;
  trimLeftTrailingSpaces(input);
  trimRightTrailingSpaces(input);
  input = input.substr(1, input.length() - 2);
  stringstream ss;
  ss.str(input);
  string item;
  char delim = ',';
  while (getline(ss, item, delim)) {
    output.push_back(stoi(item));
  }
  return output;
}

int main() {
  string line;
  while (getline(cin, line)) {
    vector<int> piles = stringToIntegerVector(line);

    int ret = Solution().stoneGameII(piles);
    string out = to_string(ret);
    cout << out << endl;

    int ret1 = Solution1().stoneGameII(piles);
    string out1 = to_string(ret1);
    cout << out1 << endl;
  }
  return 0;
}