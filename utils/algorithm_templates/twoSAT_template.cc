const int MAX_VAR = 10;

struct TwoSAT {
	// node_number * 2: marked if node is chosen to be true.
	// node_number * 2 + 1: marked if node is chosen to be false.
  int n;
  vector<int> graph[MAX_VAR * 2];
  bool mark[MAX_VAR * 2];
  int markedElem[MAX_VAR * 2], counter;

  bool dfs(int x) {
    if (mark[x^1]) {
      return false;
    }
    if (mark[x]) {
      return true;
    }
    mark[x] = true;
    markedElem[counter++] = x;
    for (int i = 0; i < graph[x].size(); i++) {
      if (!dfs(graph[x][i])) {
        return false;
      }
    }
    return true;
  }

  // n: number of variables.
  void init(int n) {
    this->n = n;
    for (int i = 0; i < n * 2; i++) {
      graph[i].clear();
    }
    memset(mark, 0, sizeof(mark));
  }

	void addClause(int x, bool notX, int y, bool notY) {
		x = x * 2 + (notX ? 1 : 0);
		y = y * 2 + (notY ? 1 : 0);

		graph[x ^ 1].push_back(y);
		graph[y ^ 1].push_back(x);
	}

  bool solve() {
    for (int i = 0; i < n * 2; i += 2) {
      if (!mark[i] && !mark[i + 1]) {
        counter = 0;
        if (!dfs(i)) {
          while (counter > 0) {
            mark[markedElem[--counter]] = false;
          }
          if (!dfs(i + 1)) {
            return false;
          }
        }
      }
    }
    return true;
  }

  bool getValue(int x) {
  	return mark[2 * x];
  }
};