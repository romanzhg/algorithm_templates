#include "common.h"

const vector<pair<int, int>> kDirections = {
    {-1, 0},
    {1, 0},
    {0, -1},
    {0, 1},
};

struct Node {
  int x, y, dist;
  Node(int x, int y, int dist) : x(x), y(y), dist(dist) {};
  bool operator<(const Node& o) const {
    if (x == o.x) {
      return y < o.y;
    }
    return x < o.x;
  }
};

class Solution {
  int rows, cols;
 public:
  bool IsValidPoint(int x, int y, vector<string> grid) {
    if (x < 0 || x >= rows || y < 0 || y >= cols) {
      return false;
    }
    if (grid[x][y] != '.') {
      return false;
    }
    return true;
  }

  int GetShortestPath(pair<int, int> from, pair<int, int> to, const vector<string>& grid) {
    queue<Node> to_expand;
    set<Node> visited;
    to_expand.push({from.first, from.second, 0});

    while (!to_expand.empty()) {
      Node cur = to_expand.front(); to_expand.pop();
      if (cur.x == to.first && cur.y == to.second) {
        return cur.dist;
      }
      if (visited.find(cur) != visited.end()) {
        continue;
      }
      visited.insert(cur);

      for (const pair<int, int>& delta : kDirections) {
        int new_x = cur.x + delta.first;
        int new_y = cur.y + delta.second;
        if (IsValidPoint(new_x, new_y, grid)) {
          to_expand.push({new_x, new_y, cur.dist + 1});
        }
      }
    }
    return -1;
  }

  pair<int, int> FindChar(const vector<string>& grid, char c) {
    int rows = grid.size();
    int cols = grid[0].size();
    for (int i = 0; i < rows; i++) {
      for (int j = 0; j < cols; j++) {
        if (grid[i][j] == c) {
          return {i, j};
        }
      }
    }
    exit(-1);
  }

  int GetSteps(vector<int> seq, vector<string> grid) {
    vector<pair<int, int>> path_points;
    path_points.push_back(FindChar(grid, '@'));
    grid[path_points[0].first][path_points[0].second] = '.';
    for (int v : seq) {
      auto tmp = FindChar(grid, 'a' + v);
      grid[tmp.first][tmp.second] = '.';
      path_points.push_back(tmp);
    }
    int rtn = 0;
    for (int i = 0; i < path_points.size() - 1; i++) {
      int tmp = GetShortestPath(path_points[i], path_points[i + 1], grid);
      if (tmp == -1) {
        return -1;
      }
      pair<int, int> to_update = FindChar(grid, 'A' + seq[i]);
      grid[to_update.first][to_update.second] = '.';
      rtn += tmp;
    }
    return rtn;
  }

  int helper(vector<int>& seq, vector<bool>& used, const vector<string>& grid, int keys) {
    if (seq.size() == keys) {
      return GetSteps(seq, grid);
    }

    int rtn = INT_MAX;
    for (int i = 0; i < keys; i++) {
      if (used[i]) {
        continue;
      }
      used[i] = true;
      seq.push_back(i);
      int tmp = helper(seq, used, grid, keys);
      if (tmp != -1) {
        rtn = min(rtn, tmp);
      }
      seq.pop_back();
      used[i] = false;
    }
    if (rtn == INT_MAX) {
      return -1;
    } else {
      return rtn;
    }
  }

  int shortestPathAllKeys(vector<string>& grid) {
    rows = grid.size();
    cols = grid[0].size();
    int keys = 0;
    for (const string& r : grid) {
      for (char c : r) {
        if (c >= 'a' && c <= 'z') {
          keys++;
        }
      }
    }

    vector<bool> used(keys, false);
    vector<int> seq;
    return helper(seq, used, grid, keys);
  }
};