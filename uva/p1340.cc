// p1340 / la3218
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

// Point2 are now used in map, the operator is defined in a way that
// points close to each other is considered as one.
bool operator < (const Point2& a, const Point2& b) {
	if (dcmp(b.x - a.x) == 0) {
		if (dcmp(b.y - a.y) == 0) {
			return false;
		} else {
			return a.y < b.y;
		}
	} else {
		return a.x < b.x;
	}
}

bool operator == (const Point2& a, const Point2& b) {
  return dcmp(b.x - a.x) == 0 && dcmp(b.y - a.y) == 0;
}

bool operator != (const Point2& a, const Point2& b) {
  return !(dcmp(b.x - a.x) == 0 && dcmp(b.y - a.y) == 0);
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

// Point p is on left of the defined by v and o.
bool onLeft(const Vector2& v, const Point2& o, const Point2& p) {
	return crossProduct(v, p - o) > 0;
}

// This gives an undirected angle. 0 ~ pi.
double angleBetweenVector2(const Vector2& a, const Vector2& b) {
	return acos(dotProduct(a, b) / lengthOfVector2(a) / lengthOfVector2(b));
}

// Get the intersection of two lines.
// Assumes there is an intersection.
// Line 1 represented by a + tv (v is the direction vector).
// Line 2 represented by b + sw (w is the direction vector).
Point2 getLineIntersection(const Point2& a, const Vector2& v,
	const Point2& b, const Vector2& w) {
  Vector2 u = a - b;
  double t = crossProduct(w, u) / crossProduct(v, w);
  return a + v * t;
}

bool areSegmentsProperIntersecting(const Point2& a1, const Point2& a2,
	const Point2& b1, const Point2& b2) {
	double c1 = crossProduct(a2 - a1, b1 - a1),
	    c2 = crossProduct(a2 - a1, b2 - a1),
	    c3 = crossProduct(b2 - b1, a1 - b1),
	    c4 = crossProduct(b2 - b1, a2 - b1);
	return dcmp(c1) * dcmp(c2) < 0 && dcmp(c3) * dcmp(c4) < 0;
}

// Angle of an vector. In radian, from positive x axis.
// The value is in range (-PI, PI].
// Positive for counter-clockwise rotation from the positive x axis, negative for the
// clockwise rotation.
// Please note the value range of atan2 is different from the value range of atan.
// Since the signs can be used to calculate the quadrant.
double angleOfVector2(const Vector2& v) {
	if (v.x == 0 && v.y == 0) {
		return 0;
	}
	return atan2(v.y, v.x);
}

#define MAX_INPUT_SIZE 110
Point2 inputPoints[MAX_INPUT_SIZE];
map<Point2, set<Point2>> adjMap;
vector<set<Point2>> pointsOnLines;

// The angle between original and the first point is (-PI/2, PI/2).
Point2 findFirst(const Point2& original, const set<Point2>& adjPoints) {
	Point2 rtn;
	double minAngle = PI;
  for (const Point2& p : adjPoints) {
  	double angle = angleOfVector2(p - original);
  	if (minAngle > angle) {
  		minAngle = angle;
  		rtn = p;
  	}
  }
  return rtn;
}

Point2 findNext(const Point2& prev, const Point2& current, const set<Point2>& adjPoints) {
	bool foundOnRight = false;
	double rightAngle = -10;
	double leftAngle = 10;
	Point2 tmp1; // The point on right. Maybe assign this an invalid number as sentinel.
	Point2 tmp2; // The point on left. Maybe assign this an invalid number as sentinel.
	for (const Point2& p : adjPoints) {
		if (p == prev) {
			continue;
		}
		double angle = angleBetweenVector2(current - prev, p - current);
		if (!onLeft(current - prev, prev, p)) {
			if (angle > rightAngle) {
				rightAngle = angle;
				tmp1 = p;
				foundOnRight = true;
			}
		} else {
			if (angle < leftAngle) {
				leftAngle = angle;
				tmp2 = p;
			}
		}
	}

	if (foundOnRight) {
		return tmp1;
	} else {
		return tmp2;
	}
}

void updatesPoints(set<Point2>::iterator it, const set<Point2>& pointsOnLine) {
	if (pointsOnLine.size() < 2) {
		exit(-1);
	}

	if (it == pointsOnLine.begin()) {
		adjMap[*it].insert(*next(it));
	} else if (it == prev(pointsOnLine.end())) {
		adjMap[*it].insert(*prev(it));
	} else {
		adjMap[*it].insert(*next(it));
		adjMap[*it].insert(*prev(it));
	}
}

int main () {
	int pointCount; // Point count is also line count.
	while (scanf("%d", &pointCount) == 1) {
		adjMap.clear();
		pointsOnLines.clear();
		for (int i = 0; i < pointCount; i++) {
			scanf("%lf%lf", &inputPoints[i].x, &inputPoints[i].y);
		}
		pointsOnLines.resize(pointCount);

		// Adj relationship between original points.
		for (int i = 0; i < pointCount; i++) {
			pointsOnLines[i].insert(inputPoints[i]);
			pointsOnLines[i].insert(inputPoints[(i+1) % pointCount]);
		}

		// Add intersection points.
		for (int i = 0; i < pointCount; i++) {
			for (int j = i + 1; j < pointCount; j++) {
				Point2 a1 = inputPoints[i];
				Point2 a2 = inputPoints[(i + 1) % pointCount];
				Point2 b1 = inputPoints[j];
				Point2 b2 = inputPoints[(j + 1) % pointCount];

				if (areSegmentsProperIntersecting(a1, a2, b1, b2)) {
					Point2 intersection = getLineIntersection(a1, a2 - a1, b1, b2 - b1);
					pointsOnLines[i].insert(intersection);
					pointsOnLines[j].insert(intersection);
				}
			}
		}

		// Mark adjacent points on the same line.
		for (int i = 0; i < pointCount; i++) {
			for (set<Point2>::iterator it = pointsOnLines[i].begin();
					 it != pointsOnLines[i].end();
					 it++) {
				updatesPoints(it, pointsOnLines[i]);
			}
		}

		vector<Point2> rtn;
		
		// Start from the lowest point.
		Point2 original = adjMap.begin()->first;
		Point2 prev = original;
		rtn.push_back(prev);
		Point2 current = findFirst(original, adjMap[original]);

		while (current != original) {
			rtn.push_back(current);
			Point2 next = findNext(prev, current, adjMap[current]);
			prev = current;
			current = next;
		}

		printf("%d\n", (int) rtn.size());
		for (int i = 0; i < rtn.size(); i++) {
			printf("%.4lf %.4lf\n", rtn[i].x, rtn[i].y);
		}
	}
	return 0;
}