Index of problems

Computational geometry

p11178: basic.

p1342: basic.

p11796: basic.

p1308: high precision.

p10652: convex hull.

p11168: convex hull.

p10256: convex hull, intersections.

p1453: convex hull.

p1396: plane, intersections.

p1318: plane.

p1340: plane.

p12296: plane.

p11275: 3d geometry.

p1469: 3d geometry.

p1438: 3d geometry.

p1100: 3d geometry.

p12313: ray tracer.

Brute force/back tracking/branch prune/search

p725:

p11059:

p140: dfs / visit all nodes in the solution tree(permutation of a set) and prune branches.

p1354: dfs / visit all nodes in the solution tree and prune branches.
The solution tree is n - 1 level of (n - "level index") choose 2 combinations.

p10603: Dijkstra, state encoding.

p1601: Optimizations for searching through an implicit graph.

Optimization used:
1. Pre computed moves.
2. State compression.
3. Bidirectional search.
4. Visited check implemented as array check(from 4.92s to 1.21s).

This program allocates 216MB memory, mainly for visitedLeft and visitedRight.

p11212: IDAStar with an admissible heuristic.

p11212_1: iterative deepening based on DFS.

p11212_2: iterative deepening which utilized AStar, but this particular implementation does
not require the heuristic to be admissible.

p1343: AStar search.

p1374: iterative deepening + problem specific optimization.

p1602: generate all the combinations first. Duplication check with set<set<pair<int, int>>>.

p1603: Iterative deepening. State representation is the trick here.

p1603_1: DFS with optimality pruning.

p1603_2: IDAstar, with a straight forward but less effecient state representation.

p1603_3: iterative deepening, with a straight forward but less effecient state representation.

p1603_4: iterative deepening, with the least effecient state representation.

p208: DFS.

p307: N-partition problem.

p11882: search/pruning.

p11694: backtracking, sort for a better filling order.

Dynamic programming

p1025:

p437: 

p1347:

p116:

p1625: Memset a 100MB space takes 0.06 seconds on a 3Hz/8GB Mac. Avoid using
memset often.

p10003:

p10817:

p1218:

p12186:

p1220:

p1252:

p1412:

p1627:

p12099:

p1439: graph coloring.

Misc

p12171:

p122:

p210:

p297:

p442:

p12657:

p514:

p699:

p839:
		
p11988:

Data structures

p1428: BIT.

p11235: Range minimum query.

p1400: segment tree. range maximum sum sequence query.

p11107: Longest common prefix, suffix array.

p11992: interval tree for range set/add and query range min/max/sum.

p1479: treap.

p11922: splay tree.

Graph

p1146: 2-SAT

p1391: 2-SAT

p247: Transitive Closure.

poj_3608: Interval cover(k).

p12661:

p1349:

p1658:

p753:

p1515: