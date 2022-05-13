/**
 * Note the use of mem to reuse nodes, the only information on a node is how it connects to other nodes,
 * so it can be reused.
 */
#include "common.h"

/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode(int x) : val(x), left(NULL), right(NULL) {}
 * };
 */
class Solution {
 public:
  map<int, vector<TreeNode*>> memo;

  vector<TreeNode*> helper(int N) {
    if (memo.find(N) != memo.end()) {
      return memo[N];
    }

    int r = N - 1;
    vector<TreeNode*> rtn;
    for (int i = 1; i <= r - 1; i++) {
      vector<TreeNode*> left = helper(i);
      vector<TreeNode*> right = helper(r - i);
      if (left.empty() || right.empty()) {
        continue;
      }

      for (TreeNode* l : left) {
        for (TreeNode* r : right) {
          TreeNode* tmp = new TreeNode(0);
          tmp->left = l;
          tmp->right = r;
          rtn.push_back(tmp);
        }
      }
    }
    memo[N] = rtn;
    return rtn;
  }

  vector<TreeNode*> allPossibleFBT(int N) {
    memo[0] = {NULL};
    memo[1] = {new TreeNode(0)};
    return helper(N);
  }
};