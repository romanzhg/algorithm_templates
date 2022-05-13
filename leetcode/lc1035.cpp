/**
 * This problem is essentially LCS(longest common subsequence).
 *
 * Solution1 is what I can think of, it utilize the fact that the matching should be greedy.
 * For a number in A, it should go with the first match in B.
 *
 * Solution uses the LCS, note the transition is also greedy, it the last two chars are a match,
 * always take the match.
 *
 * Solution can be implemented in the reversed order, that dp[i][j] means the LCS between the first
 * i chars in A and the first j chars in B(in this case dp[0][*] and dp[*][0] make sense, for
 * example, the first or second array is empty), and the search order would be
 * i = 1 -> len, j = 1 -> len.
 */
#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <set>
#include <utility>
#include <map>
#include <unordered_map>
#include <cstring>

using namespace std;

class Solution1 {
 public:
  int helper(int a, int b, const vector<int>& A, const vector<int>& B) {
    int a_limit = A.size();
    int b_limit = B.size();
    if (a >= a_limit || b >= b_limit) {
      return 0;
    }
    if (containsKey(a, b)) {
      return dp[a][b];
    }

    int rtn = 0;
    for (int tmp_a = a; tmp_a < a_limit; tmp_a++) {
      for (int tmp_b = b; tmp_b < b_limit; tmp_b++) {
        if (A[tmp_a] == B[tmp_b]) {
          rtn = max(rtn, helper(tmp_a + 1, tmp_b + 1, A, B) + 1);
          break;
        }
      }
    }
    dp[a][b] = rtn;
    return rtn;
  }

  int maxUncrossedLines(vector<int>& A, vector<int>& B) {
    dp.clear();
    return helper(0, 0, A, B);
  }

  bool containsKey(int i, int j) {
    return dp.find(i) != dp.end() && dp[i].find(j) != dp[i].end();
  }

  map<int, map<int, int>> dp;
};

class Solution {
 public:
  int helper(int a, int b, const vector<int>& A, const vector<int>& B) {
    if (a < 0 || b < 0) {
      return 0;
    }
    if (containsKey(a, b)) {
      return dp[a][b];
    }

    int rtn;
    if (A[a] == B[b]) {
      rtn = helper(a - 1, b - 1, A, B) + 1;
    } else {
      rtn = max(helper(a - 1, b, A, B), helper(a, b - 1, A, B));
    }
    dp[a][b] = rtn;
    return rtn;
  }

  int maxUncrossedLines(vector<int>& A, vector<int>& B) {
    return helper(A.size() - 1, B.size() - 1, A, B);
  }

  bool containsKey(int i, int j) {
    return dp.find(i) != dp.end() && dp[i].find(j) != dp[i].end();
  }

  unordered_map<int, unordered_map<int, int>> dp;
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
    vector<int> A = stringToIntegerVector(line);
    getline(cin, line);
    vector<int> B = stringToIntegerVector(line);

    int ret = (new Solution())->maxUncrossedLines(A, B);

    string out = to_string(ret);
    cout << out << endl;
  }
  return 0;
}