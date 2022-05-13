/**
 * Beware of the type conversion.
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
  int UpdateTreeWithSum(TreeNode* root) {
    if (root == NULL) {
      return 0;
    }
    int l = UpdateTreeWithSum(root->left);
    int r = UpdateTreeWithSum(root->right);
    root->val = l + r + root->val;
    return root->val;
  }

  void GetMinDiff(TreeNode* root, double v, double& min_diff) {
    if (root == NULL) {
      return;
    }
    min_diff = min(min_diff, abs(root->val - v));
    GetMinDiff(root->left, v, min_diff);
    GetMinDiff(root->right, v, min_diff);
  }

  int maxProduct(TreeNode* root) {
    int overall_sum = UpdateTreeWithSum(root);
    double v = (double)overall_sum / 2.0;
    double min_diff = 1E9;
    GetMinDiff(root, v, min_diff);

    long long a = round(v + min_diff);
    long long b = round(v - min_diff);
    return ((a * b) % (int)(1e9 + 7));
  }
};