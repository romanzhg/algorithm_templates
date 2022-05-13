/**
 * Easy problem, basic programming skills, drop off events need to be handled before pickup events.
 *
 */

struct Event {
  int diff, pos;
  Event(int diff, int pos) : diff(diff), pos(pos) {};

  bool operator<(const Event& o) {
    if (pos == o.pos) {
      return diff < o.diff;
    } else {
      return pos < o.pos;
    }
  }
};

class Solution {
 public:
  bool carPooling(vector<vector<int>>& trips, int capacity) {
    vector<Event> events;
    for (const vector<int>& trip : trips) {
      events.push_back(Event(trip[0], trip[1]));
      events.push_back(Event(-trip[0], trip[2]));
    }
    sort(events.begin(), events.end());

    int tmp = 0;
    for (const Event& e : events) {
      tmp += e.diff;
      if (tmp > capacity) {
        return false;
      }
    }
    return true;
  }
};