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
#define INF 1000000000

#define MAX_WIDTH_SUM 2200
#define MAX_N 75

int dp[MAX_WIDTH_SUM][MAX_WIDTH_SUM];

int bookCount;
struct Book {
  int height;
  int width;
  Book (int height = 0, int width = 0) : height(height), width(width) {};

  bool operator < (const Book& o) const {
    return height > o.height;
  }
};

vector<Book> books;

int main() {
  int testCases;
  scanf("%d", &testCases);
  while (testCases--) {
    books.clear();
    scanf("%d", &bookCount);

    int widthSum = 0;
    for (int i = 0; i < bookCount; i++) {
      Book tmp;
      scanf("%d %d", &tmp.height, &tmp.width);
      widthSum += tmp.width;
      books.push_back(tmp);
    }

    sort(books.begin(), books.end());

    // For signed integer, this actually set the value to -1.
    memset(dp, 0x77, sizeof(dp));
    dp[0][0] = 0;

    int curWidthSum = 0;
    for (int i = 1; i < bookCount; i++) {
      // The valid value of j and k is always smaller than the sum
      // of width of current books.
      curWidthSum += books[i].width;
      // From back to front to avoid same level interference.
      for (int j = curWidthSum + 1; j >= 0; j--) {
        for (int k = curWidthSum + 1; k >= 0; k--) {
          if (dp[j][k] != 0x77777777) {
            // Pruning in dynamic programming.
            // If current level is wider than the previous level for
            // more than 30, it is always good to put the book into
            // previous level.
            if (j - (curWidthSum - j - k) <= 30) {
              dp[j + books[i].width][k] = min(dp[j + books[i].width][k],
                j == 0 ? dp[j][k] + books[i].height : dp[j][k]);
            }

            if (k - j <= 30) {
              dp[j][k + books[i].width] = min(dp[j][k + books[i].width],
                k == 0 ? dp[j][k] + books[i].height : dp[j][k]);
            }
          }
        }
      }
    }

    int minValue = INF;
    for (int j = curWidthSum + 1; j > 0; j--) {
      for (int k = curWidthSum + 1; k > 0; k--) {
        if (dp[j][k] != 0x77777777) {
          int maxWidth = 0;
          maxWidth = max(maxWidth, j);
          maxWidth = max(maxWidth, k);
          maxWidth = max(maxWidth, widthSum - j - k);

          int totalHeight = books[0].height + dp[j][k];
          minValue = min(minValue, totalHeight * maxWidth);
        }
      }
    }
    printf("%d\n", minValue);
  }
  return 0;
}