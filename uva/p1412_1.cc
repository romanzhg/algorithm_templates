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
map<vector<int>, int> holdingStates;
int gCurStock;

// Remaining stock does not have to be all allocated.
void genHolding(vector<int>& tmp, int remainingStocks, int index) {
  if (index >= stockCount) {
    holdingStates[tmp] = gCurStock++;
    return;
  }

  for (int v = 0; v <= min(remainingStocks, stocks[index].holdingLimit); v++) {
    tmp[index] = v;
    genHolding(tmp, remainingStocks - v, index + 1);
  }
}

int genAllHoldingState() {
  holdingStates.clear();
  gCurStock = 0;
  vector<int> holding(stockCount, 0);
  genHolding(holding, gHoldingLimit, 0);
  return holdingStates.size();
}

int getGHolding(const vector<int>& state) {
  int rtn = 0;
  for (int v : state) {
    rtn += v;
  }
  return rtn;
}

void update(const vector<int>& state, int endingDate, double cash,
  double dp[][MAX_DAYS], string prevOp, string prevOps[][MAX_DAYS],
  int prevState, int prevStates[][MAX_DAYS]) {
  if (endingDate > days) {
    return;
  }

  int stateIndex = holdingStates[state];
  if (cash < dp[stateIndex][endingDate]) {
    return;
  }

  dp[stateIndex][endingDate] = cash;
  prevOps[stateIndex][endingDate] = prevOp;
  prevStates[stateIndex][endingDate] = prevState;

  // Hold.
  update(state, endingDate + 1, cash, dp,
    "HOLD", prevOps, stateIndex, prevStates);

  // Buy.
  for (int i = 0; i < stockCount; i++) {
    if (state[i] < stocks[i].holdingLimit
      && stocks[i].price[endingDate] * stocks[i].buyUnit <= cash
      && getGHolding(state) < gHoldingLimit) {
      vector<int> newState(state);
      newState[i]++;
      update(newState, endingDate + 1,
        cash - stocks[i].price[endingDate] * stocks[i].buyUnit, dp,
        "BUY " + stocks[i].name, prevOps, stateIndex, prevStates);
    }
  }

  // Sell.
  for (int i = 0; i < stockCount; i++) {
    if (state[i] > 0) {
      vector<int> newState(state);
      newState[i]--;
      update(newState, endingDate + 1,
        cash + stocks[i].price[endingDate] * stocks[i].buyUnit, dp,
        "SELL " + stocks[i].name, prevOps, stateIndex, prevStates);
    }
  }
}

int main() {
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

    double dp[stateCount][MAX_DAYS];
    int prevStates[stateCount][MAX_DAYS];
    string prevOps[stateCount][MAX_DAYS];
    for (int i = 0; i < stateCount; i++) {
      for (int j = 0; j < days + 1; j++) {
        dp[i][j] = -1;
      }
    }
    update(initState, 0, initCash, dp,
      "", prevOps, -1, prevStates);

    printf("%.2lf\n", dp[holdingStates[initState]][days]);

    vector<string> strList;
    int curStateIndex = holdingStates[initState];
    for (int i = days; i > 0; i--) {
      strList.push_back(prevOps[curStateIndex][i]);
      curStateIndex = prevStates[curStateIndex][i];
    }
    reverse(strList.begin(), strList.end());
    for (const string& str : strList) {
      printf("%s\n", str.c_str());
    }
    printf("\n");
  }
  return 0;
}