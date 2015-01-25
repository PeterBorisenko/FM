/*
 * TEA5676_.h
 *
 * Created: 25.01.2015 15:02:05
 *  Author: Disgust
 */ 


#ifndef TEA5676__H_
#define TEA5676__H_

//i2c Address and modes
#define TEA5676_ADDR	0b1100000
#define	RD				0x01
#define WR				0x00
#define TEA_R	((TEA5676_ADDR << 1)&RD)
#define TEA_W	((TEA5676_ADDR << 1)&WR)

// Data structures

// WRITE
typedef union {
	struct{
		uint8_t MUTE:1;
		uint8_t SM:1;
		uint8_t PLL:6;
	};
	uint8_t reg;
} Sbyte1_t;

typedef union {
	struct{
		uint8_t PLL:8;
	};
	uint8_t reg;
} Sbyte2_t;

typedef union {
	struct{
		uint8_t SUD:1;
		uint8_t SSL:2;
		uint8_t HLSI:1;
		uint8_t MS:1;
		uint8_t MR:1;
		uint8_t SWP1:1;
	};
	uint8_t reg;
} Sbyte3_t;

typedef union {
	struct{
		uint8_t SWP2:1;
		uint8_t STBY:1;
		uint8_t BL:1;
		uint8_t XTAL:1;
		uint8_t SMUTE:1;
		uint8_t HCC:1;
		uint8_t SNC:1;
		uint8_t SI:1;
	};
	uint8_t reg;
} Sbyte4_t;

typedef union {
	struct{
		uint8_t PLLREF:1;
		uint8_t DCT:1;
		const uint8_t res:6;
	};
	uint8_t reg;
} Sbyte5_t;

typedef struct {
	Sbyte1_t b1;
	Sbyte2_t b2;
	Sbyte3_t b3;
	Sbyte4_t b4;
	Sbyte5_t b5;
} Smessage_t;

// READ
typedef union {
	struct{
		uint8_t RF:1;
		uint8_t BLF:1;
		uint8_t PLL:6;
	};
	uint8_t reg;
} Rbyte1_t;

typedef union {
	struct{
		uint8_t PLL:8;
	};
	uint8_t reg;
} Rbyte2_t;

typedef union {
	struct{
		uint8_t STEREO:1;
		uint8_t IF:7;
	};
	uint8_t reg;
} Rbyte3_t;

typedef union {
	struct{
		uint8_t LEV:4;
		uint8_t CI:3;
	};
	uint8_t reg;
} Rbyte4_t;

typedef union {
	uint8_t reg;
} Rbyte5_t;

typedef struct {
	Rbyte1_t b1;
	Rbyte2_t b2;
	Rbyte3_t b3;
	Rbyte4_t b4;
	Rbyte5_t b5;
} Rmessage_t;

#endif /* TEA5676__H_ */