/**
 * Sorter the input, so interval with smaller start time will have a smaller index.
 * Process interval start and interval end events.
 * Current pending intervals are stored in a set.
 * If the current ending interval is not the first interval in the pending set, it is included by some other intervals.
 * Note the comparator implemented for type Event, if two events has same value, the one with larger id should be
 * processed first, this corresponds to the situation that two intervals end at the same time.
 *
 * Solution1 sorts the intervals on left bound, and only counts for increasing right
 * bound(if right bound is not increasing, then it is covered).
 */
#include "common.h"

struct Event {
  // type: 0 for start, 1 for end.
  int id, value, type;
  Event(int id, int value, int type) : id(id), value(value), type(type) {};

  bool operator<(const Event& o) const {
    if (value == o.value) {
      return id > o.id;
    }
    return value < o.value;
  }

};

class Solution {
 public:
  int removeCoveredIntervals(vector<vector<int>>& intervals) {
    sort(intervals.begin(), intervals.end());

    vector<Event> events;
    for (int i = 0; i < intervals.size(); i++) {
      events.push_back(Event(i, intervals[i][0], 0));
      events.push_back(Event(i, intervals[i][1], 1));
    }

    sort(events.begin(), events.end());

    set<int> pending_events;
    int rtn = 0;
    for (const Event& e : events) {
      if (e.type == 0) {
        pending_events.insert(e.id);
      } else {
        if (*pending_events.begin() != e.id) {
          rtn++;
        }
        pending_events.erase(e.id);
      }
    }
    return intervals.size() - rtn;
  }
};

class Solution1 {
 public:
  int removeCoveredIntervals(vector<vector<int>>& intervals) {
    sort(intervals.begin(), intervals.end());

    int rtn = 0, right = -1;
    for (const vector<int>& i : intervals) {
      if (right < i[1]) {
        rtn++;
        right = i[1];
      }
    }

    return rtn;
  }
};