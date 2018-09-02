//ICC-AVR application builder : 2007-05-22 9:48:28
// Target : M8
// Crystal: 11.059Mhz

#include <iom8v.h>
#include <macros.h>

#include "typedef.h"
//#include "lcd1602.H"
#include "main.h"
#include "serial.h"
#include "drs.h"


byte	bInputFromHostState = 0;
byte	bPwrOnCnt=0;
byte	bTaskFlag;

byte	in_buffer[8];
byte	bComParam;
byte	bComValue;

dword   x_delay=11000/2;
dword   y_delay=11000/2;
dword   z_delay=11000/2;

byte bStopLoop = 0;

//const byte version[]="DRS-V107";


//--------------------------
// main
//--------------------------
void main(){
	dword i;
	byte spk_cnt=0;
	 
	init_devices();
	// init variables
	bInputFromHostState = 0;
	show_new_version();

	bPwrOnCnt = 0;	

	SetRlyAddWater(0);
	SetRlyAddHot(0);	
    SetSpk(1);
	//SetLedLwt(1);
   	//SetLedRun(1);
   	//SetLedPwr(1);
	//SetLedErr(1);	


   for(i=0; i<0x1ffff; i++)
   	WDR();
  

   SetAllOff();
   SetSpk(0);
      
   debug_led();  
   //DRS_Init();
   
   
 while (1) {
    WDR();
    
	if (( bTaskFlag & BIT_SPK) == BIT_SPK)
	{


	}
	///////////////////////////////////////////////////////////////////
	// 1 sec task hook 
	//////////////////////////////////////////////////////////////////


	if (( bTaskFlag & BIT_1SEC_TASK) == BIT_1SEC_TASK)
	{
		bTaskFlag &= ~BIT_1SEC_TASK;
		bPwrOnCnt++;
		//uart_tx('w');
		

//--------------------------------
// 1 sec task, here
//--------------------------------

		
 		debug_led();
		//ChkHotWaterTimeout();
		//ChkAddWaterTimeout();
		//ChkErrAlarmTimeout();
		
		//ChkAdValue();
	}
	
	///////////////////////////////////////////////////////////////////
	// 20ms task hook 
	//////////////////////////////////////////////////////////////////
	if (( bTaskFlag & BIT_5MS_TASK) == BIT_5MS_TASK)
	// 1 sec task
	{
		bTaskFlag &= ~BIT_5MS_TASK;

//--------------------------------
// 5 ms task, here
//--------------------------------

		t5ms();
		//ChkAdValue();
		//tDrsMain();
		//ChkOnOffKey();

		//ChkLowWater();		
		//ChkHighWater();
		//ChkHotWater();


		
	}

	/////////////////////////////////////////////////////////////////
	// Misc things to do
	/////////////////////////////////////////////////////////////////
	
	if (( bTaskFlag & BIT_PROCESS_CMD) == BIT_PROCESS_CMD)
	{
		
		ProcessCommand();
		
		
		bTaskFlag &= ~BIT_PROCESS_CMD;
	}
	/////////////////////////////////////////////////////////////////
	// Enter Idle
	/////////////////////////////////////////////////////////////////

  }
 
	 
}

//--------------------------
// port_init
//--------------------------
void port_init(void)
{
 PORTB = 0x00;
 DDRB  = BIT0+BIT1+BIT2+BIT3+BIT4+BIT5;
 PORTC = 0xff; 
 DDRC  = 0xff; 

			   
 PORTD = 0x02; // 0000 0010
 DDRD  = 0xa2;
               // 1010 0010
			   // pd1  tx, output
               // pd2  input dbg_key#
               // pd3  ad power 
			   // pd4  ad power 
			   // pd5  output dbg_led#
			   // pd7  output spk
			   
}



//--------------------------
// watchdog_init
//--------------------------
//Watchdog initialize
// prescale: 2048K
void watchdog_init(void)
{
 WDR(); //this prevents a timout on enabling
 WDTCR = 0x0F; //WATCHDOG ENABLED - dont forget to issue WDRs
}

 	   // 11.0592m clock

//TIMER0 initialize - prescale:256
// desired value: 5mSec
// actual value:  4.977mSec (0.5%)
void timer0_init(void)
{
 TCCR0 = 0x00; //stop
 TCNT0 = 0x29; //set count
 TCCR0 = 0x04; //start timer
}


//TIMER1 initialize - prescale:1
// WGM: 0) Normal, TOP=0xFFFF
// desired value: 1uSec
// actual value:  0.995uSec (0.5%)
void timer1_init(void)
{
 TCCR1B = 0x00; //stop
 TCNT1H = 0xFF; //setup
 TCNT1L = 0xF5;
 OCR1AH = 0x00;
 OCR1AL = 0x0B;
 OCR1BH = 0x00;
 OCR1BL = 0x0B;
 ICR1H  = 0x00;
 ICR1L  = 0x0B;
 TCCR1A = 0x00;
 TCCR1B = 0x01; //start Timer
}
//--------------------------
// 20ms timer
//--------------------------
#pragma interrupt_handler timer0_ovf_isr:10
void timer0_ovf_isr(void)
{
 static byte bT0SecCnt;

 TCNT0 = 0x29; //reload counter value

 if (bT0SecCnt < 200)
 	bT0SecCnt++;
 else
 {
 	bT0SecCnt = 0;
	bTaskFlag |= BIT_1SEC_TASK;
 }
 

	bTaskFlag |= BIT_5MS_TASK; 
}

#if 0
//UART0 initialize
// desired baud rate: 57600
// actual: baud rate:57599 (0.0%)
// char size: 8 bit
// parity: Disabled
void uart0_init(void)
{
 UCSRB = 0x00; //disable while setting baud rate
 UCSRA = 0x02;
 UCSRC = BIT(URSEL) | 0x06;
 UBRRL = 0x17; //set baud rate lo
 UBRRH = 0x00; //set baud rate hi
 UCSRB = 0x98;
}

#endif
#if 1
//UART0 initialize
// desired baud rate: 115200
// actual: baud rate:115198 (0.0%)
// char size: 8 bit
// parity: Disabled
void uart0_init(void)
{
 UCSRB = 0x00; //disable while setting baud rate
 UCSRA = 0x02;
 UCSRC = BIT(URSEL) | 0x06;
 UBRRL = 0x0B; //set baud rate lo
 UBRRH = 0x00; //set baud rate hi
 UCSRB = 0x98;
}
#endif

#if 0
//--------------------------
// uart0_init
//--------------------------
//UART0 initialize
// desired baud rate: 9600
// actual: baud rate:9600 (0.0%)
void uart0_init(void)
{
 UCSRB = 0x00; //disable while setting baud rate
 UCSRA = 0x00;
 UCSRC = BIT(URSEL) | 0x06;
 UBRRL = 0x47; //set baud rate lo
 UBRRH = 0x00; //set baud rate hi
 UCSRB = 0x98;
}
#endif

#if 0
//UART0 initialize
// desired baud rate: 9600
// actual: baud rate:9600 (0.0%)
// char size: 8 bit
// parity: Disabled
void uart0_init(void)
{
 UCSRB = 0x00; //disable while setting baud rate
 UCSRA = 0x00;
 UCSRC = BIT(URSEL) | 0x06;
 UBRRL = 0x47; //set baud rate lo
 UBRRH = 0x00; //set baud rate hi
 UCSRB = 0x00;
}
#endif




//--------------------------
// uart0_rx_isr
//--------------------------
#pragma interrupt_handler uart0_rx_isr:12
void uart0_rx_isr(void)
{
 //uart has received a character in UDR
 byte c;
 
 debug_led();
 
 c = UDR;
 //UDR = c;
 InputFromHostStateMachine(c);

}


//--------------------------
// init_devices
//--------------------------
//call this routine to initialize all peripherals
void init_devices(void)
{
 //stop errant interrupts until set up
 CLI(); //disable all interrupts
 port_init();
 //watchdog_init();
 timer0_init();
 timer1_init();
 uart0_init();
 //adc_init();

 MCUCR = 0x00;
 GICR  = 0x00;
 TIMSK = 0x01; //timer interrupt sources
 SEI(); //re-enable interrupts
 //all peripherals are now initialized
}









//---------------------
// show_new_version
//---------------------

void	
show_new_version(void)
{
 uart_tx('s');
 uart_tx('C');
 uart_tx('N');
 uart_tx('C');
 uart_tx('-');
 uart_tx('0');
 uart_tx('0');
 uart_tx('1');
 
		
		
}


//--------------------------
// 5ms task
// 5ms
//--------------------------
void t5ms(void)
{
 	



}

//==============================================================================
//                               sCNC Module
//==============================================================================

//--------------------------
// InputFromHostStateMachine
//--------------------------
void
InputFromHostStateMachine(unsigned char c){
	//unsigned short	bitsAddr1;		// 16bits address
	//unsigned char	bitsData1;		// 8bits data	

	switch (bInputFromHostState){
	case	0:
			if ( c == '[' )
				bInputFromHostState = 1;
			break;

	case	1:
	case	2:
	case	3:
	case	4:
	case	5:
	case	6:
	case	7:
	case	8:

			if ( c == '[' )
			{
				bInputFromHostState = 1;
				break;
			}
			if ( c == ']' )
			{
				bInputFromHostState = 0;
				break;
			}
			else
			if ( ((c <= '9') && (c >='0')) || ((c <= 'F') && (c >='A')) )
			{
				in_buffer[bInputFromHostState-1] =  c;	
				bInputFromHostState++;
			}

			else
				bInputFromHostState = 0;
			break;

	case	9:
			if ( c == ']' )
			{
			// send RF signal
			//EA = 0;
			bComParam = (hex_to_bin(in_buffer[0]) << 4) + hex_to_bin(in_buffer[1]) ;
			//bComValue = (in_buffer[2] << 4) + in_buffer[3];
			//ProcessCommand();
			
			if (bTaskFlag & BIT_PROCESS_CMD)
			   bStopLoop = 1;
			   
			
			bTaskFlag |=BIT_PROCESS_CMD;
			

			
			bInputFromHostState = 0;
			//EA = 1;
			//uart_tx(bComParam);

			}
			break;
			

		
	default:
				bInputFromHostState = 0;

	}

}
/*
serial port 9600, 8/n/1


host -> sCNC
--------------------------------------------------
[MMPPPPPP]
M,P -:   [0-9],[A-F]


[01000000]  - 关闭驱动
[02000000]  - 打开驱动
[03000000]  - 打开主轴电机
[04000000]  - 关闭主轴电机
[05000000]  - 打开排风电机
[06000000]  - 关闭排风电机
[07NNNN00]  - x -N step
[08NNNN00]  - x +N step
[09NNNN00]  - y +N step
[0ANNNN00]  - y -N step
[0BNNNN00]  - z +N step
[0CNNNN00]   -  z -N step

[0DNNNNN]   -  x us/step
[0ENNNNN]   -  y us/step
[0FNNNNN]   -  z us/step


sCNC -> host
---------------------------------------------------
OK  -- !

*/
//-------------------
// ProcessCommand
//-------------------
void
ProcessCommand(void){
	byte	i,b1,b2,b3;
	dword 	 d1;
	//float	 f1;
	
	switch(bComParam){

//[01000000]  - 关闭驱动

	//---------------------------------------------
	case 1:
	    SetLedPwr(0);
		uart_tx('!');	

	break;

//[02000000]  - 打开驱动

	//---------------------------------------------
	case 2:
	    SetLedPwr(1);
		uart_tx('!');
		
	//bTaskFlag |= BIT_UPDATE_LCD;	


	break;
//[03000000]  - 打开主轴电机
	//---------------------------------------------
	case 3:
	SetRlyAddWater(1);

		uart_tx('!');
		
	//bTaskFlag |= BIT_UPDATE_LCD;
		break;


//[04000000]  - 关闭主轴电机
//---------------------------------------------
	case 4:
	SetRlyAddWater(0);

		uart_tx('!');	
	//bTaskFlag |= BIT_UPDATE_LCD;
		break;
		
//[05000000]  - 打开排风电机
//---------------------------------------------
	case 5:
	SetRlyAddHot(1);	

		uart_tx('!');	
	//bTaskFlag |= BIT_UPDATE_LCD;
		break;
		
//[06000000]  - 关闭排风电机
//---------------------------------------------
	case 6:
	SetRlyAddHot(0);	

		uart_tx('!');	
	//bTaskFlag |= BIT_UPDATE_LCD;
		break;
		
		
//[07NNNN00]  - x +N step
//---------------------------------------------
	case 7:
		 b1 = (hex_to_bin(in_buffer[2]) << 4) + hex_to_bin(in_buffer[3]);
		 b2 = (hex_to_bin(in_buffer[4]) << 4) + hex_to_bin(in_buffer[5]);
		 
		 d1 = b1 + b2*256L;	 
		 x_steps(d1, 1);
		 
		uart_tx('!');	
#if 1		
	put_dword(d1);
#endif			
	//bTaskFlag |= BIT_UPDATE_LCD;
		break;
		
		
//[08NNNN00]  - x -N step
//---------------------------------------------
	case 8:
		 b1 = (hex_to_bin(in_buffer[2]) << 4) + hex_to_bin(in_buffer[3]);
		 b2 = (hex_to_bin(in_buffer[4]) << 4) + hex_to_bin(in_buffer[5]);
		 
		 d1 = b1 + b2*256L;	 
		 x_steps(d1, 0);

		uart_tx('!');	
#if 1		
	put_dword(d1);
#endif			
	//bTaskFlag |= BIT_UPDATE_LCD;
		break;
		
//[09NNNN00]  - y +N step
//---------------------------------------------
	case 9:
		 b1 = (hex_to_bin(in_buffer[2]) << 4) + hex_to_bin(in_buffer[3]);
		 b2 = (hex_to_bin(in_buffer[4]) << 4) + hex_to_bin(in_buffer[5]);
		 
		 d1 = b1 + b2*256L;	 
		 y_steps(d1, 1);
		uart_tx('!');	
#if 1		
	put_dword(d1);
#endif			
	//bTaskFlag |= BIT_UPDATE_LCD;
		break;
		
//[0ANNNN00]  - y -N step
//---------------------------------------------
	case 0x0a:
		 b1 = (hex_to_bin(in_buffer[2]) << 4) + hex_to_bin(in_buffer[3]);
		 b2 = (hex_to_bin(in_buffer[4]) << 4) + hex_to_bin(in_buffer[5]);
		 
		 d1 = b1 + b2*256L;	 
		 y_steps(d1, 0);
		uart_tx('!');	
#if 1		
	put_dword(d1);
#endif			
	//bTaskFlag |= BIT_UPDATE_LCD;
		break;
		
		
//[0BNNNN00]  - z +N step
//---------------------------------------------
	case 0x0b:
		 b1 = (hex_to_bin(in_buffer[2]) << 4) + hex_to_bin(in_buffer[3]);
		 b2 = (hex_to_bin(in_buffer[4]) << 4) + hex_to_bin(in_buffer[5]);
		 
		 d1 = b1 + b2*256L;	 
		 z_steps(d1, 1);

		uart_tx('!');	
#if 1		
	put_dword(d1);
#endif	
		
	//bTaskFlag |= BIT_UPDATE_LCD;
		break;
		
		
//[0CNNNN00]  - z -N step
//---------------------------------------------
	case 0x0c:
		 b1 = (hex_to_bin(in_buffer[2]) << 4) + hex_to_bin(in_buffer[3]);
		 b2 = (hex_to_bin(in_buffer[4]) << 4) + hex_to_bin(in_buffer[5]);
		 
		 d1 = b1 + b2*256L;	 
		 z_steps(d1, 0);	
		uart_tx('!');	
		
#if 1		
	put_dword(d1);
#endif		
	//bTaskFlag |= BIT_UPDATE_LCD;
		break;															


//[0DNNNNN]   -  x us/step
//---------------------------------------------
	 											   
	case 0x0d:
	 b1 = (hex_to_bin(in_buffer[2]) << 4) + hex_to_bin(in_buffer[3]);
		 b2 = (hex_to_bin(in_buffer[4]) << 4) + hex_to_bin(in_buffer[5]);
		 b3 = (hex_to_bin(in_buffer[6]) << 4) + hex_to_bin(in_buffer[7]);
	 d1 = b1 + b2*256L ;
	 d1 = d1 + b3*256*256L;	 
	 //d1 = d1 * (10000/2)/110592;
	 //f1 = d1;
	 //f1 = f1 * 10000/110592/2;
	 //d1 = (dword)f1;
	 x_delay = d1;
	
		uart_tx('!');

#if 1		
	put_dword(d1);
#endif

	//bTaskFlag |= BIT_UPDATE_LCD;
		break;	
//[0ENNNNN]   -  y us/step
//---------------------------------------------
	case 0x0e:
		 b1 = (hex_to_bin(in_buffer[2]) << 4) + hex_to_bin(in_buffer[3]);
		 b2 = (hex_to_bin(in_buffer[4]) << 4) + hex_to_bin(in_buffer[5]);
		 b3 = (hex_to_bin(in_buffer[6]) << 4) + hex_to_bin(in_buffer[7]);
	 d1 = b1 + b2*256L + b3*256*256L;	 
	 //d1 = d1 * 1000000/11059200 /2;
	 y_delay = d1;


		uart_tx('!');	
	//bTaskFlag |= BIT_UPDATE_LCD;
#if 1		
	put_dword(d1);
#endif

		break;	
		
//[0FNNNNN]   -  z us/step
//---------------------------------------------
	case 0x0f:
	 b1 = (hex_to_bin(in_buffer[2]) << 4) + hex_to_bin(in_buffer[3]);
		 b2 = (hex_to_bin(in_buffer[4]) << 4) + hex_to_bin(in_buffer[5]);
		 b3 = (hex_to_bin(in_buffer[6]) << 4) + hex_to_bin(in_buffer[7]);
	 d1 = b1 + b2*256L + b3*256*256L;	
	 //d1 = d1 * 1000000/11059200 /2;
	 z_delay = d1;
	

		uart_tx('!');	

#if 1		
	put_dword(d1);
#endif
	
	//bTaskFlag |= BIT_UPDATE_LCD;
		break;	
		
		
						
	default:
		break;
	}
}



void dly_t1_us(dword t1){
 dword i;
 word wcnt;
#if 0


 
 i1 = t1 & 0xffff; 
 TCCR1B = 0x00; //stop
 TCNT1H = 0x00; //setup
 TCNT1L = 0x00;
 TCCR1B = 0x01; //start
 
 if (i1 > 0xfff0)
 	i1 = 0xfff0;
	
 while( (TCNT1H * 256 + TCNT1L) < i1 )
 {
  		// wait while
		debug_led();
		
 }
 
 if ((t1 >> 16) > 0)
 {
  for(i=0; i<=(t1>>16); i++)
  {
   TCCR1B = 0x00; //stop
   TCNT1H = 0x00; //setup
   TCNT1L = 0x00;
   TCCR1B = 0x01; //stop   

   while( (TCNT1H * 256 + TCNT1L) < 0xfff0 )
   	  {
  		// wait while
		debug_led();
		
 	 }  
  }
 }
#endif
	  i = t1/2/2/2;
	  while(i--){
	  
	  };
}


// PC0 x clk
// PC1 x dir
// PC2 y clk
// PC3 y dir
// PC4 z clk
// PC5 z dir

//-----------------------------
// x_steps
//-----------------------------
void
x_steps(dword d1, byte direction){
			  
	   dword i;

	   if (direction)
	   	  PORTC |= BIT1;
	   else
  	   	  PORTC &= ~BIT1;
		  

	  for(i=0; i<d1; i++)
	  {
	  if (bStopLoop)
	  {
	   	  bStopLoop = 0;
	  	 return;
	  }	 	  
		  debug_led();  
	   PORTC &= ~BIT0;
	   dly_t1_us(x_delay);
	   PORTC |= BIT0;
	   dly_t1_us(x_delay);
	  }
}
//-----------------------------
// y_steps
//-----------------------------

void
y_steps(dword d1, byte direction){

	   dword i;

	   if (direction)
	   	  PORTC |= BIT3;
	   else
  	   	  PORTC &= ~BIT3;
		  

	  for(i=0; i<d1; i++)
	  {
	  if (bStopLoop)
	  {
	   	  bStopLoop = 0;
	  	 return;
	  }	 
	  debug_led();
	  
	   PORTC &= ~BIT2;
	   dly_t1_us(y_delay);
	   PORTC |= BIT2;
	   dly_t1_us(y_delay);
	  }

}
//-----------------------------
// z_steps
//-----------------------------

void
z_steps(dword d1, byte direction){
	   dword i;

	   if (direction)
	   	  PORTC |= BIT5;
	   else
  	   	  PORTC &= ~BIT5;
		  

	  for(i=0; i<d1; i++)
	  {
	  
	  if (bStopLoop)
	  {
	   	  bStopLoop = 0;
	  	 return;
	  }	 	  
	  debug_led();
	   PORTC &= ~BIT4;
	   dly_t1_us(z_delay);
	   PORTC |= BIT4;
	   dly_t1_us(z_delay);
	  }

}

