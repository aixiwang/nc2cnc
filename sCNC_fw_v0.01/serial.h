#ifndef	_SERIAL_H
#define	_SERIAL_H
#include "typedef.h"

void			init_serial(void);
unsigned char 	hex_to_bin(char value);
char 			hexit(unsigned char value);
void 			wait_us(unsigned short delay);			 
void 			uart_tx(byte c);
byte 			uart_rx(void);
void uart_2hex( unsigned char value );
void put_dword(dword d1);

//byte	has_char(void);
//byte	can_send_char(void);
//byte			fifo_input(byte c);
//byte			fifo_output(byte* p);
//byte			ps2_out_fifo_input(byte c);
//byte			ps2_out_fifo_output(byte* p);
//unsigned char 	twobit_2_onebit(unsigned char value);
//bit 			rx_wait_fail(unsigned short delay, bit whilePinState);
//void			uart2_tx(unsigned short wA0A15, unsigned char bA16A23);
//unsigned char	uart2_rx(unsigned short* pAddr);
//void			uart2_2hex(unsigned char value);
//void			uart2_newline( void );
//void			uart2_string( char * pStr );
//unsigned char 	getchar2(void);
//unsigned char 	uart2_has_char(void);
void debug_led(void);

#endif	/* _SERIAL_H */



