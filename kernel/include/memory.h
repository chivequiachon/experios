#ifndef __MEMORY_H
#define __MEMORY_H

#include <stdint.h>
#include "multiboot2.h"

void print_memory_map(MemoryTag *tag, uint64_t *y);
void print_elf_sections(MemoryTag *tag, uint64_t *y, void *misc);
void print_memory_areas(uint64_t mboot_addr, uint64_t row, uint32_t tag_type, void *misc);

#endif
