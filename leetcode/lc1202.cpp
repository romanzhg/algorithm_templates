/**
 * Union-Find to build groups, then sort chars inside that group.
 *
 */
#include "common.h"

class Solution {
  vector<int> parent;

  void Union(int a, int b) {
    int pa = Find(a);
    int pb = Find(b);
    parent[pa] = pb;
  }

  int Find(int a) {
    if (a == parent[a]) {
      return a;
    }
    int pa = Find(parent[a]);
    parent[a] = pa;
    return pa;
  }

 public:
  string smallestStringWithSwaps(string s, vector<vector<int>>& pairs) {
    parent.resize(s.size());
    for (int i = 0; i < s.size(); i++) {
      parent[i] = i;
    }
    for (const vector<int>& p : pairs) {
      Union(p[0], p[1]);
    }
    map<int, vector<int>> groups;
    for (int i = 0; i < s.size(); i++) {
      groups[Find(i)].push_back(i);
    }
    string rtn(s.size(), ' ');
    for (auto& kv: groups) {
      vector<char> chars;
      for (int i : kv.second) {
        chars.push_back(s[i]);
      }
      sort(chars.begin(), chars.end());
      sort(kv.second.begin(), kv.second.end());
      for (int i = 0; i < chars.size(); i++) {
        rtn[kv.second[i]] = chars[i];
      }
    }
    return rtn;
  }
};