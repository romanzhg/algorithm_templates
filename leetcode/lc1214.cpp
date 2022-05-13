/**
 * This program demos use (void *) as a map key to store results of search.
 *
 * Another solution would be similar to two pointers.
 * Two pointers are normally for one array, but it also works for two arrays(?).
 */

/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode(int x) : val(x), left(NULL), right(NULL) {}
 * };
 */

#include "common.h"

class Solution {
  map<void *, map<void *, bool>> dp;
 public:
  bool ContainsKey(TreeNode* n1, TreeNode* n2) {
    return dp.find((void *)n1) != dp.end() && dp[n1].find((void *)n2) != dp[n1].end();
  }

  bool twoSumBSTs(TreeNode* root1, TreeNode* root2, int target) {
    if (root1 == NULL || root2 == NULL) {
      return false;
    }
    if (ContainsKey(root1, root2)) {
      return dp[root1][root2];
    }

    int v = root1->val + root2->val;

    bool rtn;
    if (v == target) {
      rtn = true;
    } else if (v > target) {
      rtn = twoSumBSTs(root1->left, root2, target) || twoSumBSTs(root1, root2->left, target);
    } else {
      rtn = twoSumBSTs(root1->right, root2, target) || twoSumBSTs(root1, root2->right, target);
    }
    dp[root1][root2] = rtn;
    return rtn;
  }
};