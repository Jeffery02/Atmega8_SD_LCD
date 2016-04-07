# For programming the Atmega
name = LCD_test
micro = atmega328p

# For setting the fuses of the Atmega
clock = ext
EESAVE = true

# Makes makefile easier to read
write_eeprom = -U eeprom:w:$(name).eep
write_flash = -U flash:w:$(name).hex:i

# Determines whether to program the EEPROM
ifeq ($(EESAVE), true)
high = -U hfuse:w:0xd1:m
else
high = -U hfuse:w:0xd9:m
endif

# Compiles the code and programs the Atmega
LCD: $(name).c
	avr-gcc -g -O3 -mmcu=$(micro) -c $(name).c LCD_Sound.c LCD_GFX.c
	avr-gcc -g -O3 -mmcu=$(micro) -o $(name).elf $(name).o LCD_Sound.o LCD_GFX.o
	avr-objcopy -j .text -j .data -O ihex $(name).elf $(name).hex
	sudo avrdude -c USBasp -p $(micro) $(write_flash)

LCD_Compile: $(name).c
	avr-gcc -g -O3 -mmcu=$(micro) -c $(name).c LCD_Sound.c LCD_GFX.c
	avr-gcc -g -O3 -mmcu=$(micro) -o $(name).elf $(name).o LCD_Sound.o LCD_GFX.o
	avr-objcopy -j .text -j .data -O ihex $(name).elf $(name).hex
	avr-objcopy -j .eeprom --change-section-lma .eeprom=0 -O ihex $(name).elf $(name).eep

LCD_Flash: $(name).hex
	sudo avrdude -c USBasp -p $(micro) $(write_flash)

LCD_EEPROM: $(name).eep
	sudo avrdude -c USBasp -p $(micro) $(write_eeprom)

# Determines which clock to use for which Atmega
ifeq ($(micro), atmega8)
ifeq ($(clock), ext)
low = -U lfuse:w:0xdf:m
else
low = -U lfuse:w:0xe4:m
endif
endif

ifeq ($(micro), atmega328p)
ifeq ($(clock), ext)
low = -U lfuse:w:0xee:m
else
low = -U lfuse:w:0xe2:m
endif
endif

setFuse:
	avrdude -c USBasp -p $(micro) $(low) $(high)
