// p1396 / la3890
#include <cstdio>
#include <cstdlib>
#include <climits>
#include <cmath>
#include <iostream>
#include <queue>
#include <deque>
#include <vector>
#include <utility>
#include <algorithm>
#include <functional>
#include <cstring>
#include <string>
#include <map>
#include <set>
#include <stack>
#include <list>
#include <limits>
using namespace std;

const double PI = acos(-1);
const double TWO_PI = 2 * PI;
const double EPS = 1e-10;

// Return -1 if the value is smaller than 0.
// Return 0 if the value is equal to 0.
// Return 1 if the value is bigger than 0.
int dcmp(double x) {
  if (fabs(x) < EPS) {
    return 0;
  } else {
    return x < 0 ? -1 : 1;
  }
}

struct Point2 {
  double x, y;
  Point2() : x(0), y(0) {};
  Point2(double x, double y) : x(x), y(y) {};
};

typedef Point2 Vector2;
typedef vector<Point2> Polygon;

Vector2 operator + (const Vector2& a, const Vector2& b) {
  return Vector2(a.x + b.x, a.y + b.y);
}

Vector2 operator - (const Vector2& a, const Vector2& b) {
  return Vector2(a.x - b.x, a.y - b.y);
}

Vector2 operator * (const Vector2& v, double s) {
  return Vector2(v.x * s, v.y * s);
}

Vector2 operator / (const Vector2& v, double s) {
  return Vector2(v.x / s, v.y / s);
}

bool operator < (const Point2& a, const Point2& b) {
  return a.x < b.x || (a.x == b.x && a.y < b.y);
}

bool operator == (const Point2& a, const Point2& b) {
  return dcmp(a.x - b.x) == 0 && dcmp(a.y - b.y) == 0;
}

double dotProduct(const Vector2& a, const Vector2& b) {
  return a.x * b.x + a.y * b.y;
}

double crossProduct(const Vector2& a, const Vector2& b) {
  return a.x * b.y - a.y * b.x;
}

double lengthOfVector2(const Vector2& a) {
  return sqrt(dotProduct(a, a));
}

// A line is represented by p + tv.
struct Line2 {
  // Direction vector.
  Vector2 v;
  Point2 p;
  // The angle from positive x axis.
  // Positive for rotating counter-clockwise.
  // Negative for rotating clockwise.
  double ang;

  Line2() {};

  Line2(Point2 p, Vector2 v): p(p), v(v) {
    ang = atan2(v.y, v.x);
  };

  // Get the point at p + tv.
  Point2 point(double t) {
    return p + v * t;
  };

  Vector2 getDirection() const {
    return v;
  };
};

bool operator < (const Line2& a, const Line2& b) {
  return a.ang < b.ang;
}

// Get the intersection of two lines.
// Assumes there is an intersection.
Point2 getLineIntersection(const Line2& a, const Line2& b) {
  Vector2 u = a.p - b.p;
  double t = crossProduct(b.v, u) / crossProduct(a.v, b.v);
  return a.p + a.v * t;
}

Vector2 getNormal(const Vector2& a) {
  double l = lengthOfVector2(a);
  return Vector2(-a.y/l, a.x/l);
}

// Point p is on left of line l.
bool onLeft(const Line2& l, const Point2& p) {
  return crossProduct(l.v, p - l.p) > 0;
}

// Get the half plane intersection of the input lines.
void halfPlaneIntersection(Line2* lines, int n, Polygon& polygon) {
  if (n <= 1) return;

  sort(lines, lines + n);
  deque<Point2> points;
  deque<Line2> intersectingLines;
  intersectingLines.push_back(lines[0]);

  for (int i = 1; i < n; i++) {
    while (intersectingLines.size() > 1 && !onLeft(lines[i], points.back())) {
      intersectingLines.pop_back();
      points.pop_back();
    }
    while (intersectingLines.size() > 1 && !onLeft(lines[i], points.front())) {
      intersectingLines.pop_front();
      points.pop_front();
    }

    intersectingLines.push_back(lines[i]);

    int last = intersectingLines.size() - 1;
    Line2 tmpA = intersectingLines[last - 1];
    Line2 tmpB = intersectingLines[last];
    if (fabs(crossProduct(tmpA.v, tmpB.v)) < EPS) {
      // tmpA and tmpB are parallel, get the left one.
      intersectingLines.pop_back();
      if (points.size() >= 1) {
        points.pop_back();  
      }
      if (onLeft(tmpA, tmpB.p)) {
        // tmpB is on left of tmpA, keep tmpB
        intersectingLines.back() = tmpB;
      }
    }
    last = intersectingLines.size() - 1;
    if (intersectingLines.size() > 1) {
      points.push_back(getLineIntersection(intersectingLines[last], intersectingLines[last - 1]));
    }
  }

  while (intersectingLines.size() > 1 && !onLeft(intersectingLines.front(), points.back())) {
    intersectingLines.pop_back();
    points.pop_back();
  }

  // At most one intersection point, open area.
  if (points.size() <= 1) {
    return;
  }
  
  points.push_back(getLineIntersection(intersectingLines.back(), intersectingLines.front()));
  polygon.insert(polygon.end(), points.begin(), points.end());
}

#define MAX_INPUT_SIZE 110
Point2 points[MAX_INPUT_SIZE];
Vector2 direction[MAX_INPUT_SIZE];
Vector2 normDirection[MAX_INPUT_SIZE];

Line2 lines[MAX_INPUT_SIZE];

// One digit finer than the required error.
const double ERROR_VAL = 1e-6;

int main() {
  int pointsCount;
  while (true) {
    scanf("%d", &pointsCount);
    if (pointsCount == 0) {
      break;
    }
    for (int i = 0; i < pointsCount; i++) {
      scanf("%lf %lf", &points[i].x, &points[i].y);
    }
    for (int i = 0; i < pointsCount; i++) {
      lines[i] = Line2(points[i], points[(i+1) % pointsCount] - points[i]);
      direction[i] = points[(i+1) % pointsCount] - points[i];
      normDirection[i] = getNormal(direction[i]);
    }
    double low = 0, high = 20000;
    while (high - low >= ERROR_VAL) {
      double mid = low + (high - low) / 2.0;
      for (int i = 0; i < pointsCount; i++) {
        lines[i] = Line2(points[i] + normDirection[i] * mid, direction[i]);
      }
      Polygon intersections;
      halfPlaneIntersection(lines, pointsCount, intersections);
      if (intersections.size() == 0) {
        high = mid;
      } else {
        low = mid;
      }
    }
    printf("%.6lf\n", low + (high - low) / 2.0);
  }
  return 0;
}