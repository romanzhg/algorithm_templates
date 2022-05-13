#include <cstdio>
#include <cstdlib>
#include <climits>
#include <iostream>
#include <queue>
#include <deque>
#include <vector>
#include <utility>
#include <algorithm>
#include <functional>
#include <cstring>
#include <string>
#include <map>
#include <set>
#include <stack>
using namespace std;

#define MAX_INPUT_SIZE 1010

int expected[MAX_INPUT_SIZE];

void solve(int number_of_blocks) {
  stack<int> station;
  int counter = 1;

  for (int i = 0; i < number_of_blocks; i++) {
  	int tmp = expected[i];
  	if (!station.empty() && station.top() == tmp) {
  		station.pop();
  		continue;
  	}

  	bool matched = false;
  	while (counter <= number_of_blocks) {
  		if (counter == tmp) {
  			counter++;
  			matched = true;
  			break;
  		} else {
  			station.push(counter);
  			counter++;
  		}
  	}
  	if (matched == false) {
  		printf("No\n");
  		return;
  	}
  }
  printf("Yes\n");
}

int main() {
  int number_of_blocks;
  while(true) {
  	scanf("%d", &number_of_blocks);
  	if (number_of_blocks == 0) {
  		// The last input.
  		break;
  	}
  	while (true) {
	  	int tmp;
	  	scanf("%d", &tmp);
	  	if (tmp == 0) {
	  		// end of a block.
	  		break;
	  	} else {
	  		expected[0] = tmp;
	  	}
	  	for (int i = 1; i < number_of_blocks; i++) {
	  		scanf("%d", &expected[i]);
	  	}

	  	solve(number_of_blocks);
  	}
  	// After one block, there should be an empty line.
  	printf("\n");
  }
  return 0;
}