// tetrahedron - four faces.
// polyhedron - plural noun: polyhedra

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

// Distance from p to the plane (p0, n).
// n needs to have length 1.
double distanceToPlane(const Point3& p, const Point3& p0, const Vector3& n) {
	// Without fabs, the distance is directed. Positive means it is in the same
	// direction as n.
	return fabs(dotProduct(p - p0, n));
}

// Get the projection of point p on the plane (p0, n).
Point3 getPlaneProjection(const Point3& p, const Point3& p0, const Vector3& n) {
	return p - n * dotProduct(p - p0, n);
}

// This function assumes the line and the plane has an intersection.
Point3 getLinePlaneIntersection(const Point3& p1, const Point3& p2, const Point3& p0, const Vector3& n) {
  Vector3 v = p2 - p1;
  double t = dotProduct(n, p0 - p1) / dotProduct(n, p2 - p1);
  return p1 + v * t;
}

// Area of the parallelogram defined by (a -> b), (a -> c).
// In 3 dimentional space, the area of parallelogram is the magnitude of the cross product.
// The area is un-directional.
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
	const Point3& a, const Point3& b, Point3& p) {
	Vector3 n = crossProduct(p1 - p0, p2 - p0);
	if (dcmp(dotProduct(n, b - a)) == 0) {
		return false;
	}
	double t = dotProduct(n, p0 - a) / dotProduct(n, b - a);
	if (dcmp(t) < 0 || dcmp(t - 1) > 0) {
		return false;
	}
	p = a + (b - a) * t;
	return isPointInTriangle(p, p0, p1, p2);
}

// Distance from point p to the line defined by (a, b).
double distanceToLine(const Point3& p, const Point3& a, const Point3& b) {
  Vector3 v1 = b - a, v2 = p - a;
  return lengthOfVector3(crossProduct(v1, v2)) / lengthOfVector3(v1);
}

double distanceToSegment(const Point3& p, const Point3& a, const Point3& b) {
	if (a == b) {
		return lengthOfVector3(p - a);
	}
	Vector3 v1 = b - a, v2 = p - a, v3 = p - b;
	if (dcmp(dotProduct(v1, v2)) < 0) {
		return lengthOfVector3(v2);
	} else if (dcmp(dotProduct(v1, v3)) > 0) {
		return lengthOfVector3(v3);
	} else {
		return lengthOfVector3(crossProduct(v1, v2)) / lengthOfVector3(v1);
	}
}

// Return the volume * 6 of the tetrahedron defined by (a, b, c, d).
// If viewed from the outside, b, c, d is in counter-clockwise order, then
// the result is positive.
double getVolume(const Point3& a, const Point3& b, const Point3& c, const Point3& d) {
	return dotProduct(d - a, crossProduct(b - a, c - a)) / 6;
}

/************************************************************************/
/************************************************************************/
/************************************************************************/

struct Face {
	// Index of the points that constitutes this face.
  int v[3];

  Face(int a, int b, int c) {
  	v[0] = a;
  	v[1] = b;
  	v[2] = c;
  }

  Vector3 getNormal(const Point3* points) const {
  	Vector3 rtn = crossProduct(points[v[1]] - points[v[0]], points[v[2]] - points[v[0]]);
  	rtn = rtn / lengthOfVector3(rtn);
  	return rtn;
  }

  // Returns if the i-th point can see this face.
  // A point can see a face if it is on the left side of face (a, b, c)
  // (the order of a, b, c defines the dirction).
  bool canSee(int i, const Point3* points) const {
  	return dotProduct(points[i] - points[v[0]], getNormal(points)) > 0;
  }
};

// A random number in [0, 1](inclusive).
double rand01() {
  return rand() / (double) RAND_MAX;
}

double randeps() {
  return (rand01() - 0.5) * EPS;
}

Point3 addNoise(const Point3& p) {
  return Point3(p.x + randeps(), p.y + randeps(), p.z + randeps());
}

// This function add noise to the input, another implementation would be add
// noise before feeding in data.
//
// If there are four points on the same face, the result would be wrong.
// For example, a face could be missing. The error results in value difference.
vector<Face> convexHull3d(Point3* original, int n, Point3* p) {
	for (int i = 0; i < n; i++) {
		p[i] = addNoise(original[i]);
	}

	vector<Face> cur;

	//If the face on the left of this edge is visible.
	bool vis[n][n];
	memset(vis, 0, sizeof(bool) * n * n);

	cur.push_back(Face(0, 1, 2));
	cur.push_back(Face(2, 1, 0));

	for (int i = 3; i < n; i++) {
		vector<Face> next;
		for (int j = 0; j < cur.size(); j++) {
			const Face& f = cur[j];
			bool canSee = f.canSee(p, i);
			if (!canSee) {
				next.push_back(f);
			}
			for (int k = 0; k < 3; k++) {
				vis[f.v[k]][f.v[(k + 1) % 3]] = canSee;
			}
		}

		for (int j = 0; j < cur.size(); j++) {
			for (int k = 0; k < 3; k++) {
				int a = cur[j].v[k], b = cur[j].v[(k + 1) % 3];
				if (vis[a][b] != vis[b][a] && vis[a][b]) {
					next.push_back(Face(a, b, i));
				}
			}
		}

		cur = next;
	}
	return cur;
}

// The gravity center of a tetrahedron is the mean of its four vertices.
Point3 getTetrahedronGravityCenter(const Point3& a, const Point3& b,
	const Point3& c, const Point3& d) {
	double x = 0, y = 0, z = 0;
	x = a.x + b.x + c.x + d.x;
	y = a.y + b.y + c.y + d.y;
	z = a.z + b.z + c.z + d.z;
	return Point3(x / 4, y / 4, z / 4);
}

// Get the center of gravity of a polyhedron.
// The start point could be chosen randomly. But it is prefered to be some
// point near the middle of the polyhedron.
Point3 getCenterOfConvexPolyhedron(const vector<Face>& faces, const Point3* points) {
	Point3 start(0, 0, 0);
	double x = 0, y = 0, z = 0;
	double totalVolume = 0;
	for (Face face : faces) {
		double v = getVolume(start, points[face.v[0]],
			points[face.v[1]], points[face.v[2]]);
		Point3 center = getTetrahedronGravityCenter(start, points[face.v[0]],
			points[face.v[1]], points[face.v[2]]);
		totalVolume += v;
		x += v * center.x;
		y += v * center.y;
		z += v * center.z;
	}

	return Point3(x / totalVolume, y / totalVolume, z / totalVolume);
}
























