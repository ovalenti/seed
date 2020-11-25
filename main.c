/*
 * Copyright (c) 2020, Olivier Valentin
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */
#define _DEFAULT_SOURCE
#define _XOPEN_SOURCE 500
#include <stdlib.h>
#undef _XOPEN_SOURCE
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <termios.h>
#include <string.h>

#include "globals.h"
#include "interpreter.h"

static char heap[2 * 1024];

static int pts_master;

static void init_terminal() {
	int cpid;
	int pts_slave;
	char *slave_path;
	char pts_xterm_argument[64];

	pts_master = open("/dev/ptmx", O_RDWR | O_NOCTTY);
	grantpt(pts_master);
	unlockpt(pts_master);

	slave_path = ptsname(pts_master);

	pts_slave = open(slave_path, O_RDWR | O_NOCTTY);

	snprintf(pts_xterm_argument, sizeof(pts_xterm_argument), "-S%s/%d", slave_path, pts_slave);

	cpid = fork();
	if (cpid == 0) {
		execlp("xterm", "xterm", pts_xterm_argument, NULL);
		exit(0);
	}
}

void io_outc(char c) {
	write(pts_master, &c, 1);
}

char io_inc() {
	static int init = 0;
	char c;
	if (!init) {
//		char buff[8];
		struct termios termios = { 0 };
		cfmakeraw(&termios);
		tcsetattr(pts_master, TCSANOW, &termios);
		// discard initialization stuff
//		read(pts_master, buff, sizeof(buff));
		init = 1;
	}
	read(pts_master, &c, 1);
	return c;
}

static void print_debug_context(const char *location) {
	int stack_idx;
	const char *p;

	for (p = location; *p != ' ' && *p != '}' && *p != ']'; p++)
		write(STDOUT_FILENO, p, 1);
	write(STDOUT_FILENO, p, 1);

	printf(" | ");

	for (stack_idx = stack_ptr; stack_idx; stack_idx--)
		printf(" %lu", stack[stack_idx - 1]);
}

void debug(const char *location, int *step, int *callee_step) {
	if (*step) {
		static char cmd_prev[32] = "n";
		char cmd_curr[32];
		int resume = 0;

		print_debug_context(location);

		do {
			printf("\n> ");
			fgets(cmd_curr, sizeof(cmd_curr), stdin);
			if (*cmd_curr == '\n')
				strcpy(cmd_curr, cmd_prev);

			if (cmd_curr[0] == 'n') {
				*callee_step = 0;
				resume = 1;
			}
			if (cmd_curr[0] == 's') {
				*callee_step = 1;
				resume = 1;
			}
			if (cmd_curr[0] == 'c') {
				*callee_step = 0;
				*step = 0;
				resume = 1;
			}

			strcpy(cmd_prev, cmd_curr);
			if (!resume)
				printf("unknown command");
		} while (!resume);
	}
}

static int extra_builtin_basic_io(char **pos) {
	if (eat("[INC]", pos)) {
		stack[stack_ptr++] = io_inc();
	} else if (eat("[OUTC]", pos)) {
		io_outc(stack[--stack_ptr]);
	} else
		return 0;
	return !0;
}

static void print_symbol(char *symbol_name) {
	const char *undef = "undefined: ";
	char c;
	while ((c = *(undef++))) io_outc(c);
	while ((c = *(symbol_name++)) && c != '}' && c != ']') io_outc(c);

	exit(2);
}

int main(void) {
	init_terminal();
	symbol_not_found = &print_symbol;
	extra_builtin = &extra_builtin_basic_io;
	*scope_find("{HEAP}" + 1, 0) = (unsigned long)heap;
	execute_at(program, 0);
}
