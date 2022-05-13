/**
 *
 * This problem is essentially recording path in a dp.
 *
 * Solution shows it is possible to use global variable(a matrix of 1000 * 1000) in leetcode.
 *
 * Solution1 improves the calculation order.
 *
 * There is another solution that is based on Solution1 order and build the rcs string in the dp
 * directly.
 *
 */
#include <algorithm>
#include <cfloat>
#include <cmath>
#include <cstring>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>
#include <queue>
#include <deque>
#include <cassert>

using namespace std;

/*****************************************************************************/
int dp[1010][1010];

class Solution {
 public:
  map<pair<int, int>, pair<int, int>> prev;

  int GetLcs(int i, int j, const string& str1, const string& str2) {
    if (i == 0 || j == 0) {
      return 0;
    }
    if (dp[i][j] > 0) {
      return dp[i][j];
    }

    int rtn;
    if (str1[i - 1] == str2[j - 1]) {
      prev[{i, j}] = {i - 1, j - 1};
      rtn = GetLcs(i - 1, j - 1, str1, str2) + 1;
    } else {
      int a = GetLcs(i, j - 1, str1, str2);
      int b = GetLcs(i - 1, j, str1, str2);
      if (a > b) {
        prev[{i, j}] = {i, j - 1};
      } else {
        prev[{i, j}] = {i - 1, j};
      }
      rtn = max(a, b);
    }
    dp[i][j] = rtn;
    return rtn;
  }

  vector<pair<int, int>> GetMatches(int i, int j, const string& str1, const string& str2) {
    vector<pair<int, int>> rtn;
    while (i > 0 && j > 0) {
      if (str1[i - 1] == str2[j - 1]) {
        rtn.push_back({i - 1, j - 1});
      }
      pair<int, int> tmp = prev[{i, j}];
      i = tmp.first;
      j = tmp.second;
    }

    reverse(rtn.begin(), rtn.end());
    return rtn;
  }

  string shortestCommonSupersequence(string str1, string str2) {
    prev.clear();
    memset(dp, 0, sizeof(dp));

    int tmp = GetLcs(str1.size(), str2.size(), str1, str2);
    vector<pair<int, int>> matches = GetMatches(str1.size(), str2.size(), str1, str2);

    string rtn;
    int start_index_1 = 0;
    int start_index_2 = 0;
    for (const pair<int, int>& v : matches) {
      while (start_index_1 < v.first) {
        rtn += str1[start_index_1];
        start_index_1++;
      }
      start_index_1 = v.first + 1;

      while (start_index_2 < v.second) {
        rtn += str2[start_index_2];
        start_index_2++;
      }
      start_index_2 = v.second + 1;
      rtn += str1[v.first];
    }
    while (start_index_1 < str1.size()) {
      rtn += str1[start_index_1++];
    }
    while (start_index_2 < str2.size()) {
      rtn += str2[start_index_2++];
    }

    return rtn;
  }
};

/*****************************************************************************/
class Solution1 {
 public:
  map<pair<int, int>, pair<int, int>> prev;

  vector<pair<int, int>> GetMatches(int i, int j, const string& str1, const string& str2) {
    vector<pair<int, int>> rtn;
    while (i > 0 && j > 0) {
      if (str1[i - 1] == str2[j - 1]) {
        rtn.push_back({i - 1, j - 1});
      }
      pair<int, int> tmp = prev[{i, j}];
      i = tmp.first;
      j = tmp.second;
    }

    reverse(rtn.begin(), rtn.end());
    return rtn;
  }

  string shortestCommonSupersequence(string str1, string str2) {
    prev.clear();

    // Allocation a matrix as vector won't exceed the memory limit.
    vector<vector<int>> dp(str1.size() + 1, vector<int>(str2.size() + 1, 0));

    for (int i = 1; i <= (int)str1.size(); i++) {
      for (int j = 1; j <= (int)str2.size(); j++) {
        if (str1[i - 1] == str2[j - 1]) {
          dp[i][j] = dp[i - 1][j - 1] + 1;
          prev[{i, j}] = {i - 1, j - 1};
        } else {
          dp[i][j] = max(dp[i][j - 1], dp[i - 1][j]);
          prev[{i, j}] = (dp[i][j - 1] > dp[i - 1][j])
              ? make_pair(i, j - 1) : make_pair(i - 1, j);
        }
      }
    }
    vector<pair<int, int>> matches = GetMatches(str1.size(), str2.size(), str1, str2);

    string rtn;
    int start_index_1 = 0;
    int start_index_2 = 0;
    for (const pair<int, int>& v : matches) {
      while (start_index_1 < v.first) {
        rtn += str1[start_index_1];
        start_index_1++;
      }
      start_index_1 = v.first + 1;

      while (start_index_2 < v.second) {
        rtn += str2[start_index_2];
        start_index_2++;
      }
      start_index_2 = v.second + 1;
      rtn += str1[v.first];
    }
    while (start_index_1 < str1.size()) {
      rtn += str1[start_index_1++];
    }
    while (start_index_2 < str2.size()) {
      rtn += str2[start_index_2++];
    }

    return rtn;
  }
};

string stringToString(string input) {
  assert(input.length() >= 2);
  string result;
  for (int i = 1; i < input.length() -1; i++) {
    char currentChar = input[i];
    if (input[i] == '\\') {
      char nextChar = input[i+1];
      switch (nextChar) {
        case '\"': result.push_back('\"'); break;
        case '/' : result.push_back('/'); break;
        case '\\': result.push_back('\\'); break;
        case 'b' : result.push_back('\b'); break;
        case 'f' : result.push_back('\f'); break;
        case 'r' : result.push_back('\r'); break;
        case 'n' : result.push_back('\n'); break;
        case 't' : result.push_back('\t'); break;
        default: break;
      }
      i++;
    } else {
      result.push_back(currentChar);
    }
  }
  return result;
}

int main() {
  string line;
  while (getline(cin, line)) {
    string str1 = stringToString(line);
    getline(cin, line);
    string str2 = stringToString(line);

    string ret = Solution().shortestCommonSupersequence(str1, str2);

    string out = (ret);
    cout << out << endl;
  }
  return 0;
}