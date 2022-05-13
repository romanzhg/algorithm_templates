#include "common.h"

class Solution {
  // Return sum and size of the subtree, for a subtree deleted, both sum and size would be 0, update deleted nodes.
  pair<int, int> helper(int cur, const vector<int>& value, const map<int, vector<int>>& children, int& deleted_nodes) {
    int cur_value = value[cur];
    int child_sum = 0;
    int child_size_sum = 0;
    if (children.find(cur) != children.end()) {
      for (int child : children.at(cur)) {
        auto rtn = helper(child, value, children, deleted_nodes);
        child_sum += rtn.first;
        child_size_sum += rtn.second;
      }
    }
    if (cur_value + child_sum == 0) {
      deleted_nodes += child_size_sum + 1;
      return {0, 0};
    } else {
      return {cur_value + child_sum, child_size_sum + 1};
    }
  }

 public:
  int deleteTreeNodes(int nodes, vector<int>& parent, vector<int>& value) {
    int root;
    map<int, vector<int>> children;
    for (int i = 0; i < parent.size(); i++) {
      children[parent[i]].push_back(i);
      if (parent[i] == -1) {
        root = i;
      }
    }

    int deleted_nodes = 0;
    helper(root, value, children, deleted_nodes);
    return nodes - deleted_nodes;
  }
};