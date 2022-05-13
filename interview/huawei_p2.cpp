//题目2(难度★★★)：
//
//         题目描述:
//
//         给你 m 个整数，你可以按照一定的顺序将他们首尾相连构成一个新的组合数字。请输出值最大的组合数字。如整数1，23，组合起来有两种情况，123和231，因为231 > 123，所以231是答案。
//
//
//
//         输入:
//
//                   整数数组元素个数，整数数组
//
//
//
//         输出:
//
//           最大的组合数的字符串
//
//
//
//         样例:
//
//                   输入
//
//                   3, [123, 456, 78]
//
//
//
//                   输出
//
//                   "78456123"
/**
 *
 * Greedy, treat integers as string, compared combined strings, append larger ones to the result first.
 *
 * Test cases(should be treated as a single line input string):
 *   3, [123, 456, 78]
 *   4, [911,78,456,8]
 *   5, [5, 534, 53, 2, 8]
 *   5, [5, 536, 53, 2, 8]
 */
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

int main() {
  string input_line;
  getline(cin, input_line);

  vector<string> numbers;
  numbers.push_back("");

  for (char c : input_line) {
    if (isdigit(c)) {
      numbers.back() += c;
    } else if (c == ',') {
      numbers.push_back("");
    } else {
      // Do nothing.
      ;
    }
  }

  numbers.erase(numbers.begin());

  sort(numbers.begin(), numbers.end(), [] (const string& l, const string& r) {
      string combined_l = l + r;
      string combined_r = r + l;
      return combined_l < combined_r;
    });

  reverse(numbers.begin(), numbers.end());
  string result = "";
  for (string v : numbers) {
    result += v;
  }

  cout << result << endl;
  return 0;
}