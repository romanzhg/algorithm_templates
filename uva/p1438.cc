#include <cfloat>
#include <climits>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <algorithm>
#include <deque>
#include <functional>
#include <iostream>
#include <limits>
#include <list>
#include <map>
#include <queue>
#include <set>
#include <stack>
#include <string>
#include <vector>
#include <utility>

using namespace std;

const double EPS = 1e-10;

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

// Distance from p to the plane (p0, n).
// n needs to have length 1.
double distanceToPlane(const Point3& p, const Point3& p0, const Vector3& n) {
	return fabs(dotProduct(p - p0, n));
}

// Return the volume of the tetrahedron defined by (a, b, c, d).
// If viewed from the outside, b, c, d is in counter-clockwise order,
// the result will be positive.
double getVolume(const Point3& a, const Point3& b, const Point3& c, const Point3& d) {
	return dotProduct(d - a, crossProduct(b - a, c - a)) / 6;
}

double lengthOfVector3(const Vector3& a) {
	return sqrt(dotProduct(a, a));
}

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
  bool canSee(const Point3* points, int i) const {
  	return dotProduct(points[i] - points[v[0]], getNormal(points)) > 0;
  }
};

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

Point3 getTetrahedronCenter(const Point3& a, const Point3& b,
	const Point3& c, const Point3& d) {
	double x = 0, y = 0, z = 0;
	x = a.x + b.x + c.x + d.x;
	y = a.y + b.y + c.y + d.y;
	z = a.z + b.z + c.z + d.z;
	return Point3(x / 4, y / 4, z / 4);
}

// Solution.
#define INPUT_SIZE 105

Point3 polyhedron1[INPUT_SIZE], polyhedron2[INPUT_SIZE];
Point3 disturbedPolyhedron1[INPUT_SIZE], disturbedPolyhedron2[INPUT_SIZE];

Point3 getCenter(const vector<Face>& faces, const Point3* points) {
	Point3 start(0, 0, 0);
	double x = 0, y = 0, z = 0;
	double totalVolume = 0;
	for (Face face : faces) {
		double v = getVolume(start, points[face.v[0]],
			points[face.v[1]], points[face.v[2]]);
		Point3 center = getTetrahedronCenter(start, points[face.v[0]],
			points[face.v[1]], points[face.v[2]]);
		totalVolume += v;
		x += v * center.x;
		y += v * center.y;
		z += v * center.z;
	}

	return Point3(x / totalVolume, y / totalVolume, z / totalVolume);
}

double getMinDist(const vector<Face>& faces, const Point3* points, const Point3& center) {
	double minValue = DBL_MAX;
	for (Face face : faces) {
		minValue = min(minValue,
			distanceToPlane(center, points[face.v[0]], face.getNormal(points)));
	}
	return minValue;
}

int main() {
	while (true) {
		int vertices_1, vertices_2;
		if (scanf("%d", &vertices_1) != 1) {
			break;
		}
		for (int i = 0; i < vertices_1; i++) {
			scanf("%lf %lf %lf", &polyhedron1[i].x, &polyhedron1[i].y, &polyhedron1[i].z);
		}

		scanf("%d", &vertices_2);
		for (int i = 0; i < vertices_2; i++) {
			scanf("%lf %lf %lf", &polyhedron2[i].x, &polyhedron2[i].y, &polyhedron2[i].z);
		}

		vector<Face> convexpolyhedron1 = convexHull3d(polyhedron1, vertices_1, disturbedPolyhedron1);
		vector<Face> convexpolyhedron2 = convexHull3d(polyhedron2, vertices_2, disturbedPolyhedron2);
		Point3 aCenter = getCenter(convexpolyhedron1, polyhedron1);
		Point3 bCenter = getCenter(convexpolyhedron2, polyhedron2);
		double minDistA = getMinDist(convexpolyhedron1, polyhedron1, aCenter);
		double minDistB = getMinDist(convexpolyhedron2, polyhedron2, bCenter);

		printf("%.6lf\n", minDistA + minDistB);
	}
	return 0;
}