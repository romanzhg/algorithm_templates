/**
 * Note Solution1 can be greatly simplified, the simplified version is Solution.
 *
 * odd_reachable and even_reachable can be calculated from back to front, so the calculation can be
 * combined with odd_next and even_next.
 *
 * When calculatin from back to front, we can use a map to record the lowest index of each value, so
 * there is no need to define Elem1 and Elem2.
 *
 * To get prev/next value in a binary search tree, we need to traverse the tree.
 * If the current node has a right child, just go right and keep on going left.
 * Else, go up until reached a parent that is larger.
 * The c++ RedBlack tree source is attached.
 *

  https://code.woboq.org/gcc/libstdc++-v3/src/c++98/tree.cc.html

  static _Rb_tree_node_base*
  local_Rb_tree_increment(_Rb_tree_node_base* __x) throw ()
  {
    if (__x->_M_right != 0)
      {
        __x = __x->_M_right;
        while (__x->_M_left != 0)
          __x = __x->_M_left;
      }
    else
      {
        _Rb_tree_node_base* __y = __x->_M_parent;
        while (__x == __y->_M_right)
          {
            __x = __y;
            __y = __y->_M_parent;
          }
        if (__x->_M_right != __y)
          __x = __y;
      }
    return __x;
  }

 */
#include "common.h"

class Solution {

 public:
  int oddEvenJumps(vector<int>& A) {
    int size = A.size();
    vector<bool> odd_reachable(size, false);
    vector<bool> even_reachable(size, false);

    map<int, int> value_to_index;
    for (int i = size - 1; i >= 0; i--) {
      int odd_next, even_next;
      int cur_val = A[i];
      if (value_to_index.lower_bound(cur_val) == value_to_index.end()) {
        odd_next = -1;
      } else {
        odd_next = value_to_index.lower_bound(cur_val)->second;
      }
      auto it = value_to_index.upper_bound(cur_val);
      if (it != value_to_index.begin()) {
        even_next = std::prev(it)->second;
      } else {
        even_next = -1;
      }
      value_to_index[cur_val] = i;

      if (i == size - 1) {
        odd_reachable[i] = true;
        even_reachable[i] = true;
      } else {
        if (odd_next != -1) {
          odd_reachable[i] = even_reachable[odd_next];
        }
        if (even_next != -1) {
          even_reachable[i] = odd_reachable[even_next];
        }
      }

    }

    int rtn = 0;
    for (int i = 0; i < size; i++) {
      if (odd_reachable[i]) {
        rtn++;
      }
    }
    return rtn;
  }
};

/******************************************************************************/
struct Elem1 {
  int value, index;
  Elem1(int value, int index) : value(value), index(index) {};
  bool operator<(const Elem1& o) const {
    if (value != o.value) {
      return value < o.value;
    }
    return index < o.index;
  }
};
struct Elem2 {
  int value, index;
  Elem2(int value, int index) : value(value), index(index) {};
  bool operator<(const Elem2& o) const {
    if (value != o.value) {
      return value > o.value;
    }
    return index < o.index;
  }
};

class Solution1 {
  vector<int> odd_reachable;
  vector<int> even_reachable;
  int size;

  int dfs(int index, bool is_odd, const vector<int>& odd_next, const vector<int>& even_next) {
    if (index == -1) {
      return 0;
    }
    if (is_odd) {
      if (odd_reachable[index] != -1) {
        return odd_reachable[index];
      }
    } else {
      if (even_reachable[index] != -1) {
        return even_reachable[index];
      }
    }

    if (index == size - 1) {
      return 1;
    }

    if (is_odd) {
      int v = dfs(odd_next[index], !is_odd, odd_next, even_next);
      odd_reachable[index] = v;
      return v;
    } else {
      int v = dfs(even_next[index], !is_odd, odd_next, even_next);
      even_reachable[index] = v;
      return v;
    }
  }

 public:
  int oddEvenJumps(vector<int>& A) {
    size = A.size();

    vector<int> odd_next(size, -1);
    vector<int> even_next(size, -1);

    set<Elem1> odd_jump_set;
    set<Elem2> even_jump_set;
    for (int i = size - 1; i >= 0; i--) {
      auto it0 = odd_jump_set.lower_bound({A[i], i});
      if (it0 != odd_jump_set.end()) {
        odd_next[i] = it0->index;
      }
      odd_jump_set.insert(Elem1(A[i], i));

      auto it1 = even_jump_set.lower_bound({A[i], i});
      if (it1 != even_jump_set.end()) {
        even_next[i] = it1->index;
      }
      even_jump_set.insert(Elem2(A[i], i));
    }

    odd_reachable = vector<int>(size, -1);
    even_reachable = vector<int>(size, -1);
    odd_reachable[size - 1] = 1;
    even_reachable[size - 1] = 1;
    for (int i = 0; i < size; i++) {
      if (odd_reachable[i] == -1) {
        dfs(i, true, odd_next, even_next);
      }
      if (even_reachable[i] == -1) {
        dfs(i, false, odd_next, even_next);
      }
    }
    int rtn = 0;
    for (int i = 0; i < size; i++) {
      if (odd_reachable[i] == 1) {
        rtn++;
      }
    }
    return rtn;
  }
};