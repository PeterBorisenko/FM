/*
 * Atmega48_FMRadio_TEA5676.c
 *
 * Created: 23.01.2015 19:41:03
 *  Author: Disgust
 */ 


#include <avr/io.h>
#include "src/Defines.h"
#include "src/LCD.h"

#define F_CPU 8_000_000UL

// Определение выхода для управления громкостью
#define VOLport PORTB
#define VOLreg DDRB
#define VOLpin PINB2 // OC1B

// Определение кнопок
#define BUTTONport PORTB
#define BUTTONreg  DDRB
#define BB PINB0 // PCINT0
#define BR PINB1 // PCINT1
#define BC PINB3 // PCINT3
#define BL PINB4 // PCINT4
#define BU PINB5 // PCINT5

int main(void)
{
	PowerReduction();
	//i2c_init();
	VOLUME_init();
	BUTTON_init();
	LCD_Init();
	//EEPROM_init();
	
	sei();
    while(1)
    {
        //TODO: Menu handler
		
		//TODO: Command sending func that sends only if anything changes 
    }
}

void VOLUME_init() {
	BIT_clear(VOLreg, VOLpin); //VOL - output
	BIT_clear(VOLport, VOLpin);  //VOL = 0 
	//TODO: PWM set up
}

void BUTTON_init() {
	BUTTONreg&= ~(1 << BB)& ~(1 << BU)& ~(1 << BL)& ~(1 << BC)& ~(1 << BR); // Buttons is inputs
	BUTTONport|= (1 << BB)|(1 << BU)|(1 << BL)|(1 << BC)|(1 << BR); // with Pull-UP
	PCICR= 0b001; // Pin change on PCINT7..0 enabled
	PCMSK0|= (1 << BB)|(1 << BU)|(1 << BL)|(1 << BC)|(1 << BR); // Unmask corresponding button's interrupts
}

void PowerReduction() {
	PRR|= (1 << PRSPI)|(1 << PRADC)|(1 << PRUSART0)|(1 << PRTIM1)|(1 << PRTIM0);
}

ISR(PCINT0_vect) {
	//TODO: Button handler
	return;
}
