// Note this program differs from p10817 in the way that it only expand
// to viable states, instead of trying all states.
// This approach is similar to DFS, chose the min cost branch to some goal
// state. If an end state is not goal, return cost INF, to indicate do not chose.
// While dp[i][j][k] memorize the search result.
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

// dp[i][j][k]: with [i, all teachers) (the remaining) teachers, the
// min cost to achieve state j, k.
// If state j, k is not desirable, its value will be be INF(do not count).
// If state j, k is not achieveable, its value will be -1.
int dp[MAX_TEACHERS][1 << MAX_COURSES][1 << MAX_COURSES];

int courseCount, teachers, applicants;

// Value in canTeach is an 8 bit integer.
int canTeach[MAX_TEACHERS];
int salary[MAX_TEACHERS];

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

int getValue(int i, int s0, int s1, int s2) {
  if (i == (teachers + applicants)) {
    return s2 == (1 << courseCount) - 1 ? 0 : INF;
  }

  int& ans = dp[i][s1][s2];
  if (ans >= 0) {
    return ans;
  }

  ans = INF;
  if (i >= teachers) {
    ans = getValue(i + 1, s0, s1, s2);
  }

  int m0 = canTeach[i] & s0, m1 = canTeach[i] & s1;
  s0 ^= m0;
  s1 = (s1 ^ m1) | m0;
  s2 |= m1;
  ans = min(ans, salary[i] + getValue(i + 1, s0, s1, s2));
  return ans;
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
    memset(salary, 0, sizeof(salary));

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

    for (int i = 0; i < (teachers + applicants); i++) {
      for (int j = 0; j < (1 << courseCount); j++) {
        for (int k = 0; k < (1 << courseCount); k++) {
          dp[i][j][k] = -1;
        }
      }
    }

    printf("%d\n", getValue(0, (1 << courseCount) - 1, 0, 0));
  }
  return 0;
}