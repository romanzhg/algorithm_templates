#include <cstdio>
#include <cstdlib>
#include <climits>
#include <cmath>
#include <queue>
#include <vector>
#include <utility>
#include <algorithm>
#include <functional>
#include <cstring>
#include <string>
#include <map>
#include <set>

using namespace std;

struct Circle {
  double x;
  double y;
  double r;
  int id;
};

// Solution.
#define MAX_INPUT_SIZE 40000

int circlesCount;

Circle circles[MAX_INPUT_SIZE];

// Return true if a is included in b, false otherwise.
// The two circles do not intersect.
bool isCircleIncluded(const Circle& a, const Circle& b) {
  if (a.r >= b.r) {
    return false;
  }
  double xDiff = b.x - a.x;
  double yDiff = b.y - a.y;
  double dist = sqrt(xDiff * xDiff + yDiff * yDiff);
  if (dist < (b.r - a.r)) {
    return true;
  } else {
    return false;
  }
}

bool isCircleIncluded(const Circle& c, const set<pair<double, int> >& currentSet) {
  set<pair<double, int> >::iterator n = currentSet.lower_bound(make_pair(c.y, 0));
  if (n != currentSet.end() && isCircleIncluded(c, circles[n->second - 1])) {
    return true;
  }
  if (n == currentSet.begin()) {
    return false;
  }

  n--;
  if (isCircleIncluded(c, circles[n->second - 1])) {
    return true;
  }
  return false;
}

void solve() {
  // Vector of <x, id>.
  vector<pair<double, int> > events;
  for (int i = 0; i < circlesCount; i++) {
    events.push_back(make_pair(circles[i].x - circles[i].r, circles[i].id));
    events.push_back(make_pair(circles[i].x + circles[i].r, -circles[i].id));
  }
  sort(events.begin(), events.end());

  set<int> rtn;
  // Set of <y, id>.
  set<pair<double, int> > currentSet;
  for (int i = 0; i < events.size(); i++) {
    int id = events[i].second;
    if (id > 0) {
      if (!isCircleIncluded(circles[id - 1], currentSet)) {
        currentSet.insert(make_pair(circles[id - 1].y, id));
        rtn.insert(id);
      }
    } else {
      set<pair<double, int> >::iterator tmpIt = currentSet.find(make_pair(circles[(-id) - 1].y, -id));
      if (tmpIt != currentSet.end()) {
        currentSet.erase(tmpIt);
      }
    }
  }

  printf("%lu\n", rtn.size());
  for (set<int>::iterator it = rtn.begin(); it != rtn.end(); it++) {
    printf("%d ", *it);
  }
  printf("\n");
}

int main() {
  while (scanf("%d", &circlesCount) == 1) {
  	for (int i = 0; i < circlesCount; i++) {
  		scanf("%lf %lf %lf", &circles[i].r, &circles[i].x, &circles[i].y);
  		circles[i].id = i + 1;
  	}
  	solve();
  }
  return 0;
}