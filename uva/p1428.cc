// The first element of the BIT(binary indexed tree) is not used.
// So to build an BIT for a normal array of integers, the normal array
// needs to put its first element at index 1, and range sum can be found by
// calling sum(endIndex) - sum(startIndex - 1), to get all previous sum, the start
// index is 1. sum(0) is always 0.
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

const int MAX_VALUE = 100000;
const int INPUT_SIZE = 20000 + 100;

int input[INPUT_SIZE];
int smallerThanLeft[INPUT_SIZE];
int smallerThanRight[INPUT_SIZE];

int bitArray[MAX_VALUE + 1];

int testCaseSize;

// Add value v to element indexed x, for binary indexed tree bit.
void add(int x, int v, int* bit) {
  while (x <= MAX_VALUE) {
    bit[x] += v;
    x += x & (-x);
  }
}

// Sum of elements indexed 1 ~ x, for binary indexed tree bit.
int getSum(int x, int* bit) {
  int rtn = 0;
  while (x > 0) {
    rtn += bit[x];
    x -= x & (-x);
  }
  return rtn;
}

int main() {
  int t;
  scanf("%d", &t);
  while (t--) {
    scanf("%d", &testCaseSize);
    for (int i = 1; i <= testCaseSize; i++) {
      scanf("%d", &input[i]);
    }

    memset(bitArray, 0, sizeof(bitArray));
    for (int i = 1; i <= testCaseSize; ++i) {
      add(input[i], 1, bitArray);
      smallerThanLeft[i] = getSum(input[i] - 1, bitArray);
    }

    memset(bitArray, 0, sizeof(bitArray));
    for (int i = testCaseSize; i >= 1; --i) {
      add(input[i], 1, bitArray);
      smallerThanRight[i] = getSum(input[i] - 1, bitArray);
    }

    long long result = 0;
    for (int i = 1; i <= testCaseSize; i++) {
      result += smallerThanLeft[i] * (testCaseSize - i - smallerThanRight[i])
          + (i - 1 - smallerThanLeft[i]) * smallerThanRight[i];
    }
    printf("%lld\n", result);
  }
  return 0;
}