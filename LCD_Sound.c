#include "LCD_test.h"
#include "LCD_Sound.h"

void sound(uint Hz, uint ms) {
	uint scaler;
	if(Hz < 200)	{return;}		// return if frequency is too low for speaker
	TCCR1B = (TCCR1B & 0xF8) + ((0<<CS12) | (1<<CS11) | (0<<CS10));	//sets pre-scaler to 001:1 010:8 011:64 100:256 101:1024)
	switch(TCCR1B & 0x07) {		// Calculate pre-scaler value
		case 1:
			scaler = 1;
			break;
		case 2:
			scaler = 8;
			break;
		case 3:
			scaler = 64;
			break;
		case 4:
			scaler = 256;
			break;
		case 5:
			scaler = 1024;
			break;
	}
	OCR1A = (F_CPU / Hz / scaler / 2) - 1;	// CTC (Clear Timer on Compare) cycle counter limit
	uint i = ms;
	while((i--) > toneGap) {		// Use delay function with a variable
		_delay_ms(1);
	}
	TCCR1B &= 0XF8;			// Set pre-scaler to 0
	_delay_ms(toneGap);
}