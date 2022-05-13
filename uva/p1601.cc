// Run time: 1.210s.
// Optimization used:
// 1. Pre computed moves.
// 2. State compression.
// 3. Bidirectional search.
// 4. Visited check implemented as array check(from 4.92s to 1.21s).
// This program allocates 216MB memory, mainly for visitedLeft and visitedRight.
#include <cfloat>
#include <climits>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
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
#include <unordered_set>
#include <utility>

using namespace std;
typedef long long ll;
#define INF 1000000000

struct State {
	int ah;
	int aw;
	int bh;
	int bw;
	int ch;
	int cw;
	State(int ah = -1, int aw = -1, int bh = -1,
		int bw = -1, int ch = -1, int cw = -1)
		: ah(ah), aw(aw), bh(bh), bw(bw), ch(ch), cw(cw){};
};

#ifndef ONLINE_JUDGE
#endif

char maze[16][16];
vector<pair<int, int>> newLocations[16][16];

// States expended from source.
bool visitedLeft[300][300][300];
queue<State> statesLeft;

// States expended from target.
bool visitedRight[300][300][300];
queue<State> statesRight;

unsigned int encode(const State& state) {
	// The value should have 24 bits set.
	// The upper 8 bits are set to 0.
	unsigned int rtn = 0;
	unsigned int submask = 0x0000000F;
	rtn = rtn | ((state.cw - 1) & submask);
	rtn = rtn | (((state.ch - 1) & submask) << 4);
	rtn = rtn | (((state.bw - 1) & submask) << 8);
	rtn = rtn | (((state.bh - 1) & submask) << 12);
	rtn = rtn | (((state.aw - 1) & submask) << 16);
	rtn = rtn | (((state.ah - 1) & submask) << 20);

	return rtn;
}

// count with value 0 for push the new state.
// count with value 1 for processing ah and aw.
// count with value 2 for processing bh and bw.
// count with value 3 for processing ch and cw.
// Note: it is possible to pass in the array as a one dimensional array.
// We just need some routine that knows the size of the second and third
// dimension.
void expand(const State& curState, State& newState, int count,
	queue<State>& states, bool visited[][300][300]) {
	if (count == 0) {
		unsigned int newEncoded = encode(newState);

		if (visited[newState.ah * 16 + newState.aw]
					[newState.bh * 16 + newState.bw]
					[newState.ch * 16 + newState.cw] == true) {
			return;
		}
		visited[newState.ah * 16 + newState.aw]
					[newState.bh * 16 + newState.bw]
					[newState.ch * 16 + newState.cw] = true;

		states.push(newState);
		return;
	}
	if (count == 1) {
		for (const pair<int, int>& newloc : newLocations[curState.ah][curState.aw]) {
			if (newloc.first == newState.ch && newloc.second == newState.cw) {
				continue;
			}
			if (newloc.first == newState.bh && newloc.second == newState.bw) {
				continue;
			}
			if (newloc.first == curState.ch && newloc.second == curState.cw
				&& newState.ch == curState.ah && newState.cw == curState.aw) {
				continue;
			}
			if (newloc.first == curState.bh && newloc.second == curState.bw
				&& newState.bh == curState.ah && newState.bw == curState.aw) {
				continue;
			}
			newState.ah = newloc.first;
			newState.aw = newloc.second;
			expand(curState, newState, count - 1, states, visited);
		}
	}
	if (count == 2) {
		for (const pair<int, int>& newloc : newLocations[curState.bh][curState.bw]) {
			// New locations conflict.
			if (newloc.first == newState.ch && newloc.second == newState.cw) {
				continue;
			}
			// Swap.
			if (newloc.first == curState.ch && newloc.second == curState.cw
				&& newState.ch == curState.bh && newState.cw == curState.bw) {
				continue;
			}
			newState.bh = newloc.first;
			newState.bw = newloc.second;
			expand(curState, newState, count - 1, states, visited);
		}
	}
	if (count == 3) {
		for (const pair<int, int>& newloc : newLocations[curState.ch][curState.cw]) {
			newState.ch = newloc.first;
			newState.cw = newloc.second;
			expand(curState, newState, count - 1, states, visited);
		}
	}
}

void solve(const State& initial, const State& target, int count) {
	statesLeft = queue<State>();
	statesRight = queue<State>();

	int dist = 0;
	statesLeft.push(initial);
	int levelSizeLeft = 1;
	visitedLeft[initial.ah * 16 + initial.aw]
		[initial.bh * 16 + initial.bw]
		[initial.ch * 16 + initial.cw] = true;

	statesRight.push(target);
	int levelSizeRight = 1;
	visitedRight[target.ah * 16 + target.aw]
		[target.bh * 16 + target.bw]
		[target.ch * 16 + target.cw] = true;

	while (true) {
		while (true) {
			State tmp = statesLeft.front();
			statesLeft.pop();
			levelSizeLeft--;

			unsigned int tmpEncoded = encode(tmp);
			if (visitedRight[tmp.ah * 16 + tmp.aw]
							[tmp.bh * 16 + tmp.bw]
							[tmp.ch * 16 + tmp.cw] == true) {
				goto endLoop;
			}

			State newState = tmp;
			expand(tmp, newState, count, statesLeft, visitedLeft);

			if (levelSizeLeft == 0) {
				levelSizeLeft = statesLeft.size();
				dist++;
				break;
			}
		}

		while (true) {
			State tmp = statesRight.front();
			statesRight.pop();
			levelSizeRight--;

			unsigned int tmpEncoded = encode(tmp);
			if (visitedLeft[tmp.ah * 16 + tmp.aw]
							[tmp.bh * 16 + tmp.bw]
							[tmp.ch * 16 + tmp.cw] == true) {
				goto endLoop;
			}

			State newState = tmp;
			expand(tmp, newState, count, statesRight, visitedRight);

			if (levelSizeRight == 0) {
				levelSizeRight = statesRight.size();
				dist++;
				break;
			}
		}
	}

endLoop:
	printf("%d\n", dist);
}

int hMove[5] = {0, 1, -1, 0, 0};
int wMove[5] = {0, 0, 0, 1, -1};

void updateNewLocations(int h, int w) {
	for (int i = 1; i < h - 1; i++) {
		for (int j = 1; j < w - 1; j++) {
			newLocations[i][j].clear();
			for (int k = 0; k < 5; k++) {
				if (maze[i][j] == ' '
					&& maze[i + hMove[k]][j + wMove[k]] == ' ') {
					newLocations[i][j].push_back(make_pair(i + hMove[k], j + wMove[k]));
				}
			}
		}
	}
}

int main() {
	#ifndef ONLINE_JUDGE
	clock_t t;
	t = clock();
	#endif

	int w, h, n;
	while (scanf("%d %d %d", &w, &h, &n) == 3 && (w + h + n) != 0) {
		memset(visitedLeft, 0, sizeof(visitedLeft));
		memset(visitedRight, 0, sizeof(visitedRight));

		State initial, target;
		for (int i = 0; i < h; i++) {
			for (int j = 0; j < w; j++) {
				char c = getchar();
				while (c == '\n') {
					c = getchar();
				}
				if (c == '#') {
					maze[i][j] = '#';
				} else {
					maze[i][j] = ' ';
				}
				if (c == 'a') {
					initial.ah = i;
					initial.aw = j;
				}
				if (c == 'b') {
					initial.bh = i;
					initial.bw = j;
				}
				if (c == 'c') {
					initial.ch = i;
					initial.cw = j;
				}
				if (c == 'A') {
					target.ah = i;
					target.aw = j;
				}
				if (c == 'B') {
					target.bh = i;
					target.bw = j;
				}
				if (c == 'C') {
					target.ch = i;
					target.cw = j;
				}
			}
		}
		updateNewLocations(h, w);
		solve(initial, target, n);
	}

	#ifndef ONLINE_JUDGE
	t = clock() - t;
  printf ("The program took %f seconds.\n", ((float)t) / CLOCKS_PER_SEC);
  #endif

	return 0;
}