#include <cstdio>
#include <cstdlib>
#include <climits>
#include <queue>
#include <vector>
#include <utility>
#include <algorithm>
#include <functional>
#include <cstring>
#include <string>
#include <map>
using namespace std;

#define INF 100000000

#define MAX_TICKETS 9
#define MAX_CITIES 31

double diff = 0.01;

// Horses associated with each ticket.
int horses[MAX_TICKETS];

int graph_matrix[MAX_CITIES][MAX_CITIES];

void add_edge(int from, int to, int cost) {
  graph_matrix[from][to] = cost;
  graph_matrix[to][from] = cost;
}


double dp[1 << MAX_TICKETS][MAX_CITIES];

int tickets;
int cities;

void solve(int a, int b) {
	// Initialize the data structure
	int S = (1 << tickets) - 1;
	for (int i = 0; i <= S; i++) {
		for (int j = 0; j < MAX_CITIES; j++) {
			dp[i][j] = INF;
		}
	}

  dp[S][a] = 0;
  double rtn = INF;
  // Iterate over state, seems to be an optimization over the recursive approach.
  for (int state = S; state >= 0; --state) {
  	rtn = min(rtn, dp[state][b]);
  	for (int from = 0; from < cities; from++) {
	  	for (int ticket = 0; ticket < tickets; ticket++) {
	  		if (state >> ticket & 1) {
		  		for (int to = 0; to < cities; to++) {
		  			if (graph_matrix[from][to] > 0) {
		  				dp[state & ~(1 << ticket)][to] =
		  				min(dp[state & ~(1 << ticket)][to],
		  					  dp[state][from] + ((double)graph_matrix[from][to]
		  					  	/ (double)horses[ticket]));
		  			}
		  		}
	  		}
	  	}
    }
  }

  
  if (rtn > (INF - 10)) {
  	printf("Impossible\n");
  } else {
  	printf("%f\n", rtn);
  }
}

int main() {
  int n, m, p, a, b;
  while (scanf("%d %d %d %d %d", &n, &m, &p, &a, &b)) {
  	if (n == 0) {
  		break;
  	}
  	tickets = n;
  	cities = m;
  	for (int i = 0; i < m; i++) {
  		for (int j = 0; j < m; j++) {
  			graph_matrix[i][j] = 0;
  		}
  	}

  	for (int i = 0; i < n; i++) {
  		scanf("%d", &(horses[i]));
  	}

  	for (int i = 0; i < p; i++) {
  		int from, to, cost;
  		scanf("%d %d %d", &from, &to, &cost);
  		add_edge(from - 1, to - 1, cost);
  	}
  	solve(a - 1, b - 1);
  }
  return 0;
}



/*

#include <cstdio>
#include <cstdlib>
#include <climits>
#include <queue>
#include <vector>
#include <utility>
#include <algorithm>
#include <functional>
#include <cstring>
#include <string>
#include <map>
using namespace std;

#define INF 100000000

#define MAX_TICKETS 9
#define MAX_CITIES 31

double diff = 0.01;

// Horses associated with each ticket.
int horses[MAX_TICKETS];

// pair<int, int>: to, cost.
vector<pair<int, int> > graph[MAX_CITIES];

void add_edge(int from, int to, int cost) {
  graph[from].push_back(make_pair(to, cost));
  graph[to].push_back(make_pair(from, cost));
}

double dp[1 << MAX_TICKETS][MAX_CITIES];

int tickets;

void helper(int S, int cur, double time) {
	// Handle cases to remove cycles. The graph is a DAG, and in DAG, some nodes
	// may be visited multiple times.
  if (S == 0) {
  	dp[S][cur] = min(dp[S][cur], time);
  	return;
  } else {
  	if (dp[S][cur] < time - diff) {
  		return;
  	} else {
  		dp[S][cur] = time;
  	}
  }
  
  for (int i = 0; i < tickets; i++) {
  	if (S & (1 << i)) {
  		int tmpS = S - (1 << i);
  		for (int j = 0; j < graph[cur].size(); j++) {
  			pair<int, int>& p = graph[cur][j];
  			helper(tmpS, p.first, time + ((double)p.second) / (double)horses[i]);
  		}
  	}
  }
}

void solve(int a, int b) {
	// Initialize the data structure
	int S = (1 << tickets) - 1;
	for (int i = 0; i <= S; i++) {
		for (int j = 0; j < MAX_CITIES; j++) {
			dp[i][j] = INF;
		}
	}

  helper(S, a, 0);

  double rtn = INF;
  for (int i = S; i >= 0; --i) {
  	rtn = min(rtn, dp[i][b]);
  }
  if (rtn > (INF - 10)) {
  	printf("Impossible\n");
  } else {
  	printf("%f\n", rtn);
  }
}

int main() {
  int n, m, p, a, b;
  while (scanf("%d %d %d %d %d", &n, &m, &p, &a, &b)) {
  	if (n == 0) {
  		break;
  	}
  	tickets = n;
  	for (int i = 0; i < m; i++) {
  		// Remove previous edges.
  		graph[i].clear();
  	}

  	for (int i = 0; i < n; i++) {
  		scanf("%d", &(horses[i]));
  	}

  	for (int i = 0; i < p; i++) {
  		int from, to, cost;
  		scanf("%d %d %d", &from, &to, &cost);
  		add_edge(from - 1, to - 1, cost);
  	}
  	solve(a - 1, b - 1);
  }
  return 0;
}

*/