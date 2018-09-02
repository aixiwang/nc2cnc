////////////////////////////////////////////////////////////////////////////////
// SERIAL
// Copyright(c) 2006 by Aixi.Wang, All rights reserved.
// V1.00 -- 02/01/2007

////////////////////////////////////////////////////////////////////////////////
#include <iom8v.h>
#include <macros.h>

#include "typedef.h"
#include "serial.h"
#include "main.h"


//-----------------
// init_serial
//-----------------
void
init_serial(void){


}


//-----------------
// wait_us
//-----------------
void wait_us(unsigned short delay)
{


}


#if 0		// remove the fifo code
//------------------------------------
// fifo_input
//------------------------------------

byte
fifo_input(byte c){
	ps2_data_buf[ps2_data_head] = c;
	ps2_data_head++;
	if (ps2_data_head>=32)
	{
		ps2_data_head = 0;
		return 0;
	}
	else
		return 1;
}

//------------------------------------
// fifo_output
//------------------------------------
byte			
fifo_output(byte* p){
	if (ps2_data_head == ps2_data_tail)
			return 0;

		*p = ps2_data_buf[ps2_data_tail];
		ps2_data_tail++;
		if (ps2_data_tail>=32)	
			ps2_data_tail = 0;
	
 	return 1;

}

//------------------------------------
// ps2_out_fifo_input
//------------------------------------
byte
ps2_out_fifo_input(byte c){
	ps2_out_buf[ps2_out_head] = c;
	ps2_out_head++;
	if (ps2_out_head>=32)
	{
		ps2_out_head = 0;
		return 0;
	}
	else
		return 1;
}

//------------------------------------
// ps2_out_fifo_output
//------------------------------------
byte			
ps2_out_fifo_output(byte* p){
	if (ps2_out_head == ps2_out_tail)
			return 0;

		*p = ps2_out_buf[ps2_out_tail];
		ps2_out_tail++;
		if (ps2_out_tail>=32)	
			ps2_out_tail = 0;
	
 	return 1;

}

#endif



//------------------
// uart_tx
//------------------
void 
uart_tx(byte c){

 word i;
 UDR = c;
 for(i=0; i<200; i++){};


}


//------------------
// uart_rx
//------------------
byte 
uart_rx(void){

	  return 0;
	  	
}



//--------------------------
// hex_to_bin
//--------------------------
unsigned char hex_to_bin( char value ) {

	unsigned char result;
	result =value - '0';
	if ( result > 9 ) result -= ('A' - '9' - 1);

	return( result );

}

//--------------------------
// hexit
//--------------------------

char hexit( unsigned char value ){

	char result;
	result = value & 0x0F;
	result += '0';
	if (result > '9') result += ('A' - '9' - 1);

	return( result );

}


//--------------------------
// uart_2hex
//--------------------------
void uart_2hex( unsigned char value ) {

	uart_tx( hexit( value >> 4 ) );
	uart_tx( hexit( value >> 0 ) );

}


#if 0
//--------------------------
// uart2_2hex
//--------------------------
void uart2_2hex( unsigned char value ) {

//	uart2_tx( hexit( value >> 4 ) );
//	uart2_tx( hexit( value >> 0 ) );

}


//--------------------------
// uart2_string
//--------------------------
void	uart2_string( char * pStr ) {
	while (*pStr) {
//		uart2_tx( *(pStr++) );
	}
}


//---------------
// uart_newline
//---------------
void uart2_newline( void ) {
//	uart2_tx(0x0a);
//	uart2_tx(0x0d);
}
#endif


void 
debug_led(void){
				
	PORTD	^= BIT5;


}

//------------------
// put_dword(
//------------------

void
put_dword(dword d1){

#if 0			
		uart_tx( hexit((d1 >> 28)& 0x0f) );
		uart_tx( hexit((d1 >> 24)& 0x0f) );
		uart_tx( hexit((d1 >> 20)& 0x0f) );
		uart_tx( hexit((d1 >> 16)& 0x0f) );
		uart_tx( hexit((d1 >> 12)& 0x0f) );
		uart_tx( hexit((d1 >> 8)& 0x0f) );
		uart_tx( hexit((d1 >> 4)& 0x0f) );
		uart_tx( hexit((d1 >> 0)& 0x0f) );
#endif		
}