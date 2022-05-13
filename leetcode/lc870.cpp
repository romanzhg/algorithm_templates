/**
 * Elem is for sort values while remember their index in the original array.
 *
 * After A and B are both sorted, match as much as possible(for each element in B, find the first match in A).
 */

#include "common.h"

struct Elem {
  int index, value;
  Elem(int index, int value) : index(index), value(value) {};
  bool operator<(const Elem& o) const {
    return value < o.value;
  }
};

class Solution {

 public:
  vector<int> advantageCount(vector<int>& A, vector<int>& B) {
    sort(A.begin(), A.end());
    vector<Elem> tmp_b;
    for (int i = 0; i < B.size(); i++) {
      tmp_b.push_back({i, B[i]});
    }
    sort(tmp_b.begin(), tmp_b.end());

    vector<int> rtn(A.size());
    vector<int> used(A.size(), false);
    int counter = 0;
    int i = 0;
    for (; i < tmp_b.size();) {
      if (counter == A.size()) {
        break;
      }
      if (A[counter] > tmp_b[i].value) {
        rtn[tmp_b[i].index] = A[counter];
        used[counter] = true;
        counter++;
        i++;
      } else {
        counter++;
      }
    }

    counter = 0;
    for (; i < tmp_b.size(); i++) {
      while (used[counter]) {
        counter++;
      }
      rtn[tmp_b[i].index] = A[counter++];
    }
    return rtn;
  }
};