/*
 * Copyright (c) 2020, Olivier Valentin
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "interpreter.h"

#include "globals.h"

unsigned long stack[256];
unsigned long stack_ptr = 0;

int (*extra_builtin)(char** loc) = 0;
void (*symbol_not_found)(char *symbol_name) = 0;

#define SCOPE_LEN 256
static struct scope {
	unsigned long name;
	unsigned long value;
} scope[SCOPE_LEN] = { 0 };

static int scope_used = 0;

static int str_match(char *v1, char *v2) {
	while (*v1 == *v2 && *v1 != '}' && *v1 != ']') {
		v1++; v2++;
	}
	return (*v1 == '}' || *v1 == ']') && (*v2 == '}' || *v2 == ']');
}

unsigned long* scope_find(char* name, int debug_value_expected) {
	struct scope *p = scope, *end = scope + scope_used;

	while (p < end) {
		if (str_match(name, (char*)p->name))
			break;
		p++;
	}
	if (p == end && debug_value_expected && symbol_not_found)
		symbol_not_found(name);
	if (p == end) {
		scope_used++;
		p->name = (unsigned long)name;
	}
	return &p->value;
}

int eat(const char *word, char **location) {
	char *ptr = *location;
	while (*word == *ptr) {
		word++;
		ptr++;
		if (*word == '\0') {
			*location = ptr;
			return 1;
		}
	}
	return 0;
}

static void until(const char character, char **location) {
	while (**location != character) (*location)++;
}

static void skip(char **location) {
	while (!0) {
		switch (**location) {
		case ' ':
		case '\t':
		case '\n':
		case '\r':
			(*location)++;
			break;
		default:
			return;
		}
	};
}

static int parse_digit(char c) {
	if (c >= '0' && c <= '9')
		return c - '0';
	if (c >= 'A' && c <= 'F')
		return c - 'A' + 10;
	if (c >= 'a' && c <= 'f')
		return c - 'a' + 10;
	return -1;
}

static void push_value(char **location) {
	if (**location == '{') {
		int depth = 1;
		(*location)++;
		stack[stack_ptr++] = (unsigned long)(*location);
		while (depth) {
			if (**location == '}') depth--;
			if (**location == '{') depth++;
			(*location)++;
		}
	} else {
		int base = 10;
		unsigned long value = 0;
		int digit;

		if (eat("0x", location))
			base = 16;

		while ((digit = parse_digit(**location)) >= 0) {
			value = value * base;
			value += digit;
			(*location)++;
		}

		stack[stack_ptr++] = value;
	}
}

static void swap(unsigned long *a, unsigned long *b) {
	unsigned long v = *a;
	*a = *b;
	*b = v;
}

#define BIN_ARITH(op) \
		unsigned long v = stack[--stack_ptr]; \
		stack[stack_ptr - 1] = stack[stack_ptr - 1] op v;

void execute_at(char *loc) {
	while (1) {
		skip(&loc);

		if (eat("[DROP]", &loc)) {
			stack_ptr--;
		} else if (eat("[DUP]", &loc)) {
			stack[stack_ptr] = stack[stack_ptr - 1];
			stack_ptr++;
		} else if (eat("[ROT]", &loc)) {
			int r = stack[--stack_ptr]; // pop
			unsigned long *p1 = &stack[stack_ptr - r], *p2 = p1 - 1;
			while (r--)
				swap(p1++, p2++);
		} else if (eat("[PEEK]", &loc)) {
			unsigned char* addr = (unsigned char*)stack[stack_ptr - 1];
			stack[stack_ptr - 1] = *addr;
		} else if (eat("[POKE]", &loc)) {
			unsigned char* addr = (unsigned char*)stack[--stack_ptr];
			unsigned long v = stack[--stack_ptr];
			*addr = v;
		} else if (eat("[ADD]", &loc)) { BIN_ARITH(+)
		} else if (eat("[SUB]", &loc)) { BIN_ARITH(-)
		} else if (eat("[MUL]", &loc)) { BIN_ARITH(*)
		} else if (eat("[DIV]", &loc)) { BIN_ARITH(/)
		} else if (eat("[MOD]", &loc)) { BIN_ARITH(%)
		} else if (eat("[SHIFT]", &loc)) { BIN_ARITH(<<)
		} else if (eat("[AND]", &loc)) { BIN_ARITH(&)
		} else if (eat("[OR]", &loc)) { BIN_ARITH(|)
		} else if (eat("[XOR]", &loc)) { BIN_ARITH(^)
		} else if (eat("[GE]", &loc)) { BIN_ARITH(>=)
		} else if (eat("[BNOT]", &loc)) {
			stack[stack_ptr - 1] = ~stack[stack_ptr - 1];
		} else if (eat("!", &loc)) {
			execute_at((char *)stack[--stack_ptr]);
		} else if (eat("??", &loc)) {
			unsigned long loop = stack[--stack_ptr];
			do {
				execute_at((char*)loop);
			} while (stack[--stack_ptr]);
		} else if (eat("?", &loc)) {
			unsigned long cond = stack[--stack_ptr];
			unsigned long jump = stack[--stack_ptr];
			if (cond)
				execute_at((char *)jump);
		} else if (eat("}", &loc)) {
			return;
		} else if (eat("#", &loc)) {
			until('\n', &loc);
		} else if (eat("*", &loc)) {
			stack[stack_ptr - 1] = *scope_find((char*)stack[stack_ptr - 1], 1);
		} else if (eat("[STORE]", &loc)) {
			unsigned long name = stack[--stack_ptr];
			*scope_find((char*)name, 0) = stack[--stack_ptr];
		} else if (extra_builtin && extra_builtin(&loc)) {
			// nothing
		} else if (*loc == '[') {
			execute_at((char *)*scope_find(loc + 1, 1));
			until(']', &loc);
			loc++;
		} else {
			push_value(&loc);
		}
	}
}
