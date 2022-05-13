#include <cstdio>
#include <cstdlib>
#include <climits>
#include <queue>
#include <vector>
#include <utility>
#include <algorithm>
#include <functional>
#include <cstring>
#include <string>
#include <map>
// unordered_map is not supported.
using namespace std;

#define MAX_N 1010
#define MAX_RELATION 10010
#define INF 100000000

int al[MAX_RELATION], bl[MAX_RELATION], dl[MAX_RELATION];
int ad[MAX_RELATION], bd[MAX_RELATION], dd[MAX_RELATION];
int d[MAX_N]; // shortest distance from source.
int N, ML, MD;

void solve() {
  fill(d, d + N, INF);
  d[0] = 0;
  
  for (int k = 0; k < N; k++) {
    for (int i = 0; i + 1 < N; i++) {
      if (d[i+1] < INF) {
        d[i] = min(d[i], d[i + 1] + 0 /* the edge from i+1 to i*/);
      }
    }

    for (int i = 0; i < ML; i++) {
      if (d[al[i] - 1] < INF) {
        d[bl[i] - 1] = min(d[bl[i] - 1], d[al[i] - 1] + dl[i]);
      }
    }

    for (int i = 0; i < MD; i++) {
      if (d[bd[i] - 1] < INF) {
        d[ad[i] - 1] = min(d[ad[i] - 1], d[bd[i] - 1] - dd[i]);
      }
    }
  }

  int result = d[N-1];
  if (d[0] < 0) {
    result = -1;
  } else if (result == INF) {
    result = -2;
  }
  printf("%d\n", result);
}

int main() {
  while (scanf("%d %d %d", &N, &ML, &MD) != EOF) {
    for (int i = 0; i < ML; i++) {
      scanf("%d %d %d", &al[i], &bl[i], &dl[i]);
    }
    for (int i = 0; i < MD; i++) {
      scanf("%d %d %d", &ad[i], &bd[i], &dd[i]);
    }
    solve();
  }
  return 0;
}
