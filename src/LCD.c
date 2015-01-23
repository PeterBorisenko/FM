/*
 * LCD.c
 *
 * Created: 4/25/2014 10:06:24 PM
 *  Author: Peter
 */ 
 #include "LCD.h"

int LCD_Busy()
{
    uint8_t input= 0;
    control;
    readlcd;
	#ifdef LCD_8bit
		LCD_DREG&= ~(1 << LCD_D0)&~(1 << LCD_D1)&~(1 << LCD_D2)&~(1 << LCD_D3)&~(1 << LCD_D4)&~(1 << LCD_D5)&~(1 << LCD_D6)&~(1 << LCD_D7); // переключить пины на вход
		LCD_DPORT&= ~(1 << LCD_D0)&~(1 << LCD_D1)&~(1 << LCD_D2)&~(1 << LCD_D3)&~(1 << LCD_D4)&~(1 << LCD_D5)&~(1 << LCD_D6)&~(1 << LCD_D7); // очистить выход
	#else
		LCD_DREG&= ~(1 << LCD_D0)&~(1 << LCD_D1)&~(1 << LCD_D2)&~(1 << LCD_D3); // переключить пины на вход
		LCD_DPORT&= ~(1 << LCD_D0)&~(1 << LCD_D1)&~(1 << LCD_D2)&~(1 << LCD_D3); // очистить выход
	#endif
    
    enable;
    _delay_us(40);
    disable;
	#ifdef LCD_8bit
	input= LCD_IN;
	
	LCD_DPORT&= ~(1 << LCD_D0)&~(1 << LCD_D1)&~(1 << LCD_D2)&~(1 << LCD_D3)&~(1 << LCD_D4)&~(1 << LCD_D5)&~(1 << LCD_D6)&~(1 << LCD_D7);
	LCD_DREG|= (1 << LCD_D0)|(1 << LCD_D1)|(1 << LCD_D2)|(1 << LCD_D3)|(1 << LCD_D4)|(1 << LCD_D5)|(1 << LCD_D6)|(1 << LCD_D7);
	#else
	input= LCD_IN << 4;
	enable;
	_delay_us(40);
	disable;
	input|= LCD_IN;
	
	LCD_DPORT&= ~(1 << LCD_D0)&~(1 << LCD_D1)&~(1 << LCD_D2)&~(1 << LCD_D3);
	LCD_DREG|= (1 << LCD_D0)|(1 << LCD_D1)|(1 << LCD_D2)|(1 << LCD_D3);
	#endif
	if(BIT_read(input, 7))
	{
		return 1;
	}
    return 0;
}

 void LCD_Clear()
 {
    while(LCD_Busy());
    control;
    writelcd;
	#ifdef LCD_8bit
		BIT_write(LCD_DPORT, LCD_D7, 0);
		BIT_write(LCD_DPORT, LCD_D6, 0);
		BIT_write(LCD_DPORT, LCD_D5, 0);
		BIT_write(LCD_DPORT, LCD_D4, 0);
		BIT_write(LCD_DPORT, LCD_D3, 0);
		BIT_write(LCD_DPORT, LCD_D2, 0);
		BIT_write(LCD_DPORT, LCD_D1, 0);
		BIT_write(LCD_DPORT, LCD_D0, 1);
	#else
		BIT_write(LCD_DPORT, LCD_D3, 0);
		BIT_write(LCD_DPORT, LCD_D2, 0);
		BIT_write(LCD_DPORT, LCD_D1, 0);
		BIT_write(LCD_DPORT, LCD_D0, 0);
		enable;
		_delay_us(40);
		disable;
		BIT_write(LCD_DPORT, LCD_D3, 0);
		BIT_write(LCD_DPORT, LCD_D2, 0);
		BIT_write(LCD_DPORT, LCD_D1, 0);
		BIT_write(LCD_DPORT, LCD_D0, 1);
	#endif
    enable;
    _delay_us(40);
    disable;
    _delay_ms(2);
 }

 void LCD_Init()
 {
    _delay_ms(15);
    LCD_CREG= (1 << LCD_EN)|(1 << LCD_RS)|(1 << LCD_RW); // управл€ющие на выход
    while(LCD_Busy());
    control; 
    //enable;
	writelcd;
	#ifdef LCD_8bit // шина 8 бит, 2 строки, символ 5x8 точек
		BIT_write(LCD_DPORT, LCD_D3, 0);
		BIT_write(LCD_DPORT, LCD_D2, 0);
		BIT_write(LCD_DPORT, LCD_D1, 1);
		BIT_write(LCD_DPORT, LCD_D0, 1);
		enable;
		_delay_us(40);
		disable;
		while(LCD_Busy());
		control;
		BIT_write(LCD_DPORT, LCD_D3, 1);
		BIT_write(LCD_DPORT, LCD_D2, 0);
		BIT_write(LCD_DPORT, LCD_D1, 0);
		BIT_write(LCD_DPORT, LCD_D0, 0);
		enable;
		_delay_us(40);
		disable;
		while(LCD_Busy());
		//enable; // инкремент счетчика, сдвига экрана нет
		BIT_write(LCD_DPORT, LCD_D7, 0);
		BIT_write(LCD_DPORT, LCD_D6, 0);
		BIT_write(LCD_DPORT, LCD_D5, 0);
		BIT_write(LCD_DPORT, LCD_D4, 0);
		BIT_write(LCD_DPORT, LCD_D3, 0);
		BIT_write(LCD_DPORT, LCD_D2, 1);
		BIT_write(LCD_DPORT, LCD_D1, 1);
		BIT_write(LCD_DPORT, LCD_D0, 0);
	#else // шина 4 бита, 2 строки, символ 5x8 точек
		BIT_write(LCD_DPORT, LCD_D3, 0);
		BIT_write(LCD_DPORT, LCD_D2, 0);
		BIT_write(LCD_DPORT, LCD_D1, 1);
		BIT_write(LCD_DPORT, LCD_D0, 0);
		enable;
		_delay_us(40);
		disable;
		while(LCD_Busy());
		control;
		BIT_write(LCD_DPORT, LCD_D3, 1);
		BIT_write(LCD_DPORT, LCD_D2, 0);
		BIT_write(LCD_DPORT, LCD_D1, 0);
		BIT_write(LCD_DPORT, LCD_D0, 0);
		enable;
		_delay_us(40);
		disable;
		while(LCD_Busy());
		//enable; // инкремент счетчика, сдвига экрана нет
		BIT_write(LCD_DPORT, LCD_D3, 0);
		BIT_write(LCD_DPORT, LCD_D2, 0);
		BIT_write(LCD_DPORT, LCD_D1, 0);
		BIT_write(LCD_DPORT, LCD_D0, 0);
		enable;
		_delay_us(40);
		disable;
		while(LCD_Busy());
		control;
		BIT_write(LCD_DPORT, LCD_D3, 0);
		BIT_write(LCD_DPORT, LCD_D2, 1);
		BIT_write(LCD_DPORT, LCD_D1, 1);
		BIT_write(LCD_DPORT, LCD_D0, 0);
	#endif
    
    enable;
    _delay_us(40);
    disable;
    _delay_us(40);
    LCD_Clear();
 }

void LCD_SetCursor(uint8_t row, uint8_t col)
{
    uint8_t addr= (row * LCD_LENGTH + col); // конвертаци€ номера строки и столбца в адрес
    while(LCD_Busy());
    control; 
    //enable;
	writelcd;
	#ifdef LCD_8bit
		BIT_write(LCD_DPORT, LCD_D7, 1);
		BIT_write(LCD_DPORT, LCD_D6, (addr >> 6)&0x01);
		BIT_write(LCD_DPORT, LCD_D5, (addr >> 5)&0x01);
		BIT_write(LCD_DPORT, LCD_D4, (addr >> 4)&0x01);
		BIT_write(LCD_DPORT, LCD_D3, (addr >> 3)&0x01);
		BIT_write(LCD_DPORT, LCD_D2, (addr >> 2)&0x01);
		BIT_write(LCD_DPORT, LCD_D1, (addr >> 1)&0x01);
		BIT_write(LCD_DPORT, LCD_D0, addr&0x01);
	#else
		BIT_write(LCD_DPORT, LCD_D3, 1);
		BIT_write(LCD_DPORT, LCD_D2, (addr >> 6)&0x01);
		BIT_write(LCD_DPORT, LCD_D1, (addr >> 5)&0x01);
		BIT_write(LCD_DPORT, LCD_D0, (addr >> 4)&0x01);
		enable;
		_delay_us(40);
		disable;
		while(LCD_Busy());
		control;
		BIT_write(LCD_DPORT, LCD_D3, (addr >> 3)&0x01);
		BIT_write(LCD_DPORT, LCD_D2, (addr >> 2)&0x01);
		BIT_write(LCD_DPORT, LCD_D1, (addr >> 1)&0x01);
		BIT_write(LCD_DPORT, LCD_D0, addr&0x01);
	#endif                             
    enable;
    _delay_us(40);
    disable;
    _delay_us(40);
}

void LCD_Write(char * bytes, uint8_t sz, uint8_t posY, uint8_t posX )
{
    LCD_SetCursor(posY, posX);
    
    for (int i = sz - 1; i > 0; i--)
    {
        while(LCD_Busy());
        cli();
        data;
        //enable;
		writelcd;
		#ifdef LCD_8bit
			BIT_write(LCD_DPORT, LCD_D7, (*bytes >> 7)&0x01);
			BIT_write(LCD_DPORT, LCD_D6, (*bytes >> 6)&0x01);
			BIT_write(LCD_DPORT, LCD_D5, (*bytes >> 5)&0x01);
			BIT_write(LCD_DPORT, LCD_D4, (*bytes >> 4)&0x01);
			BIT_write(LCD_DPORT, LCD_D3, (*bytes >> 3)&0x01);
			BIT_write(LCD_DPORT, LCD_D2, (*bytes >> 2)&0x01);
			BIT_write(LCD_DPORT, LCD_D1, (*bytes >> 1)&0x01);
			BIT_write(LCD_DPORT, LCD_D0, *bytes&0x01);
		#else
			BIT_write(LCD_DPORT, LCD_D3, (*bytes >> 7)&0x01);
			BIT_write(LCD_DPORT, LCD_D2, (*bytes >> 6)&0x01);
			BIT_write(LCD_DPORT, LCD_D1, (*bytes >> 5)&0x01);
			BIT_write(LCD_DPORT, LCD_D0, (*bytes >> 4)&0x01);
			enable;
			_delay_us(40);
			disable;
			while(LCD_Busy());
			control;
			BIT_write(LCD_DPORT, LCD_D3, (*bytes >> 3)&0x01);
			BIT_write(LCD_DPORT, LCD_D2, (*bytes >> 2)&0x01);
			BIT_write(LCD_DPORT, LCD_D1, (*bytes >> 1)&0x01);
			BIT_write(LCD_DPORT, LCD_D0, *bytes&0x01);
		#endif
        enable;
        _delay_us(40);
        disable;
        sei();
        _delay_us(40);
        bytes++;
    }
    
}

void LCD_turnOn()
{
	
	LCD_Backlight(1);
	while(LCD_Busy());
	control; // включить экран, курсора нет
	//enable;
	writelcd;
	#ifdef LCD_8bit
		BIT_write(LCD_DPORT, LCD_D3, 1);
		BIT_write(LCD_DPORT, LCD_D2, 1);
		BIT_write(LCD_DPORT, LCD_D1, 1);
		BIT_write(LCD_DPORT, LCD_D0, 0);
		BIT_write(LCD_DPORT, LCD_D7, 0);
		BIT_write(LCD_DPORT, LCD_D6, 0);
		BIT_write(LCD_DPORT, LCD_D5, 0);
		BIT_write(LCD_DPORT, LCD_D4, 0);
		enable;
		_delay_us(40);
		disable;
		_delay_us(40);
		LCD_SetCursor(0, 0);
	#else
		BIT_write(LCD_DPORT, LCD_D3, 0);
		BIT_write(LCD_DPORT, LCD_D2, 0);
		BIT_write(LCD_DPORT, LCD_D1, 0);
		BIT_write(LCD_DPORT, LCD_D0, 0);
		enable;
		_delay_us(40);
		disable;
		while(LCD_Busy());
		control;
		BIT_write(LCD_DPORT, LCD_D3, 1);
		BIT_write(LCD_DPORT, LCD_D2, 1);
		BIT_write(LCD_DPORT, LCD_D1, 1);
		BIT_write(LCD_DPORT, LCD_D0, 0);
	#endif
	enable;
	_delay_us(40);
	disable;
	_delay_us(40);
	LCD_SetCursor(0, 0);
	
    
}

void LCD_turnOff()
{
	LCD_Backlight(0);
	while(LCD_Busy());
	control; // выключить экран, курсора нет
	//enable;
	writelcd;
	#ifdef LCD_8bit
		BIT_write(LCD_DPORT, LCD_D3, 1);
		BIT_write(LCD_DPORT, LCD_D2, 0);
		BIT_write(LCD_DPORT, LCD_D1, 1);
		BIT_write(LCD_DPORT, LCD_D0, 0);
		BIT_write(LCD_DPORT, LCD_D7, 0);
		BIT_write(LCD_DPORT, LCD_D6, 0);
		BIT_write(LCD_DPORT, LCD_D5, 0);
		BIT_write(LCD_DPORT, LCD_D4, 0);
	#else
		BIT_write(LCD_DPORT, LCD_D3, 0);
		BIT_write(LCD_DPORT, LCD_D2, 0);
		BIT_write(LCD_DPORT, LCD_D1, 0);
		BIT_write(LCD_DPORT, LCD_D0, 0);
		enable;
		_delay_us(40);
		disable;
		while(LCD_Busy());
		control;
		BIT_write(LCD_DPORT, LCD_D3, 1);
		BIT_write(LCD_DPORT, LCD_D2, 0);
		BIT_write(LCD_DPORT, LCD_D1, 1);
		BIT_write(LCD_DPORT, LCD_D0, 0);
	#endif
	enable;
	_delay_us(40);
	disable;
	_delay_us(40);
	LCD_DPORT&= ~(1 << LCD_D0)&~(1 << LCD_D1)&~(1 << LCD_D2)&~(1 << LCD_D3);
	
}

void LCD_Backlight(uint8_t state) {
	state?(BIT_set(LCD_CPORT, LCD_BKL)):(BIT_clear(LCD_CPORT, LCD_BKL));
}