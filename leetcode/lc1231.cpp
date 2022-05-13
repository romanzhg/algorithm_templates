/**
 * Solution1 is dp, it is correct, but TLE.
 *
 * Note that when testing Solution1, one test case has more than 5000 chars, and the system will trunc it
 * to 4096 chars, so the c++ program receive only part of the input, and always gives a wrong result.
 * This also happens to java.
 * This happens only when invoking the program from command line in a way like:
 * $> ./a.out {paste the input}
 * A simple fix would be safe the input in a file and redirect the file.
 * $> ./a.out < input_file
 *
 * Solution is based on binary search. Note the average is lean to above, and [min_v, max_v](both inclusive)
 * is the valid range. Should think of when we should have the average lean to above and when we should
 * have it lean to below. Note this is different from finding an insertion position.
 *
 */
#include "common.h"

class Solution1 {
 public:
  int maximizeSweetness(vector<int>& sweetness, int K) {
    int sweetness_size = sweetness.size();
    vector<int> sums(sweetness_size + 1);
    sums[0] = 0;
    for (int i = 0; i < sweetness_size; i++) {
      sums[i + 1] = sums[i] + sweetness[i];
    }

    vector<vector<int>> dp(sweetness_size, vector<int>(K + 2, 0));

    for (int i = 0; i < sweetness_size; i++) {
      dp[i][1] = sums[i + 1];
    }

    for (int k = 2; k <= K + 1; k++) {
      for (int i = 0; i < sweetness_size; i++) {
        if (k > i + 1) {
          continue;
        }
        dp[i][k] = 0;

        for (int l = 0; l < i; l++) {
          if (k - 1 > l + 1) {
            continue;
          }
          if (sums[i + 1] - sums[l + 1] < dp[i][k]) {
            break;
          }
          dp[i][k] = max(dp[i][k], min(dp[l][k - 1], sums[i + 1] - sums[l + 1]));
        }
      }
    }

    return dp[sweetness_size - 1][K + 1];
  }
};

class Solution {
 public:
  int maximizeSweetness(vector<int>& sweetness, int K) {
    int sum_v = 0;
    for (int s : sweetness) {
      sum_v += s;
    }

    int min_v = 1, max_v = (sum_v / (K + 1));
    while (min_v < max_v) {
      int mid = (max_v + min_v + 1) / 2;

      int count = 0;
      int groups = 1;
      for (int s : sweetness) {
        count += s;
        if (count >= mid) {
          count = 0;
          groups++;
          if (groups > (K + 1)) {
            break;
          }
        }
      }

      if (groups > (K + 1)) {
        min_v = mid;
      } else {
        max_v = mid - 1;
      }
    }
    return min_v;
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

int main() {
  string line;
  while (getline(cin, line)) {
    vector<int> sweetness = stringToIntegerVector(line);
    getline(cin, line);
    int K = stringToInteger(line);

    int ret = Solution().maximizeSweetness(sweetness, K);

    string out = to_string(ret);
    cout << out << endl;
  }
  return 0;
}