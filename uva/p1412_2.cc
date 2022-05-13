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

#define MAX_DAYS 110

// 12870
#define MAX_STOCK_STATES 12900

double dp[MAX_DAYS][MAX_STOCK_STATES];
int prevStates[MAX_DAYS][MAX_STOCK_STATES];
int prevOps[MAX_DAYS][MAX_STOCK_STATES];

double initCash;
int days;
int stockCount;
int gHoldingLimit;

struct Stock {
  string name;
  int buyUnit;
  int holdingLimit;
  vector<double> price;
};

vector<Stock> stocks;
map<vector<int>, int> holdingStatesMap;
vector<vector<int>> holdingStatesVec;
vector<int> holdingCount;
int gCurStock;

// Remaining stock does not have to be all allocated.
void genHolding(vector<int>& tmp, int remainingStocks, int index) {
  if (index >= stockCount) {
    holdingStatesMap[tmp] = gCurStock++;
    holdingStatesVec.push_back(tmp);
    holdingCount.push_back(gHoldingLimit - remainingStocks);
    return;
  }

  for (int v = 0; v <= min(remainingStocks, stocks[index].holdingLimit); v++) {
    tmp[index] = v;
    genHolding(tmp, remainingStocks - v, index + 1);
  }
}

int genAllHoldingState() {
  holdingStatesMap.clear();
  holdingStatesVec.clear();
  holdingCount.clear();
  gCurStock = 0;
  vector<int> holding(stockCount, 0);
  genHolding(holding, gHoldingLimit, 0);
  return holdingStatesMap.size();
}

void updateInternal(int day, int newStateIndex, int newCash,
  int stateIndex, int op,
  vector<vector<double>>& dp,
  vector<vector<int>>& prevStates,
  vector<vector<int>>& prevOps) {
  if (newCash > dp[day + 1][newStateIndex]) {
    dp[day + 1][newStateIndex] = newCash;
    prevStates[day + 1][newStateIndex] = stateIndex;
    prevOps[day + 1][newStateIndex] = op;
  }
}

bool existFutureHigher(int curDate, const vector<double>& price) {
  bool rtn = false;
  for (int i = curDate + 1; i < price.size(); i++) {
    if (price[i] > price[curDate]) {
      return true;
    }
  }
  return rtn;
}

void update(int day, int stateIndex, vector<vector<double>>& dp,
  vector<vector<int>>& prevStates,
  vector<vector<int>>& prevOps) {
  double curCash = dp[day][stateIndex];
  vector<int>& curState = holdingStatesVec[stateIndex];

  // Hold.
  updateInternal(day, stateIndex, curCash, stateIndex, 0,
    dp, prevStates, prevOps);

  // Buy.
  for (int i = 0; i < stockCount; i++) {
    if (curState[i] < stocks[i].holdingLimit
      && holdingCount[stateIndex] < gHoldingLimit
      && holdingCount[stateIndex] <= (days - day) 
      && existFutureHigher(day, stocks[i].price)
      && curCash >= stocks[i].price[day] * stocks[i].buyUnit - 1e-3) {
      curState[i]++;
      int newStateIndex = holdingStatesMap[curState];

      updateInternal(day, newStateIndex,
        curCash - stocks[i].price[day] * stocks[i].buyUnit,
        stateIndex, i + 1, dp, prevStates, prevOps);
      curState[i]--;
    }
  }

  // Sell.
  for (int i = 0; i < stockCount; i++) {
    if (curState[i] > 0) {
      curState[i]--;
      int newStateIndex = holdingStatesMap[curState];

      updateInternal(day, newStateIndex,
        curCash + stocks[i].price[day] * stocks[i].buyUnit,
        stateIndex, - i - 1, dp, prevStates, prevOps);
      curState[i]++;
    }
  }
}

int main() {
  #ifndef ONLINE_JUDGE
  clock_t t;
  t = clock();
  #endif

  while (scanf("%lf %d %d %d", &initCash, &days,
    &stockCount, &gHoldingLimit) == 4) {
    stocks.clear();
    for (int i = 0; i < stockCount; i++) {
      Stock s;
      s.price.clear();
      char tmp[256];
      scanf("%s %d %d", tmp, &s.buyUnit, &s.holdingLimit);
      s.name = tmp;
      for (int j = 0; j < days; j++) {
        double tmpDouble;
        scanf("%lf", &tmpDouble);
        s.price.push_back(tmpDouble);
      }
      stocks.push_back(s);
    }

    int stateCount = genAllHoldingState();
    vector<int> initState;
    for (int i = 0; i < stockCount; i++) {
      initState.push_back(0);
    }

    dp[holdingStatesMap[initState]][0] = initCash;
    for (int i = 0; i < days; i++) {
      for (int j = 0; j < stateCount; j++) {
        if (dp[i][j] < -1) {
          continue;
        }
        update(i, j, dp, prevStates, prevOps);
      }
    }

    printf("%.2lf\n", dp[days][holdingStatesMap[initState]]);

    vector<int> strList;
    int curStateIndex = holdingStatesMap[initState];
    for (int i = days; i > 0; i--) {
      strList.push_back(prevOps[i][curStateIndex]);
      curStateIndex = prevStates[i][curStateIndex];
    }
    reverse(strList.begin(), strList.end());
    for (int op : strList) {
      if (op == 0) {
        printf("HOLD\n");
      } else if (op > 0) {
        printf("BUY %s\n", stocks[op - 1].name.c_str());
      } else {
        printf("SELL %s\n", stocks[- op - 1].name.c_str());
      }
    }
    printf("\n");
  }
  #ifndef ONLINE_JUDGE
  t = clock() - t;
  printf ("This program took %f seconds.\n", ((float)t) / CLOCKS_PER_SEC);
  #endif

  return 0;
}