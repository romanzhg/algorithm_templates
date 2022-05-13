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

#ifndef ONLINE_JUDGE
#endif

int numbers[20];
int main() {
	int numberCount;
	int caseNumber = 1;
	while (scanf("%d", &numberCount) == 1) {
		for (int i = 0; i < numberCount; i++) {
			scanf("%d", &numbers[i]);
		}
		ll maxValue = 0;
		for (int i = 0; i < numberCount; i++) {
			for (int j = i; j < numberCount; j++) {
				ll tmp = 1;
				for (int k = i; k <= j; k++) {
					tmp = tmp * numbers[k];
				}
				maxValue = max(maxValue, tmp);
			}
		}
		printf("Case #%d: The maximum product is %lld.\n\n", caseNumber++, maxValue);
	}
	return 0;
}