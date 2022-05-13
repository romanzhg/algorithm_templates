// Build a suffix array.
// Note the upper_bound function used to find the bucket a value is in.
// void helper(const vector<int>& rangeEndPoints, int index) {
//   // Pointer to the first element greater or equal to the input.
//   auto it = upper_bound(rangeEndPoints.begin(),
//                         rangeEndPoints.end(),
//                         index);
//   int inputIndex = distance(rangeEndPoints.begin(), it);
//   flags[inputIndex] = true;
// }
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
#include <unordered_map>
#include <unordered_set>
#include <utility>

using namespace std;
typedef long long ll;

const int MAX_LINES = 105;
const int MAX_LINE_SIZE = 1050;
const int MAX_LEN = MAX_LINES * MAX_LINE_SIZE;

bool flags[MAX_LINES];
char input[MAX_LINES][MAX_LINE_SIZE];

int lifeForms;

string data;

// For separator.
int c1 = 0;
int c2 = 0;

string getSeparator() {
	string rtn;
	rtn.push_back('A' + c1);
	c1 = (c1 + 1) % 26;
	rtn.push_back('A' + c2);
	if (c1 == 0) {
		c2++;
	}
	return rtn;
}

// For suffix array.
int sa[MAX_LEN], t[MAX_LEN], t2[MAX_LEN], c[MAX_LEN], dataLen;

// Input m here should be the max len.
void build_sa(int m) {
	int *x = t, *y = t2;

	for (int i = 0; i < m; i++) {
		c[i] = 0;
	}
	for (int i = 0; i < dataLen; i++) {
		c[x[i] = data[i]]++;
	}
	for (int i = 1; i < m; i++) {
		c[i] += c[i - 1];
	}
	for (int i = dataLen - 1; i >= 0; i--) {
		sa[--c[x[i]]] = i;
	}
	for (int k = 1; k <= dataLen; k <<= 1) {
		int tmpCounter = 0;
		for (int i = dataLen - k; i < dataLen; i++) {
			y[tmpCounter++] = i;
		}
		for (int i = 0; i < dataLen; i++) {
			if (sa[i] >= k) {
				y[tmpCounter++] = sa[i] - k;
			}
		}

		for (int i = 0; i < m; i++) {
			c[i] = 0;
		}
		for (int i = 0; i < dataLen; i++) {
			c[x[y[i]]]++;
		}
		for (int i = 0; i < m; i++) {
			c[i] += c[i - 1];
		}
		for (int i = dataLen - 1; i >= 0; i--) {
			sa[--c[x[y[i]]]] = y[i];
		}

		swap(x, y);
		tmpCounter = 1;
		x[sa[0]] = 0;
		for (int i = 1; i < dataLen; i++) {
			x[sa[i]] = (y[sa[i - 1]] == y[sa[i]]
				&& y[sa[i - 1] + k] == y[sa[i] + k]) ? tmpCounter - 1 : tmpCounter++;
		}
		if (tmpCounter >= dataLen) {
			break;
		}
		m = tmpCounter;
	}
}

// Rank and height.
// sa contains rankToIndex.
int indexToRank[MAX_LEN], height[MAX_LEN];
void getHeight() {
	int k = 0;
	for (int i = 0; i < dataLen; i++) {
		indexToRank[sa[i]] = i;
	}
	for (int i = 0; i < dataLen; i++) {
		if (k) {
			k--;
		}
		if (indexToRank[i] == 0) {
			continue;
		}

		int j = sa[indexToRank[i] - 1];
		while (data[i + k] == data[j + k]) {
			k++;
		}
		height[indexToRank[i]] = k;
	}
}


// Helper function.
void helper(const vector<int>& rangeEndPoints, int index) {
	// Pointer to the first element greater or equal to the input.
	auto it = upper_bound(rangeEndPoints.begin(),
												rangeEndPoints.end(),
												index);
	int inputIndex = distance(rangeEndPoints.begin(), it);
	flags[inputIndex] = true;
}

int countFlags() {
	int counter = 0;
	for (int i = 0; i < lifeForms; i++) {
		if (flags[i]) {
			counter++;
		}
	}
	return counter;
}

// Binary search in the height array.
// If the input "len" is valid, add the index of range start.
bool isLenValid(int len, vector<int>& rangeStart,
	const vector<int>& rangeEndPoints) {
	if (len == 0) {
		return true;
	}

	// Pointers to the height array.
	int start = 2 * lifeForms;
	int end;
	bool rtn = false;

	while (start < dataLen) {
		memset(flags, 0, sizeof(flags));
		end = start;
		while (height[end] >= len && end < dataLen) {
			helper(rangeEndPoints, sa[end]);
			helper(rangeEndPoints, sa[end - 1]);
			end++;
		}
		if (countFlags() >= (lifeForms / 2 + 1)) {
			rangeStart.push_back(start);
			rtn = true;
		}
		start = end + 1;
	}
	return rtn;
}

bool isFirstTestCase = true;

int main() {
	while (true) {
		scanf("%d", &lifeForms);
		if (!lifeForms) break;

		if (isFirstTestCase) {
			isFirstTestCase = false;
		} else {
			printf("\n");
		}

		data.clear();
		c1 = 0;
		c2 = 0;

		int inputMaxLen = 0;
		vector<int> rangeEndPoints;
		for (int i = 0; i < lifeForms; i++) {
			scanf("%s", input[i]);
			inputMaxLen = max(inputMaxLen, (int)strlen(input[i]));
		}
		for (int i = 0; i < lifeForms; i++) {
			data.append(input[i]);
			data.append(getSeparator());
			rangeEndPoints.push_back(data.size());
		}
		dataLen = data.size();

		if (lifeForms == 1) {
			printf("%s\n", input[0]);
			continue;
		}

		build_sa(MAX_LEN);

		getHeight();

		int lower, higher;
		lower = 0;
		higher = inputMaxLen;
		vector<int> rangeStart;
		rangeStart.clear();
		while (lower < higher) {
			int mid = lower + (higher - lower + 1) / 2;
			vector<int> tmpRangeStart;
			if (isLenValid(mid, tmpRangeStart, rangeEndPoints)) {
				lower = mid;
				rangeStart = tmpRangeStart;
			} else {
				higher = mid - 1;
			}
		}

		if (lower == 0) {
			printf("?\n");
		} else {
			bool printed = false;
			for (int tmpRank : rangeStart) {
				string tmpStr = data.substr(sa[tmpRank], lower);
				if (tmpStr.size() == 1 && 
					((tmpStr[0] <= 'Z' && tmpStr[0] >= 'A'))) {
					continue;
				} else {
					printed = true;
					printf("%s\n", tmpStr.c_str());
				}
			}
			if (!printed) {
				printf("?\n");
			}
		}
	}
	return 0;
}
