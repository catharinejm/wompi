
ARMGNU = arm-none-eabi

COPTS = -Wall -ggdb -O2 -std=gnu11 -nostdlib -nostartfiles -ffreestanding -mcpu=cortex-a7
ASOPTS = -g -mcpu=cortex-a7

TARGETS = prog.bin prog.list prog.elf

CC = $(ARMGNU)-gcc $(COPTS)
AS = $(ARMGNU)-as $(ASOPTS)

CSOURCES = $(wildcard *.c)
SSOURCES = $(wildcard *.s)
COBJS = $(CSOURCES:.c=.o)
SOBJS = $(SSOURCES:.s=.o)
OBJS =  $(SOBJS) $(COBJS)

all : $(TARGETS)

clean :
	rm -f *.o $(TARGETS)


prog.bin : prog.elf
	$(ARMGNU)-objcopy prog.elf -O binary prog.bin

prog.elf: loader $(OBJS)
	$(ARMGNU)-ld $(OBJS) -T loader -o prog.elf

prog.list: prog.elf
	$(ARMGNU)-objdump -D prog.elf > prog.list

install : gcc
	rm -f /Volumes/RPI/kernel.img
	cp -v kernel.img /Volumes/RPI/kernel.img
	diskutil eject RPI

rebuild : clean all

redo : clean install
