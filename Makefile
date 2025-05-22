# Tools
AS      = i686-elf-as
CC      = i686-elf-gcc
LD      = linker.ld

CFLAGS  = -std=gnu99 -ffreestanding -fno-builtin -fno-stack-protector -nostdlib -nodefaultlibs -O2 -Wall -Wextra -Iinc
LDFLAGS = -ffreestanding -O2 -nostdlib 

# Sources
SRC_DIR = src
OBJ_DIR = obj

C_SOURCES = $(wildcard $(SRC_DIR)/*.c)
OBJECTS  = $(OBJ_DIR)/boot.o $(C_SOURCES:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
TARGET   = kfs.bin

# Default target
all: $(TARGET)

# Create obj directory if not present
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# Compile assembly file
$(OBJ_DIR)/boot.o: boot.s | $(OBJ_DIR)
	$(AS) $< -o $@

# Compile C files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) -c $< -o $@ $(CFLAGS)

# Link objects into kernel binary
$(TARGET): $(OBJECTS)
	$(CC) -T $(LD) -o $@ $(LDFLAGS) $^ -lgcc -Iinc
	mkdir -p isodir/boot/grub
	cp grub.cfg isodir/boot/grub/grub.cfg
	cp $(TARGET) isodir/boot/$(TARGET)
	grub-mkrescue -o kfs.iso isodir

# Clean build artifacts
clean:
	rm -rf $(OBJ_DIR) *.o kfs.iso $(TARGET) isodir