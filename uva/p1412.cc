// Compare to p1412_2, this program is optimized by save all the
// state transitions. (Actually I'm not sure about how big the
// speed up is.)
// Compare to p1412_1, this program eleminates un-necessary updates
// by update level by level instead of the dfs way.
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

#define MAX_STOCKS 8

double dp[MAX_DAYS][MAX_STOCK_STATES];
int prevStates[MAX_DAYS][MAX_STOCK_STATES];
int prevOps[MAX_DAYS][MAX_STOCK_STATES];

double initCash;
int days;
int stockCount;
int gHoldingLimit;

struct Stock {
  string name;
  int holdingLimit;
  vector<double> price;
};

vector<Stock> stocks;
map<vector<int>, int> holdingStatesMap;
vector<vector<int>> holdingStatesVec;

// nextBuy[s][i]: at current state if it is possible to buy
// stock i, the next state.
int nextBuy[MAX_STOCK_STATES][MAX_STOCKS];
int nextSell[MAX_STOCK_STATES][MAX_STOCKS];

int gCurStock;

// Remaining stock does not have to be all allocated.
void genHolding(vector<int>& tmp, int remainingStocks, int index) {
  if (index >= stockCount) {
    holdingStatesMap[tmp] = gCurStock++;
    holdingStatesVec.push_back(tmp);
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
  gCurStock = 0;
  vector<int> holding(stockCount, 0);
  genHolding(holding, gHoldingLimit, 0);
  return holdingStatesMap.size();
}

int getGHolding(const vector<int>& state) {
  int rtn = 0;
  for (int v : state) {
    rtn += v;
  }
  return rtn;
}

void genNextOps() {
  int statesSize = holdingStatesVec.size();
  memset(nextBuy, 0x77, sizeof(nextBuy));
  memset(nextSell, 0x77, sizeof(nextSell));

  for (int i = 0; i < statesSize; i++) {
    vector<int>& curState = holdingStatesVec[i];
    for (int j = 0; j < stockCount; j++) {
      if (curState[j] < stocks[j].holdingLimit
        && getGHolding(curState) < gHoldingLimit) {
        curState[j]++;
        nextBuy[i][j] = holdingStatesMap[curState];
        curState[j]--;
      }
      if (curState[j] > 0) {
        curState[j]--;
        nextSell[i][j] = holdingStatesMap[curState];
        curState[j]++;
      }
    }
  }
}

void updateInternal(int day, int newStateIndex, double newCash,
  int stateIndex, int op) {
  if (newCash > dp[day + 1][newStateIndex]) {
    dp[day + 1][newStateIndex] = newCash;
    prevStates[day + 1][newStateIndex] = stateIndex;
    prevOps[day + 1][newStateIndex] = op;
  }
}

void update(int day, int stateIndex) {
  double curCash = dp[day][stateIndex];

  // Hold.
  updateInternal(day, stateIndex, curCash, stateIndex, 0);
  
  for (int i = 0; i < stockCount; i++) {
    // Buy.
    if (nextBuy[stateIndex][i] != 0x77777777
      && curCash >= stocks[i].price[day]) {
      updateInternal(day, nextBuy[stateIndex][i],
        curCash - stocks[i].price[day],
        stateIndex, i + 1);
    }

    // Sell.
    if (nextSell[stateIndex][i] != 0x77777777) {
      updateInternal(day, nextSell[stateIndex][i],
        curCash + stocks[i].price[day],
        stateIndex, - i - 1);
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
      int buyUnit;
      char tmp[256];
      scanf("%s %d %d", tmp, &buyUnit, &s.holdingLimit);
      s.name = tmp;
      for (int j = 0; j < days; j++) {
        double tmpDouble;
        scanf("%lf", &tmpDouble);
        s.price.push_back(tmpDouble * buyUnit);
      }
      stocks.push_back(s);
    }

    int stateCount = genAllHoldingState();
    genNextOps();

    vector<int> initState;
    for (int i = 0; i < stockCount; i++) {
      initState.push_back(0);
    }

    for (int i = 0; i <= days; i++) {
      for (int j = 0; j < stateCount; j++) {
        dp[i][j] = -10000;
      }
    }

    dp[holdingStatesMap[initState]][0] = initCash;
    for (int i = 0; i < days; i++) {
      for (int j = 0; j < stateCount; j++) {
        if (dp[i][j] < -1) {
          continue;
        }
        update(i, j);
      }
    }

    printf("%.2lf\n", dp[days][0]);

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