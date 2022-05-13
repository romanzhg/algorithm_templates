/**
 * Post-order. Count the move needed at child side.
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
  int rtn;
 public:
  int GetNeed(TreeNode* node) {
    if (node == NULL) {
      return 0;
    }
    int left_need = GetNeed(node->left);
    int right_need = GetNeed(node->right);
    int cur_need = 1 - node->val;
    int total_need = cur_need + left_need + right_need;
    rtn += abs(total_need);
    return total_need;
  }

  int distributeCoins(TreeNode* root) {
    rtn = 0;
    GetNeed(root);
    return rtn;
  }
};