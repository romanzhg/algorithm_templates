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

const double PI = acos(-1);
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

double lengthOfVector2(const Vector2& a) {
	return sqrt(dotProduct(a, a));
}

// Sign of the product represents the relative angle between a and b.
double crossProduct(const Vector2& a, const Vector2& b) {
  return a.x * b.y - a.y * b.x;
}

double degreeToRadian(double degree) {
	return degree / 180.0 * PI;
}

// Counter clockwise. A negative rad means clockwise.
Vector2 rotateVector2(const Vector2& a, double rad) {
	return Vector2(a.x * cos(rad) - a.y * sin(rad), a.x * sin(rad) + a.y * cos(rad));
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

// Get the directed area of a polygon.
// Input is an array pointed by p with size n.
// Points in the array needs to be adjacent to each other. If they are in counter-clock
// order, the area is positive, otherwise the area is negative.
double polygonArea(const vector<Point2>& p) {
	int n = p.size();
  double area = 0;
  for (int i = 1; i < n - 1; i++) {
  	area += crossProduct(p[i] - p[0], p[i + 1] - p[0]);
  }
  return area / 2;
}

// Solution.
vector<Point2> points;
double totalBoardsArea;

void addCornerPoints(double x, double y, double w, double h, double angle) {
	Vector2 a = Vector2(0, h / 2);
	Vector2 b = Vector2(- w / 2, 0);
	a = rotateVector2(a, angle);
	b = rotateVector2(b, angle);
	Point2 center(x, y);
	points.push_back(center + a + b);
	points.push_back(center + a - b);
	points.push_back(center - a + b);
	points.push_back(center - a - b);
}

int main() {
	int test_cases;
	scanf("%d", &test_cases);
	while (test_cases--) {
		points.clear();
		totalBoardsArea = 0;
		int boards;
		scanf("%d", &boards);
		double x, y, w, h, a;
		for (int i = 0; i < boards; i++) {
			scanf("%lf %lf %lf %lf %lf", &x, &y, &w, &h, &a);
			a = -degreeToRadian(a);
			totalBoardsArea += w * h;
			addCornerPoints(x, y, w, h, a);
		}
		// 1. Get the convex hull.
		// 2. Get the area of convex hull.
		// 3. Get the area percentage.
		double area = polygonArea(getConvexHull(points));
		printf("%.1f %%\n", totalBoardsArea * 100 / area);
	}
	return 0;
}