/**
 * Solution1 search the space in a DFS way.
 * By establishing a clear contract(robot status not change before and after a call to Expand()),
 * the program is easy to write.
 *
 * Solution tries to search the space in a BFS way, which is hard to implement(the implementation
 * was not tested).
 */

#include "common.h"

/**
 *
 *
 * // This is the robot's control interface.
 * // You should not implement it, or speculate about its implementation
 * class Robot {
 *   public:
 *     // Returns true if the cell in front is open and robot moves into the cell.
 *     // Returns false if the cell in front is blocked and robot stays in the current cell.
 *     bool move();
 *
 *     // Robot will stay in the same cell after calling turnLeft/turnRight.
 *     // Each turn will be 90 degrees.
 *     void turnLeft();
 *     void turnRight();
 *
 *     // Clean the current cell.
 *     void clean();
 * };
 */
struct Point {
  int x, y;

  bool operator<(const Point& o) const {
    if (x == o.x) {
      return y < o.y;
    } else {
      return x < o.x;
    }
  }
};

const vector<pair<int, int>> kDirectionToDelta = {
    {-1, 0},
    {0, 1},
    {1, 0},
    {0, -1}
};

struct Status {
  // For direction:
  // NORTH = 0,
  // EAST = 1,
  // SOUTH = 2,
  // WEST = 3,
  int x, y, d;

  bool operator<(const Status& o) const {
    if (x != o.x) {
      return x < o.x;
    }
    if (y != o.y) {
      return y < o.y;
    }
    return d < o.d;
  }

  bool operator==(const Status& o) const {
    return x == o.x && y == o.y && d == o.d;
  }

  bool operator!=(const Status& o) const {
    return !operator==(o);
  }

};

class Solution {
 public:
  // Return a list a path from the current status to a status in the target position.
  // If the robot is already at the target, return only the initial status.
  vector<Status> FindPath(Point target, const Status& s, const set<Point>& passable) {
    vector<Status> rtn;

    set<Status> visited;

    queue<Status> to_expand;
    map<Status, Status> prev;

    to_expand.push(s);
    while (!to_expand.empty()) {
      Status cur = to_expand.front(); to_expand.pop();
      if (cur.x == target.x && cur.y == target.y) {
        while (cur != s) {
          rtn.push_back(cur);
          cur = prev[cur];
        }
        rtn.push_back(s);
        reverse(rtn.begin(), rtn.end());
        return rtn;
      }
      if (visited.find(cur) != visited.end()) {
        continue;
      }
      visited.insert(cur);
      Status next_0 = cur;
      next_0.d = (next_0.d + 1) % 4;

      prev[next_0] = cur;
      to_expand.push(next_0);

      Status next_1 = cur;
      next_1.x += kDirectionToDelta[s.d].first;
      next_1.y += kDirectionToDelta[s.d].second;
      if (passable.find({next_1.x, next_1.y}) != passable.end()) {
        to_expand.push(next_1);
      }
    }
    exit(-1);
  }

  void MoveFromStatusToStatus(Status from, Status to, Robot& robot) {
    if (from.x != to.x || from.y != to.y) {
      robot.move();
    } else {
      while (from.d != to.d) {
        from.d = (from.d + 1) % 4;
        robot.turnRight();
      }
    }
  }

  // Need passable and current status.
  void MoveToPoint(Point target, Robot& robot, Status& s, const set<Point>& passable) {
    vector<Status> path = FindPath(target, s, passable);

    for (int i = 1; i < path.size(); i++) {
      MoveFromStatusToStatus(path[i - 1], path[i]);
    }

    s = path.back();
  }

  // Checks the point in front, and restores the previous position and direction.
  // Does not change robot status.
  void Expand(queue<Point>& to_clean, Robot& robot, const Status& s, const set<Point>& passable) {
    // If the move succeed, this would be the new coordinate.
    int new_x = s.x + kDirectionToDelta[s.d].first;
    int new_y = s.y + kDirectionToDelta[s.d].second;

    // We already expanded to that point, no need to expand again.
    if (passable.find({new_x, new_y}) != passable.end()) {
      return;
    }

    if (robot.move()) {
      passable.insert({new_x, new_y});
      to_clean.push({new_x, new_y});

      // Restore.
      robot.turnRight();
      robot.turnRight();
      robot.move();
      robot.turnRight();
      robot.turnRight();

    } else {
      return;
    }
  }

  void TurnRight(Robot& robot, Status& s) {
    robot.turnRight();
    s.d = (s.d + 1) % 4;
  }

  void cleanRoom(Robot& robot) {
    Status s(Point(0, 0), 0);

    set<Point> passable;
    passable.insert({0, 0});

    queue<Point> to_clean;
    to_clean.push({0, 0});

    while (!to_clean.empty()) {
      Point c = to_clean.front(); to_clean.pop();
      MoveToPoint(c, robot, s, passable);
      robot.clean();

      Expand(to_clean, robot, s, passable);
      TurnRight(robot, s);
      Expand(to_clean, robot, s, passable);
      TurnRight(robot, s);
      Expand(to_clean, robot, s, passable);
      TurnRight(robot, s);
      Expand(to_clean, robot, s, passable);
    }
  }
};

class Solution1 {
 public:
  set<Point> visited;

  void Expand(int x, int y, int dir, Robot& r) {
    r.clean();
    visited.insert({x, y});
    for (int i = 0; i < 4; i++) {
      int new_x = x + kDirectionToDelta[dir].first;
      int new_y = y + kDirectionToDelta[dir].second;
      if (visited.find({new_x, new_y}) == visited.end()) {
        // Go in and restore.
        if (r.move()) {
          Expand(new_x, new_y, dir, r);
          r.turnRight();
          r.turnRight();
          r.move();
          r.turnRight();
          r.turnRight();
        }
      }
      dir = (dir + 1) % 4;
      r.turnRight();
    }
  }

  void cleanRoom(Robot& robot) {
    visited.clear();
    Expand(0, 0, 0, robot);
  }
};