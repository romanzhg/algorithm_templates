#include <cstdio>
#include <cstdlib>
#include <climits>
#include <queue>
#include <vector>
#include <utility>
#include <algorithm>
#include <functional>
#include <cstring>
#include <string>
#include <map>
#include <set>
using namespace std;

// Notes: for this problem, both DFS and BFS works.
// But since the current implementation of DFS put states in stack, it fails
// some test cases for stack overflow/Segmentation fault.
// BFS put states in heap, which doesn't have this problem.

// This version is shorter since it implements discretization in a different way.
// Basically just sort the vector and call unique.
// The index can be found by calling lower_bound.
#define MAX_N 128
#define MAX_BOXES 55
#define MAX_RANGE 1010

// Directions.
vector<int> dx = {1,-1,0,0,0,0};
vector<int> dy = {0,0,1,-1,0,0};
vector<int> dz = {0,0,0,0,1,-1};

vector<int> x_points;
vector<int> y_points;
vector<int> z_points;
bool occupied[MAX_N][MAX_N][MAX_N];
bool visited[MAX_N][MAX_N][MAX_N];

int input_data[MAX_BOXES][6];

// Data structure for solve.
int total_vol;
int total_area;

bool outOfRange(int x, int y, int z) {
  if (x < 0 || x >= (x_points.size() - 1)) {
    return true;
  }
  if (y < 0 || y >= (y_points.size() - 1)) {
    return true;
  }
  if (z < 0 || z >= (z_points.size() - 1)) {
    return true;
  }
  return false;
}

// Returns if an adj cell is air, returns false for out of bound and occupied.
bool isAir(int x, int y, int z) {
  if (outOfRange(x, y, z)) {
    return false;
  }
  if (occupied[x][y][z]) {
    return false;
  }
  return true;
}

// Flood fill the whole vol.
void dfs(int x, int y, int z) {
  if (outOfRange(x, y, z)) {
    return;
  }
  if (visited[x][y][z]) {
    return;
  }
  if (occupied[x][y][z]) {
    return;
  }
  visited[x][y][z] = true;

  int x_length = x_points[x + 1] - x_points[x];
  int y_length = y_points[y + 1] - y_points[y];
  int z_length = z_points[z + 1] - z_points[z];

  // Add the vol of this cell.
  total_vol += x_length * y_length * z_length;

  // Add the surface area of this cell.
  for (int i = 0; i < 6; i++) {
    if (!isAir(x + dx[i], y + dy[i], z + dz[i])) {
      if (dx[i] != 0) {
        total_area += y_length * z_length;
      } else if (dy[i] != 0) {
        total_area += x_length * z_length;
      } else {
        total_area += x_length * y_length;
      }
    }
  }

  // Expend to the 6 directions.
  for (int i = 0; i < 6; i++) {
    dfs(x + dx[i], y + dy[i], z + dz[i]);
  }
}

int getIndex(vector<int>& array, int value) {
  return distance(array.begin(),
    lower_bound(array.begin(), array.end(), value));
}

void discretize(vector<int>& input) {
  sort(input.begin(), input.end());
  unique(input.begin(), input.end());
}

int main() {
  int test_cases;
  scanf("%d", &test_cases);
  for (int i = 0; i < test_cases; i++) {
    // initialize data structures
    x_points.clear();
    y_points.clear();
    z_points.clear();

    memset(occupied, 0, sizeof(occupied));
    memset(visited, 0, sizeof(visited));

    total_vol = 0;
    total_area = 0;

    int boxes;
    scanf("%d", &boxes);

    x_points.push_back(0);
    x_points.push_back(MAX_RANGE);

    y_points.push_back(0);
    y_points.push_back(MAX_RANGE);

    z_points.push_back(0);
    z_points.push_back(MAX_RANGE);


    for (int j = 0; j < boxes; j++) {
      int x0, y0, z0, x, y, z;
      scanf("%d %d %d %d %d %d", &x0, &y0, &z0, &x, &y, &z);
      x_points.push_back(x0);
      x_points.push_back(x0 + x);

      y_points.push_back(y0);
      y_points.push_back(y0 + y);

      z_points.push_back(z0);
      z_points.push_back(z0 + z);

      input_data[j][0] = x0;
      input_data[j][1] = y0;
      input_data[j][2] = z0;
      input_data[j][3] = x;
      input_data[j][4] = y;
      input_data[j][5] = z;
    }

    discretize(x_points);
    discretize(y_points);
    discretize(z_points);

    // Mark the occupied matrix.
    for (int j = 0; j < boxes; j++) {
      int x_start = getIndex(x_points, input_data[j][0]);
      int x_end = getIndex(x_points, input_data[j][0] + input_data[j][3]);
      int y_start = getIndex(y_points, input_data[j][1]);
      int y_end = getIndex(y_points, input_data[j][1] + input_data[j][4]);
      int z_start = getIndex(z_points, input_data[j][2]);
      int z_end = getIndex(z_points, input_data[j][2] + input_data[j][5]);

      for (int a = x_start; a < x_end; a++) {
        for (int b = y_start; b < y_end; b++) {
          for (int c = z_start; c < z_end; c++) {
            occupied[a][b][c] = true;
          }
        }
      }
    }

    dfs(0, 0, 0);

    printf("%d %d\n", total_area - MAX_RANGE * MAX_RANGE * 6,
      MAX_RANGE * MAX_RANGE * MAX_RANGE - total_vol);
  }

  return 0;
}