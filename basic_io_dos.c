/*
 * basic_io_dos.c
 *
 *  Created on: Jan 18, 2021
 *      Author: ovalentin
 */


void basic_io_outc(char c) {
	asm(
		"movb $0x0e, %%ah\n"
		"movb %0, %%al\n"
		"int $0x10\n"
			:: "m" (c) : "ax"
		);
}

char basic_io_inc(void) {
	return 0;
}
