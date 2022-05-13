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
typedef vector<Point2> Polygon;

Vector2 operator + (const Vector2& a, const Vector2& b) {
  return Vector2(a.x + b.x, a.y + b.y);
}

Vector2 operator - (const Vector2& a, const Vector2& b) {
  return Vector2(a.x - b.x, a.y - b.y);
}

Vector2 operator * (const Vector2& v, double s) {
  return Vector2(v.x * s, v.y * s);
}

Vector2 operator / (const Vector2& v, double s) {
  return Vector2(v.x / s, v.y / s);
}

// Two versions of comparator. The dcmp version is used when
// we need to dedup points, for example when multiple lines
// intersect at the same point.
bool operator < (const Point2& a, const Point2& b) {
	return a.x < b.x || (a.x == b.x && a.y < b.y);
}

bool operator < (const Point2& a, const Point2& b) {
	if (dcmp(b.x - a.x) == 0) {
		if (dcmp(b.y - a.y) == 0) {
			return false;
		} else {
			return a.y < b.y;
		}
	} else {
		return a.x < b.x;
	}
}

bool operator == (const Point2& a, const Point2& b) {
	return dcmp(a.x - b.x) == 0 && dcmp(a.y - b.y) == 0;
}

bool operator != (const Point2& a, const Point2& b) {
  return !(dcmp(b.x - a.x) == 0 && dcmp(b.y - a.y) == 0);
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

struct Circle {
  Point2 c;
  double r;
  Circle(): c(Point2()), r(0) {};
  Circle(double x, double y, double radius) {
  	c = Point2(x, y);
  	r = radius;
  };
  Circle(Point2 c, double r) : c(c), r(r) {};

  // Get the point at radian a of the circle.
  Point2 point2(double a) {
  	return Point2(c.x + cos(a) * r, c.y + sin(a) * r);
  };
};

// A line is represented by p + tv.
struct Line2 {
	// Direction vector.
	Vector2 v;
	Point2 p;
	// The angle from positive x axis.
	// Positive for rotating counter-clockwise.
	// Negative for rotating clockwise.
	double ang;

	Line2() {};
	Line2(Vector2 v, Point2 p): v(v), p(p) {
		ang = atan2(v.y, v.x);
	};

	// Get the point at p + tv.
	Point2 point(double t) const {
		return p + v * t;
	};

	Vector2 getDirection() const {
		return v;
	};

	Vector2 getNormalDirection() const {
		return Vector2(-v.y, v.x);
	};
};

bool operator < (const Line2& a, const Line2& b) {
	return a.ang < b.ang;
}

// Point p is on left of line l.
bool onLeft(const Line2& l, const Point2& p) {
	return crossProduct(l.v, p - l.p) > 0;
}

bool isParallel(const Vector2& a, const Vector2& b) {
  return fabs(crossProduct(a, b)) < EPS;
}

// This gives an undirected angle. [0, pi].
double angleBetweenVector2(const Vector2& a, const Vector2& b) {
	return acos(dotProduct(a, b) / lengthOfVector2(a) / lengthOfVector2(b));
}

// Area of the parallelogram defined by vector (a->b) and vector (a->c).
// The area is directional.
double areaOfParallelogram(const Point2& a, const Point2& b, const Point2& c) {
  return crossProduct(b - a, c - a);
}

// Counter clockwise. A negative rad means clockwise.
Vector2 rotateVector2(const Vector2& a, double rad) {
	return Vector2(a.x * cos(rad) - a.y * sin(rad), a.x * sin(rad) + a.y * cos(rad));
}

// Get a normal vector of the input vection, with unit length.
// The input cannot be a zero vector.
Vector2 getNormal(const Vector2& a) {
	double l = lengthOfVector2(a);
	return Vector2(-a.y / l, a.x / l);
}

// Get the intersection of two lines.
// Assumes there is an intersection.
// Line 1 represented by a + tv (v is the direction vector).
// Line 2 represented by b + sw (w is the direction vector).
Point2 getLineIntersection(const Point2& a, const Vector2& v,
	const Point2& b, const Vector2& w) {
  Vector2 u = a - b;
  double t = crossProduct(w, u) / crossProduct(v, w);
  return a + v * t;
}

// Get the intersection of two lines.
// Assumes there is an intersection.
Point2 getLineIntersection(const Line2& a, const Line2& b) {
  Vector2 u = a.p - b.p;
  double t = crossProduct(b.v, u) / crossProduct(a.v, b.v);
  return a.p + a.v * t;
}

double distanceToLine(const Point2& p, const Point2& a, const Point2& b) {
  Vector2 v1 = b - a;
  Vector2 v2 = p - a;
  return fabs(crossProduct(v1, v2) / lengthOfVector2(v1));
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

// Get the projection of point p on the line defined by a and b.
Point2 lineProjection(const Point2& p, const Point2& a, const Point2& b) {
  Vector2 v = b - a;
  return a + v * (dotProduct(v, p - a) / dotProduct(v, v));
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

// Returns true if point p is on the line segment (a1, a2).
// Endpoints not included.
bool isOnSegment(const Point2& p, const Point2& a1, const Point2& a2) {
  return dcmp(crossProduct(a1 - p, a2 - p)) == 0 && dcmp(dotProduct(a1-p, a2 - p)) < 0;
}

// Get the directed area of a polygon.
// Input is an array pointed by p with size n.
// Points in the array needs to be adjacent to each other. If they are in counter-clock
// order, the area is positive, otherwise the area is negative.
double getPolygonArea(Point2* p, int n) {
  double area = 0;
  for (int i = 1; i < n - 1; i++) {
  	area += crossProduct(p[i] - p[0], p[i + 1] - p[0]);
  }
  return area / 2;
}

double getPolygonArea(const Polygon& polygon) {
  double area = 0;
  int n = polygon.size();
  for (int i = 1; i < n - 1; i++) {
  	area += crossProduct(polygon[i] - polygon[0],
  											 polygon[i + 1] - polygon[0]);
  }
  return area / 2;
}

// Angle of an vector. In radian, from positive x axis.
// The value is in range (-PI, PI].
// Positive for counter-clockwise rotation from the positive x axis, negative for the
// clockwise rotation.
// Please note the value range of atan2 is different from the value range of atan.
// Since the signs can be used to calculate the quadrant.
double angleOfVector2(const Vector2& v) {
	if (v.x == 0 && v.y == 0) {
		return 0;
	}
	return atan2(v.y, v.x);
}

double degreeToRadian(double degree) {
	return degree / 180.0 * PI;
}

/************************************************************************/
/************************************************************************/
/************************************************************************/

int getLineCircleIntersection(const Line2& line, const Circle& circle,
	/* outputs: */ double& t1, double& t2, vector<Point2>& sol) {
	double a = line.v.x,
		b = line.p.x - circle.c.x,
		c = line.v.y,
		d = line.p.y - circle.c.y;
	double e = a * a + c * c,
	  f = 2 * (a * b + c * d),
	  g = b * b + d * d - circle.r * circle.r;
	double delta = f * f - 4 * e * g;
	if (dcmp(delta) < 0) {
		return 0;
	} else if (dcmp(delta) == 0) {
		t1 = t2 = -(f / (2 * e));
		sol.push_back(line.point(t1));
		return 1;
	} else {
		t1 = (-f - sqrt(delta)) / (2 * e);
		sol.push_back(line.point(t1));
		t2 = (-f + sqrt(delta)) / (2 * e);
		sol.push_back(line.point(t2));
		return 2;
	}
}

bool areLineSegAndCircleIntersect(const Point2& a, const Point2& b, const Circle& c) {
  vector<Point2> intersections;
  getLineCircleIntersection(Line2(b - a, a), c, intersections);
  for (const Point2& p : intersections) {
  	if (isOnSegment(p, a, b)) {
  		return true;
  	}
  }
  return false;
}

// Get the intersection points of two circles.
int getCircleCircleIntersection(const Circle& c1, const Circle& c2,
	vector<Point2>& sol) {
	double d = lengthOfVector2(c1.c - c2.c);
	if (dcmp(d) == 0) {
		if (dcmp(c1.r - c2.r) == 0) {
			return -1;
		} else {
			return 0;
		}
	}
	if (dcmp(c1.r + c2.r - d) < 0) {
		return 0;
	}
	if (dcmp(fabs(c1.r - c2.r) - d) > 0) {
		return 0;
	}

	double a = angleOfVector2(c2.c - c1.c);
	double da = acos((c1.r * c1.r + d * d - c2.r * c2.r) / (2 * c1.r * d));

	Point2 p1 = c1.point(a - da), p2 = c1.point(a + da);
	sol.push_back(p1);
	if (p1 == p2) {
		return 1;
	}
	sol.push_back(p2);
	return 2;
}

// Gets the tangent lines between a point and a circle.
// v is the directional vector of the tangent line.
int getTangents(const Point2& p, const Circle& c, vector<Vector2>& v) {
	Vector2 u = c.c - p;
	double dist = lengthOfVector2(u);
	if (dist < c.r) {
		return 0;
	} else if (dcmp(dist - C.r) == 0) {
		v.push_back(rotateVector2(u, PI / 2));
		return 1;
	} else {
		double ang = asin(c.r / dist);
		v.push_back(rotateVector2(u, ang));
		v.push_back(rotateVector2(u, -ang));
		return 2;
	}
}

// Gets the tangent lines between c1 and c2, returns the number of lines.
// a[i] and b[i] contains the tangent point of the ith line on c1 and c2.
int getTangents(const Circle& c1, const Circle& c2,
	vector<Point2>& a, vector<Point2>& b) {
	// Sort, c1 should be the bigger circle.
	if (c1.r < c2.r) {
		swap(c1, c2);
		swap(a, b);
	}

	double dist = lengthOfVector2(c2.c - c1.c);
	double rDiff = c1.r - c2.r;
	double rSum = c1.r + c2.r;

	// Infinite tangent lines.
	if (dcmp(dist) == 0 && dcmp(rDiff) == 0) {
		return -1;
	}

	// One circle includes another, no tangent lines.
	if (dist < rDiff) {
		return 0;
	}

	// The angle of the vector passing through centers of the two circles.
	double baseAngle = angleOfVector2(c2.c - c1.c);
	
	// Inner tangent.
	if (dcmp(dist, rDiff) == 0) {
		a.push_back(c1.point2(baseAngle));
		b.push_back(c2.point2(baseAngle));
		return 1;
	}

	int count = 0;
	// dist > rDiff.
	double diffAngle = acos((c1.r - c2.r) / dist);
	a.push_back(c1.point2(baseAngle + diffAngle));
	b.push_back(c2.point2(baseAngle + diffAngle));
	a.push_back(c1.point2(baseAngle - diffAngle));
	b.push_back(c2.point2(baseAngle - diffAngle));
	count += 2;

	if (dcmp(dist - rSum) == 0) {
		a.push_back(c1.point2(baseAngle));
		b.push_back(c2.point2(baseAngle + PI))
		count++;
	} else if (dcmp(dist - rSum) > 0) {
		double diffAngle = acos((c1.r + c2.r) / dist);
		a.push_back(c1.point2(baseAngle + diffAngle));
		b.push_back(c2.point2(PI + baseAngle + diffAngle));

		a.push_back(c1.point2(baseAngle - diffAngle));
		b.push_back(c2.point2(PI + baseAngle - diffAngle));
		count += 2;
	}
	return count;
}


/************************************************************************/
/************************************************************************/
/************************************************************************/
// Returns -1 if the point is on boarder.
// Returns 1 if the point is in the polygon.
// Returns 0 if the point is out of the polygon.
int isPointInPolygon(const Point2& p, const Polygon& poly) {
	int wn = 0;  // winding number
	int polySize = poly.size();
	for (int i = 0; i < polySize; i++) {
		if (isOnSegment(p, poly[i], poly[(i+1) % polySize])) {
			return -1;
		}
		int k = dcmp(crossProduct(poly[(i+1) % polySize] - poly[i], p - poly[i]));
		int d1 = dcmp(poly[i].y - p.y);
		int d2 = dcmp(poly[(i+1) % polySize].y - p.y);
		if (k > 0 && d1 <= 0 && d2 > 0) wn++;
		if (k < 0 && d2 <= 0 && d1 > 0) wn--;
	}
	if (wn != 0) {
		return 1;
	} else {
		return 0;
	}
}

// Returns -1 if the point is on boarder.
// Returns 1 if the point is in the polygon.
// Returns 0 if the point is out of the polygon.
int isPointInConvexPolygon(const Point2& p, const Polygon& poly) {
	int polySize = poly.size();
	for (int i = 0; i < polySize; i++) {
		if (isOnSegment(p, poly[i], poly[(i+1) % polySize])) {
			return -1;
		}
		if (!onLeft(Line2(poly[(i+1) % polySize] - poly[i], poly[i]), p)) {
			return 0;
		}
	}
	return 1;
}

// Please note the comparison of cross produc is <= 0.
// If the input has only 1 point, the output contains that point.
// If the input has has 2 or more points on a line, the output will contain
// two endpoints of the line. For example (1,1), (2,2), (3,3), (4,4) -> (1,1), (4,4).
//
// If the comparison is changed to < 0, vertex is allowed the convex hull edges.
// If the input has only 1 point, the output contains that point.
// If the input has has 2 or more points on a line, the output will contain
// all of the endpoints. For example (1,1), (2,2), (3,3), (4,4) -> (1,1), (2,2), (3,3),
// (4,4), (3,3), (2,2).
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

/************************************************************************/
/************************************************************************/
/************************************************************************/

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
// This implementation is more error prone, since atan2 may throw error in some implementation
// if both y and x is close to 0, and cause runtime error in oj systems.
// To prevent this, we have to advance lowNext and highNext in a loop, to make sure the delta x
// and delta y are large enough.
double rotatingCaliper(Point2* convexHull, int convexHullSize) {
	if (convexHullSize <= 1) {
		return 0;
	}
	if (convexHullSize == 2) {
		return getDistSqr(convexHull[1], convexHull[0]);
	}

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
	if (convexHullSize <= 1) {
		return 0;
	}
	if (convexHullSize == 2) {
		return getDistSqr(convexHull[1], convexHull[0]);
	}

	double distSqr = 0;
  // The opposite point.
  int k = 2;

  for (int i = 0; i < convexHullSize; i++) {
  	// Largest area means longest distance.
  	while (
  		areaOfParallelogram(
  			convexHull[k],
  			convexHull[i],
  			convexHull[(i + 1) % convexHullSize])
  		< areaOfParallelogram(
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

/************************************************************************/
/************************************************************************/
/************************************************************************/

// Cut the input polygon by the line (a->b), return the polygon on left.
Polygon cutPolygon(const Polygon& polygon, const Point2& a, const Point2& b) {
  Polygon newPolygon;
  int polygonSize = polygon.size();
  for (int i = 0; i < polygonSize; i++) {
  	Point2 c = polygon[i];
  	Point2 d = polygon[(i + 1) % polygonSize];
  	if (dcmp(crossProduct(b - a, c - a) >= 0)) {
  		// Point c is on the right of the cutting line.
  		newPolygon.push_back(c);
  	}
  	if (dcmp(crossProduct(b - a, d - c)) != 0) {
  		// The two lines are not parallel.
  		Point2 intersectionPoint = getLineIntersection(a, b - a, c, d - c);
  		if (isOnSegment(intersectionPoint, d, c)) {
  			newPolygon.push_back(intersectionPoint);
  		}
  	}
  }
  return newPolygon;
}

// Get the half plane intersection of the input lines.
int getHalfPlaneIntersection(Line2* lines, int n, Point2 * poly) {
    sort(lines, lines + n);
 
    int first = 0, last = 0;
    Point2 points[n];
    Line2 dequeLines[n];
    dequeLines[0] = lines[0];
 
    for (int i = 1; i < n; i++) {
        while (first < last && !onLeft(lines[i], points[last - 1])) {
        	last--;
        }
        while (first < last && !onLeft(lines[i], points[first])) {
        	first++;
        }

        dequeLines[++last] = lines[i];
 
        if (fabs(crossProduct(dequeLines[last].v, dequeLines[last - 1].v)) < EPS)
        {
            last--;
            if (onLeft(dequeLines[last],lines[i].p)) {
            	dequeLines[last] = lines[i];
            }
        }
 
        if (first < last) {
        	 points[last - 1] = getLineIntersection(dequeLines[last - 1], dequeLines[last]);
        }
    }
 
    while (first < last && !onLeft(dequeLines[first], points[last - 1])) {
    	 last--;
    }
 
    if (last - first <= 1) {
    	 return 0;
    }
 
    points[last] = getLineIntersection(dequeLines[last], dequeLines[first]);
 
    int m = 0;
    for (int i = first; i <= last; i++) {
    	 poly[m++] = points[i];
    }
    return m;
}

// Another implementation, use deque instead of first and last to maintain the index.
// Get the half plane intersection of the input lines.
void halfPlaneIntersection(Line2* lines, int n, Polygon& polygon) {
  if (n <= 1) return;

  sort(lines, lines + n);
  deque<Point2> points;
  deque<Line2> intersectingLines;
  intersectingLines.push_back(lines[0]);

  for (int i = 1; i < n; i++) {
    while (intersectingLines.size() > 1 && !onLeft(lines[i], points.back())) {
      intersectingLines.pop_back();
      points.pop_back();
    }
    while (intersectingLines.size() > 1 && !onLeft(lines[i], points.front())) {
      intersectingLines.pop_front();
      points.pop_front();
    }

    intersectingLines.push_back(lines[i]);

    int last = intersectingLines.size() - 1;
    Line2 tmpA = intersectingLines[last - 1];
    Line2 tmpB = intersectingLines[last];
    if (fabs(crossProduct(tmpA.v, tmpB.v)) < EPS) {
      // tmpA and tmpB are parallel, get the left one.
      intersectingLines.pop_back();
      if (points.size() >= 1) {
        points.pop_back();  
      }
      if (onLeft(tmpA, tmpB.p)) {
        // tmpB is on left of tmpA, keep tmpB
        intersectingLines.back() = tmpB;
      }
    }
    last = intersectingLines.size() - 1;
    if (intersectingLines.size() > 1) {
      points.push_back(getLineIntersection(intersectingLines[last], intersectingLines[last - 1]));
    }
  }

  while (intersectingLines.size() > 1 && !onLeft(intersectingLines.front(), points.back())) {
    intersectingLines.pop_back();
    points.pop_back();
  }

  // At most one intersection point, open area.
  if (points.size() <= 1) {
    return;
  }
  
  points.push_back(getLineIntersection(intersectingLines.back(), intersectingLines.front()));
  polygon.insert(polygon.end(), points.begin(), points.end());
}
