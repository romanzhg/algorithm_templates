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
using namespace std;

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

Vector2 multiplyScalar(const Vector2& v, double s) {
  return Vector2(v.x * s, v.y * s);
}

Vector2 divideScalar(const Vector2& v, double s) {
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

// Sign of the product represents the relative angle between a and b.
double crossProduct(const Vector2& a, const Vector2& b) {
  return a.x * b.y - a.y * b.x;
}

// Returns true if point p is on the line segment (a1, a2).
// Endpoints included.
bool isOnSegment(const Point2& p, const Point2& a1, const Point2& a2) {
  return dcmp(crossProduct(a1 - p, a2 - p)) == 0 && dcmp(dotProduct(a1-p, a2 - p)) < 0;
}

vector<Point2> getConvexHull(vector<Point2> points) {
  sort(points.begin(), points.end());
  int pointCount = points.size();

  vector<Point2> convexHull;
  for (int i = 0; i < pointCount; i++) {
    while (convexHull.size() > 1) {
      int lastIndex = convexHull.size() - 1;
      if (crossProduct(convexHull[lastIndex] - convexHull[lastIndex - 1],
        points[i] - convexHull[lastIndex - 1]) <= 0) {
        convexHull.pop_back();
      } else {
        break;
      }
    }
    convexHull.push_back(points[i]);
  }

  int lowerChainSize = convexHull.size();
  for (int i = pointCount - 2; i >= 0; i--) {
    while (convexHull.size() > lowerChainSize) {
      int lastIndex = convexHull.size() - 1;
      if (crossProduct(convexHull[lastIndex] - convexHull[lastIndex - 1],
        points[i] - convexHull[lastIndex - 1]) <= 0) {
        convexHull.pop_back();
      } else {
        break;
      }
    }
    convexHull.push_back(points[i]);
  }
  // Do not put the original point twice.
  if (pointCount > 1) {
    convexHull.pop_back();
  }
  return convexHull;
}

typedef vector<Point2> Polygon;

int isPointInOrOnPolygon(const Point2& p, const Polygon& poly) {
	int wn = 0;  // winding number
	int polySize = poly.size();
	for (int i = 0; i < polySize; i++) {
		if (isOnSegment(p, poly[i], poly[(i+1) % polySize])) {
			return true;
		}
		int k = dcmp(crossProduct(poly[(i+1) % polySize] - poly[i], p - poly[i]));
		int d1 = dcmp(poly[i].y - p.y);
		int d2 = dcmp(poly[(i+1) % polySize].y - p.y);
		if (k > 0 && d1 <= 0 && d2 > 0) wn++;
		if (k < 0 && d2 <= 0 && d1 > 0) wn--;
	}
	if (wn != 0) {
		return true;
	} else {
		return false;
	}
}

// Returns true if the two line segments intersect with each other.
// This function assumes the intersection point will not be the endpoint of either
// line segment. If the intersection point could be the endpoints, run additional
// judgements.
bool areSegmentsProperIntersecting(const Point2& a1, const Point2& a2,
	const Point2& b1, const Point2& b2) {
	double c1 = crossProduct(a2 - a1, b1 - a1),
	    c2 = crossProduct(a2 - a1, b2 - a1),
	    c3 = crossProduct(b2 - b1, a1 - b1),
	    c4 = crossProduct(b2 - b1, a2 - b1);
	return dcmp(c1) * dcmp(c2) < 0 && dcmp(c3) * dcmp(c4) < 0;
}

int main() {
	int m, c;
	while (scanf("%d %d", &m, &c)) {
		if (m == 0 && c == 0) {
			break;
		}
		vector<Point2> mPoints, cPoints;
		double x, y;
		for (int i = 0; i < m; i++) {
			scanf("%lf %lf", &x, &y);
			mPoints.push_back(Point2(x, y));
		}
		for (int i = 0; i < c; i++) {
			scanf("%lf %lf", &x, &y);
			cPoints.push_back(Point2(x, y));
		}
		vector<Point2> mConvexHull(getConvexHull(mPoints));
		vector<Point2> cConvexHull(getConvexHull(cPoints));

		bool valid = true;
		// Check for intersections.
		for (int i = 0; i < mConvexHull.size(); i++) {
			for (int j = 0; j < cConvexHull.size(); j++) {
				Point2& mPoints2 = mConvexHull[(i + 1) % mConvexHull.size()];
				Point2& mPoints1 = mConvexHull[i];
				Point2& cPoints2 = cConvexHull[(j + 1) % cConvexHull.size()];
				Point2& cPoints1 = cConvexHull[j];

				if (areSegmentsProperIntersecting(mPoints2, mPoints1, cPoints2, cPoints1)
						|| isOnSegment(mPoints2, cPoints2, cPoints1)
						|| isOnSegment(mPoints1, cPoints2, cPoints1)
						|| isOnSegment(cPoints2, mPoints2, mPoints1)
						|| isOnSegment(cPoints1, mPoints2, mPoints1)) {
					valid = false;
					goto endloop;
				}
			}
		}

		// Check if m is in c or c is in m.
		if (isPointInOrOnPolygon(mConvexHull[0], cConvexHull)) {
			valid = false;
		}
		if (isPointInOrOnPolygon(cConvexHull[0], mConvexHull)) {
			valid = false;
		}
		endloop:
		if (valid) {
			printf("Yes\n");
		} else {
			printf("No\n");
		}
	}
	return 0;
}