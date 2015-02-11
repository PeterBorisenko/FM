/*
 * Defines.h
 *
 * Created: 4/25/2014 10:17:40 PM
 *  Author: Peter
 */ 


#ifndef DEFINES_H_
#define DEFINES_H_

///константы///
#define MAX_TEMP 20.0
#define MIN_TEMP 15.0
#define MAX_TOL 1.0
#define MIN_TOL 0.0

///макросы///
#define BIT_set(x,y) x|=(1<<y)
#define BIT_setN(x,y,z) x|=(z<<y)
#define BIT_clear(x,y) x&=~(1<<y)
#define BIT_read(x,y) (((x)>>(y))&0x01)
#define BIT_write(x,y,z) ((z)?(BIT_setN(x,y,z)):(BIT_clear(x,y)))

#define HI(x) (x>>8)
#define LO(x) (x^0xFF)

#define MIN(a,b) ((a)<(b)?(a):(b))
#define MAX(a,b) ((a)>(b)?(a):(b))
#define CONSTRAIN(amt,low,high) ((amt)<(low)?(low):((amt)>(high)?(high):(amt)))
#define CIRCLE(amt, low, high) ((amt)<(low)?(high):((amt)>(high)?(low):(amt)))
#define ROUND(x) ((x)>=0?(long)((x)+0.5):(long)((x)-0.5))

#endif /* DEFINES_H_ */