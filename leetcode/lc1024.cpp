/**
 * Interval cover, greedy.
 * Each round remove impossible intervals and chose the one with farest end.
 * All solutions requires the input clips to be sorted(O(nlogn)), so only the time consumed by
 * the other part is compared here.
 *
 * Solution use vector to store all the intervals. The problem is that the vector erase operation
 * may be time consuming(each removal may take n, because the afterwards vectors will be shifted),
 * so the overall time complexity will be O(n ^ 2).
 *
 * Solution1 use set to store all the intervals. Each removal takes logn, so the overall time
 * complexity will be O(nlogn).
 *
 * Solution2 use two pointers, the overall time complexity is O(n).
 */
#include "common.h"

class Solution {
 public:
  int videoStitching(vector<vector<int>>& clips, int T) {
    int cur_start_time = 0;
    sort(clips.begin(), clips.end());

    int count = 0;
    while (cur_start_time < T && !clips.empty()) {
      for (int i = clips.size() - 1; i >= 0; i--) {
        if (clips[i][1] <= cur_start_time) {
          clips.erase(clips.begin() + i);
        } else {
          clips[i][0] = max(clips[i][0], cur_start_time);
        }
      }

      int cur_end_time = -1;
      for (const vector<int>& interval : clips) {
        if (interval[0] == cur_start_time) {
          cur_end_time = max(cur_end_time, interval[1]);
        } else {
          break;
        }
      }
      if (cur_end_time == -1) {
        return -1;
      } else {
        cur_start_time = cur_end_time;
        count++;
      }
    }

    return cur_start_time >= T ? count : -1;
  }
};



class Solution1 {
 public:
  int videoStitching(vector<vector<int>>& clips, int T) {
    int cur_start_time = 0;
    set<vector<int>> sorted_clips(clips.begin(), clips.end());

    int count = 0;
    while (cur_start_time < T && !sorted_clips.empty()) {
      for (auto it = sorted_clips.begin(); it != sorted_clips.end();) {
        if ((*it)[1] <= cur_start_time) {
          it = sorted_clips.erase(it);
        } else if ((*it)[0] > cur_start_time) {
          break;
        } else {
          it++;
        }
      }

      int cur_end_time = -1;
      for (const vector<int>& interval : sorted_clips) {
        if (interval[0] <= cur_start_time) {
          cur_end_time = max(cur_end_time, interval[1]);
        } else {
          break;
        }
      }
      if (cur_end_time == -1) {
        return -1;
      } else {
        cur_start_time = cur_end_time;
        count++;
      }
    }

    return cur_start_time >= T ? count : -1;
  }
};


class Solution2 {
 public:
  int videoStitching(vector<vector<int>>& clips, int T) {
    int cur_start_time = 0;
    sort(clips.begin(), clips.end());

    int count = 0;
    int start_ptr = 0;
    while (cur_start_time < T && start_ptr < clips.size()) {
      int end_ptr = start_ptr;
      for (; end_ptr < clips.size(); end_ptr++) {
        if (clips[end_ptr][0] > cur_start_time) {
          break;
        }
      }

      int cur_end_time = cur_start_time;
      for (; start_ptr < end_ptr; start_ptr++) {
        cur_end_time = max(cur_end_time, clips[start_ptr][1]);
      }

      // This condition can be removed, and change to
      // if (end_ptr == start_ptr) {
      //   return -1;
      // }
      // after the first for loop which updates end_ptr.
      // start_ptr and end_ptr equals means there is no valid interval to cover the start.
      if (cur_end_time == cur_start_time) {
        return -1;
      }
      cur_start_time = cur_end_time;
      start_ptr = end_ptr;
      count++;
    }

    return cur_start_time >= T ? count : -1;
  }
};

