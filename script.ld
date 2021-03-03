SECTIONS
{
	.s1 0x0000 : { dos_psp.o(.data) }
	.s2 0x0100 : { *(.start) }
	.prg : { *(*) }
}