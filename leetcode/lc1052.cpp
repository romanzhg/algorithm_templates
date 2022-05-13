/**
 * Need to think of the start and end conditions.
 */
class Solution {
 public:
  int maxSatisfied(vector<int>& customers, vector<int>& grumpy, int X) {
    int len = customers.size();
    vector<int> modified_customers(len, 0);

    int non_grumpy_sum = 0;
    for (int i = 0; i < len; i++) {
      if (grumpy[i] == 1) {
        modified_customers[i] = customers[i];
      } else {
        non_grumpy_sum += customers[i];
      }
    }

    int tmp_sum = 0;
    for (int i = 0; i < X; i++) {
      tmp_sum += modified_customers[i];
    }
    int max_sum = tmp_sum;
    for (int i = 1; i + X - 1 < len; i++) {
      // Iterate through remaining sliding windows, the first step should be
      // update the previous one.
      tmp_sum -= modified_customers[i - 1];
      tmp_sum += modified_customers[i + X - 1];
      max_sum = max(max_sum, tmp_sum);
    }

    return max_sum + non_grumpy_sum;
  }
};