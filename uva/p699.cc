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

#define MAX_N 10000

int sum_array[MAX_N];
bool is_empty_tree;
int min_index, max_index;

void scan_tree(int index) {
	int value = 0;
	cin >> value;
	if (value == -1) {
		return;
	}

	min_index = min(min_index, index);
	max_index = max(max_index, index);

	is_empty_tree = false;
	sum_array[index] += value;
	scan_tree(index - 1);
	scan_tree(index + 1);
}

int main() {
	int case_num = 1;
	while (true) {
		is_empty_tree = true;

		memset(sum_array, 0, sizeof(sum_array));

		min_index = MAX_N / 2;
		max_index = MAX_N / 2;
		scan_tree(MAX_N / 2);
		if (is_empty_tree) {
			break;
		}

		// Print result.

		printf("Case %d:\n", case_num++);
		printf("%d", sum_array[min_index]);
		for (int i = min_index + 1; i <= max_index; i++) {
			printf(" %d", sum_array[i]);
		}
		printf("\n\n");
	}
	return 0;
}