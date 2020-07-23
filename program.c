/*
 * Copyright (c) 2020, Olivier Valentin
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "globals.h"

char* program =
#include "base.seed"
#include "basic_io.seed"
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
		"[PRINTINT] [NL]"

		"5 [FIBO]"
		"[PRINTINT] [NL]"

		"10 [FIBO]"
		"[PRINTINT] [NL]"
		"}";
