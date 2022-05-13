// p1453 / la4728
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

// The area formed by the 3 points.
double areaOfTriangle(const Point2& a, const Point2& b, const Point2& c) {
  return crossProduct(b-a, c-a);
}

// Calculate convex hull. Input output as array.
int getConvexHull(Point2* p, int n, Point2* ch) {
  sort(p, p + n);
  int m = 0;
  for (int i = 0; i < n; i++) {
  	while (m > 1 && crossProduct(ch[m-1] - ch[m-2], p[i] - ch[m-2]) <= 0) {
  		m--;
  	}
  	ch[m++] = p[i];
  }
  int k = m;
  for (int i = n-2; i >=0; i--) {
		while (m > k && crossProduct(ch[m-1] - ch[m-2], p[i] - ch[m-2]) <= 0) {
  		m--;
  	}
  	ch[m++] = p[i];
  }
  if (n > 1) m--;
  return m;
}

double normalizeAngle(double angle) {
	if (angle < 0) {
		return angle + TWO_PI;
	} else {
		while (angle > TWO_PI) {
			angle -= TWO_PI;
		}
		return angle;
	}
}

double getDistSqr(const Vector2& a, const Vector2& b) {
	double dx = (b.x - a.x);
	double dy = (b.y - a.y);

  return dx * dx + dy * dy;
}

// Simulates a rotating caliper, to get the farest points pair in a convex hull.
double rotatingCaliper(Point2* convexHull, int convexHullSize) {
	  double distSqr = 0;
	  double rotatedAngle = 0;

	  int lowIndex = 0, highIndex = 0;
	  for (int i = 0; i < convexHullSize; i++) {
	  	if (convexHull[i].y < convexHull[lowIndex].y) {
	  		lowIndex = i;
	  	}
	  	if (convexHull[i].y > convexHull[highIndex].y) {
	  		highIndex = i;
	  	}
	  }

	  // Define 2 lines, the one passing low has the direction of x axis positive,
	  // the one passing high has the direction of negative x axis.
	  double lowAng = 0;
	  double highAng = PI;

	  while (rotatedAngle <= PI) {
	  	int lowNext = (lowIndex + 1) % convexHullSize;
	  	Vector2 lowVec = convexHull[lowNext] - convexHull[lowIndex];
	  	while (dcmp(lowVec.x) == 0 && dcmp(lowVec.y) == 0) {
	  		lowNext = (lowNext + 1) % convexHullSize;
	  		lowVec = convexHull[lowNext] - convexHull[lowIndex];
	  	}

	  	int highNext = (highIndex + 1) % convexHullSize;
	  	Vector2 highVec = convexHull[highNext] - convexHull[highIndex];
	  	while (dcmp(highVec.x) == 0 && dcmp(highVec.y) == 0) {
	  		highNext = (highNext + 1) % convexHullSize;
	  		highVec = convexHull[highNext] - convexHull[highIndex];
	  	}

	  	distSqr = max(distSqr, getDistSqr(convexHull[lowIndex], convexHull[highIndex]));
	  	double lowToRotate = normalizeAngle(normalizeAngle(atan2(lowVec.y, lowVec.x)) - lowAng);
	  	double highToRotate = normalizeAngle(normalizeAngle(atan2(highVec.y, highVec.x)) - highAng);
	  	if (dcmp(lowToRotate - highToRotate) == 0) {
	  		distSqr = max(distSqr, getDistSqr(convexHull[lowIndex], convexHull[highNext]));
	  		distSqr = max(distSqr, getDistSqr(convexHull[lowNext], convexHull[highIndex]));
	  		lowIndex = lowNext;
	  		highIndex = highNext;
	  		lowAng += lowToRotate;
	  		highAng += highToRotate;
	  		rotatedAngle += lowToRotate;
	  	} else if (lowToRotate < highToRotate) {
	  		lowIndex = lowNext;
	  		lowAng += lowToRotate;
	  		highAng += lowToRotate;
	  		rotatedAngle += lowToRotate;
	  	} else {
	  		highIndex = highNext;
	  		lowAng += highToRotate;
	  		highAng += highToRotate;
	  		rotatedAngle += highToRotate;
	  	}
	  	lowAng = normalizeAngle(lowAng);
	  	highAng = normalizeAngle(highAng);
	  }
	  return distSqr;
}

// Simulates a rotating caliper, to get the farest points pair in a convex hull.
double rotatingCaliperSimplified(Point2* convexHull, int convexHullSize) {
	double distSqr = 0;
  // The opposite point.
  int k = 2;

  for (int i = 0; i < convexHullSize; i++) {
  	while (
  		areaOfTriangle(
  			convexHull[k],
  			convexHull[i],
  			convexHull[(i + 1) % convexHullSize])
  		< areaOfTriangle(
  					convexHull[(k + 1) % convexHullSize],
  					convexHull[i],
  					convexHull[(i + 1) % convexHullSize])) {
  		k = (k + 1) % convexHullSize; 
  	}

  	distSqr = max(
      max(distSqr, getDistSqr(convexHull[k], convexHull[i])),
      getDistSqr(convexHull[k], convexHull[(i + 1) % convexHullSize]));
  }
  return distSqr;
}

#define MAX_INPUT_SIZE  404000

Point2 points[MAX_INPUT_SIZE];
Point2 convexHull[MAX_INPUT_SIZE];

int main() {
	int test_cases;
	scanf("%d", &test_cases);
	while(test_cases--) {
		int squares;
		scanf("%d", &squares);
		double x, y, w;
		for (int i = 0; i < squares; i++) {
			scanf("%lf%lf%lf", &x, &y, &w);
			points[i * 4 + 0].x = x;
			points[i * 4 + 0].y = y;
			points[i * 4 + 1].x = x + w;
			points[i * 4 + 1].y = y;
			points[i * 4 + 2].x = x;
			points[i * 4 + 2].y = y + w;
			points[i * 4 + 3].x = x + w;
			points[i * 4 + 3].y = y+w;
		}
		int convexHullSize = getConvexHull(points, squares * 4, convexHull);
	  printf("%d\n", (int) rotatingCaliper(convexHull, convexHullSize));
	}
	return 0;
}