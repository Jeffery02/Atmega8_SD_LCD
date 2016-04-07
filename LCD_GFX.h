
// Required Libraries
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "GFX_ASCII.h"	// ASCII table for GFX LCDs

// Function Prototypes
void initGFX(uint);
void outGFX(char, char);
void setWindow(uint, uint, uint, uint);
uint RGBto16(char, char, char);
void setPixel(uint, uint, uint);
void fillRect(uint, uint, uint, uint, uint);
void setTextColor(uint);
void printCharGFX(uint, uint, char, uint);
void printStrGFX(uint, uint, char*, uint);
void printString(char*);

// Function Definitions
#define print 0
#define command 1

#define ColMax  240   // Number of columns
#define RowMax  320   // Number of rows
/*
#define ColMax  128   // Number of columns
#define RowMax  160   // Number of rows
*/
#define charWidth   6     // Column width of characters
#define charHeight  8     // Row height of characters

#define charX (ColMax/charWidth)    // Number of characters that can fit horizontally
#define charY (RowMax/charHeight)   // Number of characters that can fit vertically

// Color Definitions
#define BLACK   0x0000
#define RED     0xF800
#define GREEN   0x07E0
#define BLUE    0x001F
#define YELLOW  0xFFE0 
#define MAGENTA 0xF81F 
#define CYAN    0x07FF
#define WHITE   0xFFFF

extern uint LCD_Cursor;
extern uint Background_Color;