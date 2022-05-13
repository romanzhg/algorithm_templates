// An alternative implementation of suffix array, need some cleanup.
// For indexToRank, the rank is 1 based, while in rankToIndex, the rank is 0 based.
// This implements the idea of one way of buiding suffix array, but in actual
// implementation there are some shortcuts to take which makes the code much 
// more cleaner.
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

int asciiBuckets[ASCII_LEN];
int bucketToCount[MAX_LEN];
int rankToIndex0[MAX_LEN];
int rankToIndex1[MAX_LEN];

int getLessSignificantValue(int* array, int index, int k) {
  index += k;
  if (index >= strLen) {
    return 0;
  }
  return array[index];
}

int getMoreSignificantValue(int* array, int index) {
  return array[index];
}

bool isDiff(int* array, int indexA, int indexB, int k) {
  if (getMoreSignificantValue(array, indexA)
    != getMoreSignificantValue(array, indexB)) {
    return true;
  }
  if (getLessSignificantValue(array, indexA, k)
    != getLessSignificantValue(array, indexB, k)) {
    return true;
  }
  return false;
}

int main() {
  string str = "banana";
  strLen = str.size();

  memset(asciiBuckets, 0, sizeof(asciiBuckets));
  for (int i = 0; i < str.size(); i++) {
    if (asciiBuckets[str[i]] == 0) {
      asciiBuckets[str[i]]++;
    }
  }
  for (int i = 1; i < ASCII_LEN; i++) {
    asciiBuckets[i] = asciiBuckets[i] + asciiBuckets[i - 1];
  }

  // suffixArray contains index to rank, here the rank is 1 based.
  int indexToRank[MAX_LEN];
  int altIndexToRank[MAX_LEN];
  for (int i = 0; i < str.size(); i++) {
    indexToRank[i] = asciiBuckets[str[i]];
  }

  int *array0, *array1;
  for (int k = 1; k < strLen; k = k << 1) {
    array0 = rankToIndex0;
    array1 = rankToIndex1;

    // The first round, compute rankToIndex1, base on the less significant value.
    // TODO: make sure bucketToCount is zeroed after the computations? 
    memset(bucketToCount, 0, sizeof(bucketToCount));
    for (int dataIndex = 0; dataIndex < strLen; dataIndex++) {
      int bucketIndex = getLessSignificantValue(indexToRank, dataIndex, k);
      bucketToCount[bucketIndex]++;
    }

    for (int i = 1; i < MAX_LEN; i++) {
      bucketToCount[i] = bucketToCount[i] + bucketToCount[i - 1];
    }

    for (int dataIndex = 0; dataIndex < strLen; dataIndex++) {
      int bucketIndex = getLessSignificantValue(indexToRank, dataIndex, k);
      int rank = --bucketToCount[bucketIndex];
      array1[rank] = dataIndex;
    }

    // move rankToIndex1 to rankToIndex0.
    memset(bucketToCount, 0, sizeof(bucketToCount));
    swap(array0, array1);

    // compute rankToIndex1, base on the more significant value.
    for (int rank = 0; rank < strLen; rank++) {
      int bucketIndex = getMoreSignificantValue(indexToRank, array0[rank]);
      bucketToCount[bucketIndex]++;
    }
    for (int i = 1; i < MAX_LEN; i++) {
      bucketToCount[i] = bucketToCount[i] + bucketToCount[i - 1];
    }
    for (int rank = strLen - 1; rank >= 0; rank--) {
      int bucketIndex = getMoreSignificantValue(indexToRank, array0[rank]);
      int newRank = --bucketToCount[bucketIndex];
      array1[newRank] = array0[rank];
    }

    // Transform rankToIndex1(rank 0 based) to indexToRank(rank 1 based).
    memcpy(altIndexToRank, indexToRank, sizeof(indexToRank));

    int adjRank;
    for (int rank = 0; rank < strLen; rank++) {
      if (rank == 0) {
        adjRank = 1;
      } else {
        if (isDiff(altIndexToRank, array1[rank], array1[rank - 1], k)) {
          adjRank++;
        }
      }
      indexToRank[array1[rank]] = adjRank;
    }
    memset(array1, 0, MAX_LEN);
  }

  // suffix array(index to rank, rank start at 1) to rank to index.
  int rankToIndex[MAX_LEN];
  for (int i = 0; i < strLen; i++) {
    rankToIndex[indexToRank[i] - 1] = i;
  }
  // Contains rank to index.
  printf("the final result:\n");
  for (int i = 0; i < strLen; i++) {
    printf("%d ", rankToIndex[i]);
  }
  printf("\n");

  return 0;
}