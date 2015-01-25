/*
 * TWI_Mega48.h
 *
 * Created: 25.01.2015 15:38:30
 *  Author: Disgust
 */ 


#ifndef TWI_MEGA48_H_
#define TWI_MEGA48_H_

#ifndef F_CPU
	#define F_CPU	8000000UL
#endif

// TWI Clock predefines
#define SCL_FREQ	400000UL
#define TWI_PSK		1
#define TWI_BR		((((F_CPU/SCL_FREQ)-16)/2)/TWI_PSK)

// TWI States




void i2cInit();
void i2cStart();
void i2cRestart();
void i2cStop();
uint8_t i2cError();
void i2cWrite(uint8_t);
void i2cRead(uint8_t *);
void i2cHandler();

#endif /* TWI_MEGA48_H_ */