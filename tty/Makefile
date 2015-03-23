
ARMGNU = arm-none-eabi

COPTS = -Wall -g -O2 -std=gnu11 -nostdlib -nostartfiles -ffreestanding -mcpu=cortex-a7
ASOPTS = -g -mcpu=cortex-a7

CC = $(ARMGNU)-gcc $(COPTS)
AS = $(ARMGNU)-as $(ASOPTS)

CSOURCES = $(wildcard *.c)
SSOURCES = $(wildcard *.s)
COBJS = $(CSOURCES:.c=.o)
SOBJS = $(SSOURCES:.s=.o)
OBJS = $(COBJS) $(SOBJS)

all : prog.bin

clean :
	rm -f *.o
	rm -f *.bin
	rm -f *.hex
	rm -f *.elf
	rm -f *.list
	rm -f *.img


kernel.img : loader $(OBJS)
	$(ARMGNU)-ld $(OBJS) -T loader -o prog.elf
	$(ARMGNU)-objdump -D prog.elf > prog.list
	$(ARMGNU)-objcopy prog.elf -O ihex prog.hex
	$(ARMGNU)-objcopy prog.elf -O binary kernel.img

prog.bin: kernel.img
	cp kernel.img prog.bin

install : gcc
	rm -f /Volumes/RPI/kernel.img
	cp -v kernel.img /Volumes/RPI/kernel.img
	diskutil eject RPI

rebuild : clean all

redo : clean install
