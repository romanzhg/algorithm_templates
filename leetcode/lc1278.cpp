/**
 * In the first loop, longer len depends on a shorter len.
 * In the second loop, higher i,j depends on lower i, j.
 * pd[i][j] : min changes to make string [i, j) a palindrome.
 * dp[i][j] : min changes to make string [0, i] with k split a palindrome.
 */
#include "common.h"

class Solution {
 public:
  int palindromePartition(string s, int k) {
    int str_len = s.size();
    vector<vector<int>> pd(101, vector<int>(101, 0));
    vector<vector<int>> dp(101, vector<int>(101, 0));

    // Iterate over len first, longer len depends on a shorter len.
    for (int len = 2; len <= str_len; len++) {
      for (int i = 0; i + len <= str_len; i++) {
        if (s[i] == s[i + len - 1]) {
          pd[i][i + len] = pd[i + 1][i + len - 1];
        } else {
          pd[i][i + len] = pd[i + 1][i + len - 1] + 1;
        }
      }
    }

    for (int i = 0; i < str_len; i++) {
      dp[i][1] = pd[0][i + 1];
    }

    for (int i = 0; i < str_len; i++) {
      for (int j = 2; j <= k; j++) {
        if (i + 1 < j) {
          // i + 1 is the string length, it won't make sense to have more splits than the length.
          continue;
        }
        dp[i][j] = pd[0][i + 1];
        for (int l = 0; l <= i - 1; l++) {
          dp[i][j] = min(dp[i][j], dp[l][j - 1] + pd[l + 1][i + 1]);
        }
      }
    }
    return dp[str_len - 1][k];
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

int stringToInteger(string input) {
  return stoi(input);
}

int main() {
  string line;
  while (getline(cin, line)) {
    string s = stringToString(line);
    getline(cin, line);
    int k = stringToInteger(line);

    int ret = Solution().palindromePartition(s, k);

    string out = to_string(ret);
    cout << out << endl;
  }
  return 0;
}