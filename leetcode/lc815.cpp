/**
 * Multi src/dest shortest dist with bfs.
 * Note the way the adjacent list was built.
 */

#include "common.h"

struct Elem {
  int id, dist;
  Elem(int id, int dist) : id(id), dist(dist) {};
};

class Solution {
 public:
  // -2 for no path.
  int GetShortestPath(const vector<vector<bool>>& adj,
                      const vector<int>& start_routes,
                      const vector<int>& dest_routes) {
    set<int> dests(dest_routes.begin(), dest_routes.end());

    queue<Elem> q;
    for (int from : start_routes) {
      q.push({from, 0});
    }

    vector<bool> visited(adj.size(), false);

    while (!q.empty()) {
      Elem cur = q.front(); q.pop();

      if (dests.find(cur.id) != dests.end()) {
        return cur.dist;
      }

      if (visited[cur.id]) {
        continue;
      }
      visited[cur.id] = true;

      for (int i = 0; i < adj.size(); i++) {
        if (adj[cur.id][i] && !visited[i]) {
          q.push({i, cur.dist + 1});
        }
      }
    }
    return -2;
  }

  int numBusesToDestination(vector<vector<int>>& routes, int S, int T) {
    if (S == T) {
      return 0;
    }
    int set_count = routes.size();
    vector<vector<bool>> adj(set_count, vector<bool>(set_count, false));

    map<int, vector<int>> stop_to_route;
    for (int route = 0; route < set_count; route++) {
      for (int stop : routes[route]) {
        stop_to_route[stop].push_back(route);
      }
    }

    for (const auto& kv : stop_to_route) {
      const vector<int>& connected_routes = kv.second;
      for (int i = 0; i < connected_routes.size(); i++) {
        for (int j = i + 1; j < connected_routes.size(); j++) {
          adj[connected_routes[i]][connected_routes[j]] = true;
          adj[connected_routes[j]][connected_routes[i]] = true;
        }
      }
    }

    vector<int> start_routes = stop_to_route[S];
    vector<int> dest_routes = stop_to_route[T];
    if (start_routes.empty() || dest_routes.empty()) {
      return -1;
    }
    return GetShortestPath(adj, start_routes, dest_routes) + 1;
  }
};