// Similar to the algorithm finding 2-partition(divide a set of
// integers to two equal subset).
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

#define MAX_COURSES 8
#define MAX_TEACHERS 150

// Courses are indexed from 0 to 7.
// In the problem they are given with id from 1 to 8.
// dp[i][j][k]: min salary when the j courses can be taught by exectly one teacher,
// and k courses can be taught by at least two teachers, with first i teachers 
// taken into consideration.
int dp[MAX_TEACHERS][1 << MAX_COURSES][1 << MAX_COURSES];

int courseCount, teachers, applicants;

// Value in canTeach is an 8 bit integer.
int canTeach[MAX_TEACHERS];
int salary[MAX_TEACHERS];

// n in [0, 31]
uint32_t setBit(uint32_t value, int32_t n) {
  return value | (1U << n);
}

// n in [0, 31]
uint32_t clearBit(uint32_t value, int32_t n) {
  return value & (~(1U << n));
}

// n in [0, 31]
bool isBitSet(uint32_t value, uint32_t n) {
  return value & (1U << n);
}

// Split the input string by delimiter, return results in a vector.
// Delimiters are not returned.
void split(const string& str, char delim, vector<string>& result) {
  size_t cur = 0;
  size_t pos = str.find(delim);
  while (pos != string::npos) {
    if (pos != cur) {
      result.push_back(str.substr(cur, pos - cur));
    }
    pos++;
    cur = pos;
    pos = str.find(delim, pos);
  }
  if (cur != str.length()) {
    result.push_back(str.substr(cur));
  }
}

void printdp(int i) {
  for (int j = 0; j < (1 << courseCount); j++) {
    for (int k = 0; k < (1 << courseCount); k++) {
      printf("%d  ", dp[i][j][k]);
    }
    printf("\n");
  }
}
void solve() {
  for (int i = 0; i < MAX_TEACHERS; i++) {
    for (int j = 0; j < (1 << courseCount); j++) {
      for (int k = 0; k < (1 << courseCount); k++) {
        dp[i][j][k] = INF;
      }
    }
  }
  dp[0][0][0] = 0;

  for (int i = 1; i <= (teachers + applicants); i++) {
    int curSalary = salary[i - 1];
    int curCanTeach = canTeach[i - 1];

    if (i > teachers) {
      for (int j = 0; j < (1 << courseCount); j++) {
        for (int k = 0; k < (1 << courseCount); k++) {
          dp[i][j][k] = dp[i - 1][j][k];
        }
      }
    }

    for (int j = 0; j < (1 << courseCount); j++) {
      for (int k = 0; k < (1 << courseCount); k++) {
        // If a bit is not set in j but set in canTeach, set it.
        // If a bit is set in j and canTeach, but not in k, remove the bit in j
        // and set it in k.
        // A bit cannot be set in j and k simutaniously.
        if (dp[i - 1][j][k] == INF) {
          continue;
        }
        int newJ = j, newK = k;
        for (int b = 0; b < courseCount; b++) {
          if (isBitSet(k, b)) {
            continue;
          } else if (isBitSet(j, b) && isBitSet(curCanTeach, b)) {
            newK = setBit(newK, b);
            newJ = clearBit(newJ, b);
          } else if (!isBitSet(j, b) && isBitSet(curCanTeach, b)) {
            newJ = setBit(newJ, b);
          }
        }
        if (i <= teachers) {
          dp[i][newJ][newK] = dp[i - 1][j][k] + curSalary;
        } else {
          dp[i][newJ][newK] =
              min(dp[i][newJ][newK], dp[i - 1][j][k] + curSalary);
          if (dp[teachers][newJ][newK] != INF) {
            dp[i][newJ][newK] = max(dp[i][newJ][newK], dp[teachers][newJ][newK]);
          }
        }
      }
    }
  }
}

int main() {
  string line;
  while (true) {
    getline(cin, line);
    vector<string> result;
    split(line, ' ', result);

    courseCount = stoi(result[0].c_str());
    teachers = stoi(result[1].c_str());
    applicants = stoi(result[2].c_str());

    if (courseCount + teachers + applicants == 0) {
      break;
    }

    memset(canTeach, 0, sizeof(canTeach));

    for (int i = 0; i < teachers + applicants; i++) {
      getline(cin, line);
      result.clear();
      split(line, ' ', result);
      salary[i] = stoi(result[0]);
      for (int j = 1; j < result.size(); j++) {
        int course = stoi(result[j]);
        canTeach[i] = setBit(canTeach[i], course - 1);
      }
    }

    solve();
    printf("%d\n", dp[teachers + applicants][0][(1 << courseCount) - 1]);
  }
  return 0;
}