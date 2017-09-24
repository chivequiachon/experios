BIN_NAME=experios
CC=x86_64-elf-gcc
CFLAGS=-nostdlib -nodefaultlibs -nostartfiles -std=gnu99 -ffreestanding -O2 -Wall -Wextra -mno-red-zone
OBJ_DIR=kernel/obj

KERNEL_CC_LIST:=\
kernel/src/video.o \
kernel/src/memory.o \
kernel/src/kernel.o
#kernel/src/kernel.o

KERNEL_AS_LIST:=\
kernel/asm/multiboot_header.o \
kernel/asm/boot.o \
kernel/asm/long_mode_init.o

KERNEL_INCLUDE_DIR=kernel/include

.PHONY: all clean iso run

all: clean kernel iso

clean:
	find -type f \( -name "*.o" -o -name "*.bin" -o -name "*.iso" \) -delete

kernel: $(KERNEL_CC_LIST) $(KERNEL_AS_LIST)
	@ld -m elf_x86_64 -n -o $(BIN_NAME).bin -T linker/linker.ld $(OBJ_DIR)/*

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@ -I$(KERNEL_INCLUDE_DIR)
	mv kernel/src/*.o $(OBJ_DIR)

kernel/%.o: kernel/%.asm
	@nasm -felf64 $< -o $@
	mv kernel/asm/*.o $(OBJ_DIR)

iso: 
	cp $(BIN_NAME).bin iso/boot/$(BIN_NAME).bin
	grub-mkrescue -o $(BIN_NAME).iso iso/
	cp $(BIN_NAME).iso iso/$(BIN_NAME).iso

run:
	@qemu-system-x86_64 -curses -cdrom iso/$(BIN_NAME).iso

