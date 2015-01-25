/*
 * Atmega48_FMRadio_TEA5676.c
 *
 * Created: 23.01.2015 19:41:03
 *  Author: Disgust
 */ 
#define F_CPU 8000000UL

#include <avr/io.h>
#include "src/Defines.h"
#include "src/LCD.h"
#include "src/TEA5676_.h"

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

// Прескалер для ШИМ
#define TIM0PSC 0x04

// Переменные из памяти
volatile static uint8_t StoredVOL= 0;
volatile static uint8_t StroredFREQ= 0x00;

// Переменные для TEA5676
Smessage_t Config;
Rmessage_t Status;

int main(void)
{
	i2cInit();
	volumeInit();
	//EEPROM_init();
	buttonInit();
	LCD_Init();
	TEA5676Init();
	
	powerReduction();
	sei();
    while(1)
    {
		//TODO: Menu handler
		
		//TODO: Command sending func that sends only if anything changes 
    }
}

void TEA5676Init() {
	i2cStart();
	while (!(TWCR&~(1 << TWINT))); // wait while TWIN==1 and TWSTA==0
	i2cWrite(&TEA_W); // send SLA_W
	while (!(TWCR&~(1 << TWINT)));
	i2cWrite(Config.b1);
	while (!(TWCR&~(1 << TWINT)));
	i2cWrite(Config.b2);
	while (!(TWCR&~(1 << TWINT)));
	i2cWrite(Config.b3);
	while (!(TWCR&~(1 << TWINT)));
	i2cWrite(Config.b4);
	while (!(TWCR&~(1 << TWINT)));
	i2cWrite(Config.b5);
	while (!(TWCR&~(1 << TWINT)));
	i2cStop();
}

void volumeInit() {
	BIT_clear(VOLreg, VOLpin); //VOL - output
	BIT_clear(VOLport, VOLpin);  //VOL = 0 
	//TODO: PWM set up
	TCCR0A|= (WGM00 << 0x03); // FAST PWM Mode
	TCCR0A|= (1 << COM0B1); // Clear OC0B on compare match, set OC0B at BOTTOM (non-inverting PWM mode)
	OCR0B= StoredVOL; // Volume 0
	TCCR0B|= (CS00 << TIM0PSC); // Prescaler set and turn timer ON
}

void buttonInit() {
	BUTTONreg&= ~(1 << BB)& ~(1 << BU)& ~(1 << BL)& ~(1 << BC)& ~(1 << BR); // Buttons is inputs
	BUTTONport|= (1 << BB)|(1 << BU)|(1 << BL)|(1 << BC)|(1 << BR); // with Pull-UP
	PCICR= 0b001; // Pin change on PCINT7..0 enabled
	PCMSK0|= (1 << BB)|(1 << BU)|(1 << BL)|(1 << BC)|(1 << BR); // Unmask corresponding button's interrupts
}

void powerReduction() {
	PRR|= (1 << PRSPI)|(1 << PRADC)|(1 << PRUSART0)|(1 << PRTIM1);
}

void sleep() {
	
}

void wakeUp() {
	
}

ISR(PCINT0_vect) {
	//TODO: Button handler
	return;
}

ISR(TWI_vect) {
	i2cHandler();
	return;
}