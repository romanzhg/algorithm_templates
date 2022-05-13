/**
 * Notes:
 *   Solution1 is a plain search solution, which works.
 *
 *   Solution is a dp solution, but it is an dp solution with sparse dp matrix.
 *   dp[i][j] means the longest sequence end with index "i" with a step size "j".
 *   If for dp[i], key "j" doesn't exist, then the value is 1.
 *   The sparse dp matrix is represented by two level of unordered_map.
 *   Note using unordered_map is faster than map.
 *
 * Side notes:
 *   Solution1 won't work with the current structure, since it allocates a
 *   "int diff_matrix[2000][2000]" matrix with the object, which cannot fit in a
 *   stack and would result in SIGSEGV.
 *   Fixes are move "int diff_matrix[2000][2000]" out or new the Solution1 object so it uses the heap.
 *   Or use a vector of 2000 * 2000 size, a vector won't be a problem since it allocates the storage
 *   dynamically.
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
using namespace std;

class Solution1 {
 public:
  int GetDist(int i, int v, int limit) {
    for (int j = i + 1; j < limit; j++) {
      if (diff_matrix[i][j] == v) {
        return GetDist(j, v, limit) + 1;
      }
    }
    return 1;
  };

  int longestArithSeqLength(vector<int>& A) {
    for (int i = 0; i < (int) A.size(); i++) {
      for (int j = i; j < (int) A.size(); j++) {
        diff_matrix[i][j] = A[i] - A[j];
      }
    }
    int longest_dist = 0;
    for (int i = 0; i < (int) A.size(); i++) {
      if ((longest_dist + 1) >= (A.size() - i)) {
        break;
      }
      for (int j = i + 1; j < (int) A.size(); j++) {
        longest_dist = max(longest_dist, GetDist(j, diff_matrix[i][j], A.size()));
      }
    }
    return longest_dist + 1;
  };

 private:
  int diff_matrix[2000][2000];
};

class Solution {
 public:
  unordered_map<int, unordered_map<int, int>> dp;

  bool containsKey(int i, int step_size) {
    return dp.find(i) != dp.end() && dp[i].find(step_size) != dp[i].end();
  }

  int longestArithSeqLength(vector<int>& A) {
    if (A.size() <= 2) {
      return A.size();
    }

    int rtn = 0;
    for (int i = 1; i < (int) A.size(); i++) {
      for (int j = 0; j < i; j++) {
        int diff = A[i] - A[j];
        if (containsKey(j, diff)) {
          dp[i][diff] = dp[j][diff] + 1;
        } else {
          dp[i][diff] = 2;
        }
        rtn = max(rtn, dp[i][diff]);
      }
    }
    return rtn;
  };
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

    int ret = Solution().longestArithSeqLength(A);

    string out = to_string(ret);
    cout << out << endl;
  }
  return 0;
}