#define BUFF_HEIGHT 25
#define BUFF_WIDTH 80

typedef enum 
{
	Black      = 0,
	Blue       = 1,
	Green      = 2,
	Cyan       = 3,
	Red        = 4,
	Magenta    = 5,
	Brown      = 6,
	LightGray  = 7,
	DarkGray   = 8,
	LBlue      = 9,
	LGreen     = 10,
	LCyan      = 11,
	LRed       = 12,
	Pink       = 13,
	Yellow     = 14,
	White      = 15
} 	COLOR;

void printc(char x, char y, char c, COLOR bg, COLOR fg)
{
	volatile char *video = (volatile char*)0xb8000;
	char color = bg << 4 | fg;
	*(video + (x * 2) + (y * BUFF_WIDTH)) = c;
	*(video + (x * 2 + 1) + (y * BUFF_WIDTH)) = color;
}

void main()
{
	printc(0, 0, 'H', Blue, White);
	printc(1, 0, 'E', Blue, White);
	printc(2, 0, 'L', Blue, White);
	printc(3, 0, 'L', Blue, White);
	printc(4, 0, 'O', Blue, White);
	printc(5, 0, ' ', Blue, White);
	printc(6, 0, 'W', Blue, White);
	printc(7, 0, 'O', Blue, White);
	printc(8, 0, 'R', Blue, White);
	printc(9, 0, 'L', Blue, White);
	printc(10, 0, 'D', Blue, White);

	while(1){}
}
