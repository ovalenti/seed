/*
 * Copyright (c) 2020, Olivier Valentin
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include <stdio.h>

#include "globals.h"

char* program =
		"{ 1 [ROT] } {SWAP} [STORE]"
		"{"
		"	[DUP]"
		"	2 [GE]"
		"	{"
		"		[DUP]"
		"		1 [SUB]"
		"		[FIBO]"
		"		[SWAP]"
		"		2 [SUB]"
		"		[FIBO]"
		"		[ADD]"
		"	}"
		"	[SWAP] ?"
		"} {FIBO} [STORE]"

		"4 [FIBO]"
		"[PRINT]"

		"5 [FIBO]"
		"[PRINT]"

		"10 [FIBO]"
		"[PRINT]"
		"}";

void print(unsigned long v) {
	printf("%lu\n", v);
}

void put(char c) {
	putchar(c);
}
