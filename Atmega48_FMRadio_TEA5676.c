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
volatile static uint8_t backlightLevel= 128;
volatile static uint8_t LIT_TMOUT= 255;  // Backlight timeout



// Переменные для TEA5676
Smessage_t Config;
Rmessage_t Status;

// Global device state type and variable...
typedef enum{
	DEV_SLEEP,
	DEV_LCD_OFF,
	DEV_UPPER,
	DEV_MENU1,
	DEV_MENU2
	} state_t;

volatile static state_t devState;


volatile static uint8_t timestamp[5];
volatile static const uint8_t numBtns= 5;
volatile static uint8_t portbState;			// prev value of button register
volatile static uint8_t changes;			// 1 - if config changed, 0 - if not
volatile static uint8_t timeout;

// DEV_UPPER menu state
void channelLeft();
void channelRight();
void volumeUp();
void volumeDown();
void goMenu1();
void goSleep();
void goLedOff()

// DEV_MENU1 menu state
void searchLeft();
void searchRight();
void chToStoreUp();
void chToStoreDown();
void storeCh();
void goUpper();
void goMenu2();

// DEV_MENU2 secret menu level (for debugging/tune)
///

int main(void)
{
	
	i2cInit();
	volumeInit();
	//EEPROM_init();
	buttonInit();
	LCD_Init();
	powerReduction();
	sei();
    while(1)
    {
		//EEreadConfig();
		
		TEA5676sendConfig();
		sleep();
		if (changes)
		{
			TEA5676sendConfig();
			timeout= LIT_TMOUT;
		}
		
		//TEA5676readConfig();
		
		//TODO: Command sending func that sends only if anything changes 
		
		
    }
}

void TEA5676sendConfig() {
	i2cStart();
	while (!(TWCR&~(1 << TWINT))); // wait while TWIN==1 and TWSTA==0
	i2cWrite(TEA_W); // send SLA_W
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
	setLed(0);
	//TEA5676readConfig();
	//EEstoreConfig();
	//TEA5676goSleep();
	SMCR|= (0b010 << SM0); // set sleepmode to powerdown
	BIT_set(SMCR, SE);
	__asm__ __volatile__ ("sleep");
}

void wakeUp() {
	BIT_clear(SMCR, SE);
	SMCR|= (0b011 << SM0); // set sleepmode to powersave
	setLed(backlightLevel);
	devState= DEV_UPPER;
}

void goLedOff() {
	setLed(0);
	BIT_set(SMCR, SE);
	__asm__ __volatile__ ("sleep");
}

void goMenu1() {
	BIT_clear(SMCR, SE);
	
}

void setLed(uint8_t level) {
	LCD_Backlight(level);
}

ISR(PCINT0_vect) {

	uint8_t portbNow= PORTB;
	uint8_t changedUp= (portbState)&~(portbNow);
	uint8_t changedDown= ~(portbState)&(portbNow);
	portbState= portbNow;
	
	if (devState == DEV_SLEEP)
	{
		wakeUp();
	}
	else if(devState == DEV_LCD_OFF)
		setLed(backlightLevel);
		devState= DEV_UPPER;
	}
	else if() {
		for (uint8_t i= numBtns; i >= 0; i--)
		{
			//if(BIT_read(changedUp, i)) {// from HI to LO //TODO: define rise/fall check
			//	timestamp[i]= 0;
			//if(BIT_read(changedDown, i))
			//	if(timestamp[i] < longPushTime) {
			//		pushLong(button); // buttons defined in the head of file
			//  }
			//  else {
			//		push(button);
			//	}
			//}
		}
	}

	return;
}

ISR(TWI_vect) {
	i2cHandler();
	return;
}

ISR(TIMER2_OVF_vect) {
	for (uint8_t i= 0; i < numBtns; i++)
	{
		timestamp[i]++;
		timeout--;
	}
	
}