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

struct Matrix {
	int a, b;
	Matrix(): a(0), b(0) {};
	Matrix(int x, int y): a(x), b(y) {};
};

Matrix input_matrix[26];

int main() {
	int matrixes;
	cin >> matrixes;
	for (int i = 0; i < matrixes; i++) {
		string name;
		cin >> name;
		int k = name[0];
		cin >> input_matrix[k - 'A'].a;
		cin >> input_matrix[k - 'A'].b;
	}

	string line;

	while (cin >> line) {
		stack<Matrix> matrixStack;
		long result = 0;
		bool got_error = false;
		for (char c : line) {
			if (c == '(') {
				continue;
			}
			else if (c == ')') {
				if (matrixStack.size() < 2) {
					got_error = true;
					break;
				}
				Matrix first, second;
				second = matrixStack.top(); matrixStack.pop();
				first = matrixStack.top(); matrixStack.pop();
				if (first.b != second.a) {
					got_error = true;
					break;
				}
				result += first.a * first.b * second.b;
				matrixStack.push(Matrix(first.a, second.b));
			} else {
				matrixStack.push(input_matrix[c - 'A']);
			}
		}
		if (got_error) {
			printf("error\n");
		} else {
			printf("%ld\n", result);
		}
	}

	return 0;
}