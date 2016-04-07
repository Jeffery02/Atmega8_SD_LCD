
// Required Libraries
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "music.h"

#define toneGap 20

// Function Prototypes
void sound(uint, uint);