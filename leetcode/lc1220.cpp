class Solution {
  const int kMod = 1e9 + 7;
 public:
  int countVowelPermutation(int n) {
    int a = 1, e = 1, i = 1, o = 1, u = 1;

    for (int len = 1; len < n; len++) {
      int new_a = ((e + i) % kMod + u) % kMod;
      int new_e = (a + i) % kMod;
      int new_i = (e + o) % kMod;
      int new_o = i;
      int new_u = (o + i) % kMod;

      a = new_a;
      e = new_e;
      i = new_i;
      o = new_o;
      u = new_u;
    }

    return (((a + e) % kMod + (i + o)% kMod) % kMod + u) %kMod;
  }
};