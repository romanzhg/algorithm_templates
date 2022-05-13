#!/usr/bin/python

# Run by python p12296_data_gen.py

import os

str_fromat = ""
counter = 0
limit = 20

# Shared point belongs the first line.
def gen_lines_vec(l, w) :
	side_points = [[] for _ in range(4)]
	for tmp in range(0, l):
		side_points[0].append((tmp, 0))
	for tmp in range(1, l + 1):
		side_points[2].append((tmp, w))
	for tmp in range(0, w):
		side_points[1].append((l, tmp))
	for tmp in range(1, w + 1):
		side_points[3].append((0, tmp))
	rtn = []

	for index0 in range(0, 4):
		for index1 in range(index0 + 1, 4):
			for point1 in side_points[index0]:
				for point2 in side_points[index1]:
					rtn.append((point1, point2))
	return rtn

def run_test_case(test_lines_vec):
	global str_fromat
	global counter
	str_tmp = ""
	for point_pair in test_lines_vec:
		str_tmp += "{} {} {} {}\n".format(
			point_pair[0][0],
			point_pair[0][1],
			point_pair[1][0],
			point_pair[1][1])
	counter = counter + 1
	print(counter)
	final_input = str_fromat.format(str_tmp)
	print(final_input)
	return

def gen_test_cases_and_run_internal(lines_vec, tmp_result, current_index, n):
	if len(tmp_result) == n:
		run_test_case(tmp_result)
		return

	if current_index >= len(lines_vec):
		return

	if counter > limit:
		return

	# Take the current element.
	tmp_result.append(lines_vec[current_index])
	gen_test_cases_and_run_internal(lines_vec, tmp_result, current_index + 1, n)

	# Do not take the current element.
	tmp_result.pop()
	gen_test_cases_and_run_internal(lines_vec, tmp_result, current_index + 1, n)
	
	return

def gen_test_cases_and_run(l, w, n):
	global str_fromat
	str_fromat = ""
	str_fromat += str(n) + " " + str(1) + " " + str(l) + " " + str(w) + os.linesep
	str_fromat += "{}"
	str_fromat += "0 0 100" + os.linesep
	str_fromat += "0 0 0 0" + os.linesep

	lines_vec = gen_lines_vec(l, w);
	tmp_result = []

	# Generate the combinations of n elements from len(lines_vec)elements.
	gen_test_cases_and_run_internal(lines_vec, tmp_result, 0, n)
	return

def main():
	global counter
	# for l in range(1, 10):
	# 	for w in range(1, 10):
	# 		for n in range(1, 2):
	# 			gen_test_cases_and_run(l, w, n)
	counter = 0
	gen_test_cases_and_run(2, 2, 20)
	return

if __name__ == "__main__":
	main()
