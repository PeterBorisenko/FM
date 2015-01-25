/*
 * TEA5676.h
 *
 * Created: 23.01.2015 22:42:08
 *  Author: Disgust
 */ 


#ifndef TEA5676_H_
#define TEA5676_H_

//i2c Address and modes
#define TEA5676_ADDR	0b1100000
#define	RD				0x01
#define WR				0x00
#define TEA_R	((TEA5676_ADDR << 1)&RD)
#define TEA_W	((TEA5676_ADDR << 1)&WR)

// i2C Bytes count:1, 2, 3, 4, 5

// WRITE CONFIG
// 1-st data byte

/*	
*	if MUTE = 1 then L and R audio are muted
*	if MUTE = 0 then L and R audio are not muted
*/
#define MUTE	7

/*	
*	Search mode:
*	if SM = 1 then in search mode
*	if SM = 0 then not in search mode
*/
#define SM		6

/*	
*	PLL[13:8] 
*	setting of synthesizer programmable counter after search or preset
*/
#define PLL13	5
#define PLL12	4
#define PLL11	3
#define PLL10	2
#define PLL9	1
#define PLL8	0

// 2-nd data byte

/*	
*	PLL[7:0] 
*	setting of synthesizer programmable counter after search or preset
*/
#define PLL7	7
#define PLL6	6
#define PLL5	5
#define PLL4	4
#define PLL3	3
#define PLL2	2
#define PLL1	1
#define PLL0	0

// 3-rd data byte

/*	
*	Search Up/Down:
*	if SUD = 1 then search up
*	if SUD = 0 then search down
*/
#define SUD		7

/*	
*	SSL[1:0]
*	Search Stop Level
*	SSL1	:	SSL0				Search Stop Level
*	0			0					Not allowed in search mode
*	0			1					LOW; Level ADC Output= 5
*	1			0					MID; Level ADC Output= 7
*	1			1					HIGH; Level ADC Output= 10
*/
#define SSL1	6
#define SSL0	5

/*	
*	High/Low Side Injection:
*	if HLSI = 1 then high side LO injection
*	if HLSI = 0 then low side LO injection
*/
#define HLSI	4

/*	
*	Mono to Stereo:
*	if MS = 1 then forced mono
*	if MS = 0 then stereo ON
*/
#define MS		3

/*	
*	Mute Right:
*	if MR = 1 then the right audio channel is muted and forced mono
*	if MR = 0 then the right audio channel is not muted
*/
#define MR		2

/*	
*	Mute Left:
*	if ML = 1 then the left audio channel is muted and forced mono
*	if ML = 0 then the left audio channel is not muted
*/
#define ML		1

/*	
*	Software programmable port 1:
*	if SWP1 = 1 then port 1 is HIGH
*	if SWP1 = 0 then port 1 is LOW
*/
#define SWP1	0

// 4-th data byte

/*	
*	Software programmable port 2:
*	if SWP1 = 1 then port 2 is HIGH
*	if SWP1 = 0 then port 2 is LOW
*/
#define SWP2	7

/*	
*	Standby:
*	if STBY = 1 then in Standby mode
*	if STBY = 0 then not in Standby mode
*/
#define STBY	6

/*	
*	Band Limits:
*	if BL = 1 then Japanese FM band
*	if BL = 0 then US/Europe FM band
*/
#define BL		5

/*	
*	Clock frequency:
*	PLLREF	:	XTAL				Clock Frequency
*	0			0					13MHz
*	0			1					32.768KHz
*	1			0					6.5MHz
*	1			1					Not Allowed
*/
#define XTAL	4

/*	
*	Soft Mute:
*	if SMUTE = 1 then soft mute is ON
*	if SMUTE = 0 then soft mute is OFF
*/
#define SMUTE	3

/*	
*	High Cut Control:
*	if HCC = 1 then high cut control is ON
*	if HCC = 0 then high cut control is OFF
*/
#define HCC		2

/*	
*	Stereo Noise Cancelling:
*	if SNC = 1 then stereo noise cancelling is ON
*	if SNC = 0 then stereo noise cancelling is OFF
*/
#define SNC		1

/*	
*	Search Indicator:
*	if SI = 1 then pin SWPORT1 is output for the ready flag
*	if SI = 0 then pin SWPORT1 is software programmable port 1
*/
#define SI		0

// 5-th data byte

/*	
*	if PLLREF = 1 then the 6.5 MHz reference frequency for the PLL is enabled
*	if PLLREF = 0 then the 6.5 MHz reference frequency for the PLL is disabled
*	PLLREF	:	XTAL				Clock Frequency
*	0			0					13MHz
*	0			1					32.768KHz
*	1			0					6.5MHz
*	1			1					Not Allowed
*/
#define PLLREF	7

/*	
*	if DTC = 1 then the de-emphasis time constant is 75µs
*	if DTC = 0 then the de-emphasis time constant is 50µs
*/
#define DCT		6

// READ CONFIG
// 1-st data byte

/*	
*	Ready Flag:
*	if RF = 1 then a station has been found or the band limit has been reached
*	if RF = 0 then no station has been found
*/
#define RF		7

/*	
*	Band Limit Flag:
*	if BLF = 1 then the band limit has been reached
*	if BLF = 0 then the band limit has not been reached
*/
#define BLF		6

/*	
*	PLL[13:8] 
*	setting of synthesizer programmable counter after search or preset
*/
#define PLL13	5	//
#define PLL12	4
#define PLL11	3
#define PLL10	2
#define PLL9	1
#define PLL8	0

// 2-nd data byte

/*	
*	PLL[7:0] 
*	setting of synthesizer programmable counter after search or preset
*/
#define PLL7	7
#define PLL6	6
#define PLL5	5
#define PLL4	4
#define PLL3	3
#define PLL2	2
#define PLL1	1
#define PLL0	0

// 3-rd data byte

/*	
*	Stereo indication:
*	if STEREO = 1 then stereo reception
*	if STEREO = 0 then mono reception
*/
#define STEREO		7

/*	
*	IF[6:0]
*	IF counter result
*/
#define IF6		6
#define IF5		5
#define IF4		4
#define IF3		3
#define IF2		2
#define IF1		1
#define IF0		0

// 4-th data byte

/*	
*	LEV[3:0]
*	level ADC output
*/
#define LEV3	7
#define LEV2	6
#define LEV1	5
#define LEV0	4

/*	
*	CI[3:1]
*	Chip Identification:
*	these bits have to be set to logic 0
*/
#define CI3		3
#define CI2		2
#define CI1		1

// 5-th data byte
/*	
*	reserved for future extensions
*	these bits are internally set to logic 0
*/



#endif /* TEA5676_H_ */