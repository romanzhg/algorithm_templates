// Runtime 1.970s.
// Optimization used: 
// 1. Pre computed moves.
// 2. State compression.
// 3. Visited check implemented as array check.
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
#define MAX_STATES 6291456

unsigned int submask0 = 0x0000000F;

struct State {
	int ah;
	int aw;
	int bh;
	int bw;
	int ch;
	int cw;
	State(int ah = -1, int aw = -1, int bh = -1,
		int bw = -1, int ch = -1, int cw = -1)
		: ah(ah), aw(aw), bh(bh), bw(bw), ch(ch), cw(cw) {};
};

#ifndef ONLINE_JUDGE
#endif

char maze[16][16];
bool visited[300][300][300];
queue<unsigned int> states;
vector<pair<int, int>> newLocations[16][16];

unsigned int encode(const State& state) {
	// The value should have 24 bits set.
	// The upper 8 bits are set to 0.
	unsigned int rtn = 0;
	
	rtn = rtn | ((state.cw - 1) & submask0);
	rtn = rtn | (((state.ch - 1) & submask0) << 4);
	rtn = rtn | (((state.bw - 1) & submask0) << 8);
	rtn = rtn | (((state.bh - 1) & submask0) << 12);
	rtn = rtn | (((state.aw - 1) & submask0) << 16);
	rtn = rtn | (((state.ah - 1) & submask0) << 20);

	return rtn;
}

void decode(unsigned int value, State& state) {
	state.cw = (value & submask0) + 1;
	state.ch = ((value >> 4) & submask0) + 1;
	state.bw = ((value >> 8) & submask0) + 1;
	state.bh = ((value >> 12) & submask0) + 1;
	state.aw = ((value >> 16) & submask0) + 1;
	state.ah = ((value >> 20) & submask0) + 1;
}

// count with value 0 for push the new state.
// count with value 1 for processing ah and aw.
// count with value 2 for processing bh and bw.
// count with value 3 for processing ch and cw.
void expand(const State& curState, State& newState, int count) {
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

		states.push(newEncoded);
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
			expand(curState, newState, count - 1);
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
			expand(curState, newState, count - 1);
		}
	}
	if (count == 3) {
		for (const pair<int, int>& newloc : newLocations[curState.ch][curState.cw]) {
			newState.ch = newloc.first;
			newState.cw = newloc.second;
			expand(curState, newState, count - 1);
		}
	}
}

void solve(const State& initial, const State& target, int count) {
	int dist = 0;
	states = queue<unsigned int>();

	unsigned int targetEncoded = encode(target);

	states.push(encode(initial));
	visited[initial.ah * 16 + initial.aw]
		[initial.bh * 16 + initial.bw]
		[initial.ch * 16 + initial.cw] = true;

	int levelSize = 1;
	while (!states.empty()) {
		unsigned int curEncoded = states.front();
		states.pop();
		levelSize--;

		if (curEncoded == targetEncoded) {
			break;
		}

		State curState;
		decode(curEncoded, curState);
		State newState = curState;
		expand(curState, newState, count);

		if (levelSize == 0) {
			levelSize = states.size();
			dist++;
		}
	}
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
		memset(visited, 0, sizeof(visited));

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