/**
 * Solution shows the use of std::prev().
 * There is a greedy two pointer solution, always sits the last one first, and
 * sits the first one when there is room.
 * This is faster.
 */

#include "common.h"

class Solution {
 public:
  int numRescueBoats(vector<int>& people, int limit) {
    multiset<int> s(people.begin(), people.end());

    int boats = 0;
    while (!s.empty()) {
      int v = *s.begin();
      s.erase(s.begin());
      int o = limit - v;
      // upper_bound finds the first number larger than o.
      // Apply prev() on it will get one number that is smaller or equal to o.
      auto it = s.upper_bound(o);
      if (it != s.begin()) {
        it = prev(it);
        s.erase(it);
      }
      boats++;
    }
    return boats;
  }
};