#ifndef __MEMORY_H
#define __MEMORY_H

#define PAGE_SIZE 4096

#include <stdint.h>
#include "multiboot2.h"

typedef struct
{
  uint64_t number;
} Frame;

typedef struct
{
  Frame *next_free_frame;
  MemoryMapEntry *current_area;
  uint64_t *mem_areas;
  uint32_t num;
  Frame *kernel_start;
  Frame *kernel_end;
  Frame *mboot_start;
  Frame *mboot_end;
} AreaFrameAllocator;


void print_memory_map(MemoryTag *tag, uint64_t *y, uint64_t *mem_areas);
void print_elf_sections(MemoryTag *tag, uint64_t *y, void *misc);
void print_memory_areas(uint64_t mboot_addr,
                        uint64_t row,
                        uint32_t tag_type,
                        void *misc,
                        uint64_t *mem_areas);

void containing_address(Frame *f, uint64_t address);
uint8_t allocate_frame(Frame *f, AreaFrameAllocator *alloc);
uint8_t choose_next_area(AreaFrameAllocator *alloc);
void init_allocator(AreaFrameAllocator *a, uint64_t kernel_start,
                    uint64_t kernel_end, uint64_t mboot_start,
                    uint64_t mboot_end, uint64_t *mem_areas,
                    uint8_t num);

#endif
