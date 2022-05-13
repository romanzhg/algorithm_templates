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

double dotProduct(const Vector2& a, const Vector2& b) {
  return a.x * b.x + a.y * b.y;
}

double lengthOfVector2(const Vector2& a) {
	return sqrt(dotProduct(a, a));
}

double angle(const Vector2& a, const Vector2& b) {
	return acos(dotProduct(a, b) / lengthOfVector2(a) / lengthOfVector2(b));
}

// Sign of the product represents the relative angle between a and b.
double crossProduct(const Vector2& a, const Vector2& b) {
  return a.x * b.y - a.y * b.x;
}

// The area formed by the 3 points.
double area(const Point2& a, const Point2& b, const Point2& c) {
  return crossProduct(b-a, c-a);
}

Vector2 rotateVector2(const Vector2& a, double rad) {
	return Vector2(a.x * cos(rad) - a.y * sin(rad), a.x * sin(rad) + a.y * cos(rad));
}

// Assumes there are an intersection.
// Line 1 represented by P+tv (v is the direction vector).
// Line 2 represented by Q+sw (w is the direction vector).
Point2 getLineIntersection(const Point2& P, const Vector2& v,
	const Point2& Q, const Vector2& w) {
  Vector2 u = P - Q;
  double t = crossProduct(w, u) / crossProduct(v, w);
  return P + multiplyScalar(v, t);
}

double angleBetweenVector2(const Vector2& a, const Vector2& b) {
	return acos(dotProduct(a, b) / lengthOfVector2(a) / lengthOfVector2(b));
}

// The solution.
Point2 readPoint() {
	int x, y;
	scanf("%d %d", &x, &y);
  return Point2(x, y);
}

Point2 getD(const Point2& a, const Point2& b, const Point2& c) {
  Vector2 v1 = c - b;
  double angle1 = angleBetweenVector2(a - b, v1);

  Vector2 v2 = b - c;
  double angle2 = angleBetweenVector2(a - c, v2);

  Vector2 direction1 = rotateVector2(v1, angle1 / 3);
  Vector2 direction2 = rotateVector2(v2, - angle2 / 3);
  return getLineIntersection(b, direction1, c, direction2);
}

int main() {
	int test_case_count;
	Point2 a, b, c, d, e, f;
	scanf("%d", &test_case_count);

	while (test_case_count--) {
		a = readPoint();
		b = readPoint();
		c = readPoint();
		d = getD(a, b, c);
		e = getD(b, c, a);
		f = getD(c, a, b);
		printf("%.6lf %.6lf %.6lf %.6lf %.6lf %.6lf\n", d.x, d.y, e.x, e.y, f.x, f.y);
	}
	return 0;
}
