#include "LCD_test.h"
#include "LCD_GFX.h"

void initialize() {
	cli();
	SPCR = 0x50;		// Serial required
	DDRB = 0x2F;		// Serial required and Speaker Pin
	SPSR = 0x00;		// Serial status
	DDRD = SCE | RST | D_C;

	// ADC setup
	ADCSRA = (1<<ADEN) | (0<<ADPS2)| (1<<ADPS1)| (1<<ADPS0);	//enable ADC

	// PWM CTC Atmega8 Initialization
	TCCR1A |= (0<<COM1A1);	// Toggle OC1A pin
	TCCR1A |= (1<<COM1A0);

	TCCR1B |= (1<<WGM12);	// CTC mode
	TCCR1B |= (0<<CS12) | (0<<CS11) | (0<<CS10);	// Sets pre-scaler to 001:1 010:8 011:64 100:256 101:1024

	_delay_ms(5);		// Delay to make sure register initialization completes

	sei();
}

uint main(void) {
	uint i, j, k;

	char * str = "Jeffery02 - \nhas learned how to get this LCD screen  working!!!\tMaybe...\0";

	initialize();

	initGFX();

	fillRect(0, 0, ColMax, RowMax, RGBto16(0, 0, 255));

/*	k = 0;
	for (i = 0; i < ColMax; i++) {
		for (j = 0; j < RowMax; j++) {
			setPixel(i, j, k++);
		}
	}
*/
	printStrGFX(0, 0, str, RGBto16(255, 127, 0));

	while(1) {
		PORTB ^= 0x01;
	}
}



//------------general functions-----------------//


void itoa(uint j, char* temp) {			// Convert unsigned int into 4 decimal ASCII values
	uint i;
	for (i = 0; i < 4; i++) {
		temp[3-i] = (j%10) + 0x30;
		j /= 10;
	}
	temp[4] = 0x00;			// Add NULL byte to the end
	return;
}

void htoa(char hex, char* str) {		// hex to ascii
	str[0] = ((hex & 0xF0)>>4) + 0x30;
	if (str[0] > '9') {str[0] += 0x07;}
	str[1] = (hex & 0x0F) + 0x30;
	if (str[1] > '9') {str[1] += 0x07;}
	return;
}

uint getADC(char chan) {
	ADMUX = chan;		// Select which ADC MUX channel to read
	_delay_us(100);		// Stabalize ADC MUX output before retrieving value
	uint j = 0;
	ADCSRA |= (1<<ADSC);	// Start ADC conversion
	while(!(ADCSRA & (1<<ADIF)))	// Wait for ADC conversion to finish
	j = ADCL + (ADCH<<8);	// Extract ADC value
	return j;		// Return ADC value
}

//------------general functions-----------------//

//----------------interrupts--------------------//
/*
ISR(TIMER1_OVF_vect) {		//16-bit timer overflow interrupt
	PORTD ^= 0x02;
	printLCD(hex);
}
*/
//----------------interrupts--------------------//