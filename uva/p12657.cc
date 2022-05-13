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
#include <list>
using namespace std;

#define MAX_SIZE 100100

int left_ptr[MAX_SIZE];
int right_ptr[MAX_SIZE];

void link(int l, int r) {
	right_ptr[l] = r;
	left_ptr[r] = l;
}

int main() {
	int case_num = 1;
	int boxes, lines;
	while (scanf("%d %d", &boxes, &lines) == 2) {
		// Initialize the boxes.
		for (int i = 0; i <= boxes + 10; i++) {
			left_ptr[i] = i - 1;
			right_ptr[i] = i + 1;
		}
		// Initialize bool inv.
		bool inv = false;

		int op, arg1, arg2;
		for (int i = 0; i < lines; i++) {
			scanf("%d", &op);
			if (op == 4) {
				inv = !inv;
			} else {
				scanf("%d", &arg1);
				scanf("%d", &arg2);
				if (op == 3 && right_ptr[arg2] == arg1) {
					swap(arg1, arg2);
				}
				if (inv && (op == 1 || op == 2)) {
					op = 3 - op;
				}
				if (op == 1 && arg1 == left_ptr[arg2]) {
					continue;
				}
				if (op == 2 && arg1 == right_ptr[arg2]) {
					continue;
				}

				// Handle the ops.
				if (op == 1) {
					// arg1 to the left_ptr of arg2
					link(left_ptr[arg1], right_ptr[arg1]);
					int new_left_ptr = left_ptr[arg2];
					int new_right_ptr = arg2;
					link(new_left_ptr, arg1);
					link(arg1, new_right_ptr);
				} else if (op == 2) {
					// arg1 to the right_ptr of arg2
					link(left_ptr[arg1], right_ptr[arg1]);
					int new_left_ptr = arg2;
					int new_right_ptr = right_ptr[arg2];
					link(new_left_ptr, arg1);
					link(arg1, new_right_ptr);
				} else {
					// op == 3
					int arg1_left = left_ptr[arg1], arg1_right = right_ptr[arg1];
					int arg2_left = left_ptr[arg2], arg2_right = right_ptr[arg2];
					if (right_ptr[arg1] == arg2) {
						link(arg1_left, arg2);
						link(arg2, arg1);
						link(arg1, arg2_right);
					} else {
						link(arg1_left, arg2);
						link(arg2, arg1_right);

						link(arg2_left, arg1);
						link(arg1, arg2_right);
					}
				}
			}
		}

		// Output the result.
		long long result = 0;
		int head;
		if (inv) {
			head = boxes + 1;
			head = left_ptr[head];
			while (head > 0) {
				result += head;
				head = left_ptr[head];
				head = left_ptr[head];
			}
		} else {
			head = 0;
			head = right_ptr[head];
			while (head <= boxes) {
				result += head;
				head = right_ptr[head];
				head = right_ptr[head];
			}
		}
		printf("Case %d: %lld\n", case_num++, result);
	}
	return 0;
}