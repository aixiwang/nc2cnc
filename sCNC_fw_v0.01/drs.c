////////////////////////////////////////////////////////////////////////////////
// DRS
// Copyright(c) 2008 by Aixi.Wang, All rights reserved.
// V1.00 -- 06/08/2008
// V1.02 -- 06/20/2008
// V1.03 -- 06/20/2008 change water detection method
//
// V1.04 
// ---------------------------------------------------
// 1. remove power off status
// 2. add hot,water relay debounce ( 500ms debounce )
// 3. modify err process
//
// V1.05 
// ---------------------------------------------------
// 1. improve hotwater relay control

// V1.06 
// ---------------------------------------------------
// 1. modify debounce timer to 300ms
// 2. fix relay issue
// 3. remove ChkHotWaterTimeout
// 4. 15VAC 处电流0.19A
// 5. 5V处 20mA-30mA

// V1.07
// ------------------------------------------------------
// 1. update hotwater debounce timer to 600ms

////////////////////////////////////////////////////////////////////////////////
#include <iom8v.h>
#include <macros.h>

#include "typedef.h"
#include "serial.h"
#include "main.h"

#include "drs.h"



byte bDrsStatus = 0;

word wAdValue[6];
// AD0 high water 1  -- short 1
// AD1 high water 2  -- short 2
// AD2 low water 1   -- long 1
// AD3 low water 2   -- long 2
// AD4 hot water out     -- 

// INPUT
// DB9 pin5  -- short 1
// DB9 pin4  -- short 2
// DB9 pin3 -- long 1
// DB9 pin2 -- long 2
// DB9 pin1 -- hot_water
// DB9 pin8 -- GND

// LEDs
//J8 pin 1   +5V
//J8 pin 2   GND
//J8 pin 3   
//J8 pin 4   NC
//J8 pin 5   power led
//J8 pin 6   low water led
//J8 pin 7   hotting led
//J8 pin 8   error led
//[pin1] ---|》---- [pin5,6,7,8]


word wHotWaterCnt = 0;
word wAddWaterCnt = 0;
word wErrAlarmCnt = 0;

byte bErrRetryCnt = 0;

byte bHotRetryCnt = 0;
byte bWaterRetryCnt = 0;


//=============================================================================
//                                     H A L
//=============================================================================
#define IN_HIGH_WATER_LIMIT_AD                  40*1024L/50
#define IN_LOW_WATER_LIMIT_AD                  40*1024L/50


//---------------------
// IsOverWater
// return: 0 -: no
//         1 -: yes
//         2 -: error
//---------------------

byte 
IsOverWater(void){

#if 1
				  if ( (wAdValue[0] < IN_HIGH_WATER_LIMIT_AD ) || ( wAdValue[1] < IN_HIGH_WATER_LIMIT_AD) )
				  	   				  return 1;
		          else
				  	   				  return 0;
#endif

#if 0
				  if ( (wAdValue[0] < IN_HIGH_WATER_LIMIT_AD ) )
				  	   				  return 1;
		          else
				  	   				  return 0;
#endif

}


//---------------------
// IsUnderWater
// return: 0 -: no
//         1 -: yes
//         2 -: error
//---------------------

byte
IsUnderWater(void){
#if 1
				  if ( (wAdValue[2] > IN_LOW_WATER_LIMIT_AD ) || ( wAdValue[3] > IN_LOW_WATER_LIMIT_AD) )
				  	   				  return 1;
				  else
				  	   				  return 0;

#endif


#if 0
				  if ( (wAdValue[2] > IN_LOW_WATER_LIMIT_AD ) )
				  	   				  return 1;
				  else
				  	   				  return 0;

#endif

}

//---------------------
// IsWaterOut
// return: 0 -: no
//         1 -: yes
//---------------------

byte
IsHotWaterFull(void){
	
					 
				  if ( wAdValue[4] < IN_LOW_WATER_LIMIT_AD )
				  	   				  return 1;
                  else
				  	  				  return 0;
                  									  
}
//---------------------
// GetRlyAddWaterSts
//---------------------
byte 
GetRlyAddWaterSts(void){
 return ((PORTB & BIT0) >> 0);

}

//---------------------
// SetRlyAddWater
// input: 1 -: contact
//        0 -: contactless
//---------------------
//word wHotWaterCnt = 0;
//word wAddWaterCnt = 0;
void
SetRlyAddWater(byte c){
	if (c)
	{
	    if (GetRlyAddWaterSts() == 0)
		   wAddWaterCnt = 1;
		         
		PORTB |= BIT0;
	}
	else
	{
		wAddWaterCnt = 0;
		PORTB &= ~BIT0;
		
	}
}


//---------------------
// GetRlyAddHotSts
//---------------------
byte 
GetRlyAddHotSts(void){
 return ((PORTB & BIT1) >> 1);

}
//---------------------
// SetRlyAddHot
// input: 1 -: contact
//        0 -: contactless
//---------------------
void
SetRlyAddHot(byte c){
	if (c)
	{
	   if (GetRlyAddHotSts() == 0)
	   	  wHotWaterCnt = 1;
		PORTB |= BIT1;
	}
	else
	{
	   	  wHotWaterCnt = 0;	
		PORTB &= ~BIT1;
	}	
}


//---------------------
// SetLedPwr
// input: 1 -: on
//        0 -: off
//---------------------

void
SetLedPwr(byte c){
	if (c)
		PORTB |= BIT2;
	else
		PORTB &= ~BIT2;
}

//---------------------
// SetLedLwt
// input: 1 -: on
//        0 -: off
//---------------------
void
SetLedLwt(byte c){
	if (c)
		PORTB |= BIT3;
	else
		PORTB &= ~BIT3;
}

//---------------------
// SetLedRun
// input: 1 -: on
//        0 -: off
//---------------------
void
SetLedRun(byte c){
	if (c)
		PORTB |= BIT4;
	else
		PORTB &= ~BIT4;
}


//---------------------
// SetLedErr
// input: 1 -: on
//        0 -: off
//---------------------
void
SetLedErr(byte c){
	if (c)
		PORTB |= BIT5;
	else
		PORTB &= ~BIT5;
}

//---------------------
// SetSpk
// input: 1 -: on
//        0 -: off
//---------------------
extern byte bTaskFlag;
void
SetSpk(byte c){
byte dly1;
word dly2;
	if (c)
	{
		PORTD |= BIT7;
	 	bTaskFlag |= BIT_SPK;	  
	}
	else
	{
		PORTD &= ~BIT7;
	 	bTaskFlag &= ~BIT_SPK;	  
	}
		
}


//---------------------
// SetAllOff
//---------------------
void
SetAllOff(void){
	SetSpk(0);
	//PORTB &= ~(BIT0+BIT1+BIT2+BIT3+BIT4+BIT5);
	SetRlyAddWater(0);
	SetRlyAddHot(0);
	SetLedPwr(0);
	SetLedLwt(0);
	SetLedRun(0);
	SetLedErr(0);

}
//---------------------
// TurnOnAdPwr
//---------------------
void 
TurnOnAdPwr(void){
	PORTD |= BIT3 + BIT4;

}
//---------------------
// TurnOffAdPwr
//---------------------
void 
TurnOffAdPwr(void){

	PORTD &= ~(BIT3 + BIT4);
}

//---------------------
// GetOnoffKeyPin
// output: 1 -: no pressing key
//         0 -: pressing key
//---------------------

byte 
GetOnoffKeyPin(void){
  return ((PIND & BIT2) >> 2);
}

void
SetErrState(void){
	   	  bDrsStatus = 2;
		  
		  DoErr();
}

//---------------------
// DoErr
// 
//---------------------
void   
DoErr(void){
	   
	   if (wErrAlarmCnt == 0)
	       wErrAlarmCnt = 1;
	   
	   SetRlyAddWater(0);
   	   SetRlyAddHot(0);
	   
	   TurnOffAdPwr();   
   	   SetLedPwr(1);
   	   SetLedLwt(0);
   	   SetLedRun(0);
	   SetLedErr(1);
  	   SetSpk(1);
}

#if 0

//=============================================================================
//                            Misc.
//=============================================================================
byte bChkKey1Cnt = 0;
byte bChkKey2Cnt = 0;
byte bChkKey3Cnt = 0;
byte bChkKey4Cnt = 0;

byte bMiscStatus = 0;
#define BIT_ONOFF_KEY BIT0
#define BIT_HOT_WATER BIT1
#define BIT_HIGH_WATER  BIT2
#define BIT_LOW_WATER  BIT3

//---------------------
// ChkOnOffKey
// Timebase: 5ms
//---------------------
void
ChkOnOffKey(void){
// high active
// step1 -- check change
	if ( (bMiscStatus & BIT_ONOFF_KEY) == BIT_ONOFF_KEY )
		// last state euqal 1
	{
		if (GetOnoffKeyPin() == 0)
		{	
			bChkKey1Cnt++;

            if (bChkKey1Cnt > 20)           // 100ms
			{	
				bChkKey1Cnt = 0;
				bMiscStatus &= ~BIT_ONOFF_KEY;
				
				//==========================================
				// press key
				//test
				debug_led();


		   }
		}	
		
	}

	else
	{
		if (GetOnoffKeyPin() == 1)			
		{				
			bChkKey1Cnt++;
           if (bChkKey1Cnt > 20) // release debounce 50 ms
			{	
              bMiscStatus |= BIT_ONOFF_KEY;        
              bChkKey1Cnt = 0;

			// release key
			//debug_led();

			}
	
		}
		
	}

}

//---------------------
// ChkHotWater
// Timebase: 5ms
//---------------------
void
ChkHotWater(void){
// high active
// step1 -- check change
	if ( (bMiscStatus & BIT_HOT_WATER) == BIT_HOT_WATER )
		// last state euqal 1
	{
		if (IsHotWaterFull() == 0)
		{	
			bChkKey2Cnt++;

            if (bChkKey2Cnt > 120)           // 600ms
			{	
				bChkKey2Cnt = 0;
				bMiscStatus &= ~BIT_HOT_WATER;
				
			// not hot water
		   }
		}	
		
	}

	else
	{
		if (IsHotWaterFull() == 1)			
		{				
			bChkKey2Cnt++;
           if (bChkKey2Cnt > 120) // release debounce 600 ms
			{	
              bMiscStatus |= BIT_HOT_WATER;        
              bChkKey2Cnt = 0;

			// hotwater full
		

			}
	
		}
		
	}

}

//---------------------
// ChkHighWater
// Timebase: 5ms
//---------------------
void
ChkHighWater(void){
// high active
// step1 -- check change
	if ( (bMiscStatus & BIT_HIGH_WATER) == BIT_HIGH_WATER )
		// last state euqal 1
	{
		if (IsOverWater() == 0)
		{	
			bChkKey3Cnt++;

            if (bChkKey3Cnt > 120)           // 600ms
			{	
				bChkKey3Cnt = 0;
				bMiscStatus &= ~BIT_HIGH_WATER;
				
				//==========================================
				// not over water

		   }
		}	
		
	}

	else
	{
		if (IsOverWater() == 1)			
		{				
			bChkKey3Cnt++;
           if (bChkKey3Cnt > 120) // release debounce 600 ms
			{	
              bMiscStatus |= BIT_HIGH_WATER;        
              bChkKey3Cnt = 0;

				//==========================================

				// over water



			}
	
		}
		
	}

}

//---------------------
// ChkLowWater
// Timebase: 5ms
//---------------------
void
ChkLowWater(void){
// high active
// step1 -- check change
	if ( (bMiscStatus & BIT_LOW_WATER) == BIT_LOW_WATER )
		// last state euqal 1
	{
		if (IsUnderWater() == 0)
		{	
			bChkKey4Cnt++;

            if (bChkKey4Cnt > 120)           // 600ms
			{	
				bChkKey4Cnt = 0;
				bMiscStatus &= ~BIT_LOW_WATER;
				
				//==========================================
				// not under water				
				//test
				//debug_led();

		   }
		}	
		
	}

	else
	{
		if (IsUnderWater() == 1)			
		{				
			bChkKey4Cnt++;
           if (bChkKey4Cnt > 120) // release debounce 600 ms
			{	
              bMiscStatus |= BIT_LOW_WATER;        
              bChkKey4Cnt = 0;

			 // under water
        	//debug_led();

			}

		}
		
	}

}

//---------------------------------
// GetADC
//---------------------------------

word GetADC(byte c){

	unsigned short value_1=0;
	unsigned short wTmout;

	byte btemp;
	
#if 1	
	PORTC = 0x00;
	DDRC  = 0x3f;
	DDRC &=~(1 << c);
#else
	PORTC = 0x00;
	DDRC  = 0x00;

#endif
		
	//adc转换初始化
	ADCSRA	= 0x00;	//禁止AD转换

    ADMUX=  0x40 |(c & 0x0f);//
	//ADMUX=  0x40;//
	SFIOR |= 0x00;

	ACSR  = 0x80;
 	ADCSR = 0xC7;
 
	wTmout = 0;

	while( ((ADCSRA & BIT4) == 0) && ((wTmout++) < 0x8000));	// bit4

	if (wTmout >= 0x8000)
	{
			ADCSRA|=0x00;
			bDrsStatus = 2; // indicate error 
			return 0xffff;
			
	}

	value_1=ADCL;		 //首先读低位
	value_1|=(unsigned short)ADCH << 8; //然后读高位

	ADCSRA|=0x00;

	return value_1;


}

//---------------------
// ChkAdValue
// Timebase: 5ms
//---------------------
void
ChkAdValue(void){
	static byte ad_index = 0;
	int i;
	TurnOnAdPwr();
	for(i=0; i<5; i++)
	//ad_index++;
	//if (ad_index > 4)
	//	ad_index = 0;	
	// wAdValue[ad_index] = GetADC(ad_index);		 
	{
	 wAdValue[i] = GetADC(i);
#if 0	
	uart_tx('[');
	uart_2hex(i);
	uart_tx('-');
	uart_2hex(wAdValue[i] >> 8);
	uart_2hex(wAdValue[i] & 0xff);
	uart_tx(']');
#endif
	 
	}
	
	TurnOffAdPwr();
}

//=============================================================================
//                            Main Control Logic
//=============================================================================
//---------------------
// tDrsMain
// -- called by mainloop
//---------------------

void
tDrsMain(void){
byte temp1,temp2, temp3;

  switch (bDrsStatus) {
  case 0: // off

	   //DoOff(); 	   
  	   //break;
	   
  case 1:
  
  	   DDRB  = BIT0+BIT1+BIT2+BIT3+BIT4+BIT5;
	   
       SetSpk(0);
   	   SetLedPwr(1);
	   SetLedErr(0);	


if ( (bMiscStatus & BIT_HIGH_WATER) == BIT_HIGH_WATER )
   	temp1 = 1;
else
	temp1 = 0;	
	
if ( (bMiscStatus & BIT_LOW_WATER) == BIT_LOW_WATER )		
   temp2 = 1;
else
	temp2 = 0;

if ( (bMiscStatus & BIT_HOT_WATER) == BIT_HOT_WATER )
   temp3 = 1;
else
	temp3 = 0;



	
	   //-----------------------------------------------------------
  	   //step1. check err
#if 1
       if ( (temp1 == 1) && (temp2 == 1) )
	   {

		  SetErrState();
		  break;  
  	   }  
	   
	   bErrRetryCnt = 0;  

#endif

	  
#if 1	   
	   // over water => add water relay 
	   if (temp1 == 1)
	   {
	   	 SetLedLwt(0);
	     SetRlyAddWater(0);
	   }
	   else
	   {
	   	 SetLedLwt(1);
	     SetRlyAddWater(1);
	   }	 
#endif
	   
#if 1
	   // not under water && hot water not full => add hot relay 
	   if ((temp2 == 0) && (temp3 == 0))
	   {
	     SetRlyAddHot(1);
		 SetLedRun(1);		 
	   }
	   else
	   {	   
	     SetRlyAddHot(0);
		 SetLedRun(0);		 
	   }
#endif

	    
	   
	   
			 		 	  	  	   	  
  	   break;
	     
  case 2: // err
	  DoErr();   
	  
  	   break;	   
	   
  default:
      DoOff();
  	  break;
  
  }


}


//---------------------
// DoOff
//---------------------

void
DoOff(void){
   bDrsStatus = 1;
#if 0
  	   	 SetAllOff();
		 //SetLedPwr(1);
		 TurnOffAdPwr(); 
		 wErrAlarmCnt = 0;
#endif

}		  

//word wHotWaterCnt = 0;
//word wAddWaterCnt = 0;
//---------------------
// ChkHotWaterTimeout
// timebase: 1sec
//---------------------
void
ChkHotWaterTimeout(void){
 if (wHotWaterCnt == 0)
 	return;
 if (wHotWaterCnt < 180)
 {
   wHotWaterCnt++;
   if (wHotWaterCnt >= 180) // 60 sec
   {
   //hotwater timeout
   //SetErrState();
   }
 }
}
//---------------------
// ChkAddWaterTimeout
// timebase: 1sec
//---------------------
void
ChkAddWaterTimeout(void){

 if (wAddWaterCnt == 0)
 	return;
 if (wAddWaterCnt < 180)
 {
   wAddWaterCnt++;
   if (wAddWaterCnt >= 180) // 180 sec
   {
   //add water timeout
   SetErrState();   
   }
 }

}

//---------------------
// ChkErrAlarmTimeout
// timebase: 1sec
//---------------------
void
ChkErrAlarmTimeout(void){

 if (wErrAlarmCnt == 0)
 	return;
 if (wErrAlarmCnt < 180)
 {
   wErrAlarmCnt++;
   if (wErrAlarmCnt >= 30) // 30 sec
   {
   //add water timeout
   //bDrsStatus = 0;
   //DoOff(); 
   wErrAlarmCnt = 0;  
   }
 }

}

void
DRS_Init(void){

	ChkAdValue();
		
	if (IsHotWaterFull() == 1)			
          bMiscStatus |= BIT_HOT_WATER;        
    else
          bMiscStatus &= ~BIT_HOT_WATER;        
				  
	if (IsOverWater() == 1)			
          bMiscStatus |= BIT_HIGH_WATER;        
    else
          bMiscStatus &= ~BIT_HIGH_WATER;        
			
	if (IsUnderWater() == 1)			
          bMiscStatus |= BIT_LOW_WATER;  
    else
          bMiscStatus &= ~BIT_LOW_WATER;        
	     
		
				  
}
#endif

