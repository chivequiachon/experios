#include "memory.h"
#include "video.h"

void print_memory_map(MemoryTag *tag, uint64_t *y)
{
  MemoryMapTag *mmap_tag = (MemoryMapTag*)tag;
  MemoryMapTag *self_ptr = mmap_tag;
  MemoryMapEntry *start_area = self_ptr->entries;

  uint32_t entry_size = self_ptr->entry_size;
  uint64_t current_area = (uint64_t)start_area;
  uint64_t last_area = (uint64_t)self_ptr + (uint64_t)(self_ptr->size - entry_size);

  while(current_area < last_area)
  {
    MemoryMapEntry *area = (MemoryMapEntry*)current_area;
    current_area = current_area + entry_size;
    if(area->type == 1)
    {
      // print area
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
      if(mm->min > section->address)
      {
        mm->min = section->address;
      }

      if(mm->max < section->address + section->size)
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

void print_memory_areas(uint64_t mboot_addr, uint64_t row, uint32_t tag_type, void *misc)
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
          print_memory_map(tag, &y);
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

