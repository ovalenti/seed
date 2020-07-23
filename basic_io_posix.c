/*
 * Copyright (c) 2020, Olivier Valentin
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include <stdio.h>
#include <termios.h>

void basic_io_outc(char c) {
	putc(c, stdout);
}

char basic_io_inc() {
	static int init = 0;
	if (!init) {
		struct termios termios = { 0 };
		cfmakeraw(&termios);
		tcsetattr(fileno(stdin), TCSANOW, &termios);
		init = 1;
	}
	return getc(stdin);
}
