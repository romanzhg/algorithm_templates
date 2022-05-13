#include <bitset>
#include <cstdio>
#include <cstdlib>
#include <climits>
#include <cmath>
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
#include <stack>
#include <list>
using namespace std;

// n in [0, 31]
uint32_t getRightMostBits(uint32_t value, int32_t n) {
	uint32_t v = value & ((1 << n) - 1);
	return v;
}

// n in [0, 31]
uint32_t setBit(uint32_t value, int32_t n) {
	return value | (1U << n);
}

// n in [0, 31]
uint32_t clearBit(uint32_t value, int32_t n) {
	return value & (~(1U << n));
}

// n in [0, 31]
uint32_t xorBit(uint32_t value, int32_t n) {
	return value ^ (1U << n);
}

// n in [0, 31]
bool isBitSet(uint32_t value, uint32_t n) {
	return value & (1U << n);
}

// Get bytes of an integer.
union IntContainer {
    uint32_t value;
    unsigned char bytes[4];
};

// Another way to get bytes from integer.
// This function works for both int and unsigned int, because
// int/unsigned int assignment keeps the bits same.
//
// In "int32_t value" value is a variable here.
// Variable is a programming language concept, it doesn't exist
// in assembly.
// Variable refers to a block of memory, so &value here makes sense.
// The memory refered by a variable is always on stack.
//
// Variable reference in programming language is changed to memory
// reference by the compiler.
void intToBytes(int32_t value, unsigned char* bytes) {
	memcpy(bytes, (void*) &value, 4);
}

// Assumes the content of an integer is copied to bytes.
// Assumes the integer is in little endian.
// So we print32_t the most significant byte(the last byte) first.
//
// Little-endian: the sequence addresses/sends/stores the least
// significant byte first (lowest address) and the most significant
// byte last (highest address).
// Big-endian/Little-endian is machine dependent, it is not related to
// programming language.
void printIntLittleEndian(unsigned char* bytes) {
	for (int32_t i = 3; i >= 0; i--) {
		printf("%02X ", bytes[i]);
	}
	printf("\n");
}

void printIntInBits(int v) {
	printf("%s\n", (bitset<32>(v)).to_string().c_str());
}


int main() {
	unsigned char bytes[4];

	IntContainer ic;

	printf("1\n");
	ic.value = 0x1234;
	printIntLittleEndian(ic.bytes);
	printf("%d\n", ic.value);

	printf("2\n");
	ic.value = 0xF0001234;
	printIntLittleEndian(ic.bytes);
	printf("%d\n", ic.value);
	
	printf("3\n");
	intToBytes(ic.value, bytes);
	printIntLittleEndian(bytes);

	printf("4\n");
	ic.value = 2147483647;
	printIntLittleEndian(ic.bytes);
	printf("%d\n", ic.value);

	printf("5\n");
	ic.value += 1;
	printIntLittleEndian(ic.bytes);
	printf("%d\n", ic.value);

	// This block would result in the long to int
	// implicit conversion warning.
	// 
	// ic.value = 2147483648;
	// printIntLittleEndian(ic.bytes);
	// printf("%d\n", ic.value);

	printf("6\n");
	ic.value = 2147483649;
	printIntLittleEndian(ic.bytes);
	printf("%u\n", ic.value);

	printf("7\n");
	ic.value = UINT_MAX;
	printIntLittleEndian(ic.bytes);
	printf("%u\n", ic.value);

	printf("8\n");
	ic.value += 1;
	printIntLittleEndian(ic.bytes);
	printf("%u\n", ic.value);

	// Demo unsigned int to int.
	printf("9\n");
	ic.value = 0xFFFFFFFF;
	intToBytes(ic.value, bytes);
	printIntLittleEndian(bytes);

	printf("10\n");
	printIntBits(1);
	return 0;
}