/*
 * dos_psp.c
 *
 *  Created on: Jan 18, 2021
 *      Author: ovalentin
 */

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;

struct __attribute__((__packed__)) psp {
	uint16_t exit_jump;
	uint16_t next_seg;
	uint8_t reserved;
	union {
		uint8_t bin[5];
	} dos_far_call;
} psp = { .exit_jump = 42 }; // force BSS incarnation

