/*
 * Copyright (c) 2020, Olivier Valentin
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "globals.h"
#include "interpreter.h"
#include "basic_io.h"

static char heap[2 * 1024];

static int extra_builtin_basic_io(char **pos) {
	if (eat("[INC]", pos)) {
		stack[stack_ptr++] = basic_io_inc();
//	} else if (eat("[OUTC]", pos)) {
//		basic_io_outc(stack[--stack_ptr]);
	} else
		return 0;
	return !0;
}

#include <sys/mman.h>
#include <unistd.h>
void fix_heap() {
	long ps = sysconf(_SC_PAGE_SIZE);
	unsigned long mask = ~(ps - 1);
	mprotect((void*)((unsigned long)heap & mask), (sizeof(heap) & mask) + ps, PROT_READ | PROT_WRITE | PROT_EXEC);
}

int main(void) {
	fix_heap();
	extra_builtin = &extra_builtin_basic_io;
	*scope_find("{HEAP}" + 1, 0) = (unsigned long)heap;
	execute_at(program);
}
