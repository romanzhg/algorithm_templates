/**
 *
 * Increasing stack with a little modification.
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

using namespace std;


class Solution {
 public:
  struct Elem {
    int value, index;
    Elem(int value, int index) : value(value), index(index) {};
  };

  int validSubarrays(vector<int>& nums) {
    deque<Elem> dq;
    int rtn = 0;
    for (int i = 0; i < (int)nums.size(); i++) {
      int cur_value = nums[i];
      Elem cur_elem{nums[i], i};
      if (dq.empty()) {
        dq.push_back(cur_elem);
      } else {
        if (cur_value >= dq.back().value) {
          dq.push_back(cur_elem);
        } else {
          while (!dq.empty() && (cur_value < dq.back().value)) {
            rtn += i - dq.back().index;
            dq.pop_back();
          }
          dq.push_back(cur_elem);
        }
      }
    }

    while (!dq.empty()) {
      rtn += dq.back().index - dq.front().index + 1;
      dq.pop_front();
    }

    return rtn;
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
    vector<int> nums = stringToIntegerVector(line);

    int ret = Solution().validSubarrays(nums);

    string out = to_string(ret);
    cout << out << endl;
  }
  return 0;
}