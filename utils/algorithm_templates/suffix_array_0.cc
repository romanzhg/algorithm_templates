// Build suffix array.
// To give order to elements according to buckets:
// 1. Assign an integer rank(smaller rank means to smaller value):
// accumulate the buckets, and check the bucket index again.
// 2. Put elements in order according to their buckets:
// accumulate the buckets, from back to front assign elements to a new array.
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

const int ASCII_LEN = 128;
const int MAX_LEN = 1024;

int strLen;

// Ranks in indexToRank and rankToIndex are both 0 based(in some other version
// indexToRank is 1 based). This won't be a problem in this implementation since
// the count phase will give the order(an implementation detail).
int bucketToCount[MAX_LEN];
int rankToIndex[MAX_LEN];
int tmpArray0[MAX_LEN];
int tmpArray1[MAX_LEN];

bool isDiff(int* array, int indexA, int indexB, int k) {
  if (array[indexA] != array[indexB]) {
    return true;
  }
  if ((indexA + k < MAX_LEN) && (indexB + k < MAX_LEN)) {
    if (array[indexA + k] != array[indexB + k]) {
      return true;
    } else {
      return false;
    }
  } else {
    int a, b;
    if (indexA + k < MAX_LEN) {
      a = array[indexA + k];
    } else {
      a = 0;
    }
    if (indexB + k < MAX_LEN) {
      b = array[indexB + k];
    } else {
      b = 0;
    }
    return a != b;
  }
}

int main() {
  string str = "banana";
  strLen = str.size();

  int *indexToRank, *tmpRankToIndex;

  indexToRank = tmpArray0;
  tmpRankToIndex = tmpArray1;

  // At the begining we only need to compute the rank to index.
  // For index to rank we need another way to compute.
  memset(bucketToCount, 0, sizeof(bucketToCount));
  for (int index = 0; index < strLen; index++) {
    indexToRank[index] = str[index];
    bucketToCount[str[index]]++;
  }
  for (int rank = 1; rank < MAX_LEN; rank++) {
    bucketToCount[rank] += bucketToCount[rank - 1];
  }
  for (int index = strLen - 1; index >= 0; index--) {
    rankToIndex[--bucketToCount[indexToRank[index]]] = index;
  }

  for (int k = 1; k < strLen; k <<= 1) {
    int tmpRank = 0;
    for (int i = strLen - k; i < strLen; i++) {
      tmpRankToIndex[tmpRank] = i;
      tmpRank++;
    }
    for (int i = 0; i < strLen; i++) {
      if (rankToIndex[i] >= k) {
        tmpRankToIndex[tmpRank] = rankToIndex[i] - k;
        tmpRank++;
      }
    }

    memset(bucketToCount, 0, sizeof(bucketToCount));
    for (int rank = 0; rank < strLen; rank++) {
      int bucketIndex = indexToRank[tmpRankToIndex[rank]];
      bucketToCount[bucketIndex]++;
    }
    for (int i = 1; i < MAX_LEN; i++) {
      bucketToCount[i] += bucketToCount[i - 1];
    }
    for (int rank = strLen - 1; rank >= 0; rank--) {
      int bucketIndex = indexToRank[tmpRankToIndex[rank]];
      int newRank = --bucketToCount[bucketIndex];
      rankToIndex[newRank] = tmpRankToIndex[rank];
    }

    // tmpRankToIndex becomes the previous indexToRank.
    swap(indexToRank, tmpRankToIndex);

    tmpRank = 1;
    indexToRank[rankToIndex[0]] = 0;
    for (int rank = 0; rank < strLen; rank++) {
      if (rank >= 1
        && isDiff(tmpRankToIndex, rankToIndex[rank], rankToIndex[rank - 1], k)) {
        tmpRank++;
      }
      indexToRank[rankToIndex[rank]] = tmpRank;
      if (tmpRank >= strLen) {
        break;
      }
    }
  }

  for (int rank = 0; rank < strLen; rank++) {
    printf("%d ", rankToIndex[rank]);
  }
  printf("\n");

  return 0;
}