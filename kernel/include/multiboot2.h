#ifndef __MBOOT2_H
#define __MBOOT2_H

#include <stdint.h>

typedef struct
{
  uint64_t min;
  uint64_t max;
} MinMax;

typedef struct
{
  uint32_t total_size;
  uint32_t reserved;
} BootInformation;

typedef struct
{
  uint32_t type;
  uint32_t size;
} MemoryTag;

typedef struct
{
  uint64_t base_addr;
  uint64_t length;
  uint32_t type;
  uint32_t reserved;
} MemoryMapEntry;

typedef struct
{
  uint32_t type;
  uint32_t size;
  uint32_t entry_size;
  uint32_t entry_version;
  MemoryMapEntry entries[0]; 
} MemoryMapTag;

typedef struct
{
  uint32_t name_index;
  uint32_t type;
  uint64_t flags;
  uint64_t address;
  uint64_t offset;
  uint64_t size;
  uint32_t link;
  uint32_t info;
  uint64_t addr_align;
  uint64_t entry_size;
} ElfSection;

typedef struct
{
  uint32_t type;
  uint32_t size;
  uint32_t number_of_sections;
  uint32_t entry_size;
  uint32_t shndx;
  char first_section[0];
} ElfSectionsTag;

typedef struct
{
  uint64_t addr;
  uint64_t length;
} ReservedArea;

#endif
