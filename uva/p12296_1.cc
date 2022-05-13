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

bool isOnSegment(const Point2& p, const Point2& a1, const Point2& a2) {
  return dcmp(crossProduct(a1 - p, a2 - p)) == 0 && dcmp(dotProduct(a1-p, a2 - p)) < 0;
}

bool onLeft(const Line2& l, const Point2& p) {
	return crossProduct(l.v, p - l.p) > 0;
}

double getPolygonArea(const Polygon& polygon) {
  double area = 0;
  int n = polygon.size();
  for (int i = 1; i < n - 1; i++) {
  	area += crossProduct(polygon[i] - polygon[0],
  											 polygon[i + 1] - polygon[0]);
  }
  return area / 2;
}

Point2 getLineIntersection(const Point2& a, const Vector2& v,
	const Point2& b, const Vector2& w) {
  Vector2 u = a - b;
  double t = crossProduct(w, u) / crossProduct(v, w);
  return a + v * t;
}

// Cut the input polygon by the line (a->b), return the polygon on left.
Polygon cutPolygon(const Polygon& polygon, const Point2& a, const Point2& b) {
  Polygon newPolygon;
  int polygonSize = polygon.size();
  for (int i = 0; i < polygonSize; i++) {
  	Point2 c = polygon[i];
  	Point2 d = polygon[(i + 1) % polygonSize];
  	if (dcmp(crossProduct(b - a, c - a) >= 0)) {
  		// Point c is on the right of the cutting line.
  		newPolygon.push_back(c);
  	}
  	if (dcmp(crossProduct(b - a, d - c)) != 0) {
  		// The two lines are not parallel.
  		Point2 intersectionPoint = getLineIntersection(a, b - a, c, d - c);
  		if (isOnSegment(intersectionPoint, d, c)) {
  			newPolygon.push_back(intersectionPoint);
  		}
  	}
  }
  return newPolygon;
}

void cutPolygons(vector<Polygon>& polygons, const Line2& l) {
	vector<Polygon> newPolygons;
	for (const Polygon& polygon : polygons) {
		Polygon left = cutPolygon(polygon, l.p, l.p + l.v);
		Polygon right = cutPolygon(polygon, l.p + l.v, l.p);
		if (left.size() >= 3) {
			newPolygons.push_back(left);
		}
		if (right.size() >= 3) {
			newPolygons.push_back(right);
		}
	}
	polygons = newPolygons;
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

// Returns the intersection points only when there are two of them.
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

// Solution.
int main() {
	int n, m, L, W;
	while (scanf("%d%d%d%d", &n, &m, &L, &W) == 4) {
		if (n == 0 && m == 0 && L == 0 && W == 0) {
			break;
		}
		vector<Line2> lines;
		vector<Circle> disks;
		vector<Polygon> polygons;

		for (int i = 0; i < n; i++) {
			double x1, y1, x2, y2;
			scanf("%lf %lf %lf %lf", &x1, &y1, &x2, &y2);
			lines.push_back(Line2(Vector2(x2, y2) - Vector2(x1, y1), Vector2(x1, y1)));
		}

		for (int i = 0; i < m; i++) {
			double x, y, r;
			scanf("%lf %lf %lf", &x, &y, &r);
			disks.push_back(Circle(x, y, r));
		}

		Polygon original;
		original.push_back(Point2(0, 0));
		original.push_back(Point2(L, 0));
		original.push_back(Point2(L, W));
		original.push_back(Point2(0, W));
		polygons.push_back(original);

		// Get all the polygons in the area.
		for (int i = 0; i < n; i++) {
			cutPolygons(polygons, lines[i]);
		}

		// Find the intersections of polygons and disks.
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
