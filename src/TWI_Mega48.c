/*
 * TWI_Mega48.c
 *
 * Created: 25.01.2015 15:39:05
 *  Author: Disgust
 */ 

#include <avr/io.h>
#include "TWI_Mega48.h"



void i2cInit() {
	TWBR= TWI_BR;
	TWSR|= (TWI_PSK << TWPS0);
}

void i2cStart() {
	TWCR|= (1 << TWINT)|(1 << TWSTA)|(1 << TWEN);
}

void i2cRestart() {
	
}

void i2cStop() {
	TWCR|= (1 << TWSTO);
}

void i2cWrite(uint8_t dat) {
	TWDR= dat;
	TWCR|= (1 << TWINT)|(1 << TWEN);
}

void i2cRead(uint8_t * byteToStore) {
	*byteToStore= TWDR;
}

uint8_t i2cError() {
	return 0;
}

void i2cHandler() {
	
}