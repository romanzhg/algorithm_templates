// 题目3(难度★★★)：
//
//         题目描述:
//
//         有两个序列a,b，大小都为n,序列元素的值任意整数，无序；
//
//         要求：通过交换a,b中的元素，个数不变，使[序列a元素的和]与[序列b元素的和]之间的差最小。
//
//
//
//         输入:
//
//                   整数数组元素个数，整数数组a, 整数数组b
//
//
//
//         输出:
//
//           交换后的[序列a元素的和]与[序列b元素的和]之间的差值
//
//
//
//         样例:
//
//                   输入
//
//                   6, [100, 99, 98, 1, 2, 3], [1, 2, 3, 4, 5, 40]
//
//
//
//                   输出
//
//                   48
/**
 * This seems to be an NP-Hard problem. Find the exact answer by examine every
 * node in solution tree, run time is O(n ^ 2).
 *
 * One pruning is used: look at the suffix sum, if take all the remaining value cannot give a
 * better result, stop the search.
 *
 * Test case(should be treated as a single line input string):
 *   6, [100, 99, 98, 1, 2, 3], [1, 2, 3, 4, 5, 40]
 *   4, [5, 5, 9, 10], [4, 7, 7, 13]
 */

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
#include <climits>

using namespace std;

int min_diff = INT_MAX;
int total_sum = 0;
int target;
int number_count;

void dfs(const vector<int>& numbers, const vector<int>& suffix_sum, int cur_sum, int cur_index, int taken) {
  if (cur_sum > target) {
    return;
  }
  if (taken == number_count) {
    min_diff = min(min_diff, total_sum - 2 * cur_sum);
    return;
  }

  if (cur_index > numbers.size()) {
    return;
  }

  // A pruning, if take all the remaining values cannot give a better result, stop the search.
  int tmp_sum = cur_sum + suffix_sum[cur_index];
  if (tmp_sum <= target && (total_sum - 2 * tmp_sum >= min_diff)) {
    return;
  }

  // Take.
  dfs(numbers, suffix_sum, cur_sum + numbers[cur_index], cur_index + 1, taken + 1);
  // Do not take.
  dfs(numbers, suffix_sum, cur_sum, cur_index + 1, taken);
}

int main() {
  string input_line;
  getline(cin, input_line);

  vector<int> numbers;
  numbers.push_back(0);

  for (char c : input_line) {
    if (isdigit(c)) {
      numbers.back() = numbers.back() * 10 + (c - '0');
    } else if (c == ',') {
      numbers.push_back(0);
    } else {
      // Do nothing.
      ;
    }
  }

  numbers.erase(numbers.begin());

  sort(numbers.begin(), numbers.end());
  reverse(numbers.begin(), numbers.end());

  for (int v : numbers) {
    total_sum += v;
  }
  target = total_sum / 2;
  number_count = numbers.size() / 2;
  vector<int> suffix_sum(numbers.size(), 0);
  suffix_sum.back() = numbers.back();
  for (int i = numbers.size() - 2; i >= 0; i--) {
    suffix_sum[i] = numbers[i] + suffix_sum[i + 1];
  }

  dfs(numbers, suffix_sum, numbers[0], 1, 1);

  cout << min_diff << endl;
  return 0;
}