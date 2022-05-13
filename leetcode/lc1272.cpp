/**
 * Note the conditions. And always use '<' and '<=' for comparison.
 */

#include "common.h"

class Solution {
 public:
  vector<vector<int>> removeInterval(vector<vector<int>>& intervals, vector<int>& toBeRemoved) {
    vector<vector<int>> rtn;
    int r_start = toBeRemoved[0];
    int r_end = toBeRemoved[1];

    for (const vector<int>& interval : intervals) {
      int start = interval[0];
      int end = interval[1];
      if (end <= r_start || r_end <= start) {
        rtn.push_back(interval);
      } else if (r_start <= start && end <= r_end) {
        ;
      } else if (start < r_start && r_end < end) {
        rtn.push_back({start, r_start});
        rtn.push_back({r_end, end});
      } else {
        if (r_start <= start && start < r_end) {
          rtn.push_back({r_end, end});
        } else {
          rtn.push_back({start, r_start});
        }
      }
    }
    return rtn;
  }
};