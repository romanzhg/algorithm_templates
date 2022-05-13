/**
 * I don't understand why Solution worked. The dp should take the "right" value into consideration.
 * Maybe there is some hidden structure about this problem that "right" does not affect the dp structure.
 *
 * Lessons learned are for leetcode problems there needs to be a clear vision about the problem before
 * start to program, otherwise it will be a waste of time. Solution1 is based on min cost path, which turns to be
 * wrong.
 *
 */
#include "common.h"

/*****************************************************************************/
struct Edge {
  int from, to;
  int cost;
  Edge(int from, int to, int cost) : from(from), to(to), cost(cost) {};
};

vector<Edge> edges;
vector<int> graph[5000];

struct Node {
  int id;
  int cost;
  Node(int id, int cost) : id(id), cost(cost) {};

  bool operator<(const Node& o) const {
    return cost > o.cost;
  }
};

// A simplified version of Dijkstra's algorithm.
int GetDist(int from, int to, int graph_size) {
  bool visited[graph_size];
  memset(visited, 0, sizeof(visited));

  priority_queue<Node> pq;
  pq.push({from, 0});
  while (!pq.empty()) {
    Node tmp = pq.top(); pq.pop();
    if (visited[tmp.id]) {
      continue;
    }
    visited[tmp.id] = true;
    if (tmp.id == to) {
      return tmp.cost;
    }

    for (int edge_index : graph[tmp.id]) {
      Edge& e = edges[edge_index];
      pq.push({e.to, e.cost + tmp.cost});
    }
  }
  return -1;
}

class Solution1 {
 public:
  int makeArrayIncreasing(vector<int>& arr1, vector<int>& arr2) {
    sort(arr2.begin(), arr2.end());
    arr2.erase(unique(arr2.begin(), arr2.end()), arr2.end());

    int arr1_size = arr1.size(), arr2_size = arr2.size();
    int node_count = arr1_size + arr2_size;

    edges.clear();
    for (int i = 0; i < node_count; i++) {
      graph[i].clear();
    }

    // Add edges inside arr1.
    for (int i = 0; i < arr1_size - 1; i++) {
      if (arr1[i] < arr1[i + 1]) {
        graph[i].push_back(edges.size());
        edges.push_back({i, i + 1, 0});
      }
    }
    // Add edges inside arr2.
    for (int i = 0; i < arr2_size - 1; i++) {
      int node_id = i + arr1_size;
      graph[node_id].push_back(edges.size());
      edges.push_back({node_id, node_id + 1, 1});
    }
    // Add edges from arr1 to arr2.
    for (int i = 0; i < arr1_size; i++) {
      for (int j = 0; j < arr2_size; j++) {
        if (arr1[i] < arr2[j]) {
          graph[i].push_back(edges.size());
          edges.push_back({i, j + arr1_size, 1});
        }
      }
    }
    // Add edges from arr2 to arr1.
    for (int i = 0; i < arr2_size; i++) {
      for (int j = 0; j < arr1_size; j++) {
        if (arr2[i] < arr1[j]) {
          int node_id = i + arr1_size;
          graph[node_id].push_back(edges.size());
          edges.push_back({node_id, j, 0});
        }
      }
    }

    return GetDist(0, arr1_size - 1, node_count);
  }
};


/********************************************************************/
const int kIntInf = INT_MAX / 2;

int dp[2010][2010];
class Solution {
  vector<int> a1;
  vector<int> a2;

  int GetValue(int i, int j, int right) {
    if (i == 0) {
      return 0;
    }

    if (dp[i][j] > 0) {
      return dp[i][j];
    }

    int rtn = kIntInf;
    if (a1[i - 1] < right) {
      rtn = min(rtn, GetValue(i - 1, j, a1[i - 1]));
      while (j > 0 && a2[j - 1] >= right) {
        j--;
      }
      if (j > 0) {
        rtn = min(rtn, GetValue(i - 1, j - 1, a2[j - 1]) + 1);
      }
    } else {
      while (j > 0 && a2[j - 1] >= right) {
        j--;
      }
      if (j == 0) {
        rtn = kIntInf;
      } else {
        rtn = min(rtn, GetValue(i - 1, j - 1, a2[j - 1]) + 1);
      }
    }
    dp[i][j] = rtn;
    return rtn;
  }

 public:
  int makeArrayIncreasing(vector<int>& arr1, vector<int>& arr2) {
    memset(dp, 0, sizeof(dp));
    sort(arr2.begin(), arr2.end());
    arr2.erase(unique(arr2.begin(), arr2.end()), arr2.end());

    int a1_size = arr1.size(), a2_size = arr2.size();
    a1 = arr1;
    a2 = arr2;

    int rtn = GetValue(arr1.size(), arr2.size(), kIntInf);
    if (rtn == kIntInf) {
      return -1;
    } else {
      return rtn;
    }
  }
};

void trimLeftTrailingSpaces(string &input) {
  input.erase(input.begin(), find_if(input.begin(), input.end(), [](int ch) {
    return !isspace(ch);
  }));
}

void trimRightTrailingSpaces(string &input) {
  input.erase(find_if(input.rbegin(), input.rend(), [](int ch) {
    return !isspace(ch);
  }).base(), input.end());
}

vector<int> stringToIntegerVector(string input) {
  vector<int> output;
  trimLeftTrailingSpaces(input);
  trimRightTrailingSpaces(input);
  input = input.substr(1, input.length() - 2);
  stringstream ss;
  ss.str(input);
  string item;
  char delim = ',';
  while (getline(ss, item, delim)) {
    output.push_back(stoi(item));
  }
  return output;
}

int main() {
  string line;
  while (getline(cin, line)) {
    vector<int> arr1 = stringToIntegerVector(line);
    getline(cin, line);
    vector<int> arr2 = stringToIntegerVector(line);

    int ret = Solution().makeArrayIncreasing(arr1, arr2);

    string out = to_string(ret);
    cout << out << endl;
  }
  return 0;
}