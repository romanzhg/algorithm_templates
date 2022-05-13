/**
 * There are only 26 chars, get the longest sequence for each char(after swap) separately.
 * An improvement would be do the counting/build sequence only once.
 *
 */
#include "common.h"

struct Seq {
  int start, end;
  Seq(int start, int end) : start(start), end(end) {};
  int GetSize() { return end - start + 1;};
};

class Solution {
 public:
  int helper(const string& t, char c) {
    vector<Seq> seqs;

    bool seq_started = false;
    Seq cur(0, 0);
    int char_count = 0;
    for (int i = 0; i < t.size(); i++) {
      if (t[i] == c) {
        char_count++;
        if (seq_started) {
          cur.end = i;
        } else {
          cur.start = i;
          cur.end = i;
          seq_started= true;
        }
      } else {
        if (seq_started) {
          seqs.push_back(cur);
          seq_started = false;
        }
      }
    }
    if (seq_started) {
      seqs.push_back(cur);
    }

    if (seqs.size() == 0) {
      return 0;
    }
    int rtn = seqs[0].GetSize() + (seqs[0].GetSize() == char_count ? 0 : 1);

    for (int i = 1; i < seqs.size(); i++) {
      if (seqs[i].start - seqs[i - 1].end == 2) {
        int tmp_size = seqs[i].GetSize() + seqs[i - 1].GetSize();
        rtn = max(rtn, tmp_size + (tmp_size == char_count ? 0 : 1));
      }
      rtn = max(rtn, seqs[i].GetSize() + (seqs[i].GetSize() == char_count ? 0 : 1));
    }
    return rtn;
  }

  int maxRepOpt1(string text) {
    int rtn = 0;
    for (int c = (int)'a'; c <= (int)'z'; c++) {
      rtn = max(rtn, helper(text, c));
    }
    return rtn;
  }
};