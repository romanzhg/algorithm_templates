/**
 * Record the count of each number, and a count of counts, also the max number in the count array.
 * The maximum equal frequency length after remove 1 element happens when:
 *   1. All numbers appears once(max_count is 1).
 *   2. All numbers appears max_count times, except one appears 1 time.
 *   3. All numbers appears (max_count -1) times, except one appears max_count time.
 */
#include "common.h"

class Solution {
 public:
  int maxEqualFreq(vector<int>& nums) {
    vector<int> count(100001, 0), count_freq(100001, 0);
    int max_count = 0;
    int rtn = 0;
    for (int i = 0; i < (int) nums.size(); i++) {
      count[nums[i]]++;
      count_freq[count[nums[i]] - 1]--;
      count_freq[count[nums[i]]]++;

      max_count = max(max_count, count[nums[i]]);
      if (max_count == 1) {
        rtn = max(rtn, i + 1);
      }
      if (max_count * count_freq[max_count] + 1 == i + 1) {
        rtn = max(rtn, i + 1);
      }
      if (max_count + (max_count - 1) * count_freq[(max_count - 1)] == i + 1) {
        rtn = max(rtn, i + 1);
      }
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

    int ret = Solution().maxEqualFreq(nums);

    string out = to_string(ret);
    cout << out << endl;
  }
  return 0;
}