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

const double EPS = 1e-6;

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

struct Point3 {
	double x, y, z;
	Point3(double x = 0, double y = 0, double z = 0): x(x), y(y), z(z) {};
};

typedef Point3 Vector3;

Vector3 operator + (const Vector3& a, const Vector3& b) {
	return Vector3(a.x + b.x, a.y + b.y, a.z + b.z);
}

Vector3 operator - (const Vector3& a, const Vector3& b) {
	return Vector3(a.x - b.x, a.y - b.y, a.z - b.z);
}

Vector3 operator * (const Vector3& a, double p) {
	return Vector3(a.x * p, a.y * p, a.z * p);
}

Vector3 operator / (const Vector3& a, double p) {
	return Vector3(a.x / p, a.y / p, a.z / p);
}

double dotProduct(const Vector3& a, const Vector3& b) {
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

Vector3 crossProduct(const Vector3& a, const Vector3& b) {
  return Vector3(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x);
}

double lengthOfVector3(const Vector3& a) {
	return sqrt(dotProduct(a, a));
}

double angleBetweenVector3(const Vector3& a, const Vector3& b) {
	return acos(dotProduct(a, b) / lengthOfVector3(a) / lengthOfVector3(b));
}

double areaOfParallelogram(const Vector3& a, const Vector3& b, const Vector3& c) {
	return lengthOfVector3(crossProduct(b - a, c - a));
}

// This function assumes the point is on the plane defined by (p0, p1, p2).
bool isPointInTriangle(const Point3& p, const Point3& p0, const Point3& p1, const Point3& p2) {
  double area1 = areaOfParallelogram(p, p0, p1);
  double area2 = areaOfParallelogram(p, p1, p2);
  double area3 = areaOfParallelogram(p, p2, p0);
  return dcmp(area1 + area2 + area3 -  areaOfParallelogram(p0, p1, p2)) == 0;
}

// Check if the line segment defined by (a, b) is intersecting with the triangle
// defined by (p0, p1, p2). If yes, return the intersection point in p.
// This function does not consider the situation that the line segment is in the
// same plane as the triangle.
bool isTriangleLineSegIntersecting(const Point3& p0, const Point3& p1, const Point3& p2,
	const Point3& a, const Point3& b) {
	Vector3 n = crossProduct(p1 - p0, p2 - p0);
	// If the line seg is in the same plane as the triangle, they are considered not intersecting.
	if (dcmp(dotProduct(n, b - a)) == 0) {
		return false;
	}
	double t = dotProduct(n, p0 - a) / dotProduct(n, b - a);
	if (dcmp(t) < 0 || dcmp(t - 1) > 0) {
		return false;
	}
	Point3 p = a + (b - a) * t;
	return isPointInTriangle(p, p0, p1, p2);
}

Point3 triangle1[3];
Point3 triangle2[3];

bool isTrianglesIntersecting() {
	for (int i = 0; i < 3; i++) {
		if (isTriangleLineSegIntersecting(triangle2[0], triangle2[1], triangle2[2],
			triangle1[i], triangle1[(i + 1) % 3])) {
			return true;
		}
	}

	for (int i = 0; i < 3; i++) {
		if (isTriangleLineSegIntersecting(triangle1[0], triangle1[1], triangle1[2],
			triangle2[i], triangle2[(i + 1) % 3])) {
			return true;
		}
	}
	return false;
}

int main() {
	int test_cases;
	scanf("%d", &test_cases);
	while(test_cases--) {
		for (int i = 0; i < 3; i++) {
			scanf("%lf %lf %lf", &triangle1[i].x, &triangle1[i].y, &triangle1[i].z);
		}
		for (int i = 0; i < 3; i++) {
			scanf("%lf %lf %lf", &triangle2[i].x, &triangle2[i].y, &triangle2[i].z);
		}
		if (isTrianglesIntersecting()) {
			printf("1\n");
		} else {
			printf("0\n");
		}
	}
	return 0;
}