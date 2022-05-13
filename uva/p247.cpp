// The current implementation should be correct, but did not pass for strange reasons.
// The answer is validated by p247_validate_ans.py.
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

#define INF 1000000000

using namespace std;

const int MAX_NAME_LEN = 30;
const int MAX_NODES = 30;

bool reachable[MAX_NODES][MAX_NODES];
bool new_graph_connected[MAX_NODES][MAX_NODES];

int node_count, edge_count;
map<string, int> name_to_node_id;
map<int, string> node_id_to_name;

int name_count;

bool visited[MAX_NODES];
vector<string> group;

void dfs(int n) {
  visited[n] = true;
  group.push_back(node_id_to_name[n]);
  for (int i = 0; i < node_count; i++) {
    if (new_graph_connected[n][i] && !visited[i]) {
      dfs(i);
    }
  }
}

int AssignOrGetId(string name) {
  int rtn;
  if (name_to_node_id.find(name) == name_to_node_id.end()) {
    rtn = name_count;
    name_to_node_id[name] = rtn;
    node_id_to_name[rtn] = name;
    name_count++;
  } else {
    rtn = name_to_node_id[name];
  }
  return rtn;
}

void PrintCallingGroup() {
  for (int i = 0; i < (int)group.size(); i++) {
    if (i == 0) {
      cout << group[i];
    } else {
      cout << ", " << group[i];
    }
  }
}

char from_name[MAX_NAME_LEN];
char to_name[MAX_NAME_LEN];

int main() {
  int test_set_count = 0;
  while (true) {
    // Load the input.
    scanf("%d %d", &node_count, &edge_count);
    if (node_count == 0 && edge_count == 0) {
      break;
    }
    name_to_node_id.clear();
    node_id_to_name.clear();
    name_count = 0;
    memset(reachable, 0, sizeof(reachable));
    for (int i = 0; i < node_count; i++) {
      reachable[i][i] = true;
    }
    for (int i = 0; i < edge_count; i++) {
      scanf("%s %s", from_name, to_name);
      int from_id = AssignOrGetId(from_name);
      int to_id = AssignOrGetId(to_name);
      reachable[from_id][to_id] = true;
    }

    // Run bellman-ford.
    for (int k = 0; k < node_count; k++) {
      for (int i = 0; i < node_count; i++) {
        for (int j = 0; j < node_count; j++) {
          reachable[i][j] = reachable[i][j] || (reachable[i][k] && reachable[k][j]);
        }
      }
    }

    // In the new undirected graph, add an edge if two nodes can reach each other in the original graph.
    memset(new_graph_connected, 0, sizeof(new_graph_connected));
    for (int i = 0; i < node_count; i++) {
      for (int j = i + 1; j < node_count; j++) {
        if (reachable[i][j] && reachable[j][i]) {
          new_graph_connected[i][j] = true;
          new_graph_connected[j][i] = true;
        }
      }
    }

    memset(visited, 0, sizeof(visited));
    // Output connected components.
    cout << "Calling circles for data set " << ++test_set_count << ":" << endl;

    if (edge_count != 0) {
      for (int i = 0; i < node_count; i++) {
        if (node_id_to_name.find(i) == node_id_to_name.end()) {
          visited[i] = true;
        }
      }
      for (int i = 0; i < node_count; i++) {
        if (!visited[i]) {
          group.clear();
          dfs(i);
          PrintCallingGroup();
          cout << endl;
        }
      }
    }
    cout << endl;
  }

  return 0;
}