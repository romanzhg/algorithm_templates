/**
 * Greedy, unmatched right parentheses are ignored.
 */
#include "common.h"

class Solution {
 public:
  int minAddToMakeValid(string S) {
    int counter = 0, rtn = 0;
    for (char c : S) {
      if (c == '(') {
        counter++;
      } else {
        if (counter > 0) {
          counter--;
        } else {
          rtn++;
        }
      }
    }
    return rtn + counter;
  }
};