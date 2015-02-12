/*
 * Atmega48_FMRadio_TEA5676.c
 *
 * Created: 23.01.2015 19:41:03
 *  Author: Disgust
 */ 
#define F_CPU 8000000UL

#include <avr/io.h>
#include <avr/eeprom.h>
#include <avr/sleep.h>
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

typedef struct {
	uint8_t pll_hi:6;
	uint8_t pll_lo:8;
	} channel_t;

// Переменные из памяти
volatile static bit Custom;				// if custom == 0 - load factory defaults, if custom == 1 - load stored config 
volatile static uint8_t StoredVOL= 0;
volatile static uint8_t StroredFREQ= 0x00;
volatile static channel_t StoredChannels[30];
volatile static uint8_t BacklightLevel= 128;
volatile static uint8_t LightOffTimeout= 255;  // Backlight timeout

// Переменные для TEA5676
static Smessage_t Config;
static Rmessage_t Status;

// Global device state type and variable...
typedef enum{
	DEV_SLEEP,
	DEV_LCD_OFF,
	DEV_UPPER,
	DEV_MENU1,
	DEV_SEARCH,
	DEV_MENU2
	} state_t;

volatile static state_t devState;

// Variables and constants for timing and buttons handling
#define numBtns 5
const uint8_t LongPushTime= 50;
volatile static uint8_t timestamp[5];
volatile static uint8_t portbState;			// prev value of button register
volatile static bit changes;			// 1 - if local config changed, 0 - if not
volatile static uint8_t timeout;

typedef struct {
	// 	uint8_t port;
	// 	uint8_t dir;
	uint8_t pin;	// pin number
	uint8_t push;	// 0x00 - was not pushed, 0x01 - was pushed shortly, 0x02 - was pushed longly
} button_t;

static button_t buttons[numBtns];

// State change methods
// DEV_UPPER menu state
void channelLeft();
void channelRight();
void volumeUp();
void volumeDown();
void goMenu1();
void goSleep();
void goLedOff();

// DEV_MENU1 menu state
void selectUp();
void selectDown();
void valueMore();
void valueLess();
void goUpper();
void goSearch();
void goMenu2();

// DEV_SEARCH menu state
void searchLeft();
void searchRight();
void chToStoreUp();
void chToStoreDown();
void storeCh();
void goUpper();

// DEV_MENU2 secret menu level (for debugging/tune)
///


// Other methods
void volumeInit();
void buttonInit();
void EEPROM_init();
void powerReduction();

void setLed(uint8_t);
void wakeUp();

uint8_t EEloadCustom();
void EEloadSettings(uint8_t);
void EEstoreSettings();
void TEA5676sendConfig();
void TEA5676readConfig();
void TEA5676goSleep();
void TEA5676goWork();
void TEA5676setFrequency();
void TEA5676startSearch(uint8_t);


int main(void)
{
	LCD_Init();
	LCD_Write("AWSMTEK.COM", 11, 3, 0);
	i2cInit();
	buttonInit();
	
	EEPROM_init();
	powerReduction();
	Custom.val= EEloadCustom();
	EEloadSettings(Custom.val);
	volumeInit();
	sei();
	TEA5676sendConfig();
	goSleep();
	LCD_Clear();
    while(1)
    {
		if (changes.val)
		{
			TEA5676sendConfig();
			timeout= LightOffTimeout;
			changes.val= 0x0;
			//TODO: State action
		}
		
		//TODO: Display info
		
		if (timeout == 0) {
			goLedOff();
		}
    }
}

void TEA5676sendConfig() {
	cli();
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
	sei();
}


/************************************************************************/
/* INITIAL					                                            */
/************************************************************************/

void volumeInit() {
	BIT_clear(VOLreg, VOLpin); //VOL - output
	BIT_clear(VOLport, VOLpin);  //VOL = 0 
	TCCR0A|= (WGM00 << 0x03); // FAST PWM Mode
	TCCR0A|= (1 << COM0B1); // Clear OC0B on compare match, set OC0B at BOTTOM (non-inverting PWM mode)
	OCR0B= StoredVOL; // Volume 0
	TCCR0B|= (CS00 << TIM0PSC); // Prescaler set and turn timer ON
}

void buttonInit() {
	buttons[0].pin= (uint8_t)BB;
	buttons[1].pin= (uint8_t)BR;
	buttons[2].pin= (uint8_t)BC;
	buttons[3].pin= (uint8_t)BL;
	buttons[4].pin= (uint8_t)BU;
	
	BUTTONreg&= ~(1 << buttons[0].pin)& ~(1 << buttons[1].pin)& ~(1 << buttons[2].pin)& ~(1 << buttons[3].pin)& ~(1 << buttons[4].pin); // Buttons is inputs
	BUTTONport|= (1 << buttons[0].pin)|(1 << buttons[1].pin)|(1 << buttons[2].pin)|(1 << buttons[3].pin)|(1 << buttons[4].pin); // with Pull-UP
	PCICR= 0b001; // Pin change on PCINT7..0 enabled
	PCMSK0|= (1 << buttons[0].pin)|(1 << buttons[1].pin)|(1 << buttons[2].pin)|(1 << buttons[3].pin)|(1 << buttons[4].pin); // Unmask corresponding button's interrupts
	
}

void powerReduction() {
	PRR|= (1 << PRSPI)|(1 << PRADC)|(1 << PRUSART0)|(1 << PRTIM1);
}

/************************************************************************/
/* STATE CHANGING METHODS	                                            */
/************************************************************************/

void goSleep() {
	setLed(0);
	EEstoreSettings();
	TEA5676goSleep();
	//SMCR|= (0b010 << SM0); // set sleepmode to powerdown
	set_sleep_mode(SLEEP_MODE_PWR_DOWN);
	//BIT_set(SMCR, SE);
	//__asm__ __volatile__ ("sleep" "\n\t"::);
	sleep_enable();
	sleep_cpu();
}

void wakeUp() {
	//BIT_clear(SMCR, SE);
	//SMCR|= (0b011 << SM0); // set sleepmode to powersave
	sleep_disable();
	set_sleep_mode(SLEEP_MODE_PWR_SAVE);
	setLed(BacklightLevel);
	TEA5676goWork();
	devState= DEV_UPPER;
}

void goLedOff() {
	setLed(0);
	//BIT_set(SMCR, SE);
	//__volatile__ __asm__ ("sleep" "\n\t"::);
	sleep_enable();
	sleep_cpu();
}

void goMenu1() {
	BIT_clear(SMCR, SE);
	devState= DEV_MENU1;
}

void setLed(uint8_t level) {
	LCD_Backlight(level);
}

/************************************************************************/
/* INTERRUPTS                                                           */
/************************************************************************/

ISR(PCINT0_vect) {

	uint8_t portbNow= PORTB;
	uint8_t changedUp= (portbState)&~(portbNow); // changes from 0 to 1
	uint8_t changedDown= ~(portbState)&(portbNow); // changes from 1 to 0
	portbState= portbNow;
	
	if (devState == DEV_SLEEP) {
		wakeUp();
	}
	else if(devState == DEV_LCD_OFF) {
		setLed(BacklightLevel);
		devState= DEV_UPPER;
	}
	else {
		for (uint8_t i= numBtns-1; i >= 0; i--) { //TODO: Just change states and write buttons[i].push
			if(BIT_read(changedDown, buttons[i].pin)) {// from HI to LO
				timestamp[i]= 0;
			}
			if(BIT_read(changedUp, i)) {
				if(timestamp[i] < LongPushTime) {
					buttons[i].push= 0x02; // buttons defined in the head of file
				}
				else {
					buttons[i].push= 0x01;
				}
				changes.val= 0x1;
			}
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
	}
	timeout--;
}