// Notes:
// 1. Combinations (choose k from n) can be implemented by a k level nested for loop.
// For this problem it is actually easier.
// 2. EPS of 1e-10 would fail the official test cases, to be investigated.
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

const double EPS = 1e-7;

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

// Return the volume of the tetrahedron defined by (a, b, c, d).
// If viewed from the outside, b, c, d is in counter-clockwise order,
// the result will be positive.
double getVolume(const Point3& a, const Point3& b, const Point3& c, const Point3& d) {
	return dotProduct(d - a, crossProduct(b - a, c - a)) / 6;
}

double lengthOfVector3(const Vector3& a) {
	return sqrt(dotProduct(a, a));
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
  bool canSee(int i, const Point3* points) const {
  	return dotProduct(points[i] - points[v[0]], getNormal(points)) > 0;
  }

  // Return which side the point is on as compare to the face.
  int getSide(int i, const Point3* points) {
  	return dcmp(dotProduct(points[i] - points[v[0]], getNormal(points)));
  }
};

Point3 getTetrahedronCenter(const Point3& a, const Point3& b,
	const Point3& c, const Point3& d) {
	double x = 0, y = 0, z = 0;
	x = a.x + b.x + c.x + d.x;
	y = a.y + b.y + c.y + d.y;
	z = a.z + b.z + c.z + d.z;
	return Point3(x / 4, y / 4, z / 4);
}

// Get the projection of point p on the plane (p0, n).
Point3 getPlaneProjection(const Point3& p, const Point3& p0, const Vector3& n) {
	return p - n * dotProduct(p - p0, n);
}

double areaOfParallelogram(const Vector3& a, const Vector3& b, const Vector3& c) {
	return lengthOfVector3(crossProduct(b - a, c - a));
}

// The area is un-directional.
bool isPointInTriangle(const Point3& p, const Point3& p0, const Point3& p1, const Point3& p2) {
  double area1 = areaOfParallelogram(p, p0, p1);
  double area2 = areaOfParallelogram(p, p1, p2);
  double area3 = areaOfParallelogram(p, p2, p0);
  return dcmp(area1 + area2 + area3 -  areaOfParallelogram(p0, p1, p2)) == 0;
}

// Distance from point p to the line defined by (a, b).
double distanceToLine(const Point3& p, const Point3& a, const Point3& b) {
  Vector3 v1 = b - a, v2 = p - a;
  return lengthOfVector3(crossProduct(v1, v2)) / lengthOfVector3(v1);
}

// Distance from p to the plane (p0, n).
// n needs to have length 1.
double distanceToPlane(const Point3& p, const Point3& p0, const Vector3& n) {
	return fabs(dotProduct(p - p0, n));
}

void genCombinationsHelper(
	vector<vector<int>>& combinations,
	vector<int>& tmpResult, int n, int k, int c) {
	if (tmpResult.size() == k) {
		combinations.push_back(tmpResult);
		return;
	}
	if (c >= n) {
		return;
	}
	tmpResult.push_back(c);
	genCombinationsHelper(combinations, tmpResult, n, k, c + 1);

	tmpResult.pop_back();
	genCombinationsHelper(combinations, tmpResult, n, k, c + 1);
}

// Chooses k elements from n elements, returns the combinations.
void genCombinations(vector<vector<int>>& combinations, int n, int k) {
	vector<int> tmpResult;
	genCombinationsHelper(combinations, tmpResult, n, k, 0);
}

/* Solution. */
Point3 getCenterOfMass(
	const Point3& a, const Point3& b, const Point3& c,
	const Point3& d, const Point3& e) {
	double v1 = fabs(getVolume(a, b, c, d));
	double v2 = fabs(getVolume(a, b, c, e));
	Point3 c1 = getTetrahedronCenter(a, b, c, d);
	Point3 c2 = getTetrahedronCenter(a, b, c, e);
	double v = v1 + v2;
	return (c1 * v1 + c2 * v2) / (v1 + v2);
}

bool isValidBottomFace(const vector<int>& bottom, const vector<int>& other, Point3* points) {
	Face face(bottom[0], bottom[1], bottom[2]);
	int side = 1;
	for (const int& i : other) {
		side = side * face.getSide(i, points);
	}
	return side >= 0;
}

bool onSamePlane(int index, const vector<int>& face, Point3* points) {
	Face f(face[0], face[1], face[2]);
	return f.getSide(index, points) == 0;
}

vector<vector<Point3>> getBottomFaces(
	const Point3& a, const Point3& b, const Point3& c,
	const Point3& d, const Point3& e) {
	vector<vector<Point3>> rtn;

	Point3 tmpArray[5];
	tmpArray[0] = a;
	tmpArray[1] = b;
	tmpArray[2] = c;
	tmpArray[3] = d;
	tmpArray[4] = e;
	vector<vector<int>> combinations;
	genCombinations(combinations, 5, 3);

	for (vector<int>& bottom : combinations) {
		vector<int> nonBottomPoints;
		for (int i = 0; i < 5; i++) {
			if (find(bottom.begin(), bottom.end(), i) == bottom.end()) {
				nonBottomPoints.push_back(i);
			}
		}

		vector<Point3> tmpResult;
		if (isValidBottomFace(bottom, nonBottomPoints, tmpArray)) {
			for (const int& bottomPoint: bottom) {
				tmpResult.push_back(tmpArray[bottomPoint]);
			}
		} else {
			continue;
		}

		for (int& index : nonBottomPoints) {
			if (onSamePlane(index, bottom, tmpArray)) {
				tmpResult.push_back(tmpArray[index]);
			}
		}
		rtn.push_back(tmpResult);
	}
	return rtn;
}

bool isPointInArea(const Point3& projection, const vector<Point3>& area) {
	for (int i = 0; i < area.size(); i++) {
		for (int j = i + 1; j < area.size(); j++) {
			for (int k = j + 1; k < area.size(); k++) {
				if (isPointInTriangle(projection, area[i], area[j], area[k])) {
					return true;
				}
			}
		}
	}
	return false;
}

bool isStableFace(const vector<Point3>& area, const Point3& centerOfMass) {
	Face f(0, 1, 2);
	Point3 projection = getPlaneProjection(
		centerOfMass, area[f.v[0]], f.getNormal(area.data()));
	if (!isPointInArea(projection, area)) {
		return false;
	}

	// There is another way of the judgement. Only compare with the outside edges.
	// Note the double crossProduct to divide the space with a plane through one line.
  /*
	int n = area.size();
	for (int i = 0; i < n; i++) {
		for (int j = i + 1; j < n; j++) {
			Point3 pi = area[i], pj = area[j];
			int k; for (k = 0; k < n; k++) {
				if (k != i && k != j) {
					break;
				}
			}
			Point3 pk = area[k];
			bool plus = false, minus = false;
			Point3 nor = crossProduct(crossProduct(pj - pi, pk - pi), pj - pi);
			for (int x = 0; x < n; x++) if (x!=i&&x!=j) {
				double temp = dotProduct(nor, area[x] - pi);
				if (dcmp(temp) > 0)
					plus = true;
				else if (dcmp(temp) < 0)
					minus = true;
			}
			if (plus && minus) continue;
			if (dcmp(distanceToLine(projection, pi, pj) - 0.2) < 0)
				return false;
		}
	}
	return true;
  */

  // If there are four vertices in the area, there needs to be at least 4 lines
  // satisfies the condition.
	vector<vector<int>> combinations;
	genCombinations(combinations, area.size(), 2);
	int validLineCount = 0;
	for (const vector<int>& combination : combinations) {
		if (dcmp(distanceToLine(projection, area[combination[0]], area[combination[1]]) - 0.2) >= 0) {
			validLineCount++;
		}
	}
	return validLineCount >= area.size();
}

// Distance from a point to a face.
double getDist(const vector<Point3>& bottomFace, const Point3& p) {
	Vector3 norm = crossProduct(bottomFace[1] - bottomFace[0], bottomFace[2] - bottomFace[0]);
  norm = norm / lengthOfVector3(norm);
	return distanceToPlane(p, bottomFace[0], norm);
}

bool readPoint(Point3& p) {
	return scanf("%lf %lf %lf", &p.x, &p.y, &p.z) == 3;
}

int main() {
	int testCaseNumber = 0;
	double x, y, z;
	Point3 a, b, c, d, e, f;
	while(true) {
		bool isValidTestCase = true;
		isValidTestCase = isValidTestCase && readPoint(a);
		isValidTestCase = isValidTestCase && readPoint(b);
		isValidTestCase = isValidTestCase && readPoint(c);
		isValidTestCase = isValidTestCase && readPoint(d);
		isValidTestCase = isValidTestCase && readPoint(e);
		isValidTestCase = isValidTestCase && readPoint(f);
		if (!isValidTestCase) {
			break;
		}

		Point3 centerOfMass = getCenterOfMass(a, b, c, d, e);
		vector<vector<Point3>> bottomFaces = getBottomFaces(a, b, c, d, e);
		double minDist = DBL_MAX, maxDist = 0;
		for (const vector<Point3>& bottomFace : bottomFaces) {
			if (isStableFace(bottomFace, centerOfMass)) {
				double dist = getDist(bottomFace, f);
				minDist = min(minDist, dist);
				maxDist = max(maxDist, dist);
			}
		}
		printf("Case %d: %.5lf %.5lf\n", ++testCaseNumber, minDist, maxDist);
	}
	return 0;
}

/*
0 0 0 9 0 0 0 8 0 0 0 9 1 1 -8 1 2 -1
0 0 0 7 0 0 0 7 0 0 0 7 -1 -2 -3 2 2 2
1 2 3 6 2 3 -2 6 3 -1 0 7 4 1 -2 -1 5 3
0 0 10 10 0 0 0 10 0 0 0 0 10 10 0 4 4 1
0
*/