/**
 * The is complete condition:
 * 1. It's left subtree and right subtree are both complete(an empty tree is complete).
 * 2. Left depth and right depth differ by at most 1, left depth is larger.
 * 3. Left count larger than right count.
 * 4. Either left count or right count needs to equal to 2 ^ depth - 1.
 *
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
struct TreeNode {
  int val;
  TreeNode *left;
  TreeNode *right;
  TreeNode(int x) : val(x), left(NULL), right(NULL) {}
};

struct Elem {
  int depth, count, is_complete;
  Elem(int depth, int count, int is_complete) : depth(depth), count(count), is_complete(is_complete) {};
};

class Solution {
  Elem helper(TreeNode* n) {
    if (n == NULL) {
      return {0, 0, 1};
    }
    Elem left = helper(n->left);
    Elem right = helper(n->right);
    if (!left.is_complete || !right.is_complete) {
      return {0, 0, 0};
    }

    if ((left.depth - right.depth == 1 || left.depth - right.depth == 0)
        && (left.count >= right.count)
        && ((left.count == ((1 << left.depth) - 1))
            || (right.count == ((1 << right.depth) - 1)))) {
      return {max(left.depth, right.depth) + 1, left.count + right.count + 1, 1};
    }
    return {0, 0, 0};
  }
 public:
  bool isCompleteTree(TreeNode* root) {
    return helper(root).is_complete;
  }
};

void trimLeftTrailingSpaces(string &input) {
  input.erase(input.begin(), find_if(input.begin(), input.end(), [](int ch) {
    return !isspace(ch);
  }));
}

void trimRightTrailingSpaces(string &input) {
  input.erase(find_if(input.rbegin(), input.rend(), [](int ch) {
    return !isspace(ch);
  }).base(), input.end());
}

TreeNode* stringToTreeNode(string input) {
  trimLeftTrailingSpaces(input);
  trimRightTrailingSpaces(input);
  input = input.substr(1, input.length() - 2);
  if (!input.size()) {
    return nullptr;
  }

  string item;
  stringstream ss;
  ss.str(input);

  getline(ss, item, ',');
  TreeNode* root = new TreeNode(stoi(item));
  queue<TreeNode*> nodeQueue;
  nodeQueue.push(root);

  while (true) {
    TreeNode* node = nodeQueue.front();
    nodeQueue.pop();

    if (!getline(ss, item, ',')) {
      break;
    }

    trimLeftTrailingSpaces(item);
    if (item != "null") {
      int leftNumber = stoi(item);
      node->left = new TreeNode(leftNumber);
      nodeQueue.push(node->left);
    }

    if (!getline(ss, item, ',')) {
      break;
    }

    trimLeftTrailingSpaces(item);
    if (item != "null") {
      int rightNumber = stoi(item);
      node->right = new TreeNode(rightNumber);
      nodeQueue.push(node->right);
    }
  }
  return root;
}

string boolToString(bool input) {
  return input ? "True" : "False";
}

int main() {
  string line;
  while (getline(cin, line)) {
    TreeNode* root = stringToTreeNode(line);

    bool ret = Solution().isCompleteTree(root);

    string out = boolToString(ret);
    cout << out << endl;
  }
  return 0;
}