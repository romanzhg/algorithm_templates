/**
 * Store a match in a vector, and try every start position.
 * A position in match[] needs to be recalculated only when a previous search goes over it, otherwise
 * the result can be reused.
 */
#include "common.h"

class Solution {
 public:
  bool Match(const string& S, const string& T, vector<int>& match) {
    int count = 0;
    int start = match[0] + 1;
    while (count < T.size()) {
      if (match[count] < start) {
        int i = start;
        for (; i < S.size(); i++) {
          if (T[count] == S[i]) {
            match[count] = i;
            break;
          }
        }
        if (i == S.size()) {
          return false;
        }
        count++;
        start = i + 1;

      } else {
        return true;
      }
    }
    return count == T.size();
  }

  string minWindow(string S, string T) {
    vector<int> match(T.size(), -1);

    int seq_len = INT_MAX;
    int min_start_point = -1;

    while (true) {
      if (!Match(S, T, match)) {
        break;
      } else {
        int tmp_seq_len = match.back() - match.front() + 1;
        if (tmp_seq_len < seq_len) {
          seq_len = tmp_seq_len;
          min_start_point = match.front();
        }
      }
    }

    if (seq_len < INT_MAX) {
      return S.substr(min_start_point, seq_len);
    } else {
      return "";
    }
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
    string S = stringToString(line);
    getline(cin, line);
    string T = stringToString(line);

    string ret = Solution().minWindow(S, T);

    string out = (ret);
    cout << out << endl;
  }
  return 0;
}