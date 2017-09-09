BIN_NAME=experios
CC=x86_64-elf-gcc
CFLAGS=-std=gnu99 -ffreestanding -O2 -Wall -Wextra -mno-red-zone
OBJ_DIR=kernel/obj

KERNEL_CC_LIST:=\
kernel/src/kernel.o
#kernel/cpu/gdt.o \
#kernel/cpu/idt.o \
#kernel/cpu/irq.o \
#kernel/cpu/isr.o \
#kernel/devices/timer.o \
#kernel/devices/keyboard.o \
#kernel/mem/mem2.o \
#kernel/misc/tty.o \
#kernel/misc/vga.o \
#kernel/sys/system.o \
#kernel/kernel.o \
#kernel/libc.o

KERNEL_AS_LIST:=\
kernel/asm/multiboot_header.o \
kernel/asm/boot.o \
kernel/asm/long_mode_init.o
#kernel/core.o

KERNEL_INCLUDE_DIR=kernel/include

.PHONY: all clean iso run

all: clean kernel iso

clean:
	find -type f \( -name "*.o" -o -name "*.bin" \) -delete

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

