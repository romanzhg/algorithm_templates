/**
 * Easy search with saved results.
 */

#include "common.h"
const vector<int> kModifier{-1, 0, 1};

class Solution {
 public:
  bool canCross(vector<int>& stones) {
    map<int, set<int>> reachable;
    for (int v : stones) {
      reachable[v].clear();
    }
    reachable[0].insert(0);

    for (int pos : stones) {
      for (int cur_speed : reachable[pos]) {
        for (int m : kModifier) {
          int speed = cur_speed + m;
          if (speed <= 0) {
            continue;
          }
          if (reachable.find(pos + speed) != reachable.end()) {
            reachable[pos + speed].insert(speed);
          }
        }

      }
    }
    return !reachable[stones.back()].empty();
  }
};