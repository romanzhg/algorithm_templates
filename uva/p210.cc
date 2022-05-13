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
using namespace std;

#define MAX_INPUT_SIZE 1000

// Inst
// type 0 assign, 1 print, 2 lock, 3 unlock, 4 end
typedef struct
{
	int type;
	int var;
	int value;
} Inst;

typedef struct 
{
	vector<Inst> instructions;
	int pc;
	int id;
} Prog;

char line[MAX_INPUT_SIZE];

void read_line() {
	cin.getline(line, sizeof(line));
}

int get_number(string input) {
  string assign_op = "=";
  int index = input.find_first_of(assign_op);
  string number_str = input.substr(index + 1);
  return stoi(number_str);
}

// Check if target starts with value.
bool starts_with(string target, string value) {
  if (value.size() > target.size()) {
  	return false;
  }
  for (int i = 0; i < value.size(); i++) {
  	if (value[i] != target[i]) {
  		return false;
  	}
  }
  return true;
}

deque<Prog*> ready_queue;
deque<Prog*> blocking_queue;
// How much time each type of instruction costs.
int costs[5];
int quantum;
int vars[26];

void solve() {
	bool lock_is_hold = false;
	while(true) {
		if (ready_queue.empty()) {
			break;
		}
		
		// Take a program out of the ready queue, and run it.
		Prog* current = ready_queue.front();
		ready_queue.pop_front();

		bool push_to_ready_queue = true;

		// Initialize the quantum.
		int time_left = quantum;
		while (time_left > 0) {
			// Run an instruction.
			Inst inst = current->instructions[current->pc];
			time_left -= costs[inst.type];

			if (inst.type == 0) {
				// assignment
				vars[inst.var] = inst.value;
			} else if (inst.type == 1) {
				// print
				printf("%d: %d\n", current->id, vars[inst.var]);
			} else if (inst.type == 2) {
				// lock
				if (lock_is_hold) {
					push_to_ready_queue = false;
					blocking_queue.push_back(current);
					break;
				} else {
					lock_is_hold = true;
				}
			} else if (inst.type == 3) {
				// unlock
				if (!lock_is_hold) {
					// This is an invalid situation.
					exit(-1);
				} else {
					lock_is_hold = false;
					if (blocking_queue.size() != 0) {
						ready_queue.push_front(blocking_queue.front());
						blocking_queue.pop_front();
					}
				}
			} else {
				// inst.type == 4
				// end
				push_to_ready_queue = false;
				break;
			}
			current->pc++;
		}

		if (push_to_ready_queue) {
			ready_queue.push_back(current);
		}
	}
}

int main() {
  int test_cases;
  scanf("%d", &test_cases);
  read_line();
  while(test_cases--) {
  	// Initialize data structures.
  	ready_queue.clear();
  	blocking_queue.clear();
  	for (int i = 0; i < 26; i++) {
  		vars[i] = 0;
  	}

  	int number_of_progs;
  	read_line();
  	scanf("%d %d %d %d %d %d %d", &number_of_progs, &costs[0], &costs[1],
  		&costs[2], &costs[3], &costs[4], &quantum);
  	read_line();
  	for (int i = 0; i < number_of_progs; i++) {
  		Prog* p = new Prog();
  		p->pc = 0;
  		p->id = i + 1;
  		// Build the program.
  		while (true) {
  			read_line();
  			string tmp_line(line);
  			Inst tmp_inst;
  			if (starts_with(tmp_line, "end")) {
  				tmp_inst.type = 4;
  				p->instructions.push_back(tmp_inst);
  				break;
  			} else if (starts_with(tmp_line, "unlock")) {
  				tmp_inst.type = 3;
  				p->instructions.push_back(tmp_inst);
  			} else if (starts_with(tmp_line, "lock")) {
  				tmp_inst.type = 2;
  				p->instructions.push_back(tmp_inst);
  			} else if (starts_with(tmp_line, "print")) {
  				tmp_inst.type = 1;
  				tmp_inst.var = tmp_line.back() - 'a';
  				p->instructions.push_back(tmp_inst);
  			} else {
  				// Assignment
  				tmp_inst.type = 0;
  				tmp_inst.var = tmp_line.front() - 'a';
  				tmp_inst.value = get_number(tmp_line);
  				p->instructions.push_back(tmp_inst);
  			}
  		}
  		ready_queue.push_back(p);
  	}

  	solve();
  	if (test_cases != 0) {
  		printf("\n");
  	}
  }
  return 0;
}