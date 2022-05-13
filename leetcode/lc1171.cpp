/**
 * Note the use of sentinel.
 * sum_map: key: prefix sum, value: pointer to the current node(whose value is included in the
 * prefix sum).
 * When we delete some nodes, we don't add a new value in the map.
 */
#include "common.h"

class Solution {
 public:
  ListNode* removeZeroSumSublists(ListNode* head) {
    ListNode* sentinel = new ListNode(0);
    sentinel->next = head;

    map<int, ListNode*> sum_map;

    int prefix_sum = 0;
    for (ListNode* cur = sentinel; cur != NULL; cur = cur->next) {
      prefix_sum += cur->val;
      if (sum_map.find(prefix_sum) != sum_map.end()) {
        ListNode* tmp = sum_map[prefix_sum];
        int tmp_value = prefix_sum;
        while (tmp->next != cur) {
          tmp = tmp->next;
          tmp_value += tmp->val;
          sum_map.erase(tmp_value);
        }
        sum_map[prefix_sum]->next = cur->next;
      } else {
        sum_map[prefix_sum] = cur;
      }
    }
    return sentinel->next;
  }
};