# intermediate executable
EXECUTABLE=jtag_activate.elf
# binary file to write to SD card
BINARY=kernel.img
# source file(s) used in building
SOURCES=main.s

ARMGNU ?= arm-none-eabi

LDFLAGS=-T memmap
OBJECTS=$(SOURCES:.s=.o)
LISTFILE=$(EXECUTABLE:.elf=.list)

all: $(SOURCES) $(EXECUTABLE) $(BINARY) $(LISTFILE)

$(EXECUTABLE): $(OBJECTS)
	$(ARMGNU)-ld $(OBJECTS) $(LDFLAGS) -o $@

# Create raw binary from ELF file produced by linker
$(BINARY): $(EXECUTABLE)
	$(ARMGNU)-objcopy $(EXECUTABLE) -O binary $@

# Create full disassemble listing file from ELF output file.
%.list : %.elf
	$(ARMGNU)-objdump -D $< > $@

# Assemble sourse into object file
.s.o:
	$(ARMGNU)-as $(CFLAGS) $< -o $@

clean :
	-@$(RM) $(wildcard $(OBJECTS) $(EXECUTABLE) $(BINARY) $(LISTFILE))
