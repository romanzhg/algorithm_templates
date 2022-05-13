/**
 * The point for this problem is, get the sum of current element
 * and the largest value in the remaining array.
 *
 * Solution works, note the way to access the last element in a multiset.
 *
 * Solution1 is better, it uses increasing stack to keep track of the largest value in future.
 *
 * Increasing stack: keep the largest value in a single ended range/stack.
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
#include <stack>

using namespace std;

class Solution1 {
 public:
  int maxScoreSightseeingPair(vector<int>& A) {
    stack<int> increasing_stack;
    for (int i = ((int)A.size()) - 1; i >= 0; i--) {
      int cur_value = A[i] - i;
      if (increasing_stack.empty()) {
        increasing_stack.push(cur_value);
      } else {
        if (cur_value >= increasing_stack.top()) {
          increasing_stack.push(cur_value);
        }
      }
    }

    int max_scr = -1;
    for (int i = 0; i < (((int)A.size()) - 1); i++) {
      int cur_value = A[i] - i;
      if (increasing_stack.top() == cur_value) {
        increasing_stack.pop();
      }
      max_scr = max(max_scr, A[i] + i + increasing_stack.top());
    }

    return max_scr;
  }
};

class Solution {
 public:
  int maxScoreSightseeingPair(vector<int>& A) {
    multiset<int> adj_with_dist;
    for (int i = 0; i < (int)A.size(); i++) {
      adj_with_dist.insert(A[i] - i);
    }

    int max_scr = -1;
    for (int i = 0; i < (((int)A.size()) - 1); i++) {
      adj_with_dist.erase(adj_with_dist.find(A[i] - i));
      max_scr = max(max_scr, A[i] + i + *(adj_with_dist.rbegin()));
    }

    return max_scr;
  }
};