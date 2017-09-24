#ifndef __VIDEO_H
#define __VIDEO_H

#include <stdint.h>

#define VIDEO 0xB8000

#define BUFF_HEIGHT 25
#define BUFF_WIDTH 80

#define BLACK   0x00
#define BLUE    0x01
#define GREEN   0x02
#define CYAN    0x03
#define RED     0x04
#define MAGENTA 0x05
#define BROWN   0x06
#define LGRAY   0x07
#define DGRAY   0x08
#define LBLUE   0x09
#define LGREEN  0x0A
#define LCYAN   0x0B
#define LRED    0x0C
#define PINK    0x0D
#define YELLOW  0x0E
#define WHITE   0x0F

void putc(char x, char y, char c, char bg, char fg);
void putval(char x, char y, uint64_t n, uint64_t base);
void puts(char x, char y, char *s, char bg, char fg);
void cls();

#endif
