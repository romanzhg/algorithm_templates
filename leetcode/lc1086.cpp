/**
 * Easy one. Another solution would be record all scores and sort and get the first 5.
 */

class Solution {
  struct TopFive {
    int values[5];
    int count;

    TopFive() {
      count = 0;
    }

    void PushValue(int v) {
      if (count < 5) {
        values[count] = v;
        count++;
      } else {
        int min_value = 101;
        for (int i = 0; i < 5; i++) {
          min_value = min(min_value, values[i]);
        }
        if (v > min_value) {
          for (int i = 0; i < 5; i++) {
            if (values[i] == min_value) {
              values[i] = v;
              break;
            }
          }
        }
      }
    }

    int GetAverage() const {
      int rtn = 0;
      for (int i = 0; i < count; i++) {
        rtn += values[i];
      }
      return rtn / 5;
    }
  };

 public:
  vector<vector<int>> highFive(vector<vector<int>>& items) {
    map<int, TopFive> scores;
    for (const vector<int>& v : items) {
      scores[v[0]].PushValue(v[1]);
    }

    vector<vector<int>> rtn;
    for (const auto& kv : scores) {
      rtn.push_back({kv.first, kv.second.GetAverage()});
    }
    return rtn;
  }
};