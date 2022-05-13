/**
 * Copied from the solution page:
 *
 * A preorder traversal is:
 *   (root node) (preorder of left branch) (preorder of right branch)
 * While a postorder traversal is:
 *   (postorder of left branch) (postorder of right branch) (root node)
 *
 * Find boundary and build the tree recursively.
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
  TreeNode* helper(const vector<int>& pre, const vector<int>& post,
                   int a_left, int a_right, int b_left, int b_right) {
    if (a_left > a_right || b_left > b_right) {
      return NULL;
    }
    if (a_left == a_right) {
      return new TreeNode(pre[a_left]);
    }
    TreeNode* rtn = new TreeNode(pre[a_left]);

    int tmp_a_left = a_left + 1;
    int index_tmp_b_right = b_left;
    while (post[index_tmp_b_right] != pre[tmp_a_left]) {
      index_tmp_b_right++;
    }
    int len = index_tmp_b_right - b_left + 1;

    rtn->left = helper(pre, post, tmp_a_left, tmp_a_left + len - 1, b_left, index_tmp_b_right);
    rtn->right = helper(pre, post, tmp_a_left + len, a_right, index_tmp_b_right + 1, b_right - 1);
    return rtn;

  }
  TreeNode* constructFromPrePost(vector<int>& pre, vector<int>& post) {
    return helper(pre, post, 0, pre.size() - 1, 0, post.size() - 1);
  }
};