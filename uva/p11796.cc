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

Vector2 divideScalar(const Vector2& v, double s) {
  return Vector2(v.x / s, v.y / s);
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

// Distance from p to the line segment (a, b).
double distanceToSegment(const Point2& p, const Point2& a, const Point2& b) {
	// Depends on the equal comparator defined above.
  if (a == b) {
  	return lengthOfVector2(p - a);
  }
  Vector2 v1 = b - a, v2 = p - a, v3 = p - b;
  if (dcmp(dotProduct(v1, v2)) < 0) {
  	return lengthOfVector2(v2);
  } else if (dcmp(dotProduct(v1, v3)) > 0) {
  	return lengthOfVector2(v3);
  } else {
  	return fabs(crossProduct(v1, v2) / lengthOfVector2(v1));
  }
}

// The input cannot be a zero vector.
Vector2 getNorm(const Vector2& a) {
  double l = lengthOfVector2(a);
  return divideScalar(a, l);
}

#define MAX_SIZE 100
Point2 aPoints[MAX_SIZE], bPoints[MAX_SIZE];
double minDist, maxDist;

// Max distance only happens at end points.
// Assume a doesn't move, b moves.
void update(const Point2& aLoc, const Point2& bLoc, const Vector2& movement) {
	minDist = min(minDist, distanceToSegment(aLoc, bLoc, bLoc + movement));
	maxDist = max(maxDist, lengthOfVector2(aLoc - bLoc));
	maxDist = max(maxDist, lengthOfVector2(aLoc - (bLoc + movement)));
}

int main() {
	int testCases;
	int currentCase = 0;
	scanf("%d", &testCases);
	while(testCases--) {
		int aPointCount, bPointCount;
		scanf("%d %d", &aPointCount, &bPointCount);
		for (int i = 0; i < aPointCount; i++) {
			scanf("%lf %lf", &aPoints[i].x, &aPoints[i].y);
		}
		for (int i = 0; i < bPointCount; i++) {
			scanf("%lf %lf", &bPoints[i].x, &bPoints[i].y);
		}

		double aTotalDist = 0, bTotalDist = 0;
		for (int i = 0; i < aPointCount - 1; i++) {
			aTotalDist += lengthOfVector2(aPoints[i + 1] - aPoints[i]);
		}
		for (int i = 0; i < bPointCount - 1; i++) {
			bTotalDist += lengthOfVector2(bPoints[i + 1] - bPoints[i]);
		}

		int aIndex = 0, bIndex = 0;
		minDist = 1e9, maxDist = -1e9;
		Point2 aCurrentLocation = aPoints[aIndex], bCurrentLocation = bPoints[bIndex];
		while (aIndex < (aPointCount - 1) && bIndex < (bPointCount - 1)) {
			double aDist = lengthOfVector2(aPoints[aIndex + 1] - aCurrentLocation);
			double bDist = lengthOfVector2(bPoints[bIndex + 1] - bCurrentLocation);
			double percentageToFinish = min(aDist/aTotalDist, bDist/bTotalDist);

			Vector2 aMovement = multiplyScalar(
				getNorm(aPoints[aIndex + 1] - aPoints[aIndex]), percentageToFinish * aTotalDist);
			Vector2 bMovement = multiplyScalar(
				getNorm(bPoints[bIndex + 1] - bPoints[bIndex]), percentageToFinish * bTotalDist);

			update(aCurrentLocation, bCurrentLocation, bMovement - aMovement);

			// Advance a and b.
			aCurrentLocation = aCurrentLocation + aMovement;
			bCurrentLocation = bCurrentLocation + bMovement;

			if (aCurrentLocation == aPoints[aIndex + 1]) {
				aIndex++;
			}
			if (bCurrentLocation == bPoints[bIndex + 1]) {
				bIndex++;
			}
		}
		printf("Case %d: %d\n", ++currentCase, (int) round(maxDist - minDist));
	}
	return 0;
}
