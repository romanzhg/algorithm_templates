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

struct Point2 {
  double x, y;
  Point2() : x(0), y(0) {};
  Point2(double x, double y) : x(x), y(y) {};
};

typedef Point2 Vector2;

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

Vector2 operator + (const Vector2& a, const Vector2& b) {
  return Vector2(a.x + b.x, a.y + b.y);
}

Vector2 operator - (const Vector2& a, const Vector2& b) {
  return Vector2(a.x - b.x, a.y - b.y);
}

bool operator < (const Point2& a, const Point2& b) {
	return a.x < b.x || (a.x == b.x && a.y < b.y);
}

bool operator == (const Point2& a, const Point2& b) {
	return dcmp(a.x - b.x) == 0 && dcmp(a.y - b.y) == 0;
}

Vector2 multiplyScalar(const Vector2& v, double s) {
  return Vector2(v.x * s, v.y * s);
}

double dotProduct(const Vector2& a, const Vector2& b) {
  return a.x * b.x + a.y * b.y;
}

// Sign of the product represents the relative angle between a and b.
double crossProduct(const Vector2& a, const Vector2& b) {
  return a.x * b.y - a.y * b.x;
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

// Returns true if the two line segments intersect with each other.
// This function assumes the intersection point will not be the endpoint of either
// line segment. If the intersection point could be the endpoints, run additional
// judgements.
bool isSegmentProperIntersecting(const Point2& a1, const Point2& a2,
	const Point2& b1, const Point2& b2) {
	double c1 = crossProduct(a2 - a1, b1 - a1),
	    c2 = crossProduct(a2 - a1, b2 - a1),
	    c3 = crossProduct(b2 - b1, a1 - b1),
	    c4 = crossProduct(b2 - b1, a2 - b1);
	return dcmp(c1) * dcmp(c2) < 0 && dcmp(c3) * dcmp(c4) < 0;
}

// Returns true if point p is on the line segment (a1, a2).
// Plese note the endpoints are not included inthe line segment.
bool isOnSegment(const Point2& p, const Point2& a1, const Point2& a2) {
  return dcmp(crossProduct(a1 - p, a2 - p)) == 0 && dcmp(dotProduct(a1 - p, a2 - p)) < 0;
}

// Solution.
// Define should always use a final number.
#define MAX_NODE 310
Point2 points[MAX_NODE], vertexes[MAX_NODE * MAX_NODE];

int main() {
	int n;
	int case_number = 0;
	while (scanf("%d", &n) == 1) {
		if (n == 0) {
			break;
		}
		for (int i = 0; i < n; i++) {
			scanf("%lf %lf", &points[i].x, &points[i].y);
			vertexes[i] = points[i];
		}
		n--;
		int edgeCount = n, vertexCount = n;

		// Scan through all the line segment pairs.
		for (int i = 0; i < n; i++) {
			for (int j = i+1; j < n; j++) {
				if (isSegmentProperIntersecting(points[i], points[i + 1], points[j], points[j + 1])) {
					vertexes[vertexCount++] = getLineIntersection(
											points[i], points[i + 1] - points[i],
											points[j], points[j + 1] - points[j]);
				}
			}
		}

		sort(vertexes, vertexes + vertexCount);
		vertexCount = unique(vertexes, vertexes + vertexCount) - vertexes;
		// Iterate through all line segments, and vertexes, if one vertex is on a line
		// segment, add edge count by 1.
		for (int i = 0; i < vertexCount; i++) {
			for (int j = 0; j < n; j++) {
				if (isOnSegment(vertexes[i], points[j], points[j + 1])) {
					edgeCount++;
				}
			}
		}

		printf("Case %d: There are %d pieces.\n", ++case_number, edgeCount + 2 - vertexCount);
	}
	return 0;
}