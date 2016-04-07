
#define uint	unsigned int

// Clock Frequency
#define F_CPU 20000000

// Required Libraries
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/eeprom.h>

// Function Prototypes
void initialize();
void itoa(uint, char*);
void htoa(char, char*);
uint getADC(char);

// Function Definitions

#define SCK		0x10	// Serial Clock Pin
#define SCE		0x40	// Active Low Enable Pin
#define RST		0x20	// Active Low Reset Pin
#define D_C		0x80	// High = Data; Low = Command
