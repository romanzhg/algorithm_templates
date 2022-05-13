/**
 * Extend the state from (x,y) to (x,y,k) (k is the remaining bombs), and record the
 * shortest distance to these states.
 * Use a simple heuristic to avoid eliminate a block twice.
 *
 * Note that it is not possible to keep the state (x,y) and update the distance to (dist, k),
 * since we may need to keep multiple distances at the same state. (dist1, k1), (dist2, k2)
 * cannot be compared to each other(larger k is better, smaller dist is better).
 */
#include "common.h"

const int kIntInf = INT_MAX / 2;

struct Node {
  int x, y, k, c;
  Node(int x, int y, int k, int c) : x(x), y(y), k(k), c(c) {};
};

const vector<pair<int, int>> kDelta = {
    {-1, 0},
    {1, 0},
    {0, -1},
    {0, 1}
};

int dp[40][40][80];

class Solution {
 public:
  int shortestPath(vector<vector<int>>& grid, int k) {
    int m = grid.size();
    int n = grid[0].size();

    // A map with size m * n, there are at most m + n - 3 blocks on the path.
    if (k >= (m + n - 3)) {
      return m + n - 2;
    }

    for (int i = 0; i < m ; i++) {
      for (int j = 0; j < n; j++) {
        for (int t = 0; t <= k; t++) {
          dp[i][j][t] = kIntInf;
        }
      }
    }

    queue<Node> q;
    q.push({0, 0, k, 0});

    while (!q.empty()) {
      Node tmp_node = q.front(); q.pop();
      if (tmp_node.c >= dp[tmp_node.x][tmp_node.y][tmp_node.k]) {
        continue;
      }

      dp[tmp_node.x][tmp_node.y][tmp_node.k] = tmp_node.c;

      for (const pair<int, int>& d : kDelta) {
        int new_x = tmp_node.x + d.first;
        int new_y = tmp_node.y + d.second;
        if (new_x < 0 || new_x >= m || new_y < 0 || new_y >= n) {
          continue;
        }
        if (grid[new_x][new_y] == 1) {
          // Some branch cutting here. If we every reached a blocked area(by eleminate the block),
          // there is no point to visit again.
          bool should_skip = false;
          for (int i = 0; i <= k; i++) {
            if (dp[new_x][new_y][i] != kIntInf) {
              should_skip = true;
              break;
            }
          }
          if (!should_skip && tmp_node.k > 0) {
            q.push({new_x, new_y, tmp_node.k - 1, tmp_node.c + 1});
          }
        } else {
          q.push({new_x, new_y, tmp_node.k, tmp_node.c + 1});
        }
      }
    }

    int rtn = kIntInf;
    for (int i = 0; i <= k; i++) {
      rtn = min(rtn, dp[m - 1][n - 1][i]);
    }

    if (rtn == kIntInf) {
      return -1;
    } else {
      return rtn;
    }
  }
};