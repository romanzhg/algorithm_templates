#include <cfloat>
#include <climits>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <algorithm>
#include <deque>
#include <functional>
#include <iostream>
#include <limits>
#include <list>
#include <map>
#include <queue>
#include <set>
#include <stack>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <utility>

using namespace std;
typedef long long ll;
#define INF 1000000000

#ifndef ONLINE_JUDGE
#endif

struct Edge {
  int cost;
};

struct State {

};

// priority_queue.
// Priority queue with an comparator.
class CompEdge {
 public:
  bool operator() (const Edge& lhs, const Edge& rhs) {
    return lhs.cost > rhs.cost;
  }
};

priority_queue<Edge, vector<Edge>, CompEdge> pq;

// Or a priority queue can work with a type with comparator defined.
// Priority queue by default returns the largest element, to return
// the lowest element(for example, the shortest path), redefine the
// compare operator.
struct Element {
	int dist;
	State s;
	Element(int dist = 0, State s = State()): dist(dist), s(s) {}

	bool operator < (const Element& o) const {
		return dist > o.dist;
	}
};

struct TmpElem {
  int index;
  int attachedSquare;
  TmpElem(int index, int attachedSquare)
    : index(index), attachedSquare(attachedSquare) {};

  bool operator < (const TmpElem& o) const {
    return attachedSquare > o.attachedSquare;
  }
};

void funcVectorSort() {
  vector<TmpElem> ve;
  ve.push_back(TmpElem(1, 3));
  ve.push_back(TmpElem(2, 7));
  ve.push_back(TmpElem(3, 5));
  sort(ve.begin(), ve.end());

  for (int i = 0; i < ve.size(); i++) {
    printf("attachedSquares: %d\n", ve[i].attachedSquare);
  }
  printf("\n");
}

// Timer
void funcTimer() {
  #ifndef ONLINE_JUDGE
  clock_t t;
  t = clock();
  #endif
  #ifndef ONLINE_JUDGE
  t = clock() - t;
  printf ("This program took %f seconds.\n", ((float)t) / CLOCKS_PER_SEC);
  #endif
}

// Value range
// MAX_INT 2,147,483,647

// C++ exception
// The following function will throw an exception.
// If it is not caught, the program terminates.
void funcThrow() {
  map<int, int> demoMap;
  int a = demoMap.at(123);
  printf("%d\n", a);
}

// Define a hash function to use pair with unordered set.
struct PairHash {
  std::size_t operator()(const std::pair<int, int> &x) const
  {
    return std::hash<int>()(x.first) ^ std::hash<int>()(x.second);
  }
};

void funcPairHash() {
  unordered_set<pair<int, int>, PairHash> demoSet;
  demoSet.insert(make_pair(1, 2));
  demoSet.insert(make_pair(3, 2));
  demoSet.insert(make_pair(1, 2));
  for (const pair<int, int>& elem : demoSet) {
    printf("%d %d\n", elem.first, elem.second);
  }
}

// A structure demos string comparison, can be used with priority queue.
// The default comparison is lexicographical.
// With this comparison, shorter string is considered smaller.
struct StringStruct {
  string str;

  bool operator < (const StringStruct& o) const {
    if (str.size() == o.str.size()) {
      return str > o.str;
    } else {
      return str.size() > o.str.size();
    }
  };
};

// This code is not valid, element in a set/unordered_set cannot be modified.
void updateSet() {
  set<int> tmpSet = {1, 3, 5};
  auto it = tmpSet.find(3);
  // *it = 7;

  set<int> tmpSet1 = {1, 3, 5};
  auto it1 = tmpSet1.find(3);
  // *it1 = 7;
}

// Sample constructor functions, the following two cases are same.
struct Example {
  int a, b;
  Example(int mA, int mB) : a{mA}, b{mB} { }
  Example(const Example& mE)            = default;
  Example(Example&& mE)                 = default;
  Example& operator=(const Example& mE) = default;
  Example& operator=(Example&& mE)      = default;
}

//struct Example {
//  int a, b;
//  Example(int mA, int mB) : a{mA}, b{mB}               { }
//  Example(const Example& mE) : a{mE.a}, b{mE.b}        { }
//  Example(Example&& mE) : a{move(mE.a)}, b{move(mE.b)} { }
//  Example& operator=(const Example& mE) { a = mE.a; b = mE.b; return *this; }
//  Example& operator=(Example&& mE)      { a = move(mE.a); b = move(mE.b); return *this; }
//}


int main() {
  // The following line with throw
  // libc++abi.dylib: terminating with uncaught exception of type std::out_of_range: map::at:  key not found
  // Abort trap: 6
  // map<int, int> demoMap;
  // int a = demoMap.at(123);
  // printf("%d\n", a);

  funcVectorSort();

  return 0;
}
