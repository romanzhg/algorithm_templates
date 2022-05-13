/**
 * Free char is defined as char not used as to. One free char can solve any number of cycles.
 *
 * A -> B means B needs to be performed before A(the dfs order).
 *
 * The way of counting cycles with dfs seems to work with any graph. Particularly, in the
 * graph for this problem, every vertex has only 1 outgoing edge, the bfs way of counting
 * cycle clearly works.
 *
 * I understand the problem wrongly. I didn't see that free char can be reused.
 *
 * The solution in discussion board is much simpler:
 * (note that if there is no cycle and s1 != s2, and no multiple mapping, there must be some free char)
    bool canConvert(string s1, string s2) {
        if (s1 == s2) return true;
        unordered_map<char, char> dp;
        for (int i = 0; i < s1.length(); ++i) {
            if (dp[s1[i]] != NULL && dp[s1[i]] != s2[i])
                return false;
            dp[s1[i]] = s2[i];
        }
        return set(s2.begin(), s2.end()).size() < 26;
    }
 *
 */
#include "common.h"

struct Mapping {
  char f, t;
  Mapping(char f, char t) : f(f), t(t) {};
};

struct Edge {
  int from, to;
  Edge(int from, int to) : from(from), to(to) {};
};

class Solution {
  vector<Edge> edges;
  vector<vector<int>> graph;
  vector<bool> mark;
  vector<bool> pmark;
  int cycles;
 public:
  void dfs(int node) {
    if (pmark[node]) {
      return;
    }
    if (mark[node]) {
      cycles++;
      return;
    }
    mark[node] = true;

    for (int edge_index : graph[node]) {
      dfs(edges[edge_index].to);
    }

    pmark[node] = true;
    mark[node] = false;
  }

  bool canConvert(string str1, string str2) {
    map<char, char> tmp;
    for (int i = 0; i < (int)str1.size(); i++) {
      char f = str1[i];
      char t = str2[i];
      if (tmp.find(f) == tmp.end()) {
        tmp[f] = t;
      } else {
        if (t != tmp[f]) {
          return false;
        }
      }
    }
    vector<Mapping> mappings;
    set<char> v_set;
    for (const auto& kv : tmp) {
      mappings.push_back({kv.first, kv.second});
      v_set.insert(kv.second);
    }

    int free_char_count = 26 - (int)v_set.size();

    int mapping_size = mappings.size();

    edges.clear();
    graph.clear();
    graph.resize(mapping_size);
    for (int i = 0; i < mapping_size; i++) {
      for (int j = 0; j < mapping_size; j++) {
        if (i == j) {
          continue;
        }
        if (mappings[i].t == mappings[j].f) {
          graph[i].push_back(edges.size());
          edges.push_back({i, j});
        }
      }
    }

    cycles = 0;
    mark = vector<bool>(mapping_size, false);
    pmark = vector<bool>(mapping_size, false);
    for (int i = 0; i < (int) mappings.size(); i++) {
      dfs(i);
    }

    return cycles == 0 || free_char_count > 0;
  }
};

string stringToString(string input) {
  assert(input.length() >= 2);
  string result;
  for (int i = 1; i < input.length() -1; i++) {
    char currentChar = input[i];
    if (input[i] == '\\') {
      char nextChar = input[i+1];
      switch (nextChar) {
        case '\"': result.push_back('\"'); break;
        case '/' : result.push_back('/'); break;
        case '\\': result.push_back('\\'); break;
        case 'b' : result.push_back('\b'); break;
        case 'f' : result.push_back('\f'); break;
        case 'r' : result.push_back('\r'); break;
        case 'n' : result.push_back('\n'); break;
        case 't' : result.push_back('\t'); break;
        default: break;
      }
      i++;
    } else {
      result.push_back(currentChar);
    }
  }
  return result;
}

string boolToString(bool input) {
  return input ? "True" : "False";
}

int main() {
  string line;
  while (getline(cin, line)) {
    string str1 = stringToString(line);
    getline(cin, line);
    string str2 = stringToString(line);

    bool ret = Solution().canConvert(str1, str2);

    string out = boolToString(ret);
    cout << out << endl;
  }
  return 0;
}