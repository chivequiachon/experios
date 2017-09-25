#include "memory.h"
#include "video.h"

void print_memory_map(MemoryTag *tag, uint64_t *y, uint64_t *mem_areas)
{
  MemoryMapTag *mmap_tag = (MemoryMapTag*)tag;
  MemoryMapTag *self_ptr = mmap_tag;
  MemoryMapEntry *start_area = self_ptr->entries;

  uint32_t entry_size = self_ptr->entry_size;
  uint64_t current_area = (uint64_t)start_area;
  uint64_t last_area = (uint64_t)self_ptr + (uint64_t)(self_ptr->size - entry_size);

  uint32_t i = 0;
  while(current_area < last_area)
  {
    MemoryMapEntry *area = (MemoryMapEntry*)current_area;
    current_area = current_area + entry_size;
    if(area->type == 1)
    {
      // print area
      mem_areas[i++] = (uint64_t)area;
      putval(0, *y, area->base_addr, 16);
      putval(8, *y, area->length, 16);
      (*y)++;
    }
  }
}

void print_elf_sections(MemoryTag *tag, uint64_t *y, void *misc)
{
  ElfSectionsTag *self_ptr = (ElfSectionsTag*)tag;
  ElfSection *current_section = (ElfSection*)(self_ptr->first_section);
  uint32_t remaining_sections = self_ptr->number_of_sections - 1;
  uint32_t entry_size = self_ptr->entry_size;

  MinMax *mm = (MinMax*)misc;
  mm->min = current_section->address;
  mm->max = current_section->address + current_section->size;

  while(remaining_sections > 0)
  {
    ElfSection *section = (ElfSection*)current_section;
    uint64_t next_section_addr = (uint64_t)current_section + entry_size;
    current_section = (ElfSection*)next_section_addr;
    remaining_sections -= 1;

    if(section->type == (uint32_t)0)
    {
      continue;
    }
    else
    {
      if(mm->min == 0 || mm->min > section->address)
      {
        mm->min = section->address;
      }

      if(mm->max < (section->address + section->size))
      {
        mm->max = section->address + section->size;
      }

      // print section section.addr, section.size, section.flags
      putval(0, *y, section->address, 16);
      putval(15, *y, section->size, 16);
      putval(26, *y, section->flags, 16);
      (*y)++;
    }
  }
}

void print_memory_areas(uint64_t mboot_addr,
                        uint64_t row,
                        uint32_t tag_type,
                        void *misc,
                        uint64_t *mem_areas)
{
  uint64_t y = row;

  MemoryTag *tag;
  for(tag = (MemoryTag*)(mboot_addr + 8); tag->type != 0; 
      tag = (MemoryTag*)((uint8_t*)tag + ((tag->size + 7) & ~7)))
  {
    if(tag->type == tag_type)
    {
      switch(tag->type)
      {
        case 4: // memory info
          break;
        case 6: // memory map
          // call memory map callback
          print_memory_map(tag, &y, mem_areas);
          break;
        case 9: // elf sections
          print_elf_sections(tag, &y, misc);
          break;
        default:
          break;
      }
    }
  }
}

void containing_address(Frame *f, uint64_t address)
{
  f->number = address / PAGE_SIZE;
}

uint8_t allocate_frame(Frame *f, AreaFrameAllocator *alloc)
{
  Frame free_frame = {alloc->next_free_frame->number};
  f->number = free_frame.number;

  Frame last;
  uint64_t last_addr = 
    alloc->current_area->base_addr + alloc->current_area->length - 1;
  containing_address(&last, last_addr);

  // update allocator's next free frame 
  if(f->number > last.number)
  {
    // current memory area is full, choose another area
    return choose_next_area(alloc);
  }
  else if(f->number >= alloc->kernel_start->number &&
          f->number <= alloc->kernel_end->number)
  {
    alloc->next_free_frame->number = alloc->kernel_end->number + 1;
  }
  else if(f->number >= alloc->mboot_start->number &&
          f->number <= alloc->mboot_end->number)
  {
    alloc->next_free_frame->number = alloc->mboot_end->number + 1;
  }

  return 1;
}

uint8_t choose_next_area(AreaFrameAllocator *alloc)
{
  uint8_t i;
  uint8_t flag = 0;
  for(i = 0; i < alloc->num; i++)
  {
    MemoryMapEntry *entry = (MemoryMapEntry*)(alloc->mem_areas[i]);
    uint64_t address = entry->base_addr + entry->length - 1;

    Frame last_frame;
    containing_address(&last_frame, address);

    if(last_frame.number >= alloc->next_free_frame->number)
    {
      alloc->current_area = entry;
      flag = 1;
      break;
    }
  }

  if(flag == 1)
  {
    Frame start_frame;
    containing_address(&start_frame, alloc->current_area->base_addr);
    if(alloc->next_free_frame->number < start_frame.number)
    {
      alloc->next_free_frame->number = start_frame.number;
    }
  }

  return flag;
}

void init_allocator(AreaFrameAllocator *a, uint64_t kernel_start,
                    uint64_t kernel_end, uint64_t mboot_start,
                    uint64_t mboot_end, uint64_t *mem_areas,
                    uint8_t num)
{
  a->current_area = (MemoryMapEntry*)mem_areas[0];
  a->mem_areas = mem_areas;
  a->num = num;

  containing_address(a->next_free_frame, 0);
  containing_address(a->kernel_start, kernel_start);
  containing_address(a->kernel_end, kernel_end);
  containing_address(a->mboot_start, mboot_start);
  containing_address(a->mboot_end, mboot_end);
  choose_next_area(a);
}
