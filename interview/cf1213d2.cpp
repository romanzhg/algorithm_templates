#include <climits>
#include <iostream>
#include <map>
#include <vector>

using namespace std;

int len, k;

int main() {
  scanf("%d %d", &len, &k);
  vector<int> numbers;
  for (int i = 0; i < len; i++) {
    int tmp;
    scanf("%d", &tmp);
    numbers.push_back(tmp);
  }
  int rtn = INT_MAX;
  printf("%d\n", rtn);
  return 0;
}