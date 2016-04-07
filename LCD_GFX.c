#include "LCD_test.h"
#include "LCD_GFX.h"

uint LCD_Cursor = 0;
uint Text_Color = BLACK;
uint Background_Color = WHITE;

//-------------LCD functions--------------------//

void initGFX(uint color) {		// GFX LCD Initialization and background color
	_delay_ms(5);
	PORTD &= ~RST;				// Reset GFX LCD
	_delay_ms(20);
	PORTD |= RST;
	_delay_ms(150);
						// v Factory initialization sequence (not sure what it's doing)
	outGFX(0xEF, command);
	outGFX(0x03, print);
	outGFX(0x80, print);
	outGFX(0x02, print);

	outGFX(0xCF, command);
	outGFX(0x00, print);
	outGFX(0xC1, print);
	outGFX(0x30, print);

	outGFX(0xED, command);
	outGFX(0x64, print);
	outGFX(0x03, print);
	outGFX(0x12, print);
	outGFX(0x81, print);

	outGFX(0xE8, command);
	outGFX(0x85, print);
	outGFX(0x00, print);
	outGFX(0x78, print);

	outGFX(0xCB, command);
	outGFX(0x39, print);
	outGFX(0x2C, print);
	outGFX(0x00, print);
	outGFX(0x34, print);
	outGFX(0x02, print);
	
	outGFX(0xF7, command);
	outGFX(0x20, print);

	outGFX(0xEA, command);
	outGFX(0x00, print);
	outGFX(0x00, print);
						// ^ Factory initialization sequence (not sure what it's doing)

	outGFX(0xC0, command);		// Power Control 1
	outGFX(0x23, print);

	outGFX(0xC1, command);		// Power Control 2
	outGFX(0x10, print);

	outGFX(0xC5, command);		// VCOM Control 1
	outGFX(0x3E, print);
	outGFX(0x28, print);

	outGFX(0xC7, command);		// VCOM Control 2
	outGFX(0x86, print);

	outGFX(0x36, command);		// Memory Access Control
	outGFX(0x48, print);

	outGFX(0x3A, command);		// Pixel Format Set
	outGFX(0x55, print);

	outGFX(0xB1, command);		// Frame Control
	outGFX(0x00, print);
	outGFX(0x18, print);

	outGFX(0xB6, command);		// Display Function Control
	outGFX(0x08, print);
	outGFX(0x82, print);
	outGFX(0x27, print);

	outGFX(0xF2, command);		// Gamma Function Disable
	outGFX(0x00, print);

	outGFX(0x26, command);		// Gamma Set
	outGFX(0x01, print);

	outGFX(0xE0, command);		// Positive Gamma Correction
	outGFX(0x0F, print);
	outGFX(0x31, print);
	outGFX(0x2B, print);
	outGFX(0x0C, print);
	outGFX(0x0E, print);
	outGFX(0x08, print);
	outGFX(0x4E, print);
	outGFX(0xF1, print);
	outGFX(0x37, print);
	outGFX(0x07, print);
	outGFX(0x10, print);
	outGFX(0x03, print);
	outGFX(0x0E, print);
	outGFX(0x09, print);
	outGFX(0x00, print);

	outGFX(0xE1, command);		// Negative Gamma Correction
	outGFX(0x00, print);
	outGFX(0x0E, print);
	outGFX(0x14, print);
	outGFX(0x03, print);
	outGFX(0x11, print);
	outGFX(0x07, print);
	outGFX(0x31, print);
	outGFX(0xC1, print);
	outGFX(0x48, print);
	outGFX(0x08, print);
	outGFX(0x0F, print);
	outGFX(0x0C, print);
	outGFX(0x31, print);
	outGFX(0x36, print);
	outGFX(0x0F, print);

	outGFX(0x11, command);		// Sleep OUT
	_delay_ms(120);
	outGFX(0x29, command);		// Display On

	// Initialize Background
	fillRect(0, 0, ColMax, RowMax, color);
	Background_Color = color;

	return;
}

void outGFX(char k, char mode) {
	if (mode)		// Selects data or command mode
		PORTD &= ~D_C;	// Command mode
	else
		PORTD |= D_C;	// Data (Print) Mode
	PORTD &= ~SCK;
	PORTD &= ~SCE;
	SPDR = k;		// Send Data/Command via SPI
	while(!(SPSR & 0x80));		// Wait for SPI transfer to finish
	k = SPDR;		// Not needed when trasmitting
	//_delay_us(10);	// Delay before quitting SPI

	PORTD |= SCE;
	return;
}

void setWindow(uint x0, uint y0, uint x1, uint y1) {
	outGFX(0x2A, command);		// Column Address Set
	outGFX(x0 >> 8, print);
	outGFX(x0 & 0xFF, print);	// x start
	outGFX(x1 >> 8, print);
	outGFX(x1 & 0xFF, print);	// x end

	outGFX(0x2B, command);		// Row Address Set
	outGFX(y0 >> 8, print);
	outGFX(y0 & 0xFF, print);	// y start
	outGFX(y1 >> 8, print);
	outGFX(y1 & 0xFF, print);	// y end

	outGFX(0x2C, command);		// Write to RAM
}

uint RGBto16(char R, char G, char B) {
	return (((R & 0xF8) << 8) | ((G & 0xFC) << 3) | ((B & 0xF8) >> 3));
}

void setPixel(uint x, uint y, uint color) {
	setWindow(x, y, x+1, y+1);

	PORTD |= D_C;	// Sending Data
	PORTD &= ~SCE;	// Enable LCD SPI
	SPDR = color >> 8;		// Send 8 MSBs
	while(!(SPSR & 0x80));		// Wait for SPI transfer to finish
	SPDR = color;		// Send 8 LSBs
	while(!(SPSR & 0x80));		// Wait for SPI transfer to finish
	PORTD |= SCE;	// Disable LCD SPI
}

void fillRect(uint x, uint y, uint w, uint l, uint color) {
	static uint i, j;
	setWindow(x, y, x+w-1, y+l-1);

	PORTD |= D_C;	// Sending Data
	PORTD &= ~SCE;	// Enable LCD SPI
	for (i = 0; i < w; i++) {
		for (j = 0; j < l; j++) {
			SPDR = color >> 8;			// Send 8 MSBs
			while(!(SPSR & 0x80));		// Wait for SPI transfer to finish
			SPDR = color;				// Send 8 LSBs
			while(!(SPSR & 0x80));		// Wait for SPI transfer to finish
		}
	}
	PORTD |= SCE;	// Disable LCD SPI
}

void setTextColor(uint color) {
	Text_Color = color;
	return;
}

void printCharGFX(uint x, uint y, char a, uint color) {
	static uint i, j, k;
	static char temp;
	setWindow(x, y, x+5, y+7);

	fillRect(x, y, charWidth+1, charHeight, Background_Color);	// Erase background

	for (i = 0; i < 8; i++) {
		for (j = 0; j < 6; j++) {
			if (j)						// Extract pixel positions for character
				temp = eeprom_read_byte(&ASCII[a - 0x20][j-1]);
//				temp = ASCII[a - 0x20][j-1];
			else
				temp = 0x00;			// One blank column between each character

			if ((0x01 << i) & temp)		// Check that pixel matches character model
				setPixel(x + j, y + i, color);
		}
	}
}

void printStrGFX(uint x, uint y, char* str, uint color) {
	static uint i, j;
	static char gapX, gapY;
	i = 0; j = 0;
	gapX = ColMax%charWidth; gapY = RowMax%charHeight;		// Gaps where character won't fit
	while (str[j]) {	// While not NULL byte
		if ((str[j] >= 0x20) && (str[j] < 0X80))	// Print character
			printCharGFX((x + i*charWidth)%(ColMax-gapX), (y + (i/charX)*charHeight)%(RowMax-gapY), str[j], color);
		if (str[j] == '\t')		// Indents 4 spaces
			i += 2;
		if (str[j] == '\n')		// Moves to next line
			i = ((((i/charX)+1)*charX) - 1);
		i++; j++;
	}
	LCD_Cursor = i + x + y*charX;
	return;
}

void printString(char* str) {
	static uint color, i, j, x, y;
	static char gapX, gapY;
	color = Text_Color; i = LCD_Cursor; j = 0; x = 0; y = 0;
	gapX = ColMax%charWidth; gapY = RowMax%charHeight;		// Gaps where character won't fit
	while (str[j]) {	// While not NULL byte
		if ((str[j] >= 0x20) && (str[j] < 0X80))	// Print character
			printCharGFX((x + i*charWidth)%(ColMax-gapX), (y + (i/charX)*charHeight)%(RowMax-gapY), str[j], color);
		if (str[j] == '\t')		// Indents 4 spaces
			i += 2;
		if (str[j] == '\n')		// Moves to next line
			i = ((((i/charX)+1)*charX) - 1);
		i++; j++;
	}
	LCD_Cursor = i;
	return;
}

//-------------LCD functions--------------------//
