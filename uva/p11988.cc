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

void append(list<string>& result, string& arg, bool append_to_tail) {
	if (arg.size() == 0) {
		return;
	}
	if (append_to_tail) {
		result.push_back(arg);
	} else {
		result.push_front(arg);
	}
}

int main() {
	string line;
	while (cin >> line) {
		list<string> result;
		string tmp;
		bool append_to_tail = true;
		for (char c : line) {
			if (c == '[') {
				append(result, tmp, append_to_tail);
				tmp.clear();
				append_to_tail = false;
			} else if (c == ']') {
				append(result, tmp, append_to_tail);
				tmp.clear();
				append_to_tail = true;
			} else {
				tmp += c;
			}
		}
		append(result, tmp, append_to_tail);
		string rtn;
		for (string piece : result) {
			rtn += piece;
		}
		printf("%s\n", rtn.c_str());
	}
	return 0;
}