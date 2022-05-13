// Print the sequence of moves for solving the hanoi tower.
#include <iostream>

using namespace std;

char PosToChar(int pos) {
  if (pos == 0) {
    return 'A';
  } else if (pos == 1) {
    return 'B';
  } else {
    return 'C';
  }
}

void MoveFromTo(int from, int to, int n) {
  if (n == 0) {
    return;
  }

  int middle;
  int tmp = from + to;
  if (tmp == 1) {
    middle = 2;
  } else if (tmp == 2) {
    middle = 1;
  } else {
    middle = 0;
  }
  MoveFromTo(from, middle, n - 1);
  cout << n << ":" << PosToChar(from) << "->" << PosToChar(to) << endl;
  MoveFromTo(middle, to, n - 1);
}

// 0 for A, 1 for B, 2 for C.
void PrintHanoi(int n) {
  MoveFromTo(0, 2, n);
}

int main() {
  PrintHanoi(4);
  return 0;
}