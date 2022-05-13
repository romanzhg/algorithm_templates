Notes

lc1109: Event handling. [1,2,10]: increasing event at 1, decreasing event at 3. Process decreasing events first.

lc1145: count how many nodes on each edge.

lc1143: LCS, dp. dp(i, j) = dp(i - 1, j - 1) + 1 | max(dp(i - 1, j), dp(i, j - 1)). (if there is a match at end, must take.)

lc1239: 带权最大独立集。规模较小可brute force.

lc1229: process discrete events, record end, and see if next start is {duration} away.

*lc1197: A*, worth trying.

*lc1249: use vector to implement a stack, worth trying.

*lc1188: worth trying.

*lc1167: huffman's algorithm, greedy.

lc503: rotate the largest value to the end, keep an increasing stack from back to front.

lc1048: longest path in DAG.

lc1041: simulate the whole process, if the robot returns to the same position(direction can be any), returns true, otherwise returns false.

*lc1074: 2D submatrix with target sum. O(n^3), can be reduce to O(n) 1D sub array with target sum.
1D subarray with target sum: keep a running sum and the number of occurrence of each number.
Can use prefix sum of columns to further speed up.

lc1061: simple union find.

*lc964: iterative deepening.

lc1008: similar to lc889, find boundary and build the tree recursively.

lc1117: simple programming problem, count number of modifications to make a string palindrome.

*lc996: build edges, count permutations, divide by rep!.

*lc980: bfs + steps.

*lc1032: trie.

*lc502: the capital is not consumed, just checked, this is counter intuitive.

lc787: record cheapest price and stops, push a node only when there is an update.

lc780: do not understand.

*lc699: map<int, int>: start from key, the max height is value, until another key is reached.

*lc855: coding problem, may try.

*lc773: coding problem, may try.

*lc316: recursive, greedy.

lc752: bfs.

lc656: shortest path in DAG.

*lc710: rejection sampling(throw away the rand result if it is not qualified). Skip numbers recursively to
reach the real value, may try.

*lc1331: sort and build a map, update the original array with the map.

*lc1335: dp with RangeMinimum(max)Query, build a BIT.

*lc1330: divide and conquer, divide to two sub-arrays. complexity: n^2.

lc934: normal programming problem.

lc1055: for each char in src, build an index set. Then for each char in target, find the first match. If
reaches the end of src, start another round.

*lc1125: search with prune. May try.