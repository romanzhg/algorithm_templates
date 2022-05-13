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
#define INF 1000000000

#ifndef ONLINE_JUDGE
#endif

struct State {
	int a, b, c;

	State(int a = 0, int b = 0, int c = 0): a(a), b(b), c(c) {}

	int& operator[](int i) {
		if (i == 0) {
			return a;
		} else if (i == 1) {
			return b;
		} else {
			return c;
		}
	}
};

struct Element {
	int dist;
	State s;
	Element(int dist = 0, State s = State()): dist(dist), s(s) {}

	bool operator < (const Element& o) const {
		return dist > o.dist;
	}
};

set<int> visited;

int target;
int capacity[3];
int moveFrom[6] = {0, 0, 1, 1, 2, 2};
int moveTo[6] = {1, 2, 0, 2, 0, 1};
int minDists[201];

// Encode the 3 water levels to a state.
int encode(int a, int b, int c) {
  return a * 201 * 201 + b * 201 + c;
}

void decode(int state, int& a, int& b, int& c) {
  a = state / (201 * 201);
  state = state - a * 201 * 201;
  b = state / 201;
  c = state % 201;
}

// Perform a move, return true if the move is valid.
bool move(int from, int to, State& currentState, State& newState, int& dist) {
	int fromAmount = currentState[from];
	int toCapacity = capacity[to] - currentState[to];
	if (fromAmount == 0 || toCapacity == 0) {
		return false;
	}
	int moveAmount = min(fromAmount, toCapacity);
	dist = moveAmount;
	newState = currentState;
	newState[from] = currentState[from] - moveAmount;
	newState[to] = currentState[to] + moveAmount;
	return true;
}

void moves(priority_queue<Element>& pq, State& currentState, int currentDist) {
	State newState;
	int dist;
	for (int i = 0; i < 6; i++) {
		if (move(moveFrom[i], moveTo[i], currentState, newState, dist)) {
			pq.emplace(currentDist + dist, newState);
		}
	}
}

// Return true if the target is met.
bool updateMetrics(const State& state, int dist) {
	// If the target is met, returns true.
	if (state.a == target || state.b == target || state.c == target) {
		minDists[target] = dist;
		return true;
	}
	// The first dist we ever see for a state should gives the best answer,
	// should not need the min.
	if (target > state.a && minDists[state.a] == INF) {
		minDists[state.a] = dist;
	}
	if (target > state.b && minDists[state.b] == INF) {
		minDists[state.b] = dist;
	}
	if (target > state.c && minDists[state.c] == INF) {
		minDists[state.c] = dist;
	}
	return false;
}

void solve(int a, int b, int c) {
	priority_queue<Element> pq;

	pq.emplace(0, State(0, 0, c));
	while (!pq.empty()) {
		Element tmp = pq.top();
		pq.pop();
		// Should check for repetition here and here only.
		if (visited.find(encode(tmp.s.a, tmp.s.b, tmp.s.c)) != visited.end()) {
			continue;
		}
		visited.insert(encode(tmp.s.a, tmp.s.b, tmp.s.c));

		if (updateMetrics(tmp.s, tmp.dist)) {
			break;
		}
		moves(pq, tmp.s, tmp.dist);
	}
	for (int i = target; i >= 0; i--) {
		if (minDists[i] != INF) {
			printf("%d %d\n", minDists[i], i);
			return;
		}
	}
}

int main() {
	int testCases;
	scanf("%d", &testCases);
	while (testCases--) {
		for (int i = 0; i < 201; i++) {
			minDists[i] = INF;
		}
		visited.clear();
		scanf("%d %d %d %d", &capacity[0], &capacity[1], &capacity[2], &target);
		solve(0, 0, capacity[2]);
	}
	return 0;
}