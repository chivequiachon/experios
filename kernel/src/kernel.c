#include <stdint.h>

#include "video.h"
#include "memory.h"

extern unsigned int kernel_end;

void main(uint64_t mboot_addr)
{
  cls();

  uint64_t mem_areas[2];
  MinMax mm = {0,0};

  BootInformation *bi = (BootInformation*)mboot_addr;

  puts(0, 1, "Hello User!\0", BLACK, GREEN);
  puts(0, 2, "Printing memory information:\0", BLACK, GREEN);

  puts(0, 3, "mboot_start:\0", BLACK, GREEN);
  putval(12, 3, mboot_addr, 16);

  puts(0, 4, "mboot_end:\0", BLACK, GREEN);
  uint64_t mboot_end = mboot_addr + bi->total_size;
  putval(12, 4, mboot_end, 16);

  puts(0, 5, "memory_areas:\0", BLACK, GREEN);
  print_memory_areas(mboot_addr, 6, 6, 0, mem_areas);

  puts(0, 8, "elf_sections:\0", BLACK, GREEN);
  print_memory_areas(mboot_addr, 9, 9, (void*)&mm, 0);

  puts(0, 17, "kernel_start:\0", BLACK, GREEN);
  putval(13, 17, mm.min, 16);

  puts(0, 18, "kernel_end:\0", BLACK, GREEN);
  putval(13, 18, mm.max, 16);
  puts(0, 19, "Allocating Frames:\0", BLACK, GREEN);
  
  /*AreaFrameAllocator alloc;
  init_allocator(&alloc, mm.min, mm.max, mboot_addr, mboot_end, mem_areas, 2);

  uint64_t count = 0;
  while(1)
  {
    Frame unused;
    if(!allocate_frame(&unused, &alloc))
    {
      break;
    }
    count++;
  }

  putval(21, 19, count, 10);*/

  while(1){}
}
