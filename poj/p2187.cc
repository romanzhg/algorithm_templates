#include <cstdio>
#include <cstdlib>
#include <climits>
#include <cmath>
#include <queue>
#include <vector>
#include <utility>
#include <algorithm>
#include <functional>
#include <cstring>
#include <string>
#include <map>
#include <set>

using namespace std;

const double PI = acos(-1);
const double EPS = 5*(1e-13);

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

// This gives an undirected angle. 0 ~ pi.
double angleBetweenVector2(const Vector2& a, const Vector2& b) {
  return acos(dotProduct(a, b) / lengthOfVector2(a) / lengthOfVector2(b));
}

// Sign of the product represents the relative angle between a and b.
double crossProduct(const Vector2& a, const Vector2& b) {
  return a.x * b.y - a.y * b.x;
}

// The area formed by the 3 points.
double areaOfTriangle(const Point2& a, const Point2& b, const Point2& c) {
  return crossProduct(b-a, c-a);
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
  if (pointCount > 1) {
    convexHull.pop_back();
  }
  return convexHull;
}

// Solution.
#define MAX_INPUT_SIZE 50000

double getDistSqr(const Vector2& a, const Vector2& b) {
	double dx = (b.x - a.x);
	double dy = (b.y - a.y);

  return dx * dx + dy * dy;
}

int main() {
	int inputSize;
  scanf("%d", &inputSize);

  vector<Point2> points;

  for (int i = 0; i < inputSize; i++) {
    double x, y;
  	scanf("%lf %lf", &x, &y);
    points.push_back(Point2(x, y));
  }

  vector<Point2> convexHull(getConvexHull(points));

  double distSqr = 0;

  // The opposite point.
  int k = 2;
  int convexHullSize = convexHull.size();
  for (int i = 0; i < convexHullSize - 1; i++) {
  	while (areaOfTriangle(convexHull[k], convexHull[i], convexHull[i + 1])
  		< areaOfTriangle(convexHull[(k + 1) % convexHullSize], convexHull[i], convexHull[i + 1])) {
  		k = (k + 1) % convexHullSize; 
  	}
  	distSqr = max(
      max(distSqr, getDistSqr(convexHull[k], convexHull[i])),
      getDistSqr(convexHull[k], convexHull[i+1]));
  }

  printf("%d\n", (int) round(distSqr));
  return 0;
}