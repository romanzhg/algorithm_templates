#include <cstdio>
#include <cstdlib>
#include <climits>
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
using namespace std;

// Notes: No blank after the last input. so the bool is_first is needed.
bool is_valid;

int judge() {
	int wl, dl, wr, dr;
	cin >> wl;
	cin >> dl;
	cin >> wr;
	cin >> dr;
	if (wl == 0) {
		wl = judge();
	}
	if (wr == 0) {
		wr = judge();
	} 
	if (wl * dl != wr * dr) {
		is_valid = false;
	}
	return wl + wr;
}

int main() {
	bool is_first = true;
	int test_cases;
	cin >> test_cases;
	while (test_cases--) {
		is_valid = true;
		judge();

		if (is_first) {
			is_first = false;
		} else {
			printf("\n");
		}
		if (is_valid) {
			printf("YES\n");
		} else {
			printf("NO\n");
		}

	}
	return 0;
}