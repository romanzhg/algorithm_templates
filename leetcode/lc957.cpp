/**
 * Note the boundary condition.
 *
 */
class Solution {
  void update(vector<int>& cells) {
    vector<int> prev = cells;
    for (int i = 1; i < cells.size() - 1; i++) {
      if (prev[i - 1] == prev[i + 1]) {
        cells[i] = 1;
      } else {
        cells[i] = 0;
      }
    }
    cells.front() = 0;
    cells.back() = 0;
  }

 public:
  vector<int> prisonAfterNDays(vector<int>& cells, int N) {
    int counter = 0;
    set<vector<int>> seen;
    vector<vector<int>> seq;
    int cycle;
    while (true) {
      update(cells);
      counter++;
      seq.push_back(cells);

      if (counter == N) {
        return cells;
      }
      if (seen.find(cells) != seen.end()) {
        cycle = counter - 1;
        break;
      }
      seen.insert(cells);
    }
    return seq[(N - 1) % cycle];
  }
};