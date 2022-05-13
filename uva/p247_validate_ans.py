#!/usr/bin/python3

import os

class Answer(object):
    def __init__(self, lines):
        self.answer_sets = list()
        for line in lines:
            line = line.replace(',', '')
            words = line.split(' ')
            self.answer_sets.append(set(words))

    def is_same(self, o_answer):
        if len(self.answer_sets) != len(o_answer.answer_sets):
            return False
        for elem in self.answer_sets:
            found_match = False
            for o_elem in o_answer.answer_sets:
                if elem == o_elem:
                    found_match = True
            if not found_match:
                return False

        return True

    def print_self(self):
        print("word sets:")
        for w_set in self.answer_sets:
            print(w_set)


def main():
    input_file_correct = open("correct_ans", "r")
    input_file_to_check = open("to_check_ans", "r")

    tmp_lines = list()

    correct_ans = list()
    to_check_ans = list()

    tmp_lines.clear()
    for line in input_file_correct.readlines():
        line = line.strip()
        if len(line) == 0 or line.startswith('#'):
            continue
        if line.startswith("Calling circles for data set"):
            correct_ans.append(Answer(tmp_lines))
            tmp_lines.clear()
        else:
            tmp_lines.append(line)

    tmp_lines.clear()
    for line in input_file_to_check.readlines():
        line = line.strip()
        if len(line) == 0 or line.startswith('#'):
            continue

        if line.startswith("Calling circles for data set"):
            to_check_ans.append(Answer(tmp_lines))
            tmp_lines.clear()
        else:
            tmp_lines.append(line)

    assert(len(correct_ans) == len(to_check_ans))
    for index in range(len(correct_ans)):
        if not correct_ans[index].is_same(to_check_ans[index]):
            print("correct ans: ")
            correct_ans[index].print_self()
            print("to check ans: ")
            to_check_ans[index].print_self()
            return
    print("Is valid")

if __name__ == "__main__":
    main()
