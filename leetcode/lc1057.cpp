#include "common.h"

struct Elem {
  int dist, wid, bid;
  Elem(int dist, int wid, int bid) : dist(dist), wid(wid), bid(bid) {};

  bool operator<(const Elem& o) const {
    if (dist != o.dist) {
      return dist > o.dist;
    }
    if (wid != o.wid) {
      return wid > o.wid;
    }
    return bid > o.bid;
  }
};

int GetManhattanDist(const vector<int>& a, const vector<int>& b) {
  return abs(a[0] - b[0]) + abs(a[1] - b[1]);
}

class Solution {
 public:
  vector<int> assignBikes(vector<vector<int>>& workers, vector<vector<int>>& bikes) {
    priority_queue<Elem> pq;
    for (int i = 0; i < workers.size(); i++) {
      for (int j = 0; j < bikes.size(); j++) {
        pq.push(Elem(GetManhattanDist(workers[i], bikes[j]), i, j));
      }
    }

    vector<int> rtn(workers.size());
    set<int> assigned_workers, assigned_bikes;
    while (!pq.empty()) {
      Elem e = pq.top(); pq.pop();
      if (assigned_workers.find(e.wid) != assigned_workers.end()
          || assigned_bikes.find(e.bid) != assigned_bikes.end()) {
        continue;
      }
      assigned_workers.insert(e.wid);
      assigned_bikes.insert(e.bid);
      rtn[e.wid] = e.bid;
    }
    return rtn;
  }
};