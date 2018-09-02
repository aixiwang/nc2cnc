////////////////////////////////////////////////////////////////////////////////
// LCD1602
// Copyright(c) 2007 by Aixi.Wang, All rights reserved.
////////////////////////////////////////////////////////////////////////////////

#include <iom8v.h>
#include <macros.h>

#include "typedef.h"
#include "lcd1602.h"
//#include "main.h"


//--------------------------------------------------------------------------------
// LCD1602点阵字符型LCD模块Keil C51驱动程序
//--------------------------------------------------------------------------------  
  
byte bShowLcdState = 0;
unsigned char lcd_line1[16];
unsigned char lcd_line2[16];


static const unsigned char lcdlogo[32] = {
												' ',' ',' ',' ',' ','I','P','2','L','C','D',' ',' ',' ',' ',' ',
												' ',' ',' ',' ',' ','V','1','.','0','0',' ',' ',' ',' ',' ',' '};

void SetLCM_RS(unsigned char dat){
//PB4 LCM_RS (output)
	if (dat)
		PORTB |= BIT4;
	else
		PORTB &= ~BIT4;

}

void SetLCM_RW(unsigned char dat){
//PD5 LCM_RW   (output)

	if (dat)
		PORTD |= BIT5;
	else
		PORTD &= ~BIT5;


}

void SetLCM_EN(unsigned char dat){
//PB5 LCM_EN (output)

	if (dat)
		PORTB |= BIT5;
	else
		PORTB &= ~BIT5;

}

void SetLCM_DATA(unsigned char dat){
//PB0 LCM_D4 (output)
//PB1 LCM_D5 (output)
//PB2 LCM_D6 (output)
//PB3 LCM_D7 (output)
//
//PC0 LCM_D0 (output)
//PC1 LCM_D1 (output)
//PC2 LCM_D2 (output)
//PC3 LCM_D3 (output)

	byte c;

	c = dat >> 4;
	PORTB &= 0xf0;
	PORTB |= c;

	c = dat & 0x0f;
	PORTC &= 0xf0;
	PORTC |= c;


}

void delayus(void){
 int i;
 for (i=0;i<12;i++){};

}

//-----------------------
// delay5ms
//-----------------------
void delay5ms(void)
{
 word i,j;
 for(i=0; i<5; i++)
 for (j=0;j<100;j++)
 {
	 delayus();
 }
	 //wdt_reset();
	 //WDR();
 }


//-----------------------
// delay50us
//-----------------------
void delay50us(void)
{
 word i;
 for (i=0;i<50;i++)
 {	
	 delayus();
 }
//wdt_reset();
   //WDR();
}

//-----------------------
// wr_com
//-----------------------
void 
wr_com(unsigned char comm)
{

// 8bit
  LCDE_L;
  RS_L;
  RW_L;
  LCDE_H;
  SET_LCMDATA(comm);
  delay50us();
  LCDE_L;
  delay5ms();
  //delay50us();



}
//-----------------------
// wr_data
//-----------------------
void 
wr_data(unsigned char dat)
{
  LCDE_L;
  RW_L;
  RS_H;
  LCDE_H; 
  SET_LCMDATA(dat);
  delay50us();
  LCDE_L;
  RS_L;
  delay5ms();
  //delay50us();

}


//--------------------------
// init_lcd1602
//--------------------------
void init_lcd1602(void)
{
 unsigned char i;
 //char	init_logo[32];

 delay5ms();

 wr_com(0x38);	//设置显示模式:8位2行5x7点阵 	
 delay5ms();
 wr_com(0x38);	//设置显示模式:8位2行5x7点阵 	
 wr_com(0x38);	//设置显示模式:8位2行5x7点阵 
 wr_com(0x38);	//设置显示模式:8位2行5x7点阵 

// 8bit / 4bit
 wr_com(0x06);	//文字不动，光标自动右移
 wr_com(0x01);	//清屏并光标复位
 wr_com(0x0c);	//显示器开、光标关、光标允许闪烁关	

}

//-----------------------
// display_162
//----------------------- 
void display_162(void)
{
  unsigned char i;
  
  //WDR();
  wr_com(0x80);  			//set ram address, line1
  
  for(i=0;i<16;i++)
  {
   wr_data(lcd_line1[i]);  	//display "0"
   }

  wr_com(0xc0);				// dispaly line2

  for(i=0;i<16;i++)
  {
   wr_data(lcd_line2[i]);  	//display "0"
  }
}






