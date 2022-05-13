/**
 * Recursive, record distance.
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
  vector<int> rtn;
  int target_value;
  int target_dist;
 public:

  // Return -1 if the target node is not in this subtree.
  int GetDist(TreeNode* root) {
    if (root == NULL) {
      return -1;
    }
    if (root->val == target_value) {
      GetBelow(root, target_dist);
      return 0;
    }
    int left_dist = GetDist(root->left);
    int right_dist = GetDist(root->right);
    if (left_dist == -1 && right_dist == -1) {
      return -1;
    }

    int dist;
    if (left_dist != -1) {
      dist = left_dist + 1;
    } else {
      dist = right_dist + 1;
    }

    if (dist == target_dist) {
      rtn.push_back(root->val);
    } else if (dist < target_dist) {
      if (left_dist == -1) {
        GetBelow(root->left, target_dist - dist - 1);
      } else {
        GetBelow(root->right, target_dist - dist - 1);
      }
    }
    return dist;
  }

  void GetBelow(TreeNode* cur, int K) {
    if (cur == NULL) {
      return;
    }
    if (K == 0) {
      rtn.push_back(cur->val);
      return;
    }
    GetBelow(cur->left, K - 1);
    GetBelow(cur->right, K - 1);
  }

  vector<int> distanceK(TreeNode* root, TreeNode* target, int K) {
    rtn.clear();
    target_value = target->val;
    target_dist = K;
    GetDist(root);
    return rtn;
  }
};