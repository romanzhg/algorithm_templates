// Radix sort.
// func0 implements this idea directly.
// func1 implements with 2 rankToIndex arrays, so we don't need dynamic structures.
// func2 differs from func1 by doesn't assume any pre-order, which is useful in the
// suffix array case.
// Side notes: type int[] has size data, type int* doesn't have size information.
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

int getHigherDigit(int v) {
  return v / 10;
}

int getLowerDigit(int v) {
  return v % 10;
}

// Radix sort, first version.
// data: index -> value.
// buckets: bucket_value -> index.
// data[0] is a dummy element, to make the index start from 1, which
// is needed for other algorithms.
void func0() {
  int data[] = {0, 73, 22, 93, 43, 55, 14, 28, 65, 39, 81};
  // Init, put data into buckets1.
  vector<vector<int>> buckets1(10);
  vector<vector<int>> buckets2(10);

  int dataArraySize = sizeof(data) / sizeof(data[0]);
  for (int i = 1; i < dataArraySize; i++) {
    buckets1[getLowerDigit(data[i])].push_back(i);
  }

  // Move data from buckets1 to buckets2.
  for (int i = 0; i < buckets1.size(); i++) {
    for (int j = 0; j < buckets1[i].size(); j++) {
      int curIndex = buckets1[i][j];
      buckets2[getHigherDigit(data[curIndex])].push_back(curIndex);
    }
  }

  // Now buckets2 contains rank to index.
  for (int i = 0; i < buckets2.size(); i++) {
    for (int j = 0; j < buckets2[i].size(); j++) {
      int curIndex = buckets2[i][j];
      printf("%d ", data[curIndex]);
    }
  }
  printf("\n");
}

void func1() {
  int data[] = {0, 73, 22, 93, 43, 55, 14, 28, 65, 39, 81};

  int dataArraySize = sizeof(data) / sizeof(data[0]);
  int rankToIndex0[dataArraySize];
  int rankToIndex1[dataArraySize];
  int count[dataArraySize];
  int elemBucketOrder[dataArraySize];


  // Init.
  for (int i = 0; i < dataArraySize; i++) {
    rankToIndex0[i] = i;
  }

  // First run.
  memset(count, 0, sizeof(count));
  // Do the first count.
  for (int rank = 1; rank < dataArraySize; rank++) {
    int bucketIndex = getLowerDigit(data[rankToIndex0[rank]]);
    elemBucketOrder[rank] = count[bucketIndex];
    count[bucketIndex]++;
  }
  for (int i = 1; i < dataArraySize; i++) {
    count[i] = count[i - 1] + count[i];
  }

  for (int rank = 1; rank < dataArraySize; rank++) {
    int bucketIndex = getLowerDigit(data[rankToIndex0[rank]]);
    int newRank = count[bucketIndex]
        - ((count[bucketIndex] - count[bucketIndex - 1]) - 1 - elemBucketOrder[rank]);
    rankToIndex1[newRank] = rankToIndex0[rank];
  }

  // Verify the first round.
  for (int rank = 1; rank < dataArraySize; rank++) {
    printf("%d ", data[rankToIndex1[rank]]);
  }
  printf("\n");

  // Second round.
  memset(count, 0, sizeof(count));
  memcpy(rankToIndex0, rankToIndex1, sizeof(rankToIndex0));

  for (int rank = 1; rank < dataArraySize; rank++) {
    int bucketIndex = getHigherDigit(data[rankToIndex0[rank]]);
    elemBucketOrder[rank] = count[bucketIndex];
    count[bucketIndex]++;
  }
  for (int i = 1; i < dataArraySize; i++) {
    count[i] = count[i - 1] + count[i];
  }

  for (int rank = 1; rank < dataArraySize; rank++) {
    int bucketIndex = getHigherDigit(data[rankToIndex0[rank]]);
    int newRank = count[bucketIndex]
        - ((count[bucketIndex] - count[bucketIndex - 1]) - 1 - elemBucketOrder[rank]);
    rankToIndex1[newRank] = rankToIndex0[rank];
  }

  // Verify the second round.
  for (int rank = 1; rank < dataArraySize; rank++) {
    printf("%d ", data[rankToIndex1[rank]]);
  }
  printf("\n");

}

void func2() {
  int data[] = {0, 73, 22, 93, 43, 55, 14, 28, 65, 39, 81};

  int dataArraySize = sizeof(data) / sizeof(data[0]);
  int rankToIndex0[dataArraySize];
  int rankToIndex1[dataArraySize];
  int count[dataArraySize];
  int elemBucketOrder[dataArraySize];

  int *array0;
  int *array1;
  array0 = rankToIndex0;
  array1 = rankToIndex1;

  // First run.
  memset(count, 0, sizeof(count));
  for (int dataIndex = 1; dataIndex < dataArraySize; dataIndex++) {
    int bucketIndex = getLowerDigit(data[dataIndex]);
    elemBucketOrder[dataIndex] = count[bucketIndex];
    count[bucketIndex]++;
  }
  for (int i = 1; i < dataArraySize; i++) {
    count[i] = count[i - 1] + count[i];
  }

  for (int dataIndex = 1; dataIndex < dataArraySize; dataIndex++) {
    int bucketIndex = getLowerDigit(data[dataIndex]);
    int rank = count[bucketIndex]
        - ((count[bucketIndex] - count[bucketIndex - 1]) - 1 - elemBucketOrder[dataIndex]);
    array1[rank] = dataIndex;
  }

  // Verify the first round.
  for (int rank = 1; rank < dataArraySize; rank++) {
    printf("%d ", data[array1[rank]]);
  }
  printf("\n");

  // Second round.
  memset(count, 0, sizeof(count));
  swap(array0, array1);
  
  // Note that we use dataIndex in the previous iteration, and array0 now.
  // Order matters now.
  for (int rank = 1; rank < dataArraySize; rank++) {
    int bucketIndex = getHigherDigit(data[array0[rank]]);
    elemBucketOrder[rank] = count[bucketIndex];
    count[bucketIndex]++;
  }
  for (int i = 1; i < dataArraySize; i++) {
    count[i] = count[i - 1] + count[i];
  }

  for (int rank = 1; rank < dataArraySize; rank++) {
    int bucketIndex = getHigherDigit(data[array0[rank]]);
    int newRank = count[bucketIndex]
        - ((count[bucketIndex] - count[bucketIndex - 1]) - 1 - elemBucketOrder[rank]);
    array1[newRank] = array0[rank];
  }

  // Verify the second round.
  for (int rank = 1; rank < dataArraySize; rank++) {
    printf("%d ", data[array1[rank]]);
  }
  printf("\n");
}

int main() {
  printf("func0:\n");
  func0();
  printf("func1:\n");
  func1();
  printf("func2:\n");
  func2();
  return 0;
}