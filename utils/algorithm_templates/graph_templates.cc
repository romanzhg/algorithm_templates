/*****************************************************************************/

// Basic DFS.
vector<int> graph[MAX_NODES];
int visited[MAX_NODES];

void dfs(int u) {
  visited[u] = 1;
  // PREVISIT(u);
  int edgeCount = graph[u].size();
  for (int i = 0; i < edgeCount; i++) {
    int v = graph[u][i];
    if (!visited[v]) {
      dfs(v);
    }
  }
  // POSTVISIT(u);
}

// Connected components.
void findCC() {
  currentCC = 0;
  memset(visited, 0, sizeof(visited));
  for (int u = 0; u < nodeCount; u++) {
    if (!visited[u]) {
      currentCC++;
      dfs(u);
    }
  }
}

/*****************************************************************************/

// Check if a graph is bipartite.
int color[MAX_NODES];

// @param u: any vertex.
// The color array should be initialized to 0.
// 1 for white, 2 for black. Before calling this method, needs
// to color the initial node.
bool isBipartite(int u) {
  for (int i = 0; i < graph[u].size(); i++) {
    int v = graph[u][i];
    if (color[v] == color[u]) {
      return false;
    }
    if (color[v] == 0) {
      color[v] = 3 - color[u];
      if (!isBipartite(v)) {
        return false;
      }
    }
  }
  return true;
}

/*****************************************************************************/

// Mark cut vertex.
vector<int> graph[MAX_NODES];
// The order a vertex is visited. Set at DFS previsit.
// the value start from 1. 0 means not visited.
int pre[MAX_NODES];
// The node with the lowest pre value that node u and all its
// descents are connected to.
int low[MAX_NODES];

bool isCut[MAX_NODES];

int dfsClock = 0;

// For the initial vertex, parent should be a negative number.
int dfs(int u, int parent) {
  int lowu = pre[u] = ++dfsClock;
  int childCount = 0;
  for (int i = 0; i < graph[u].size(); i++) {
    int v = graph[u][i];
    if (!pre[v]) {
      childCount++;
      int lowv = dfs(v, u);
      lowu = min(lowu, lowv);
      if (lowv >= pre[u]) {
        isCut[u] = true;
      }
    } else if (pre[v] < pre[u] && v != parent) {
      // Reverse edge. v already visited.
      lowu = min(lowu, pre[v]);
    }
  }
  if (parent < 0 && childCount == 1) {
    isCut[u] = false;
  }
  low[u] = lowu;
  return lowu;
}

/*****************************************************************************/
// Bi-connected component. (Not edge bi-connected component.)
// For undirected graph.
int pre[MAX_NODES], isCut[MAX_NODES], bccNumber[MAX_NODES];
int dfsClock = 0, bccCount = 0;

vector<int> graph[MAX_NODES];
vector<int> bcc[MAX_NODES];

struct Edge {
  int u, v;
};

stack<Edge> edgeStack;

int dfs(int u, int parent) {
  int lowu = pre[u] = ++dfsClock;
  int childCount = 0;
  for (int i = 0; i < graph[u].size(); i++) {
    int v = graph[u][i];
    Edge e = (Edge){u, v};
    if (!pre[v]) {
      edgeStack.push(e);
      childCount++;
      int lowv = dfs(v, u);
      lowu = min(lowu, lowv);
      if (lowv >= pre[u]) {
        isCut[u] = true;
        bccCount++;
        bcc[bccCount].clear();
        for (;;) {
          Edge x = edgeStack.top(); edgeStack.pop();
          if (bccNumber[x.u] != bccCount){
            bcc[bccCount].push_back(x.u);
            bccNumber[x.u] = bccCount;
          }
          if (bccNumber[x.v] != bccCount){
            bcc[bccCount].push_back(x.v);
            bccNumber[x.v] = bccCount;
          }
          if (x.u == u && x.v == v) {
            break;
          }
        }
      }
    } else if (pre[v] < pre[u] && v != parent) {
      edgeStack.push(e);
      lowu = min(lowu, pre[v]);
    }
  }
  if (parent < 0 && childCount == 1) {
    isCut[u] = false;
  }
  return lowu;
}

void findBCC(int nodeCount) {
  memset(pre, 0, sizeof(pre));
  memset(isCut, 0, sizeof(isCut));
  memset(bccNumber, 0, sizeof(bccNumber));
  dfsClock = 0;
  bccCount = 0;
  for (int i = 0; i < nodeCount; i++) {
    if (!pre[i]) {
      dfs(i, -1);
    }
  }
}

/*****************************************************************************/
// Dijkstra, for single source shortest path.

// Either the dists array or the done array can be removed.

struct HeapNode {
  int dist;
  int vertex_id;

  bool operator< (const HeapNode& o) const {
    return dist > o.dist;
  }
};

struct Edge {
  int from;
  int to;
  int dist;
};

struct Dijkstra {
  int dists[VERTEX_SIZE];
  bool done[VERTEX_SIZE];
  vector<Edge> edges;
  vector<int> adj_list[VERTEX_SIZE];
  // The preceding node in the shortest path for each node.
  int pre[VERTEX_SIZE];

  void Init() {
    edges.clear();
    for (int i = 0; i < VERTEX_SIZE; i++) {
      adj_list[i].clear();
    }
    for (int i = 0; i < VERTEX_SIZE; i++) {
      dists[i] = INF;
    }
    memset(done, 0, sizeof(done));
  }

  void AddEdge(const Edge& e) {
    edges.push_back(e);
    adj_list[e.from].push_back(edges.size() - 1);
  }

  void GetShortestDist(int src_id) {
    priority_queue<HeapNode> pq_;
    dists[src_id] = 0;
    pq_.push((HeapNode){0, src_id});

    while(!pq_.empty()) {
      HeapNode x = pq_.top(); pq_.pop();
      int vertex_id = x.vertex_id;
      // A node may be pushed multiple times, but with the bool array done here, we only process
      // each node once. The first time one node popped corresponds to its shortest distance.
      if (done[vertex_id]) {
        continue;
      }
      done[vertex_id] = true;
      for (int i = 0; i < adj_list[vertex_id].size(); i++) {
        Edge& e = edges[adj_list[vertex_id][i]];
        if (dists[e.to] > dists[vertex_id] + e.dist) {
          dists[e.to] = dists[vertex_id] + e.dist;
          pre[e.to] = adj_list[vertex_id][i];
          pq_.push((HeapNode){dists[e.to], e.to});
        }
      }
    }
  }
};

/*****************************************************************************/

// Maximum flow.
struct Edge {
  int from, to, cap, flow;
  Edge(int from, int to, int cap, int flow) :
      from(from), to(to), cap(cap), flow(flow) {}
};

// An easy but lower efficient algorithm.
struct EdmondsKarpSolver {
  int node_count, edge_count;
  vector<Edge> edges;
  // graph[i][j] is the index(in edges) of the j-th edge going out from the i-th node.
  vector<int> graph[MAX_NODES];

  // Possible improvements from source to the node.
  int arg_amount[MAX_NODES];
  int from_edge[MAX_NODES];

  void Init(int input_node_count) {
    node_count = input_node_count;
    for (int i = 0; i < node_count; i++) {
      graph[i].clear();
    }
    edges.clear();
  }

  void AddEdge(int from, int to, int cap) {
    edges.push_back(Edge(from, to, cap, 0));
    // Reverse arc.
    edges.push_back(Edge(to, from, 0, 0));
    edge_count = edges.size();
    graph[from].push_back(edge_count - 2);
    graph[to].push_back(edge_count - 1);
  }

  int MaxFlow(int src, int target) {
    int flow = 0;
    while (true) {
      memset(arg_amount, 0, sizeof(arg_amount));
      queue<int> vertex_queue;
      vertex_queue.push(src);
      arg_amount[src] = INT_INF;

      while (!vertex_queue.empty()) {
        int cur = vertex_queue.front(); vertex_queue.pop();
        for (int i = 0; i < (int) graph[cur].size(); i++) {
          Edge& e = edges[graph[cur][i]];
          if ((arg_amount[e.to] == 0) && e.cap > e.flow) {
            from_edge[e.to] = graph[cur][i];
            arg_amount[e.to] = min(arg_amount[cur], e.cap - e.flow);
            vertex_queue.push(e.to);
          }
        }
        if (arg_amount[target] != 0) {
          break;
        }
      }

      if (arg_amount[target] == 0) {
        break;
      }

      for (int u = target; u != src; u = edges[from_edge[u]].from) {
        edges[from_edge[u]].flow += arg_amount[target];
        edges[from_edge[u] ^ 1].flow -= arg_amount[target];
      }

      flow += arg_amount[target];
    }
    return flow;
  }
};

/*****************************************************************************/

// Minimum cost maximum flow.

struct Edge {
  int from, to, cap, flow, cost;
  Edge(int u, int v, int c, int f, int w) :
      from(u), to(v), cap(c), flow(f), cost(w) {}
};

struct MCMF {
  int node_count, edge_count;
  vector<Edge> edges;
  vector<int> graph[MAX_NODES];

  int in_queue[MAX_NODES];
  // Distance for Bellman-Ford, actually corresponds to the cost.
  int dist[MAX_NODES];
  int arg_amount[MAX_NODES];
  int from_edge[MAX_NODES];

  void init(int node_count) {
    this->node_count = node_count;
    for (int i = 0; i < node_count; i++) {
      graph[i].clear();
    }
    edges.clear();
  }

  void AddEdge(int from, int to, int cap, int cost) {
    edges.push_back(Edge(from, to, cap, 0, cost));
    edges.push_back(Edge(to, from, 0, 0, -cost));
    edge_count = edges.size();
    graph[from].push_back(edge_count - 2);
    graph[to].push_back(edge_count - 1);
  }

  bool BellmanFord(int src, int target, int &flow, long long &cost) {
    for (int i = 0; i < node_count; i++) {
      dist[i] = INT_INF;
    }
    memset(in_queue, 0, sizeof(in_queue));
    dist[src] = 0;
    in_queue[src] = 1;
    from_edge[src] = 0;
    arg_amount[src] = INT_INF;

    queue<int> vertex_queue;
    vertex_queue.push(src);
    while (!vertex_queue.empty()) {
      int cur_vertex = vertex_queue.front();
      vertex_queue.pop();
      in_queue[cur_vertex] = 0;
      for (int i = 0; i < graph[cur_vertex].size(); i++) {
        Edge &e = edges[graph[cur_vertex][i]];
        if (e.cap > e.flow && dist[e.to] > dist[cur_vertex] + e.cost) {
          dist[e.to] = dist[cur_vertex] + e.cost;
          from_edge[e.to] = graph[cur_vertex][i];
          arg_amount[e.to] = min(arg_amount[cur_vertex], e.cap - e.flow);
          if (!in_queue[e.to]) {
            vertex_queue.push(e.to);
            in_queue[e.to] = 1;
          }
        }
      }
    }

    if (dist[target] == INT_INF) {
      return false;
    }

    flow += arg_amount[target];
    cost += (long long) dist[target] * (long long) arg_amount[target];
    for (int u = target; u != src; u = edges[from_edge[u]].from) {
      edges[from_edge[u]].flow += arg_amount[target];
      edges[from_edge[u] ^ 1].flow -= arg_amount[target];
    }
    return true;
  }

  int MinCostMaxFlow(int s, int t, long long &cost) {
    int flow = 0;
    cost = 0;
    while (BellmanFord(s, t, flow, cost)) {};
    return flow;
  }
};

/*****************************************************************************/

