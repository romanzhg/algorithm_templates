/**
 * Similar to "select points to cover intervals"(sort on interval end).
 * Always select points to the end.
 * No need to remove duplicating intervals.
 *
 * Solution1 is less efficient, it is included just to show how to remove duplicate intervals.
 *
 * Side notes:
 * "select non overlapping intervals" (sort on interval end).
 * "select intervals to cover another interval" (sort on interval start).
 */
#include "common.h"

struct Interval {
  int start, end;
  Interval(int start, int end) : start(start), end(end) {};
  bool operator<(const Interval& o) const {
    return end < o.end;
  }
  bool InRange(int i) const {
    return i <= end && i >= start;
  }
};

class Solution {
 public:
  int intersectionSizeTwo(vector<vector<int>>& intervals) {
    vector<Interval> tmp;
    for (const vector<int>& i : intervals) {
      tmp.push_back({i[0], i[1]});
    }
    sort(tmp.begin(), tmp.end());

    set<int> point_set {tmp[0].end - 1, tmp[0].end};
    for (const Interval& i : tmp) {
      int last = *(point_set.rbegin());
      int prev_last = *(next(point_set.rbegin()));

      int elems = 0;
      if (i.InRange(last)) {
        elems++;
      }
      if (i.InRange(prev_last)) {
        elems++;
      }
      if (elems >= 2) {
        continue;
      } else if (elems == 1) {
        if (point_set.find(i.end) != point_set.end()) {
          point_set.insert(i.end - 1);
        } else {
          point_set.insert(i.end);
        }
      } else {
        point_set.insert(i.end);
        point_set.insert(i.end - 1);
      }
    }

    return point_set.size();
  }
};

class Solution1 {
 public:
  int intersectionSizeTwo(vector<vector<int>>& intervals) {
    vector<Interval> tmp;
    for (const vector<int>& i : intervals) {
      tmp.push_back({i[0], i[1]});
    }
    sort(tmp.begin(), tmp.end());

    while (true) {
      bool found_inclusion = false;
      for (int i = 0; i < tmp.size() - 1; i++) {
        if (tmp[i].start >= tmp[i + 1].start) {
          tmp.erase(tmp.begin() + i + 1);
          found_inclusion = true;
          break;
        }
      }
      if (!found_inclusion) {
        break;
      }
    }


    set<int> point_set;
    for (const Interval& i : tmp) {
      auto lb = lower_bound(point_set.begin(), point_set.end(), i.start);
      auto ub = upper_bound(point_set.begin(), point_set.end(), i.end);
      int elems = distance(lb, ub);
      if (elems >= 2) {
        continue;
      } else if (elems == 1) {
        if (point_set.find(i.end) != point_set.end()) {
          point_set.insert(i.end - 1);
        } else {
          point_set.insert(i.end);
        }
      } else {
        point_set.insert(i.end);
        point_set.insert(i.end - 1);
      }
    }

    return point_set.size();
  }
};