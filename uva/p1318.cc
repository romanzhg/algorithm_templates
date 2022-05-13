// p1318 / la2797
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

const double EPS = 1e-10;

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

Vector2 operator * (const Vector2& v, double s) {
  return Vector2(v.x * s, v.y * s);
}

Vector2 operator / (const Vector2& v, double s) {
  return Vector2(v.x / s, v.y / s);
}

double dotProduct(const Vector2& a, const Vector2& b) {
  return a.x * b.x + a.y * b.y;
}

double crossProduct(const Vector2& a, const Vector2& b) {
  return a.x * b.y - a.y * b.x;
}

double lengthOfVector2(const Vector2& a) {
	return sqrt(dotProduct(a, a));
}

bool areSegmentsProperIntersecting(const Point2& a1, const Point2& a2,
	const Point2& b1, const Point2& b2) {
	double c1 = crossProduct(a2 - a1, b1 - a1),
	    c2 = crossProduct(a2 - a1, b2 - a1),
	    c3 = crossProduct(b2 - b1, a1 - b1),
	    c4 = crossProduct(b2 - b1, a2 - b1);
	return dcmp(c1) * dcmp(c2) < 0 && dcmp(c3) * dcmp(c4) < 0;
}

#define MAX_INPUT_SIZE 1005

const double DELTA = 1e-5;
Point2 segStart[MAX_INPUT_SIZE];
Point2 segEnd[MAX_INPUT_SIZE];
vector<Point2> endPoints;

// Returns true if point p is on the line segment (a1, a2).
// Endpoints not included.
bool isOnSegment(const Point2& p, const Point2& a1, const Point2& a2) {
  return dcmp(crossProduct(a1 - p, a2 - p)) == 0 && dcmp(dotProduct(a1-p, a2 - p)) < 0;
}

bool isPointValid(const Point2& p, int lineSegs) {
	for (int i = 0; i < lineSegs; i++) {
		if (isOnSegment(p, segStart[i], segEnd[i])) {
			return false;
		}
	}
	return true;
}


int main() {
	// 1. Extend all line segments by 4e-6, put all the endpoints to a vector.
	// It is essentially a set, use vector to give each element an index.
	// 1.5 Add A infinite far point, as the exit, and the monster point.
	// 2. Build an adj matrix, for each pair, test if the points are connected.
	// (iterate through all lines).
	// 3. Expend from the monster point, see if the infinite far point can be reached.
	// Need two sets, visited and toExpend, and there is one exit condition.

	int lineSegs;
	while (scanf("%d", &lineSegs) == 1 && lineSegs != 0) {
		endPoints.clear();
		endPoints.push_back(Point2(0, 0));
		endPoints.push_back(Point2(10000, 10000));
		for (int i = 0; i < lineSegs; i++) {
			scanf("%lf %lf %lf %lf", &segStart[i].x, &segStart[i].y, &segEnd[i].x, &segEnd[i].y);
			Vector2 direction = segEnd[i] - segStart[i];
			direction = direction / lengthOfVector2(direction);
			segEnd[i] = segEnd[i] + direction * DELTA;
			segStart[i] = segStart[i] - direction * DELTA;
		}

		for (int i = 0; i < lineSegs; i++) {
			if (isPointValid(segStart[i], lineSegs)) {
				endPoints.push_back(segStart[i]);
			}
			if (isPointValid(segEnd[i], lineSegs)) {
				endPoints.push_back(segEnd[i]);
			}
		}

		int pointsCount = endPoints.size();
		bool connected[pointsCount][pointsCount];
		memset(connected, 0, pointsCount * pointsCount);

		for (int i = 0; i < pointsCount; i++) {
			connected[i][i] = true;
			for (int j = i + 1; j < pointsCount; j++) {
				bool tmpConnected = true;
				// The current line: (endPoints[j], endPoints[i])
				// The line to iterate from: lineSeg.
				for (int k = 0; k < lineSegs; k++) {
					if (areSegmentsProperIntersecting(endPoints[j], endPoints[i], segEnd[k], segStart[k])) {
						tmpConnected = false;
						break;
					}
				}
				connected[i][j] = connected[j][i] = tmpConnected;
			}
		}

		queue<int> toExpend;
		bool visited[pointsCount];
		memset(visited, 0, pointsCount);
		toExpend.push(0);
		visited[0] = true;

		while (!toExpend.empty()) {
			int current = toExpend.front();
			toExpend.pop();
			
			for (int i = 0; i < pointsCount; i++) {
				if (connected[current][i]) {
					if (!visited[i]) {
						toExpend.push(i);
						visited[i] = true;
					}
				}
			}
		}

		printf("%s\n", visited[1] ? "no" : "yes");
	}
	return 0;
}