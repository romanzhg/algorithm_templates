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

double lengthOfVector2(const Vector2& a) {
	return sqrt(dotProduct(a, a));
}

// Sign of the product represents the relative angle between a and b.
double crossProduct(const Vector2& a, const Vector2& b) {
  return a.x * b.y - a.y * b.x;
}

// A line is represented by p + tv.
struct Line2 {
	// Direction vector.
	Vector2 v;
	Point2 p;

	Line2() {
		v = Vector2();
		p = Point2();
	};
	Line2(Vector2 v, Point2 p) : v(v), p(p) {};

	// Get the point at p + tv.
	Point2 point(double t) {
		return p + multiplyScalar(v, t);
	};

	Vector2 getDirection() {
		return v;
	};

	Vector2 getNormalDirection() {
		return Vector2(v.y, -v.x);
	};
};

int getConvexHull(Point2* p, int n, Point2* ch) {
  sort(p, p + n);
  int m = 0;
  for (int i = 0; i < n; i++) {
  	while (m > 1 && crossProduct(ch[m - 1] - ch[m - 2], p[i] - ch[m - 2]) <= 0) {
  		m--;
  	}
  	ch[m++] = p[i];
  }
  int k = m;
  for (int i = n-2; i >=0; i--) {
		while (m > k && crossProduct(ch[m - 1] - ch[m - 2], p[i] - ch[m - 2]) <= 0) {
  		m--;
  	}
  	ch[m++] = p[i];
  }
  if (n > 1) m--;
  return m;
}

// Solution.
#define MAX_INPUT_SIZE 10000

Point2 points[MAX_INPUT_SIZE];
Point2 convexHull[MAX_INPUT_SIZE];

int main() {
	int test_cases;
	int case_num = 0;
	scanf("%d", &test_cases);
	while (test_cases--) {
		int lines;
		scanf("%d", &lines);
		double xSum = 0, ySum = 0;
		double result = 1e9;
		for (int i = 0; i < lines; i++) {
			scanf("%lf %lf", &points[i].x, &points[i].y);
			xSum += points[i].x;
			ySum += points[i].y;
		}
		int convexHullSize = getConvexHull(points, lines, convexHull);
		if (convexHullSize <= 2) {
			result = 0;
		} else {
			for (int i = 0; i < convexHullSize; i++) {
				Line2 l(convexHull[(i + 1) % convexHullSize] - convexHull[i], convexHull[i]);
				double A, B, C;
				A = l.getNormalDirection().x;
				B = l.getNormalDirection().y;
				C = -(A * l.p.x + B * l.p.y);
				result = min(result, fabs(A * xSum + B * ySum + C * lines) / sqrt(A * A + B * B));
			}
		}
		printf("Case #%d: %.3lf\n", ++case_num, result / lines);
	}
	return 0;
}
