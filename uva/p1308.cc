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
const double EPS = 5*(1e-13);

struct Point2 {
  double x, y;
  Point2() : x(0), y(0) {};
  Point2(double x, double y) : x(x), y(y) {};
};

typedef Point2 Vector2;

struct Circle {
  Point2 c;
  double r;
  Circle() : c(Point2()), r(0) {};
  Circle(Point2 c, double r) : c(c), r(r) {};
  Circle(double x, double y, double radius) {
    c = Point2(x, y);
    r = radius;
  }

  // Get the point at radian a of the circle.
  Point2 point2(double a) {
    return Point2(c.x + cos(a) * r, c.y + sin(a) * r);
  };
};

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

// Angle of an vector. In radian, from x axis.
double angleOfVector2(const Vector2& v) {
  return atan2(v.y, v.x);
}

double normalizeAngle(double rad) {
  return rad - PI * 2 * floor(rad / (PI * 2));
}

// Update circle to circle intersections, put the angle(in radian) of the
// intersections points of c1 into intersections.
void updateCircleCircleIntersection(const Circle& c1, const Circle& c2,
  vector<double>& intersections) {
  double dist = lengthOfVector2(c1.c - c2.c);
  if (dcmp(dist) == 0) {
    // Centered at the same point.
    return;
  }
  if (dcmp(c1.r + c2.r - dist) < 0) {
    // Away from each other.
    return;
  }
  if (dcmp(fabs(c1.r - c2.r) - dist) > 0) {
    // contained in each other.
    return;
  }
  double baseAngle = angleOfVector2(c2.c - c1.c);
  double diffAngle = acos((c1.r * c1.r + dist * dist - c2.r * c2.r) / (2 * c1.r * dist));

  intersections.push_back(normalizeAngle(baseAngle - diffAngle));
  intersections.push_back(normalizeAngle(baseAngle + diffAngle));
}

bool isInCircle(const Point2& p, const Circle& c) {
  return dcmp(c.r - lengthOfVector2(p - c.c)) > 0;
}

void updateVisibility(const Point2& p, bool* isVisible, Circle* circles, int count) {
  int visibleIndex = - 1;
  for (int i = count - 1; i >= 0; --i) {
    if (lengthOfVector2(p - circles[i].c) < circles[i].r) {
      visibleIndex = i;
      break;
    }
  }
  if (visibleIndex == -1) {
    return;
  } else {
    isVisible[visibleIndex] = true;
  }
}

// Solution.
#define INPUT_SIZE 110
Circle circles[INPUT_SIZE];
bool isVisible[INPUT_SIZE];

int main() {
  int circleCount;
  while (scanf("%d", &circleCount) == 1 && circleCount != 0) {
    vector<double> intersections[circleCount];
    for (int i = 0; i < circleCount; i++) {
      double x, y, r;
      scanf("%lf %lf %lf", &x, &y, &r);
      circles[i] = Circle(x, y, r);

      isVisible[i] = false;
      intersections[i].clear();
      intersections[i].push_back(0);
      intersections[i].push_back(2 * PI);
    }

    for (int i = 0; i < circleCount; i++) {
      for (int j = 0; j < circleCount; j++) {
        updateCircleCircleIntersection(circles[i], circles[j], intersections[i]);
      }
    }

    for (int i = 0; i < circleCount; i++) {
      sort(intersections[i].begin(), intersections[i].end());
      for (int j = 0; j < intersections[i].size() - 1; j++) {
        double angle = (intersections[i][j+1] + intersections[i][j]) / 2.0;
        double r1 = circles[i].r + 2 * EPS;
        double r2 = circles[i].r - 2 * EPS;

        Point2 p1 = Point2(circles[i].c.x + cos(angle)*r1, circles[i].c.y + sin(angle)*r1);
        Point2 p2 = Point2(circles[i].c.x + cos(angle)*r2, circles[i].c.y + sin(angle)*r2);

        updateVisibility(p1, isVisible, circles, circleCount);
        updateVisibility(p2, isVisible, circles, circleCount);
      }
    }

    int visibleCircleCount = 0;
    for (int i = 0; i < circleCount; i++) { 
      if (isVisible[i]) visibleCircleCount++;
    }
    printf("%d\n", visibleCircleCount);
  }
  return 0;
}