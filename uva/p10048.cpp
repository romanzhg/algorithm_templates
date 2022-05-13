#include <cfloat>
#include <climits>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <algorithm>
#include <deque>
#include <functional>
#include <iostream>
#include <limits>
#include <list>
#include <map>
#include <queue>
#include <set>
#include <stack>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <utility>

using namespace std;

const int INT_INF = INT_MAX / 2;

const int MAX_NODES = 110;

int noise_between[MAX_NODES][MAX_NODES];

int main() {
  int nodes, edges, queries;
  int test_case = 0;
  bool is_first_test_case = true;
  while (true) {
    scanf("%d %d %d", &nodes, &edges, &queries);

    if (nodes == 0 && edges == 0 && queries == 0) {
      break;
    }

    for (int i = 1; i <= nodes; i++) {
      for (int j = 1; j <= nodes; j++) {
        noise_between[i][j] = INT_INF;
      }
    }
    for (int i = 1; i <= nodes; i++) {
      noise_between[i][i] = 0;
    }

    for (int i = 0; i < edges; i++) {
      int from, to, noise_level;
      scanf("%d %d %d", &from, &to, &noise_level);
      noise_between[from][to] = noise_level;
      noise_between[to][from] = noise_level;
    }

    for (int k = 1; k <= nodes; k++) {
      for (int i = 1; i <= nodes; i++) {
        for (int j = 1; j <= nodes; j++) {
          noise_between[i][j] = min(noise_between[i][j], max(noise_between[i][k], noise_between[k][j]));
        }
      }
    }

    if (is_first_test_case) {
      is_first_test_case = false;
    } else {
      cout << endl;
    }
    cout << "Case #" << ++test_case << endl;
    for (int i = 0; i < queries; i++) {
      int from, to;
      scanf("%d %d", &from, &to);
      if (noise_between[from][to] == INT_INF) {
        cout << "no path" << endl;
      } else {
        cout << noise_between[from][to] << endl;
      }
    }
  }
  return 0;
}