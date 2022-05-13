/*****************************************************************************/
// Astar heuristic.
// For consistent heuristic, this tie breaking is correct, and efficient.
bool operator<(const State &o) const {
  if (past_cost + heuristic != o.past_cost + o.heuristic) {
    return past_cost + heuristic > o.past_cost + o.heuristic;
  }
  return heuristic > o.heuristic;
}

/*****************************************************************************/
// Basic AStar.
// Implementation 1.
// This version of AStar assumes that the heuristic is consistent.
// consistent heuristic implies once a node is popped, its g value is the shortest distance to it.
struct HeapNode {
  int vertex_id;
  int past_cost;
  int heuristic;

  HeapNode(int vertex_id, int past_cost, int heuristic)
      : vertex_id(vertex_id), past_cost(past_cost), heuristic(heuristic) {}

  bool operator<(const HeapNode &o) const {
    // Note that tie is broken arbitrarily, which the original paper requires favoring the target node.
    return past_cost + heuristic > o.past_cost + o.heuristic;
  }
};

// The preceeding node in the shortest path for each node.
int pre[VERTEX_SIZE];

int GetHeuristic(int vertex_id) {
  return 0;
}

set<int> GetAdj(int vertex_id) {
  return {};
}

int GetDist(int vertex_a, int vertex_b) {
  return 0;
}

// Return the shortest distance from source to target.
// Return -1 if the target cannot be reached.
int Astar(int src_id, int target_id) {
  priority_queue<HeapNode> open;
  set<int> closed;

  open.push(HeapNode(src_id, 0, GetHeuristic(src_id)));

  while (!open.empty()) {
    HeapNode cur = open.top();
    open.pop();
    if (cur.vertex_id == target_id) {
      closed.insert(cur.vertex_id);
      return cur.past_cost;
    }
    closed.insert(cur.vertex_id);
    for (int adj_vertex : GetAdj(cur.vertex_id)) {
      // A node may be pushed multiple times. But when all its adjacent nodes are in the closed set,
      // expanding it again won't make any difference.
      if (closed.find(adj_vertex) != closed.end()) {
        continue;
      }
      pre[adj_vertex] = cur.vertex_id;
      open.push(HeapNode(adj_vertex,
                         cur.past_cost + GetDist(cur.vertex_id, adj_vertex),
                         GetHeuristic(adj_vertex)));
    }
  }
  return -1;
}

/*****************************************************************************/
// Basic AStar.
// Implementation 2.
// This version of AStar is same as above but with one optimization for consistent heuristic.
// Before we expand to the adjacent nodes of the node, check if the parent node is already in
// the closed set, if yes then don't expand, since the result won't change.

int Astar(int src_id, int target_id) {
  priority_queue<HeapNode> open;
  set<int> closed;

  open.push(HeapNode(src_id, 0, GetHeuristic(src_id)));

  while (!open.empty()) {
    HeapNode cur = open.top();
    open.pop();
    if (cur.vertex_id == target_id) {
      closed.insert(cur.vertex_id);
      return cur.past_cost;
    }

    if (closed.find(adj_vertex) != closed.end()) {
      continue;
    }
    closed.insert(cur.vertex_id);

    for (int adj_vertex : GetAdj(cur.vertex_id)) {
      // A node may be pushed multiple times. But when all its adjacent nodes are in the closed set,
      // expanding it again won't make any difference.
      if (closed.find(adj_vertex) != closed.end()) {
        continue;
      }
      pre[adj_vertex] = cur.vertex_id;
      open.push(HeapNode(adj_vertex,
                         cur.past_cost + GetDist(cur.vertex_id, adj_vertex),
                         GetHeuristic(adj_vertex)));
    }
  }
  return -1;
}

/*****************************************************************************/
// Basic AStar.
// Implementation 2.1. Currently the best version.
// Derived from implementation 2. Record the best known g value for each node, do not push
// the current node if a same node with lower g value is already pushed.
int Astar(int src_id, int target_id) {
  priority_queue<HeapNode> open;
  set<int> closed;
  map<int, int> g_value;

  open.push(HeapNode(src_id, 0, GetHeuristic(src_id)));
  g_value[src_id] = 0;

  while (!open.empty()) {
    HeapNode cur = open.top();
    open.pop();
    if (cur.vertex_id == target_id) {
      closed.insert(cur.vertex_id);
      return cur.past_cost;
    }

    if (closed.find(adj_vertex) != closed.end()) {
      continue;
    }
    closed.insert(cur.vertex_id);

    for (int adj_vertex : GetAdj(cur.vertex_id)) {
      // A node may be pushed multiple times. But when all its adjacent nodes are in the closed set,
      // expanding it again won't make any difference.
      if (closed.find(adj_vertex) != closed.end()) {
        continue;
      }

      int tmp_g_value = cur.past_cost + GetDist(cur.vertex_id, adj_vertex);
      if (g_value.find(adj_vertex) != g_value.end()) {
        if (tmp_g_value >= g_value.at(adj_vertex)) {
          continue;
        }
      }
      g_value[adj_vertex] = tmp_g_value;
      pre[adj_vertex] = cur.vertex_id;
      open.push(HeapNode(adj_vertex,
                         cur.past_cost + GetDist(cur.vertex_id, adj_vertex),
                         GetHeuristic(adj_vertex)));
    }
  }
  return -1;
}


/*****************************************************************************/
// Basic AStar.
// Implementation 3.
// This implementation push a node to open only once.
// This gives a suboptimal solution, but expands much less nodes.
// Worth using as a way of relaxation.
int Astar(int src_id, int target_id) {
  priority_queue<HeapNode> open;
  set<int> closed;

  open.push(HeapNode(src_id, 0, GetHeuristic(src_id)));

  while (!open.empty()) {
    HeapNode cur = open.top();
    open.pop();
    if (cur.vertex_id == target_id) {
      closed.insert(cur.vertex_id);
      return cur.past_cost;
    }

    for (int adj_vertex : GetAdj(cur.vertex_id)) {
      // A node may be pushed multiple times. But when all its adjacent nodes are in the closed set,
      // expanding it again won't make any difference.
      if (closed.find(adj_vertex) != closed.end()) {
        continue;
      }
      closed.insert(cur.vertex_id);

      pre[adj_vertex] = cur.vertex_id;
      open.push(HeapNode(adj_vertex,
                         cur.past_cost + GetDist(cur.vertex_id, adj_vertex),
                         GetHeuristic(adj_vertex)));
    }
  }
  return -1;
}

/*****************************************************************************/
// Basic AStar.
// Implementation 4.
// This version of AStar assumes that the heuristic is admissible.
// In this version the closed set is not useful, since we always mark a node as open
// when a cheaper shortest path is found.
//
// vertex id to estimated shortest distance, with default value INF.
// When the algorithm terminates, the estimated shortest distance for closed
// nodes will be actual shortest distance.
// This corresponds to the estimated g value as in the paper.
int est_g_value[VERTEX_SIZE];

// Returns if the target is reachable.
bool Astar(int src_id, int target_id) {
  priority_queue<HeapNode> open;
  for (int i = 0; i < VERTEX_SIZE; i++) {
    est_g_value[i] = INF;
  }

  open.push(HeapNode(src_id, 0, GetHeuristic(src_id)));
  est_g_value[src_id] = 0;

  while (!open.empty()) {
    HeapNode cur = open.top();
    open.pop();
    if (cur.vertex_id == target_id) {
      return true;
    }
    for (int adj_vertex : GetAdj(cur.vertex_id)) {
      int new_est_g_value = cur.past_cost + GetDist(cur.vertex_id, adj_vertex);
      // In the original paper estimated f value is compared here.
      // Since the heuristic is always the same, we can only compare the estimated g value.
      if (new_est_g_value < est_g_value[adj_vertex]) {
        pre[adj_vertex] = cur.vertex_id;
        est_g_value[adj_vertex] = new_est_g_value;

        open.push(HeapNode(adj_vertex,
                           new_est_g_value,
                           GetHeuristic(adj_vertex)));

      }
    }
  }
  return false;
}

