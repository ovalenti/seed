/*
 * Copyright (c) 2020, Olivier Valentin
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include <stdio.h>

void basic_io_outc(char c) {
	putc(c, stdout);
}

char basic_io_inc() {
	return getc(stdin);
}
