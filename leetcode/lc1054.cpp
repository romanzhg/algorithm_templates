/**
 * Solution and Solution1 differs a little in the implementation.
 */
#include "common.h"

struct Elem {
  int v, c;
  Elem(int v, int c) : v(v), c(c) {};

  bool operator<(const Elem& o) const {
    return c < o.c;
  }
};

class Solution1 {
 public:
  vector<int> rearrangeBarcodes(vector<int>& barcodes) {
    map<int, int> code_to_count;
    for (int v : barcodes) {
      code_to_count[v]++;
    }
    vector<Elem> elems;
    for (const auto& kv : code_to_count) {
      elems.push_back({kv.first, kv.second});
    }

    make_heap(elems.begin(), elems.end());

    vector<int> rtn;
    int prev = -1;
    while (!elems.empty()) {
      pop_heap(elems.begin(), elems.end());
      Elem e = elems.back();
      elems.pop_back();
      if (e.v != prev) {
        prev = e.v;
        e.c--;
        if (e.c != 0) {
          elems.push_back(e);
          push_heap(elems.begin(), elems.end());
        }
      } else {
        pop_heap(elems.begin(), elems.end());
        Elem e1 = elems.back();
        elems.pop_back();
        prev = e1.v;
        e1.c--;
        if (e1.c != 0) {
          elems.push_back(e1);
          push_heap(elems.begin(), elems.end());
        }
        elems.push_back(e);
        push_heap(elems.begin(), elems.end());
      }
      rtn.push_back(prev);
    }
    return rtn;
  }
};

class Solution {
 public:
  vector<int> rearrangeBarcodes(vector<int>& barcodes) {
    map<int, int> code_to_count;
    for (int v : barcodes) {
      code_to_count[v]++;
    }
    vector<Elem> elems;
    for (const auto& kv : code_to_count) {
      elems.push_back({kv.first, kv.second});
    }
    make_heap(elems.begin(), elems.end());

    vector<int> rtn;
    int prev = -1;
    while (!elems.empty()) {
      pop_heap(elems.begin(), elems.end());
      Elem& e = elems.back();
      if (e.v != prev) {
        prev = e.v;
        e.c--;
      } else {
        Elem tmp = elems.back();
        elems.pop_back();
        pop_heap(elems.begin(), elems.end());
        Elem& e1 = elems.back();
        prev = e1.v;
        e1.c--;
        if (e1.c == 0) {
          elems.pop_back();
        } else {
          push_heap(elems.begin(), elems.end());
        }
        elems.push_back(tmp);
      }
      if (e.c == 0) {
        elems.pop_back();
      } else {
        push_heap(elems.begin(), elems.end());
      }
      rtn.push_back(prev);
    }
    return rtn;
  }
};

void trimLeftTrailingSpaces(string &input) {
  input.erase(input.begin(), find_if(input.begin(), input.end(), [](int ch) {
    return !isspace(ch);
  }));
}

void trimRightTrailingSpaces(string &input) {
  input.erase(find_if(input.rbegin(), input.rend(), [](int ch) {
    return !isspace(ch);
  }).base(), input.end());
}

vector<int> stringToIntegerVector(string input) {
  vector<int> output;
  trimLeftTrailingSpaces(input);
  trimRightTrailingSpaces(input);
  input = input.substr(1, input.length() - 2);
  stringstream ss;
  ss.str(input);
  string item;
  char delim = ',';
  while (getline(ss, item, delim)) {
    output.push_back(stoi(item));
  }
  return output;
}

string integerVectorToString(vector<int> list, int length = -1) {
  if (length == -1) {
    length = list.size();
  }

  if (length == 0) {
    return "[]";
  }

  string result;
  for(int index = 0; index < length; index++) {
    int number = list[index];
    result += to_string(number) + ", ";
  }
  return "[" + result.substr(0, result.length() - 2) + "]";
}

int main() {
  string line;
  while (getline(cin, line)) {
    vector<int> barcodes = stringToIntegerVector(line);

    vector<int> ret = Solution().rearrangeBarcodes(barcodes);

    string out = integerVectorToString(ret);
    cout << out << endl;
  }
  return 0;
}