#include "common.h"

struct Event {
  // type: 0 for start, 1 for end. This program should process start before end.
  int time, type;
  Event(int time, int type) : time(time), type(type) {};
  bool operator<(const Event& o) const {
    if (time == o.time) {
      return type < o.type;
    }
    return time < o.time;
  }
};

class Solution {
 public:
  vector<vector<int>> intervalIntersection(vector<vector<int>>& A, vector<vector<int>>& B) {
    vector<Event> events;
    for (const vector<int>& va : A) {
      events.push_back({va[0], 0});
      events.push_back({va[1], 1});
    }
    for (const vector<int>& vb : B) {
      events.push_back({vb[0], 0});
      events.push_back({vb[1], 1});
    }
    sort(events.begin(), events.end());
    int counter = 0;
    int interval_start, interval_end;
    vector<vector<int>> rtn;
    for (const Event& e : events) {
      if (e.type == 0) {
        counter++;
        if (counter == 2) {
          interval_start = e.time;
        }
      } else {
        if (counter == 2) {
          interval_end = e.time;
          rtn.push_back({interval_start, interval_end});
        }
        counter--;
      }
    }
    return rtn;
  }
};