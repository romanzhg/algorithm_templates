// High precision computaton:
// Whenever it is possible to use long long, we should keep on using long long.
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

typedef long long ll;
const ll LL_INF = 0x3f3f3f3f;

// Greatest common divisor.
ll gcd(ll a, ll b) {
	return b == 0 ? a : gcd(b, a % b);
}

// A rational number represented by a / b.
struct Rational {
	ll a;
	ll b;

	Rational(ll ia = 0, ll ib = 1) {
		ll divisor = gcd(ia, ib);
		ia = ia / divisor;
		ib = ib / divisor;
		if (ib < 0) {
			ia = -ia;
			ib = -ib;
		}
		this->a = ia;
		this->b = ib;
	};
};

Rational operator + (const Rational& l, const Rational& r) {
	ll divisor = gcd(l.b, r.b);
  return Rational(l.a * (r.b / divisor) + r.a * (l.b / divisor), l.b * (r.b / divisor));
}

Rational operator - (const Rational& l, const Rational& r) {
	ll divisor = gcd(l.b, r.b);
  return Rational(l.a * (r.b / divisor) - r.a * (l.b / divisor), l.b * (r.b / divisor));
}

Rational operator * (const Rational& l, const Rational& r) {
  return Rational(l.a * r.a, l.b * r.b);
}

Rational operator * (const Rational& l, const ll& r) {
  return Rational(l.a * r, l.b);
}

Rational operator / (const Rational& l, const Rational& r) {
  return Rational(l.a * r.b, r.a * l.b);
}

// All the rationals compared in this program is larger than 0.
// This operator only works when both l and r are larger than 0.
bool operator < (const Rational& l, const Rational& r) {
  return l.a * r.b < r.a * l.b;
}

int dcmp(Rational u) {
	if (u.a == 0) {
		return 0;
	} else {
		return u.a < 0 ? -1 : 1;
	}
}

struct Point3 {
	int x, y, z;
	Point3(int x = 0, int y = 0, int z = 0): x(x), y(y), z(z) {};
};

typedef Point3 Vector3;

Vector3 operator + (const Vector3& a, const Vector3& b) {
	return Vector3(a.x + b.x, a.y + b.y, a.z + b.z);
}

Vector3 operator - (const Vector3& a, const Vector3& b) {
	return Vector3(a.x - b.x, a.y - b.y, a.z - b.z);
}

Vector3 operator * (const Vector3& a, int p) {
	return Vector3(a.x * p, a.y * p, a.z * p);
}

Vector3 operator / (const Vector3& a, int p) {
	return Vector3(a.x / p, a.y / p, a.z / p);
}

bool operator == (const Vector3& a, const Vector3& b) {
	return a.x == b.x
			&& a.y == b.y
			&& a.z == b.z;
}

typedef Point3 Vector3;

ll dotProduct(const Vector3& a, const Vector3& b) {
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

Vector3 crossProduct(const Vector3& a, const Vector3& b) {
  return Vector3(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x);
}

ll lengthOfVector3Sqr(const Vector3& a) {
	return dotProduct(a, a);
}

/* Solution. */ 
bool getLineLineShortestDistPoint(const Point3& p1, const Vector3& u,
									const Point3& p2, const Vector3& v,
									Rational& s) {
	ll b = dotProduct(u, u) * dotProduct(v, v)
			- dotProduct(u, v) * dotProduct(u, v);
	if (b == 0) {
		return false;
	}
	ll a = dotProduct(u, v) * dotProduct(v, p1 - p2)
			- dotProduct(v, v) * dotProduct(u, p1 - p2);
	s = Rational(a, b);
	return true;
}

Rational distanceToSegmentSqr(const Point3& p, const Point3& a, const Point3& b) {
	if (a == b) {
		return lengthOfVector3Sqr(p - a);
	}
	Vector3 v1 = b - a, v2 = p - a, v3 = p - b;
	if (dotProduct(v1, v2) < 0) {
		return Rational(lengthOfVector3Sqr(v2));
	} else if (dotProduct(v1, v3) > 0) {
		return Rational(lengthOfVector3Sqr(v3));
	} else {
		return Rational(lengthOfVector3Sqr(crossProduct(v1, v2)), lengthOfVector3Sqr(v1));
	}
}

Rational distBetweenLines(const Point3& a, const Point3& b, const Point3& c, const Point3& d) {
  Rational s, t;
  bool flag0 = getLineLineShortestDistPoint(a, b - a, c, d - c, s);
  bool flag1 = getLineLineShortestDistPoint(c, d - c, a, b - a, t);

  if (flag0 && flag1
  		&& s.a > 0 && s.a < s.b
  		&& t.a > 0 && t.a < t.b) {
  	Vector3 u = b - a;
  	Vector3 v = d - c;
  	Rational x1 = Rational(a.x) + s * u.x;
  	Rational y1 = Rational(a.y) + s * u.y;
  	Rational z1 = Rational(a.z) + s * u.z;
  	Rational x2 = Rational(c.x) + t * v.x;
  	Rational y2 = Rational(c.y) + t * v.y;
  	Rational z2 = Rational(c.z) + t * v.z;
  	return (x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1) + (z2 - z1) * (z2 - z1);
  } else {
  	Rational minDist(LL_INF);
  	minDist = min(minDist, distanceToSegmentSqr(a, c, d));
  	minDist = min(minDist, distanceToSegmentSqr(b, c, d));
  	minDist = min(minDist, distanceToSegmentSqr(c, a, b));
  	minDist = min(minDist, distanceToSegmentSqr(d, a, b));
  	return minDist;
  }
}

int main() {
	int test_cases;
	scanf("%d", &test_cases);

	while (test_cases--) {
		Point3 a, b, c, d;
		scanf("%d %d %d", &a.x, &a.y, &a.z);
		scanf("%d %d %d", &b.x, &b.y, &b.z);
		scanf("%d %d %d", &c.x, &c.y, &c.z);
		scanf("%d %d %d", &d.x, &d.y, &d.z);

		Rational result = distBetweenLines(a, b, c, d);
		printf("%lld %lld\n", result.a, result.b);
	}

	return 0;
}