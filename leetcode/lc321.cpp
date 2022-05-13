/**
 * Not finished. Look at the solution.
 */
#include "common.h"

struct Elem {
  int a_start, b_start, k;
  Elem(int a_start, int b_start, int k) : a_start(a_start), b_start(b_start), k(k) {};
  bool operator<(const Elem& o) const {
    if (a_start != o.a_start) {
      return a_start < o.a_start;
    }
    if (b_start != o.b_start) {
      return b_start < o.b_start;
    }
    return k < o.k;
  }
};

class Solution {
 public:
  map<Elem, vector<int>> dp;
  vector<int> helper(vector<int>& a, vector<int>& b, int a_start, int b_start, int k) {
    if (k == 0) {
      return {};
    }

    Elem key(a_start, b_start, k);
    if (dp.find(key) != dp.end()) {
      return dp[key];
    }
    int a_size = a.size() - a_start;
    int b_size = b.size() - b_start;

    int to_ignored = a_size + b_size - k;
    int max_digit_a = -1, index_a = a_start,
        max_digit_b = -1, index_b = b_start;

    if (a_start < a.size()) {
      max_digit_a = a[a_start];
      index_a = a_start;
      for (int i = a_start; i <= a_start + to_ignored && i < a.size(); i++) {
        if (a[i] > max_digit_a) {
          max_digit_a = a[i];
          index_a = i;
        }
      }
    }
    if (b_start < b.size()) {
      max_digit_b = b[b_start];
      index_b = b_start;
      for (int i = b_start; i <= b_start + to_ignored && i < b.size(); i++) {
        if (b[i] > max_digit_b) {
          max_digit_b = b[i];
          index_b = i;
        }
      }
    }

    if (max_digit_a > max_digit_b) {
      vector<int> rtn;
      rtn.push_back(max_digit_a);
      auto tmp = helper(a, b, index_a + 1, b_start, k - 1);
      rtn.insert(rtn.end(), tmp.begin(), tmp.end());
      dp[key] = rtn;
      return rtn;
    } else if (max_digit_a < max_digit_b) {
      vector<int> rtn;
      rtn.push_back(max_digit_b);
      auto tmp = helper(a, b, a_start, index_b + 1, k - 1);
      rtn.insert(rtn.end(), tmp.begin(), tmp.end());
      dp[key] = rtn;
      return rtn;
    } else {
      vector<int> rtn;
      rtn.push_back(max_digit_a);
      auto tmp_a = helper(a, b, index_a + 1, b_start, k - 1);
      auto tmp_b = helper(a, b, a_start, index_b + 1, k - 1);
      bool choose_a = true;
      for (int i = 0; i < k - 1; i++) {
        if (tmp_a[i] < tmp_b[i]) {
          choose_a = false;
          break;
        } else if (tmp_a[i] > tmp_b[i]) {
          break;
        }
      }
      if (choose_a) {
        rtn.insert(rtn.end(), tmp_a.begin(), tmp_a.end());
      } else {
        rtn.insert(rtn.end(), tmp_b.begin(), tmp_b.end());
      }
      dp[key] = rtn;
      return rtn;
    }
  }

  vector<int> maxNumber(vector<int>& nums1, vector<int>& nums2, int k) {
    dp.clear();
    return helper(nums1, nums2, 0, 0, k);
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

int stringToInteger(string input) {
  return stoi(input);
}

string integerVectorToString(vector<int> list, int length = -1) {
  if (length == -1) {
    length = list.size();
  }

  if (length == 0) {
    return "[]";
  }

  string result;
  for(int index = 0; index < length; index++) {
    int number = list[index];
    result += to_string(number) + ", ";
  }
  return "[" + result.substr(0, result.length() - 2) + "]";
}

int main() {
  string line;
  while (getline(cin, line)) {
    vector<int> nums1 = stringToIntegerVector(line);
    getline(cin, line);
    vector<int> nums2 = stringToIntegerVector(line);
    getline(cin, line);
    int k = stringToInteger(line);

    vector<int> ret = Solution().maxNumber(nums1, nums2, k);

    string out = integerVectorToString(ret);
    cout << out << endl;
  }
  return 0;
}