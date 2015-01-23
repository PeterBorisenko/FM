/*
 * TEA5676.h
 *
 * Created: 23.01.2015 22:42:08
 *  Author: Disgust
 */ 


#ifndef TEA5676_H_
#define TEA5676_H_

// i2C Bits count:1, 2, 3, 4, 5

// 1-st data byte
#define MUTE	7
#define SM		6
#define PLL13	5
#define PLL12	4
#define PLL11	3
#define PLL10	2
#define PLL9	1
#define PLL8	0

// 2-nd data byte
#define PLL7	7
#define PLL6	6
#define PLL5	5
#define PLL4	4
#define PLL3	3
#define PLL2	2
#define PLL1	1
#define PLL0	0

// 3-rd data byte
#define SUD		7
#define SSL1	6
#define SSL0	5
#define HLSI	4
#define MS		3
#define MR		2
#define ML		1
#define SWP1	0

// 4-th data byte
#define SWP2	7
#define STBY	6
#define BL		5
#define XTAL	4
#define SMUTE	3
#define HCC		2
#define SNC		1
#define SI		0

// 5-th data byte
#define PLLREF	7
#define DCT		6

#endif /* TEA5676_H_ */