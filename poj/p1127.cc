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

/******************************************************/
double EPS = 1e-10;

double add(double a, double b) {
  if (abs(a + b) < EPS * (abs(a) + abs(b))) {
  	return 0;
  }
  return a + b;
}

// A vector in two dimentional space.
// It can be understood as a vector from the origin to a point in the two
// dimentional space.
struct Vector2 {
  double x, y;
  Vector2() {};
  Vector2(double x, double y) : x(x), y(y) {};
};

Vector2 addVector2(const Vector2& a, const Vector2& b) {
  return Vector2(add(a.x, b.x), add(a.y, b.y));
}

Vector2 minusVector2(const Vector2& a, const Vector2& b) {
	return Vector2(add(a.x, -b.x), add(a.y, -b.y));
}

Vector2 multiplyScalar(const Vector2& v, double s) {
  return Vector2(v.x * s, v.y * s);
}

double dotProduct(const Vector2& a, const Vector2& b) {
  return add(a.x * b.x, a.y * b.y);
}

double crossProductValue(const Vector2& a, const Vector2& b) {
  return add(a.x * b.y, -(a.y * b.x));
}

// Return true if the lines defined by (p1, q1) and (p2, q2) is parallel 
// to each other.
bool isParallel(const Vector2& p1, const Vector2& q1,
	const Vector2& p2, const Vector2& q2) {
  return crossProductValue(minusVector2(p1, q1), minusVector2(p2, q2)) == 0;
}

// Return true if point p is on the line segment defined by p1 and q1.
bool onSeg(const Vector2& p1, const Vector2& q1, const Vector2& p) {
  // 1. Get two vectors defined by p1 - p, q1 - p.
  // 2. The conditions for on seg are two vectors are parallel and they 
  // the dot product smaller than or equal to 0.
  return isParallel(p1, p, q1, p)
  		&& dotProduct(minusVector2(p1, p), minusVector2(q1, p)) <= 0;
}

// Get the intersection between the lines defined by (p1, q1) and (p2, q2).
// Assume these two lines are not parallel.
Vector2 getIntersection(Vector2 p1, Vector2 q1, Vector2 p2, Vector2 q2) {
	// q1 + t(p1 - p1): the intersection point.
	// Value of t:
	double t = crossProductValue(minusVector2(p2, q2), minusVector2(p2, q1))
  	/ crossProductValue(minusVector2(p2, q2), minusVector2(p1, q1));
  // Get the intersection point.
  return addVector2(q1, multiplyScalar(minusVector2(p1, q1), t));
}

/******************************************************/

#define MAX_LINE_SEGS 20
int lineSegs;

// End point 1 and end point 2.
Vector2 p[MAX_LINE_SEGS], q[MAX_LINE_SEGS];
bool connected[MAX_LINE_SEGS][MAX_LINE_SEGS];

void solve() {
  for (int i = 0; i < lineSegs; i++) {
    connected[i][i] = true;
    for (int j = i + 1; j < lineSegs; j++) {
      // lineSeg 1 (p[i], q[i])
      // lineSeg 2 (p[j], q[j])
      bool tmpConnected;
      if (isParallel(p[i], q[i], p[j], q[j])) {
        tmpConnected = onSeg(p[i], q[i], p[j])
            || onSeg(p[i], q[i], q[j])
            || onSeg(p[j], q[j], p[i])
            || onSeg(p[j], q[j], q[i]);
      } else {
        Vector2 intersection = getIntersection(p[i], q[i], p[j], q[j]);
        tmpConnected = onSeg(p[i], q[i], intersection) && onSeg(p[j], q[j], intersection);
      }
      connected[i][j] = tmpConnected;
      connected[j][i] = tmpConnected;
    }
  }

  for (int k = 0; k < lineSegs; k++) {
    for (int i = 0; i < lineSegs; i++) {
      for (int j = 0; j < lineSegs; j++) {
        connected[i][j] = connected[i][j] || (connected[i][k] && connected[k][j]);
      }
    }
  }
}

int main() {
  while (scanf("%d", &lineSegs) != EOF) {
    // Initialization.
    for (int i = 0; i < MAX_LINE_SEGS; i++) {
      for (int j = 0; j < MAX_LINE_SEGS; j++) {
        connected[i][j] = false;
      }
    }

    if (lineSegs == 0) {
      break;
    }
    for (int i = 0; i < lineSegs; i++) {
      scanf("%lf %lf %lf %lf", &p[i].x, &p[i].y, &q[i].x, &q[i].y);
    }
    // Build the connected graph.
    solve();

    // Answer queries.
    while (true) {
      int lineSegA, lineSegB;
      scanf("%d %d", &lineSegA, &lineSegB);
      if (lineSegA == 0 && lineSegB == 0) {
        break;
      } else {
        printf(connected[lineSegA - 1][lineSegB - 1] ? "CONNECTED\n" : "NOT CONNECTED\n");
      }
    }
  }

  return 0;
}