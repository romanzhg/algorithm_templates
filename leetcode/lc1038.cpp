/**
 * Solution uses the fact that the given tree is a binary search tree with "distinct" values.
 * Solution1 is even simpler, it maintains a to_add value, refers to the values to be added
 * to this subtree.
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
  void BstToArray(TreeNode* n, vector<int>& a) {
    if (n == NULL) {
      return;
    }
    BstToArray(n->left, a);
    a.push_back(n->val);
    BstToArray(n->right, a);
  }

  void UpdateValues(TreeNode* n, const map<int, int>& value_map) {
    if (n == NULL) {
      return;
    }
    UpdateValues(n->left, value_map);
    UpdateValues(n->right, value_map);
    n->val = value_map.at(n->val);
  }

 public:
  TreeNode* bstToGst(TreeNode* root) {
    vector<int> in_order_array;
    BstToArray(root, in_order_array);

    map<int, int> value_map;
    value_map[in_order_array.back()] = in_order_array.back();
    for (int i = in_order_array.size() - 2; i >= 0; i--) {
      int tmp = in_order_array[i] + in_order_array[i + 1];
      value_map[in_order_array[i]] = tmp;
      in_order_array[i] = tmp;
    }
    UpdateValues(root, value_map);
    return root;
  }
};

class Solution1 {
  // Updates the subtree rooted at @n(by doing the transfermation and add @to_add to all elements)
  // and returns the sum of this sub tree(before change).
  int helper(TreeNode* n, int to_add) {
    if (n == NULL) {
      return 0;
    }
    int right_sum = helper(n->right, to_add);
    int left_sum = helper(n->left, to_add + right_sum + n->val);
    int rtn = right_sum + left_sum + n->val;
    n->val += right_sum + to_add;
    return rtn;
  }
 public:
  TreeNode* bstToGst(TreeNode* root) {
    helper(root, 0);
    return root;
  }
};