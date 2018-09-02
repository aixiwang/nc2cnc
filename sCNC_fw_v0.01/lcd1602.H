#ifndef	LCD1602_H
#define	LCD1602_H

#include "typedef.h"

#define RS_H		SetLCM_RS(1) 
#define RS_L		SetLCM_RS(0) 

#define RW_H		SetLCM_RW(1) 
#define RW_L		SetLCM_RW(0) 

#define LCDE_H		SetLCM_EN(1) 
#define LCDE_L		SetLCM_EN(0) 
#define SET_LCMDATA(lcmdata) SetLCM_DATA(lcmdata)


void delay50us(void);
void delay5ms(void);
void SetLCM_DATA(unsigned char dat);
void SetLCM_RS(unsigned char dat);
void SetLCM_RW(unsigned char dat);
void SetLCM_EN(unsigned char dat);

void wr_com(unsigned char comm);
void wr_data(unsigned char dat);
void init_lcd1602(void);
void display_162(void);

extern unsigned char lcd_line1[16];
extern unsigned char lcd_line2[16];

extern byte bShowLcdState;

#endif
