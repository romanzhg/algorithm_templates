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

const int INPUT_SIZE = 100000 + 100;
int input[INPUT_SIZE];

vector<int> rangeStart;
vector<int> rangeLen;
vector<int> rangeLeftIndex;
vector<int> rangeRightIndex;

// 17 = ceil(log2(INPUT_SIZE)).
int rmqStruct[INPUT_SIZE][17];

void rmq_init() {
  int n = rangeLen.size();
  for (int i = 0; i < n; i++) {
    rmqStruct[i][0] = rangeLen[i];
  }
  for (int j = 1; (1<<j) <= n; j++) {
    for (int i = 0; i + (1<<j) - 1 < n; i++) {
      rmqStruct[i][j] = max(rmqStruct[i][j-1], rmqStruct[i + (1 << (j - 1))][j-1]);
    }
  }
}

// Range maximum query.
int rmq_query(int l, int r) {
  if (l > r) {
    return 0;
  }
  int k = 0;
  while ((1 << (k + 1)) <= (r - l + 1)) {
    k++;
  }
  return max(rmqStruct[l][k], rmqStruct[r - (1 << k) + 1][k]);
}

// Get the index of the largest element in array start which is smaller than v.
int getIndex(int v, vector<int>& a) {
  auto upper = upper_bound(a.begin(), a.end(), v);
  if (upper == a.end()) {
    return a.size() - 1;
  } else {
    return distance(a.begin(), upper) - 1;
  }
}

int solve(int l, int r) {
  int leftIndex = getIndex(l, rangeStart);
  int rightIndex = getIndex(r, rangeStart);
  if (leftIndex == rightIndex) {
    return r - l + 1;
  } else {
    return max(
      max(rangeRightIndex[leftIndex] - l + 1,
        r - rangeLeftIndex[rightIndex] + 1),
      rmq_query(leftIndex + 1, rightIndex - 1));
  }
}

int main() {
  int numbers, queries;
  while (true) {
    scanf("%d", &numbers);
    if (numbers == 0) {
      break;
    }

    scanf("%d", &queries);

    for (int i = 1; i <= numbers; i++) {
      scanf("%d", &input[i]);
    }

    rangeStart.clear();
    rangeLen.clear();
    rangeLeftIndex.clear();
    rangeRightIndex.clear();

    // Build RMQ structure.
    // Vector of start index.
    int current = -200000;
    for (int i = 1; i <= numbers; i++) {
      // The start event.
      if (input[i] != current) {
        rangeStart.push_back(i);
        rangeLeftIndex.push_back(i);
        current = input[i];
      }

      // The end event.
      if (i == numbers || current != input[i + 1]) {
        rangeRightIndex.push_back(i);
        rangeLen.push_back(i - rangeStart.back() + 1);
      }
    }

    // Build the range maximum query structure on len.
    rmq_init();

    // Answer the query.
    for (int i = 0; i < queries; i++) {
      int l, r;
      scanf("%d %d", &l, &r);
      int result = solve(l, r);
      printf("%d\n", result);
    }
  }
  return 0;
}