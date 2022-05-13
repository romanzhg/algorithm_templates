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

/************************************************************
	Generate permutations, on the fly version.
************************************************************/
// result is an array of size n, which stores the current permutation of the n
// elements(0, 1, 2, ... (n - 1))
// At each step of generating the result(tracked by cur), try all elements while
// filter against the already generated partial permutation. For a valid pick, go next.
void printPermutations(int n, int* result, int cur) {
	if (cur == n) {
		for (int i = 0; i < n; i++) {
			printf("%d ", result[i]);
		}
		printf("\n");
		return;
	}
	for (int i = 0; i < n; i++) {
		bool isValid = true;
		for (int j = 0; j < cur; j++) {
			if (result[j] == i) {
				isValid = false;
				break;
			}
		}
		if (isValid) {
			result[cur] = i;
			printPermutations(n, result, cur + 1);
		}
	}
}

// Print permutations of the elements stored in the input array "elements".
void printPermutations(int n, int* result, int* elements, int cur) {
	if (cur == n) {
		for (int i = 0; i < n; i++) {
			printf("%d ", result[i]);
		}
		printf("\n");
		return;
	}
	for (int i = 0; i < n; i++) {
		bool isValid = true;
		for (int j = 0; j < cur; j++) {
			if (result[j] == elements[i]) {
				isValid = false;
				break;
			}
		}
		if (isValid) {
			result[cur] = elements[i];
			printPermutations(n, result, elements, cur + 1);
		}
	}
}

// Print unique permutations of elements in the input array "elements" (there may
// be repetition).
// result is an array of size n, which stores the current permutation of the n
// elements.
// elements is an array of size n, containing the input elements.
// The array elements needs to be sorted, so same elements are adjacent to
// each other.
void printPermutations(int n, int* result, int* elements, int cur) {
	if (cur == n) {
		for (int i = 0; i < n; i++) {
			printf("%d ", result[i]);
		}
		printf("\n");
		return;
	}
	// When iterating through all possible elements, for repeated ones, use only
	// the first. When trying to fill in each position in the result, the same element
	// will be revisited.
	for (int i = 0; i < n; i++) {
		if (i == 0 || elements[i] != elements[i-1]) {
			// c1: number of occurance of element i in the already generated partial permutation.
			// c2: number of occurance of element i in the input.
			int c1 = 0, c2 = 0;
			for (int j = 0; j < cur; j++) {
				if (result[j] == elements[i]) {
					c1++;
				}
			}
			for (int j = 0; j < n; j++) {
				if (elements[i] == elements[j]) {
					c2++;
				}
			}
			// Use the element up to c2 times.
			if (c1 < c2) {
				result[cur] = elements[i];
				printPermutations(n, result, elements, cur + 1);
			}
		}
	}
}

// Another way to get permutations is to sort and call next_permutation.
// Call next permutation.
void func() {
  string s = "aba";
  sort(s.begin(), s.end());
  do {
    cout << s << '\n';
  } while(next_permutation(s.begin(), s.end()));
}

/************************************************************
	Generate permutations, on the fly version.
************************************************************/
// 1. Incrementally.
void printSubset(int n, int* result, int cur) {
	for (int i = 0; i < cur; i++) {
		printf("%d ", result[i]);
	}
	printf("\n");

	// Minimum possible value for the current element.
	int s = cur != 0 ? result[cur - 1] + 1 : 0;
	for (int i = s; i < n; i++) {
		result[cur] = i;
		printSubset(n, result, cur + 1);
	}
}

// 2. Bit vector.
void printSubset(int n, bool* result, int cur) {
	if (cur == n) {
		for (int i = 0; i < cur; i++) {
			if (result[i]) {
				printf("%d ", i);
			}
		}
		printf("\n");
		return;
	}
	result[cur] = 1;
	printSubset(n , result, cur + 1);
	result[cur] = 0;
	printSubset(n , result, cur + 1);
}

// 3. Binaries.
void func() {
	for (int i = 0; i < (1 << n); i++) {
		printSubset(n, i);
	}
}

// nis the value range, s is the take/not take bits.
void printSubset(int n, int s) {
	for (int i = 0; i < n; i++) {
		if (s & (1 << i)) {
			printf("%d ", i);
		}
	}
	printf("\n");
}