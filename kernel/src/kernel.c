#include <stdint.h>

#include "video.h"
#include "memory.h"

extern unsigned int kernel_end;

void main(uint64_t mboot_addr)
{
  cls();

  BootInformation *bi = (BootInformation*)mboot_addr;

  puts(0, 1, "Hello User!\0", BLACK, GREEN);
  puts(0, 2, "Printing memory information:\0", BLACK, GREEN);

  puts(0, 3, "mboot_start:\0", BLACK, GREEN);
  putval(12, 3, (uint64_t)mboot_addr, 16);

  puts(0, 4, "mboot_end:\0", BLACK, GREEN);
  putval(12, 4, (uint64_t)mboot_addr + bi->total_size, 16);

  puts(0, 5, "memory_areas:\0", BLACK, GREEN);
  print_memory_areas(mboot_addr, 6, 6, 0);

  puts(0, 8, "elf_sections:\0", BLACK, GREEN);
  MinMax mm = {0,0};
  print_memory_areas(mboot_addr, 9, 9, (void*)&mm);

  puts(0, 17, "kernel_start:\0", BLACK, GREEN);
  putval(13, 17, mm.min, 16);

  puts(0, 18, "kernel_end:\0", BLACK, GREEN);
  putval(13, 18, mm.max, 16);

  while(1){}
}
