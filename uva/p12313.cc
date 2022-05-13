// This program passes the sample test case with no reflection and refraction, also
// the sample test case with only reflection turned on.
// If refraction is turned on, the program fails with some numerical difference.
// To be further investigated.
//
// To view the picture generated, write the output in the form of PPM file.
// The following example is a text PPM file with 3 * 1 pixels, and the pixels
// are red, gree, blue respectively.
//
// P3
// 3 1
// 255
// 255 0 0 0 255 0 0 0 255
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

const double PI = acos(-1);
const double EPS = 1e-6;

int dcmp(double x) {
	if (fabs(x) < EPS) {
		return 0;
	} else {
		return x < 0 ? -1 : 1;
	}
}

// Note: all structures should have a constructor which provide a default
// value for all the fields.
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

Vector3 operator - (const Vector3& a) {
	return Vector3(-a.x, -a.y, -a.z);
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

struct Face {
  int v[3];

  Face(int a = 0, int b = 0, int c = 0) {
  	v[0] = a;
  	v[1] = b;
  	v[2] = c;
  }

  // Get the normal vector of the face with length 1.
  Vector3 getNormal(const Point3* points) const {
  	Vector3 rtn = crossProduct(points[v[1]] - points[v[0]], points[v[2]] - points[v[0]]);
  	rtn = rtn / lengthOfVector3(rtn);
  	return rtn;
  }
};

struct Color {
	double r, g, b;
	Color(double r = 0, double g = 0, double b = 0): r(r), g(g), b(b) {};
};

Color operator + (const Color& a, const Color& b) {
  return Color(a.r + b.r, a.g + b.g, a.b + b.b);
}

Color operator * (const Color& a, double b) {
  return Color(a.r * b, a.g * b, a.b * b);
}

Color operator * (const Color& a, const Color& b) {
  return Color(a.r * b.r, a.g * b.g, a.b * b.b);
}


struct Object {
	vector<Point3> vertices;
	vector<Face> faces;
	Color color;
	double refl, refr, refractiveIndex;
	Object() {
		vertices.clear();
		faces.clear();
	}
};

struct LightSource {
	Point3 pos;
	double ambient_coeff;
	Color color;
};

struct ImageParam {
	Point3 cameraPos;
	// cameraTarget is the direction, which is different from the problem spec,
	// where is treated as the "look at" point. A simple translation in this program
	// translate it.
	Vector3 cameraTarget, cameraUp;
	// Angle in radians.
	double f;
	// H lines, each lines has W colomns.
	int W, H;
};

struct Intersection {
	Point3 point;
	// Id of the object this intersection occurs.
	// For this problem, intersection only happens between vacuum and an object.
	// -1 for invalid intersection.
	int objId;

	// Index of face this intersection is on.
	int faceId;
};

// Represent a ray from point a to point b.
// Integer objId is an index to the object array, represent which
// object this ray is travelling in. -1 if the ray is in travelling in vacuum.
struct Ray {
	// Here should be information on whether this ray is in a 
	// object or not.
	Point3 a, b;
	int objId;
	Ray(Point3 a = Point3(), Point3 b = Point3(), int objId = -1) {
		this->a = a;
		this->b = b;
		this->objId = objId;
	}
};


// Solution data structures.
#define MAX_DEPTH 4
static const Color BLACK = {0, 0, 0};

// Debug functions.
void printPoint(const Point3& p) {
	printf("%.2lf %.2lf %.2lf\n", p.x, p.y, p.z);
}
// Debug function ends.

void printColor(const Color& c) {
	printf("%02x%02x%02x ", (int) round(c.r * 255.0), (int) round(c.g * 255.0), (int) round(c.b * 255.0));
}

vector<Object> objects;
LightSource lightSource;

Vector3 normalizeLength(const Vector3& v) {
	return v / lengthOfVector3(v);
}

// Get the angle between two Vector3.
// The value will be in range [0, PI/2] (included on both end).
double getAngleBetweenVectors(const Vector3& a, const Vector3& b) {
	double rtn = acos(dotProduct(a, b) / lengthOfVector3(a) / lengthOfVector3(b));
	if (rtn > (PI / 2.0)) {
		rtn = PI - rtn;
	}
	return rtn;
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

// Returns true if the ray and the face intersects.
// Output intersection distance and point.
bool rayFaceIntersect(const Ray& ray, const Object& obj, const Face& face,
	double& dist, Point3& p) {
	// See if the ray and the face are parallel.
	Vector3 faceNormal = face.getNormal(obj.vertices.data());
	double projectionLength = dotProduct(faceNormal, ray.b - ray.a);
	if (dcmp(projectionLength) == 0) {
		return false;
	}

	// Get the intersection point of the ray and the face.
	Vector3 v = ray.b - ray.a;
	double t = dotProduct(faceNormal, obj.vertices[face.v[0]] - ray.a)
		/ dotProduct(faceNormal, ray.b - ray.a);

	// t == 0 represents the current face, it should not be treated as an intersection.
	if (dcmp(t) <= 0) {
		return false;
	}

	p = ray.a + v * t;
	dist = lengthOfVector3(v * t);

	return isPointInTriangle(p, obj.vertices[face.v[0]],
		obj.vertices[face.v[1]], obj.vertices[face.v[2]]);
}

Intersection getFirstIntersection(const Ray& ray) {
	double minDist = DBL_MAX;
	Intersection rtn;
	rtn.objId = -1;
	for (int i = 0; i < objects.size(); i++) {
		if (ray.objId != -1) {
			if (ray.objId != i) {
				continue;
			}
		}
		const Object& obj = objects[i];
		for (int j = 0; j < obj.faces.size(); j++) {
			const Face face = obj.faces[j];
			double dist;
			Point3 intersectionP;
			if (rayFaceIntersect(ray, obj, face, dist, intersectionP)) {
				if (dist < minDist) {
					minDist = dist;
					rtn.point = intersectionP;
					rtn.objId = i;
					rtn.faceId = j;
				}
			}
		}
	}
	return rtn;
}

bool isLineSegFaceIntersecting(const Point3& from, const Point3& to,
	const Object& obj, const Face& face) {
	// See if the ray and the face are parallel.
	Vector3 faceNormal = face.getNormal(obj.vertices.data());
	double projectionLength = dotProduct(faceNormal, to - from);
	if (dcmp(projectionLength) == 0) {
		return false;
	}

	// Get the intersection point of the line segment and the face.
	double t = dotProduct(faceNormal, obj.vertices[face.v[0]] - from)
		/ dotProduct(faceNormal, to - from);

	// t == 0 represents the current face, it should not be treated as an intersection.
	if (dcmp(t) <= 0) {
		return false;
	}
	// t > 1 represents a intersection point beyond point "to", which can be ignored.
	if (dcmp(t - 1) > 0) {
		return false;
	}

	Vector3 v = to - from;
	Point3 p = from + v * t;
	return isPointInTriangle(p, obj.vertices[face.v[0]],
		obj.vertices[face.v[1]], obj.vertices[face.v[2]]);
}

// TODO: consider make the code simpler by consolidate the functions: multiple functions
// can be combined.
bool hasObstacle(const Point3& from, const Point3& to) {
	// The standard solution actually has a bug, if the light source and the incoming ray
	// is on different sides of the face, the light source should be considered invisible. 
	// But for simplicity, we assume this doesn't happen.
	for (const Object& obj : objects) {
		for (const Face& face : obj.faces) {
			if (isLineSegFaceIntersecting(from, to, obj, face)) {
				return true;
			}
		}
	}
	return false;
}

Color getPointColor(const Intersection& its) {
	Point3 from = its.point;
	Point3 to = lightSource.pos;
	double shade;
	const Object& obj = objects[its.objId];
	const Face& face = obj.faces[its.faceId];
	if (hasObstacle(from, to)) {
		shade = 0; 
	} else {
		Vector3 pointToLightSource = to - from;
		shade = fabs(dotProduct(normalizeLength(pointToLightSource), face.getNormal(obj.vertices.data())));
	}
	Color c = obj.color * lightSource.color
			* (lightSource.ambient_coeff + (1.0 - lightSource.ambient_coeff) * shade);
	return c;
}

// TODO: Maybe write unit test and verification routines for this function.
// Possible things to verify:
//   1. The angle between reflected ray, face norm, and original ray adds up to PI.
//   2. The reflected point and original point are on the same side of the face.
Ray getReflectedRay(const Ray& ray, const Intersection& its) {
	Point3 s = ray.a;
	Point3 tmp = s + (its.point - s) * 2.0;

	const Object& obj = objects[its.objId];
	const Face& face = obj.faces[its.faceId];

	Vector3 norm = face.getNormal(obj.vertices.data());
	Point3 reflectedPoint = tmp - norm * 2.0 * dotProduct(norm, tmp - its.point);

	// The reflected ray is always in vacuum.
	return Ray(its.point, reflectedPoint, -1);
}

Ray getRefractedRay(const Ray& ray, const Intersection& its) {
	Point3 s = ray.a;
	Vector3 v = its.point - s;

	const Object& obj = objects[its.objId];
	const Face& face = obj.faces[its.faceId];

	Vector3 norm = face.getNormal(obj.vertices.data());
	Vector3 partOnNorm = norm * (dotProduct(norm, v));
	Vector3 partOrthogonalToNorm = v - partOnNorm;

	// n1 * sin(angle1) = n2 * sin(angle2).
	double angle1 = getAngleBetweenVectors(v, norm);
	double n1, n2;
	if (ray.objId == -1) {
		n1 = 1;
		n2 = obj.refractiveIndex;
	} else {
		n1 = obj.refractiveIndex;
		n2 = 1;
	}

	// Total reflection.
	if (n1 > n2) {
		double angleCritical = asin(n2 / n1);
		if (dcmp(angle1 - angleCritical) >= 0) {
			return Ray(Point3(), Point3(), -2);
		}
	}

	// "n1 * sin(angle1) / n2" should always greater than 0.
	double angle2 = asin(n1 * sin(angle1) / n2);
	double newLength = lengthOfVector3(partOnNorm) * tan(angle2);
	Vector3 newPartOrthogonalToNorm = normalizeLength(partOrthogonalToNorm) * newLength;
	Point3 refractedPoint = its.point + partOnNorm + newPartOrthogonalToNorm;
	return Ray(its.point, refractedPoint, ray.objId == -1 ? its.objId : -1);
}

Color traceRay(int depth, const Ray& ray) {
	if (ray.objId == -2) {
		return Color(0,0,0);
	}
  Color pointColor = Color(0,0,0), reflectColor = Color(0,0,0), refractColor = Color(0,0,0);

  Intersection its = getFirstIntersection(ray);
  if (its.objId >= 0) {
  	double refl = objects[its.objId].refl;
  	double refr = objects[its.objId].refr;
  	pointColor = getPointColor(its) * (1 - refl - refr);
  	if (depth < MAX_DEPTH && refl > 0) {
  		reflectColor = traceRay(depth + 1, getReflectedRay(ray, its)) * refl;
  	}
  	if (depth < MAX_DEPTH && refr > 0) {
  		refractColor = traceRay(depth + 1, getRefractedRay(ray, its)) * refr;
  	}
  }
  return pointColor + reflectColor + refractColor;
}

void genImage(const ImageParam& imageParam) {
	// The distance from camera position to the center of image plane.
	double dist = ((double) imageParam.W) / (2.0 * tan(imageParam.f / 2.0));
	Point3 imageCenter =
		imageParam.cameraPos + normalizeLength(imageParam.cameraTarget) * dist;

	// Two vectors for the right and down direction on the image plane.
	Vector3 vRight = normalizeLength(
		crossProduct(imageParam.cameraTarget, imageParam.cameraUp));
	Vector3 vDown = normalizeLength(
		Vector3(-imageParam.cameraUp.x,
						-imageParam.cameraUp.y,
						-imageParam.cameraUp.z));

	printf("%d %d\n", imageParam.W, imageParam.H);
	for (int i = 0; i < imageParam.H; i++) {
		for (int j = 0; j < imageParam.W; j++) {
			Point3 pixelCenter = imageCenter
					+ (vRight * (j - (imageParam.W - 1) / 2.0))
					+ (vDown * (i - (imageParam.H - 1) / 2.0));
			Ray ray(imageParam.cameraPos, pixelCenter, -1);
			Color c = traceRay(0, ray);
			printColor(c);
		}
		printf("\n");
	}
	printf("\n");
}

Point3 readPoint() {
	double x, y, z;
	scanf("%lf %lf %lf", &x, &y, &z);
	return Point3(x, y, z);
}

Face readFace() {
	int a, b, c;
	scanf("%d %d %d", &a, &b, &c);
	return Face(a, b, c);
}

int main() {
	int objectCount;
	while (scanf("%d", &objectCount) == 1 && objectCount != 0) {
		// Data to be read.
		objects.clear();

		for (int i = 0; i < objectCount; i++) {
			Object o;

			// Read in object data.
			int vertexCount;
			scanf("%d", &vertexCount);
			for (int j = 0; j < vertexCount; j++) {
				o.vertices.push_back(readPoint());
			}
			int faceCount;
			scanf("%d", &faceCount);
			for (int j = 0; j < faceCount; j++) {
				o.faces.push_back(readFace());
			}
			double a, b, c, d, e, f;
			scanf("%lf %lf %lf %lf %lf %lf", &a, &b, &c, &d, &e, &f);
			o.color.r = a;
			o.color.g = b;
			o.color.b = c;
			o.refl = d;
			o.refr = e;
			o.refractiveIndex = f;
			objects.push_back(o);
		}

		// Read light source.
		scanf("%lf %lf %lf", &lightSource.pos.x,
			&lightSource.pos.y, &lightSource.pos.z);
		scanf("%lf", &lightSource.ambient_coeff);
		scanf("%lf %lf %lf", &lightSource.color.r,
			&lightSource.color.g, &lightSource.color.b);

		int imageCount;
		scanf("%d", &imageCount);
		for (int i = 0; i < imageCount; i++) {
			ImageParam imageParam;
			imageParam.cameraPos = readPoint();
			Point3 target = readPoint();
			imageParam.cameraTarget = target - imageParam.cameraPos;
			imageParam.cameraUp = readPoint();
			double angleInDegree;
			scanf("%lf", &angleInDegree);
			imageParam.f = angleInDegree / 180.0 * PI;
			scanf("%d %d", &imageParam.W, &imageParam.H);
			// Read image config.
			// Generate image and output.
			genImage(imageParam);
		}
	}
	return 0;
}

/**
1
4
1000 22 1000
1000 22 -1000
-1000 22 -1000
-1000 22 1000
2
0 2 1
0 2 3
1 1 1 0 0 0
0 0 0 0 1 1 1
1
0 0 0 0 1 0 0 0 1 90 11 1
2
3
1000 22 1000
-1000 22 -1000
1000 22 -1000
1
0 1 2 
1 0 0 0 0 0
3
1000 22 1000
-1000 22 -1000
-1000 22 1000
1
0 1 2 
0 1 0 0 0 0
0 0 0 0 1 1 1
1
0 0 0 0 1 0 0 0 1 90 11 1

Sample test input with reflection turned on.
7
4
552.8 0.0 0.0
0.0 0.0 0.0
0.0 0.0 559.2
549.6 0.0 559.2
2
0 1 2
2 3 0
1.0 1.0 1.0 0.5 0 0
4
556.0 548.8 0.0
556.0 548.8 559.2
0.0 548.8 559.2
0.0 548.8 0.0
2
0 1 2
2 3 0
1.0 1.0 1.0 0.5 0 0
4
549.6 0.0 559.2
0.0 0.0 559.2
0.0 548.8 559.2
556.0 548.8 559.2
2
0 1 2
2 3 0
1.0 1.0 1.0 0.5 0 0
4
0.0 0.0 559.2
0.0 0.0 0.0
0.0 548.8 0.0
0.0 548.8 559.2
2
0 1 2
2 3 0
0.0 1.0 0.0 0.5 0 0
4
552.8 0.0 0.0
549.6 0.0 559.2
556.0 548.8 559.2
556.0 548.8 0.0
2
0 1 2
2 3 0
1.0 0.0 0.0 0.5 0 0
8
130.0 165.0 65.0
82.0 165.0 225.0
240.0 165.0 272.0
290.0 165.0 114.0
290.0 0.0 114.0
240.0 0.0 272.0
130.0 0.0 65.0
82.0 0.0 225.0
12
0 6 3
6 4 3
2 0 3
0 2 1
7 6 0
7 0 1
4 5 3
5 2 3
6 5 4
7 5 6
2 5 1
5 7 1
1.0 1.0 1.0 0.5 0 0
8
423.0 330.0 247.0
265.0 330.0 296.0
314.0 330.0 456.0
472.0 330.0 406.0
423.0 0.0 247.0
472.0 0.0 406.0
314.0 0.0 456.0
265.0 0.0 296.0
12
2 0 3
0 2 1
0 5 3
5 0 4
6 2 3
5 6 3
7 0 1
0 7 4
2 7 1
6 7 2
7 5 4
7 6 5
1.0 1.0 1.0 0.5 0 0
278.0 548.0 79.5 0.1 1 1 1
1
278 273 -800 278 273 0 0 1 0 54.432 80 60
0

Sample test input with both reflection and refraction turned on.
7
4
552.8 0.0 0.0
0.0 0.0 0.0
0.0 0.0 559.2
549.6 0.0 559.2
2
0 1 2
2 3 0
1.0 1.0 1.0 0.5 0 0
4
556.0 548.8 0.0
556.0 548.8 559.2
0.0 548.8 559.2
0.0 548.8 0.0
2
0 1 2
2 3 0
1.0 1.0 1.0 0.5 0 0
4
549.6 0.0 559.2
0.0 0.0 559.2
0.0 548.8 559.2
556.0 548.8 559.2
2
0 1 2
2 3 0
1.0 1.0 1.0 0.5 0 0
4
0.0 0.0 559.2
0.0 0.0 0.0
0.0 548.8 0.0
0.0 548.8 559.2
2
0 1 2
2 3 0
0.0 1.0 0.0 0.5 0 0
4
552.8 0.0 0.0
549.6 0.0 559.2
556.0 548.8 559.2
556.0 548.8 0.0
2
0 1 2
2 3 0
1.0 0.0 0.0 0.5 0 0
8
130.0 165.0 65.0
82.0 165.0 225.0
240.0 165.0 272.0
290.0 165.0 114.0
290.0 0.0 114.0
240.0 0.0 272.0
130.0 0.0 65.0
82.0 0.0 225.0
12
0 6 3
6 4 3
2 0 3
0 2 1
7 6 0
7 0 1
4 5 3
5 2 3
6 5 4
7 5 6
2 5 1
5 7 1
1.0 1.0 1.0 0.1 0.8 1.333
8
423.0 330.0 247.0
265.0 330.0 296.0
314.0 330.0 456.0
472.0 330.0 406.0
423.0 0.0 247.0
472.0 0.0 406.0
314.0 0.0 456.0
265.0 0.0 296.0
12
2 0 3
0 2 1
0 5 3
5 0 4
6 2 3
5 6 3
7 0 1
0 7 4
2 7 1
6 7 2
7 5 4
7 6 5
1.0 1.0 1.0 0.1 0.8 1.333
278.0 548.0 79.5 0.1 1 1 1
1
278 273 -800 278 273 0 0 1 0 54.432 80 60
0

*/