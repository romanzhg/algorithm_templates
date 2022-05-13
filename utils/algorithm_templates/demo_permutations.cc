#include <cfloat>
#include <climits>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
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
#include <utility>

using namespace std;

/************************************************************
	Generate combinations. A common way to use this is when there is
	an array of n elements elemArray, we generate the combinations of
	k elements, and iterate through it.
	void func() {
		vector<vector<int>> combinations;
		genCombinations(combinations, elemArray.size(), k);
		for (const vector<int>& combination : combinations) {
			for (int i = 0; i < k; i++) {
				use(elemArray[combination[i]]);
			}
		}
	}

	When k is small, generating the combinations may be accomplished by
	having a k level nested for loop.
************************************************************/
void genCombinationsHelper(
	vector<vector<int>>& combinations,
	vector<int>& tmpResult, int n, int k, int c) {
	if (tmpResult.size() == k) {
		combinations.push_back(tmpResult);
		return;
	}
	if (c >= n) {
		return;
	}
	tmpResult.push_back(c);
	genCombinationsHelper(combinations, tmpResult, n, k, c + 1);

	tmpResult.pop_back();
	genCombinationsHelper(combinations, tmpResult, n, k, c + 1);
}

// Chooses k elements from n elements(0, 1, 2, 3, 4...(n - 1)), returns the combinations.
void genCombinations(vector<vector<int>>& combinations, int n, int k) {
	vector<int> tmpResult;
	genCombinationsHelper(combinations, tmpResult, n, k, 0);
}

/************************************************************
	Generate permutations.
	With this way of generating permutations, it is hard to have
	produce unique permutations on a set with repeatations.
	Basically for each repeated set, we need to record their
	position in generated permuation(need a way to encode number sequence),
	put it in a hash set and dedup.
************************************************************/
void genPermutationsHelper(vector<vector<int>>& permutations,
	vector<int>& tmpResult, vector<bool>& used) {
	if (tmpResult.size() == used.size()) {
		permutations.push_back(tmpResult);
		return;
	}

  for (int i = 0; i < used.size(); i++) {
  	if (used[i] == false) {
  		used[i] = true;
  		tmpResult.push_back(i);
  		genPermutationsHelper(permutations, tmpResult, used);
  		used[i] = false;
  		tmpResult.pop_back();
  	}
  }
}

// Returns the permutations of n elements.
void genPermutations(vector<vector<int>>& permutations, int n) {
  vector<bool> used(n, false);
  vector<int> tmpResult;
  genPermutationsHelper(permutations, tmpResult, used);
}

/************************************************************
	Permutations of n choose k.
************************************************************/

// Permutations of n (elements 0, 1, 2, 3, 4...(n - 1)) choose k.
void genPermutationsK(vector<vector<int>>& rtn, int n, int k) {
  vector<vector<int>> combinations;
  genCombinations(combinations, n, k);

  vector<vector<int>> permutations;
  genPermutations(permutations, k);

  for (const vector<int>& combination : combinations) {
  	for (const vector<int>& permutation : permutations) {
  		vector<int> tmp(k);
  		for (int i = 0; i < k; i++) {
  			tmp[i] = combination[permutation[i]];
  		}
  		rtn.push_back(tmp);
  	}
  }
}

int main() {
	vector<vector<int>> combinations;
	genCombinations(combinations, 6, 3);
	printf("the combinations:\n");
	for (const vector<int>& combination : combinations) {
		for (int v : combination) {
			printf("%d ", v);
		}
		printf("\n");
	}

	printf("\nthe permutations:\n");
	vector<vector<int>> permutations;
	genPermutationsK(permutations, 6, 3);
	for (const vector<int>& permutation : permutations) {
		for (int v : permutation) {
			printf("%d ", v);
		}
		printf("\n");
	}

	return 0;
}