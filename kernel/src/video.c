#include "video.h"


void putc(char x, char y, char c, char bg, char fg)
{
  volatile char *video = (volatile char*)VIDEO;
  char color = bg << 4 | fg;
  *(video + (x * 2) + (y * BUFF_WIDTH * 2)) = c;
  *(video + (x * 2 + 1) + (y * BUFF_WIDTH * 2)) = color;
}

void putval(char x, char y, uint64_t n, uint64_t base)
{
  const char *hex = "0123456789abcdef";

  if(n == 0)
  {
    putc(x + 1, y, *(hex), BLACK, GREEN);
    return;
  }

  uint64_t count = 0;
  uint64_t tmp = n;
  while(tmp > 0x0)
  {
    tmp /= base;
    count++;
  }

  uint64_t mod = 0;
  tmp = n;
  while(tmp > 0x0)
  {
    mod = tmp % base;
    tmp /= base;
    putc(x + count--, y, *(hex + mod), BLACK, GREEN);
  }
}

void puts(char x, char y, char *s, char bg, char fg)
{
  while(*s != '\0')
  {
    putc(x++, y, *s, bg, fg);
    s++;
  }
}

void cls()
{
  volatile char *video = (volatile char*)VIDEO;

  int i;
  for (i = 0; i < 4000; i += 2)
  {
    *(video + i) = ' ';
    *(video + i + 1) = 0x00;
  }
}
