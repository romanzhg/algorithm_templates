/**
 * Solution 1 doesn't work, it is based on connected components of the
 * adjacent elements(to the blocked elements).
 *
 * Solution is based on discrete values, record only the important points
 * and build a compressed matrix.
 *
 */
#include <algorithm>
#include <cfloat>
#include <cmath>
#include <cstring>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>
#include <queue>

using namespace std;

//class Solution1 {
//  const int kSizeLimit = 1000000;
//
//  const vector<pair<int, int>> kEightConnected = {
//      {-1, -1}, {-1, 0}, {-1, 1},
//      {0, -1}, {0, 1},
//      {1, -1}, {1, 0}, {1, 1}
//  };
//
//  const vector<pair<int, int>> kFourConnected = {
//      {-1, 0}, {1, 0}, {0, -1}, {0, 1}
//  };
//
//  bool IsPointValid(int x, int y, const vector<vector<int>> &blocked) {
//    if (x < 0 || x >= kSizeLimit || y < 0 || y >= kSizeLimit) {
//      return false;
//    }
//    for (const vector<int> &p : blocked) {
//      if (x == p[0] && y == p[1]) {
//        return false;
//      }
//    }
//    return true;
//  }
//
//  double GetDist(double x1, double y1, double x2, double y2) {
//    return sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
//  }
//
//  int Find(int a, vector<int>& parent) {
//    if (parent[a] == a) {
//      return a;
//    }
//    parent[a] = Find(parent[a], parent);
//    return parent[a];
//  }
//
//  void Union(int a, int b, vector<int>& parent) {
//    int p_a = Find(a, parent);
//    int p_b = Find(b, parent);
//    parent[p_b] = p_a;
//  }
//
//
// public:
//  bool isEscapePossible(vector<vector<int>> &blocked, vector<int> &source, vector<int> &target) {
//    if (blocked.empty()) {
//      return true;
//    }
//
//    set<pair<int, int>> tmp_points;
//    for (const vector<int> &p : blocked) {
//      int x = p[0], y = p[1];
//      for (const pair<int, int> &d : kEightConnected) {
//        int new_x = x + d.first;
//        int new_y = y + d.second;
//        if (IsPointValid(new_x, new_y, blocked)) {
//          tmp_points.insert({new_x, new_y});
//        }
//      }
//    }
//
//    vector<pair<int, int>> points(tmp_points.begin(), tmp_points.end());
//    int adj_points_size = points.size();
//    vector<int> parent(adj_points_size);
//    for (int i = 0; i < adj_points_size; i++) {
//      parent[i] = i;
//    }
//
//    for (int i = 0; i < adj_points_size; i++) {
//      for (const pair<int, int>& d : kFourConnected) {
//        int new_x = points[i].first + d.first;
//        int new_y = points[i].second + d.second;
//        if (!IsPointValid(new_x, new_y, blocked)) {
//          continue;
//        }
//        for (int j = 0; j < adj_points_size; j++) {
//          if (points[j].first == new_x && points[j].second == new_y) {
//            Union(i, j, parent);
//          }
//        }
//      }
//    }
//
//    int closest_index_src;
//    int closest_index_tgt;
//    double dist;
//
//    dist = DBL_MAX;
//    for (int i = 0; i < adj_points_size; i++) {
//      double tmp_dist = GetDist(source[0], source[1], points[i].first, points[i].second);
//      if (tmp_dist < dist) {
//        dist = tmp_dist;
//        closest_index_src = i;
//      }
//    }
//
//    dist = DBL_MAX;
//    for (int i = 0; i < adj_points_size; i++) {
//      double tmp_dist = GetDist(target[0], target[1], points[i].first, points[i].second);
//      if (tmp_dist < dist) {
//        dist = tmp_dist;
//        closest_index_tgt = i;
//      }
//    }
//
//    int p_src = Find(closest_index_src, parent);
//    int p_tgt = Find(closest_index_tgt, parent);
//
//    return p_src == p_tgt;
//  }
//};

class Solution {
  const int kLimit = 1000000;
  const vector<pair<int, int>> kFourConnected = {
      {-1, 0}, {1, 0}, {0, -1}, {0, 1}
  };

  pair<int, int> GetIndex(int x, int y, const vector<int> &x_vector, const vector<int> &y_vector) {
    int rtn_x, rtn_y;
    for (int i = 0; i < (int) x_vector.size(); i++) {
      if (x_vector[i] == x) {
        rtn_x = i;
      }
    }
    for (int i = 0; i < (int) y_vector.size(); i++) {
      if (y_vector[i] == y) {
        rtn_y = i;
      }
    }
    return {rtn_x, rtn_y};
  }

  bool IsConnected(const vector<vector<int>> &compressed, int sx, int sy, int dx, int dy) {
    int x_limit = compressed.size(), y_limit = compressed[0].size();
    vector<vector<int>> visited(x_limit, vector<int>(y_limit, 0));

    queue<pair<int, int>> to_visit;
    to_visit.push({sx, sy});

    while (!to_visit.empty()) {
      pair<int, int> tmp = to_visit.front();
      to_visit.pop();

      if (visited[tmp.first][tmp.second]) {
        continue;
      }
      visited[tmp.first][tmp.second] = 1;

      for (const pair<int, int> &delta : kFourConnected) {
        int new_x = tmp.first + delta.first;
        int new_y = tmp.second + delta.second;
        if (new_x < 0 || new_x >= x_limit || new_y < 0 || new_y >= y_limit) {
          continue;
        }
        if (compressed[new_x][new_y] == 1) {
          continue;
        }
        if (visited[new_x][new_y]) {
          continue;
        }
        to_visit.push({new_x, new_y});
      }
    }

    return visited[dx][dy] == 1;
  }

 public:
  bool isEscapePossible(vector<vector<int>> &blocked, vector<int> &source, vector<int> &target) {
    set<int> x_set;
    x_set.insert(0);
    x_set.insert(999999);
    x_set.insert(source[0]);
    x_set.insert(target[0]);
    set<int> y_set;
    y_set.insert(0);
    y_set.insert(999999);
    y_set.insert(source[1]);
    y_set.insert(target[1]);

    for (const vector<int> &p : blocked) {
      x_set.insert(p[0]);
      if ((p[0] - 1) >= 0) {
        x_set.insert(p[0] - 1);
      }
      if ((p[0] + 1) < kLimit) {
        x_set.insert(p[0] + 1);
      }

      y_set.insert(p[1]);
      if ((p[1] - 1) >= 0) {
        y_set.insert(p[1] - 1);
      }
      if ((p[1] + 1) < kLimit) {
        y_set.insert(p[1] + 1);
      }
    }

    vector<int> x_vector(x_set.begin(), x_set.end());
    vector<int> y_vector(y_set.begin(), y_set.end());
    int x_size = x_vector.size(), y_size = y_vector.size();
    vector<vector<int>> compressed(x_size, vector<int>(y_size, 0));

    for (const vector<int> &p : blocked) {
      auto tmp = GetIndex(p[0], p[1], x_vector, y_vector);
      compressed[tmp.first][tmp.second] = 1;
    }

    auto t_src = GetIndex(source[0], source[1], x_vector, y_vector);
    auto t_tgt = GetIndex(target[0], target[1], x_vector, y_vector);
    return IsConnected(compressed, t_src.first, t_src.second, t_tgt.first, t_tgt.second);
  }
};

int main() {
  Solution *sp = new Solution();
  vector<vector<int>> blocked =
      {{691938, 300406}, {710196, 624190}, {858790, 609485},
       {268029, 225806}, {200010, 188664}, {132599, 612099},
       {329444, 633495}, {196657, 757958}, {628509, 883388}};
  vector<int> src = {655988, 180910};
  vector<int> tgt = {267728, 840949};
  bool rtn = sp->isEscapePossible(blocked, src, tgt);
  cout << "result: " << rtn << endl;
  return 0;
}