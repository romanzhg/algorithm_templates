// For this question, the point comparison needs to take EPS into consideration.
// Otherwise the same point(for example, 3 lines intersects at the same point,
// in this case there would be 3 computations, ab, ac, and bc) will be comsidered
// different due to floating point computation.
//
// The well valued intersection point(for example, (1, 1)) does not un-cover this problem.
// This problem is uncovered by a intersection point at (2/3, 2/3). This test case is
// generated by the program attached.
//
// There is a easier solution for this problem, base on cutting polygon.
// That solution does not depending taking EPS into point comparison, and
// many edge cases are handled by the cutting polygon procedure.
// Implemented in p12296_1.cc
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
	return dcmp(a.x - b.x) == 0 && dcmp(a.y - b.y) == 0;
}

bool operator != (const Point2& a, const Point2& b) {
  return !(dcmp(b.x - a.x) == 0 && dcmp(b.y - a.y) == 0);
}

double dotProduct(const Vector2& a, const Vector2& b) {
  return a.x * b.x + a.y * b.y;
}

// Sign of the product represents the relative angle between a and b.
double crossProduct(const Vector2& a, const Vector2& b) {
  return a.x * b.y - a.y * b.x;
}

double lengthOfVector2(const Vector2& a) {
	return sqrt(dotProduct(a, a));
}

typedef vector<Point2> Polygon;

// A line is represented by p + tv.
struct Line2 {
	Vector2 v;
	Point2 p;

	Line2() {};

	Line2(Vector2 v, Point2 p): v(v), p(p) {};

	// Get the point at p + tv.
	Point2 point(double t) const {
		return p + v * t;
	};

};

struct Circle {
  Point2 c;
  double r;
  Circle(): c(Point2()), r(0) {};
  Circle(double x, double y, double radius) {
  	c = Point2(x, y);
  	r = radius;
  };
  Circle(Point2 c, double r) : c(c), r(r) {};

  // Get the point at radian a of the circle.
  Point2 point2(double a) {
  	return Point2(c.x + cos(a) * r, c.y + sin(a) * r);
  };
};

// Get the intersection of two lines.
// Assumes there is an intersection.
Point2 getLineIntersection(const Line2& a, const Line2& b) {
  Vector2 u = a.p - b.p;
  double t = crossProduct(b.v, u) / crossProduct(a.v, b.v);
  return a.p + a.v * t;
}

// Point p is on left of line l.
bool onLeft(const Line2& l, const Point2& p) {
	return dcmp(crossProduct(l.v, p - l.p)) > 0;
}

bool isParallel(const Vector2& a, const Vector2& b) {
  return fabs(crossProduct(a, b)) < EPS;
}

// This gives an undirected angle. [0 ~ pi].
double angleBetweenVector2(const Vector2& a, const Vector2& b) {
	return acos(dotProduct(a, b) / lengthOfVector2(a) / lengthOfVector2(b));
}

double getPolygonArea(const Polygon& polygon) {
  double area = 0;
  int n = polygon.size();
  for (int i = 1; i < n - 1; i++) {
  	area += crossProduct(polygon[i] - polygon[0], polygon[i + 1] - polygon[0]);
  }
  return area / 2;
}

// Returns true if point p is on the line segment (a1, a2).
// Endpoints not included.
bool isOnSegment(const Point2& p, const Point2& a1, const Point2& a2) {
  return dcmp(crossProduct(a1 - p, a2 - p)) == 0 && dcmp(dotProduct(a1-p, a2 - p)) < 0;
}

// The solution starts.
bool isIntersectionOutOfRange(const Point2& p, double L, double W) {
	if (dcmp(p.x) < 0) {
		return true;
	}
	if (dcmp(p.y) < 0) {
		return true;
	}
	if (dcmp(p.x - L) > 0) {
		return true;
	}
	if (dcmp(p.y - W) > 0) {
		return true;
	}
	return false;
}

// Solution sepcific storage.
vector<set<Point2>> pointsOnLines;
map<Point2, set<Point2>> adjMap;

void updatesPointsOneDirection(set<Point2>::iterator it, const set<Point2>& pointsOnLine, int index) {
	if (it == prev(pointsOnLine.end())) {
		return;
	} else {
		if (index < 2) {
			// The first two edges.
			adjMap[*it].insert(*next(it));
		} else {
			// The last two edges, points needed to be added in reverse order.
			adjMap[*next(it)].insert(*it);
		}
	}
}

void updatesPoints(set<Point2>::iterator it, const set<Point2>& pointsOnLine) {
	if (it == pointsOnLine.begin()) {
		adjMap[*it].insert(*next(it));
	} else if (it == prev(pointsOnLine.end())) {
		adjMap[*it].insert(*prev(it));
	} else {
		adjMap[*it].insert(*next(it));
		adjMap[*it].insert(*prev(it));
	}
}

bool getStartPoint(Point2& original, Point2& current) {
	for (auto it = adjMap.begin(); it != adjMap.end(); it++) {
		if (!it->second.empty()) {
			original = it->first;
			current = *(it->second.begin());
			it->second.erase(it->second.begin());
			return true;
		}
	}
	return false;
}

void findNext(const Point2& prev, const Point2& current, set<Point2>& adjPoints, Point2& next) {
	double leftAngle = -1;
	for (const Point2& p : adjPoints) {
		if (p == prev) {
			continue;
		}
		if (!onLeft(Line2(current - prev, prev), p)) {
			continue;
		}
		double angle = angleBetweenVector2(current - prev, p - current);
		if (angle > leftAngle) {
			leftAngle = angle;
			next = p;
		}
	}

	adjPoints.erase(adjPoints.find(next));
}

bool isPointInCircle(const Circle& circle, const Point2& point) {
	double distSqr = dotProduct(point - circle.c, point - circle.c);
	double radiusSqr = circle.r * circle.r;
	return dcmp(radiusSqr - distSqr) > 0;
}

int isPointInConvexPolygon(const Point2& p, const Polygon& poly) {
	int polySize = poly.size();
	for (int i = 0; i < polySize; i++) {
		if (isOnSegment(p, poly[i], poly[(i+1) % polySize])) {
			return -1;
		}
		if (!onLeft(Line2(poly[(i+1) % polySize] - poly[i], poly[i]), p)) {
			return 0;
		}
	}
	return 1;
}

// Return the intersection points only when there are two of them.
void getLineCircleIntersection(const Line2& line, const Circle& circle, vector<Point2>& sol) {
	double t;
	double a = line.v.x,
		b = line.p.x - circle.c.x,
		c = line.v.y,
		d = line.p.y - circle.c.y;
	double e = a * a + c * c,
	  f = 2 * (a * b + c * d),
	  g = b * b + d * d - circle.r * circle.r;
	double delta = f*f - 4*e*g;
	if (dcmp(delta) < 0) {
		return;
	} else if (dcmp(delta) == 0) {
		return;
	} else {
		t = (-f - sqrt(delta)) / (2 * e);
		sol.push_back(line.point(t));
		t = (-f + sqrt(delta)) / (2 * e);
		sol.push_back(line.point(t));
	}
}

bool areLineSegAndCircleIntersecting(const Point2& a, const Point2& b, const Circle& c) {
  vector<Point2> intersections;
  getLineCircleIntersection(Line2(b - a, a), c, intersections);
  for (const Point2& p : intersections) {
  	if (isOnSegment(p, a, b)) {
  		return true;
  	}
  }
  return false;
}

bool isDiskAndPolygonIntersecting(const Circle& disk, const Polygon& polygon) {
	for (int i = 0; i < polygon.size(); i++) {
		if (isPointInCircle(disk, polygon[i])) {
			return true;
		}
		if (isPointInCircle(disk, (polygon[(i + 1) % polygon.size()] + polygon[i]) / 2)) {
			return true;
		}
		if (isPointInConvexPolygon(disk.c, polygon) == 1) {
			return true;
		}
		if (areLineSegAndCircleIntersecting(polygon[(i + 1) % polygon.size()], polygon[i], disk)) {
			return true;
		}
	}
	return false;
}

bool isSameLine(const Line2& a, const Line2& b) {
	if (!isParallel(a.v, b.v)) {
		return false;
	}
	if (dcmp(a.p.x - b.p.x) == 0 && dcmp(a.p.y - b.p.y) == 0) {
		return true;
	}

	Vector2 v(a.p.x - b.p.x, a.p.y - b.p.y);
	return dcmp(angleBetweenVector2(v, b.v)) == 0;
}

int main() {
	int n, m, L, W;

	while (scanf("%d%d%d%d", &n, &m, &L, &W) == 4) {
		if (n == 0 && m == 0 && L == 0 && W == 0) {
			break;
		}

		vector<Line2> lines;
		vector<Circle> disks;

		pointsOnLines.clear();
		adjMap.clear();

		lines.push_back(Line2(Vector2(L, 0) - Vector2(0, 0), Vector2(0, 0)));
		lines.push_back(Line2(Vector2(L, W) - Vector2(L, 0), Vector2(L, 0)));
		lines.push_back(Line2(Vector2(0, W) - Vector2(L, W), Vector2(L, W)));
		lines.push_back(Line2(Vector2(0, 0) - Vector2(0, W), Vector2(0, W)));

		for (int i = 0; i < n; i++) {
			double x1, y1, x2, y2;
			scanf("%lf %lf %lf %lf", &x1, &y1, &x2, &y2);
			Line2 tmpL(Vector2(x2, y2) - Vector2(x1, y1), Vector2(x1, y1));
			// Avoid duplication with the boundaries.
			bool duplicate = false;
			for (int j = 0; j < 4; j++) {
				if (isSameLine(tmpL, lines[j])) {
					duplicate = true;
				}
			}
			if (!duplicate) {
				lines.push_back(tmpL);
			}
		}

		for (int i = 0; i < m; i++) {
			double x, y, r;
			scanf("%lf %lf %lf", &x, &y, &r);
			disks.push_back(Circle(x, y, r));
		}

		pointsOnLines.resize(lines.size());

		for (int i = 0; i < lines.size(); i++) {
			for (int j = i + 1; j < lines.size(); j++) {
				if (isParallel(lines[i].v, lines[j].v)) {
					continue;
				}

				Point2 intersection = getLineIntersection(lines[i], lines[j]);
				if (isIntersectionOutOfRange(intersection, L, W)) {
					continue;
				}
				pointsOnLines[i].insert(intersection);
				pointsOnLines[j].insert(intersection);
			}
		}

		// Handle the four boundary lines in a special way, to rule out
		// the unbounded area. 
		for (int i = 0; i < 4; i++) {
			for (set<Point2>::iterator it = pointsOnLines[i].begin();
					 it != pointsOnLines[i].end();
					 it++) {
				updatesPointsOneDirection(it, pointsOnLines[i], i);
			}
		}

		for (int i = 4; i < lines.size(); i++) {
			for (set<Point2>::iterator it = pointsOnLines[i].begin();
					 it != pointsOnLines[i].end();
					 it++) {
				updatesPoints(it, pointsOnLines[i]);
			}
		}

		// Polygons in the area.
		vector<Polygon> polygons;

		while (true) {
			Polygon polygon;
			
			// Start from the lowest point.
			Point2 original;
			Point2 prev;
			Point2 current;
			
			if (!getStartPoint(original, current)) {
				break;
			}

			prev = original;
			polygon.push_back(original);

			while (current != original) {
				polygon.push_back(current);
				Point2 nextPoint;
				findNext(prev, current, adjMap[current], nextPoint);
				prev = current;
				current = nextPoint;
			}
			
			polygons.push_back(polygon);
		}

		for (int i = 0; i < disks.size(); i++) {
			vector<double> areas;
			for (int j = 0; j < polygons.size(); j++) {
				if (isDiskAndPolygonIntersecting(disks[i], polygons[j])) {
					areas.push_back(getPolygonArea(polygons[j]));
				}
			}
			sort(areas.begin(), areas.end());
			printf("%d", (int) areas.size());
			for (int j = 0; j < areas.size(); j++) {
				printf(" %.2lf", areas[j]);
			}
			printf("\n");
		}
		printf("\n");
	}

	return 0;
}

/*
Test input
2 2 10 10
0 4 10 4
4 0 4 10
2 4 2
4 4 1
2 1 10 10
0 0 10 10
10 0 0 10
1 1 1
4 2 10 10
0 4 10 4
1 0 7 10
5 10 10 1
2 10 6 0
3 7 3
5 5 5
3 3 10 10
0 4 10 4
1 0 7 10
5 10 10 1
6 3 2
5 4 1
7 2 2
3 3 10 10
0 4 10 4
1 0 7 10
4 10 10 1
6 3 2
5 4 1
7 2 2
1 1 10 10
0 0 10 0
5 5 5
3 2 10 10
0 4 10 4
2 10 10 2
0 2 8 10
5 4 3
4 5 1
4 1 10 10
0 3 10 7
0 7 10 3
3 10 7 0
3 0 7 10
5 2 1
8 1 10 10
0 0 0 10
0 0 10 0
10 10 0 10
10 10 10 0
0 0 0 10
0 0 10 0
10 10 0 10
10 10 10 0
1 1 1
0 0 0 0

Test inputs can also be generated by the following python 2.7 code.

import os

str_fromat = ""
counter = 0
limit = 20

# Shared point belongs the first line.
def gen_lines_vec(l, w) :
	side_points = [[] for _ in range(4)]
	for tmp in range(0, l):
		side_points[0].append((tmp, 0))
	for tmp in range(1, l + 1):
		side_points[2].append((tmp, w))
	for tmp in range(0, w):
		side_points[1].append((l, tmp))
	for tmp in range(1, w + 1):
		side_points[3].append((0, tmp))
	rtn = []

	for index0 in range(0, 4):
		for index1 in range(index0 + 1, 4):
			for point1 in side_points[index0]:
				for point2 in side_points[index1]:
					rtn.append((point1, point2))
	return rtn

def run_test_case(test_lines_vec):
	global str_fromat
	global counter
	str_tmp = ""
	for point_pair in test_lines_vec:
		str_tmp += "{} {} {} {}\n".format(
			point_pair[0][0],
			point_pair[0][1],
			point_pair[1][0],
			point_pair[1][1])
	counter = counter + 1
	print(counter)
	final_input = str_fromat.format(str_tmp)
	print(final_input)
	return

def gen_test_cases_and_run_internal(lines_vec, tmp_result, current_index, n):
	if len(tmp_result) == n:
		run_test_case(tmp_result)
		return

	if current_index >= len(lines_vec):
		return

	if counter > limit:
		return

	# Take the current element.
	tmp_result.append(lines_vec[current_index])
	gen_test_cases_and_run_internal(lines_vec, tmp_result, current_index + 1, n)

	# Do not take the current element.
	tmp_result.pop()
	gen_test_cases_and_run_internal(lines_vec, tmp_result, current_index + 1, n)
	
	return


def gen_test_cases_and_run(l, w, n):
	global str_fromat
	str_fromat = ""
	str_fromat += str(n) + " " + str(1) + " " + str(l) + " " + str(w) + os.linesep
	str_fromat += "{}"
	str_fromat += "0 0 100" + os.linesep
	str_fromat += "0 0 0 0" + os.linesep

	lines_vec = gen_lines_vec(l, w);
	tmp_result = []

	# Generate the combinations of n elements from len(lines_vec) elements.
	gen_test_cases_and_run_internal(lines_vec, tmp_result, 0, n)
	return

def main():
	global counter
	# for l in range(1, 10):
	# 	for w in range(1, 10):
	# 		for n in range(1, 2):
	# 			gen_test_cases_and_run(l, w, n)
	counter = 0
	# With L and W equals to 2, there are
	# 3 points on each side, so there are 3 * 3 * 6(side combinations) 54
	# lines(it is possible to dedup and remove some lines).
	# C(54, 20) = 54! / (20! * (54 - 20)!) is quite big.
	gen_test_cases_and_run(2, 2, 20)
	return

if __name__ == "__main__":
	main()
*/