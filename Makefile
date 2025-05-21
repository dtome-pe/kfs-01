# Tools
AS      = i686-elf-as
CC      = i686-elf-gcc
LD      = linker.ld

# Flags
ASFLAGS = -f elf32
CFLAGS  = -std=gnu99 -ffreestanding -O2 -Wall -Wextra
LDFLAGS = -ffreestanding -O2 -nostdlib

# Files
SOURCES  = boot.s kernel.c
OBJECTS  = boot.o kernel.o
TARGET   = kfs.bin

# Default target
all: $(TARGET)

# Compile assembly file
boot.o: boot.s
	$(AS) $< -o $@

# Compile C file
kernel.o: kernel.c
	$(CC) -c $< -o $@ $(CFLAGS)

# Link objects into kernel binary
$(TARGET): $(OBJECTS)
	$(CC) -T $(LD) -o $@ $(LDFLAGS) $^ -lgcc
	mkdir -p isodir/boot/grub
	cp grub.cfg isodir/boot/grub/grub.cfg
	cp $(TARGET) isodir/boot/$(TARGET)
	grub-mkrescue -o kfs.iso isodir
	rm kernel.o kfs.bin boot.o

# Clean build artifacts
clean:
	rm -f *.o kfs.iso $(TARGET)
	rm -rf isodir
