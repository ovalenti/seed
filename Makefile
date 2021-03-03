CFLAGS=-m16 -nostdinc -mgeneral-regs-only -Wstrict-prototypes

OBJS=dos_to_c.o dos_psp.o basic_io_dos.o interpreter.o main.o program.o

INCLUDES=$(wildcard *.h)

all:seed.com

${OBJS}: ${INCLUDES}

dos_to_c.o: dos_to_c.S
	as --32 -o dos_to_c.o dos_to_c.S

seed.com:seed.elf
	objcopy -O binary seed.elf .seed.com
	dd if=.seed.com of=seed.com skip=1 bs=256
	rm -f .seed.com

seed.elf:${OBJS} script.ld
	ld -melf_i386 -T script.ld -o seed.elf ${OBJS}

clean:
	rm -f *.o *.elf *.com *.bin

.PHONY:all clean