/**
 * Solution implements BFS with overwrite.
 *
 * Solution1 implements Dijkstra. Note that in the Dijkstra implementation the score of each node can
 * also be recorded.
 *
 * Solution1 is more intuitive, just best first, and best is defined by the highest score.
 * In Solution there are two points to be clear about, first is we need to record the score in a
 * separate matrix, it can not reuse the original one, second is the min/max relation, which needs
 * to be clarified before coding.
 */
#include "common.h"

const vector<pair<int, int>> kDirections = {
    {-1, 0},
    {1, 0},
    {0, -1},
    {0, 1}
};

class Solution {
  int rows, cols;
 public:
  bool IsInRange(int x, int y) {
    if (x < 0 || x >= rows || y < 0 || y >= cols) {
      return false;
    }
    return true;
  }

  int maximumMinimumPath(vector<vector<int>>& A) {
    rows = A.size();
    cols = A[0].size();
    vector<vector<int>> values(rows, vector<int>(cols, INT_MIN));

    queue<pair<int, int>> to_expand;

    values[0][0] = A[0][0];

    to_expand.push({0, 0});
    while (!to_expand.empty()) {
      pair<int, int> cur = to_expand.front(); to_expand.pop();
      for (const pair<int, int>& delta : kDirections) {
        int new_x = delta.first + cur.first;
        int new_y = delta.second + cur.second;
        if (!IsInRange(new_x, new_y)) {
          continue;
        }
        int new_value = min(values[cur.first][cur.second], A[new_x][new_y]);
        if (new_value > values[new_x][new_y]) {
          values[new_x][new_y] = new_value;
          to_expand.push({new_x, new_y});
        }

      }
    }
    return values[rows - 1][cols - 1];
  }
};



struct Elem {
  int x, y, value;
  bool operator<(const Elem& o) const {
    return value < o.value;
  }
};


// Duplicated with the definition above.
//const vector<pair<int, int>> kDirections = {
//    {-1, 0},
//    {1, 0},
//    {0, -1},
//    {0, 1}
//};

class Solution1 {
  int rows, cols;
 public:
  bool IsInRange(int x, int y) {
    if (x < 0 || x >= rows || y < 0 || y >= cols) {
      return false;
    }
    return true;
  }

  int maximumMinimumPath(vector<vector<int>>& A) {
    rows = A.size();
    cols = A[0].size();

    priority_queue<Elem> to_expand;
    set<pair<int, int>> visited;

    to_expand.push({0, 0, A[0][0]});

    while (!to_expand.empty()) {
      Elem cur = to_expand.top(); to_expand.pop();
      if (cur.x == rows - 1 && cur.y == cols - 1) {
        return cur.value;
      }
      if (visited.find({cur.x, cur.y}) != visited.end()) {
        continue;
      }
      visited.insert({cur.x, cur.y});
      for (const pair<int, int>& delta : kDirections) {
        int new_x = delta.first + cur.x;
        int new_y = delta.second + cur.y;
        if (!IsInRange(new_x, new_y)) {
          continue;
        }
        int new_value = min(cur.value, A[new_x][new_y]);
        to_expand.push({new_x, new_y, new_value});
      }
    }
    return -1;
  }
};