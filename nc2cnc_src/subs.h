//-------------------------------------------------
// Copyright by Aixi Wang (aixi.wang@hotmail.com)
//
//-------------------------------------------------


#if !defined(SUBS_H)
#define SUBS_H

//#include "stdafx.h"
#include <stdio.h>
#include <windows.h>

#include "typedef.h"
//#include "py.h"




UINT	AlarmBeep(void *pArg);
int 	dump_ram_to_file(char * str_filename, byte * addr, dword bytes);
int 	read_file_to_ram(char * str_filename, byte * addr, dword bytes, dword* rel_bytes);
void 	print_256bs_hex (byte * p_byte_buffer, dword virtual_address);
byte 	InitSerial(char* str3);
byte	uart_has_char(void);
byte 	uart_rx(byte* pByte);
byte 	uart_tx(byte c);
void	delayms(dword i);
byte 	hex_to_bin( char value );
char 	hexit( unsigned char value );


word FindSubStr(char* pStr1, char* pStr2);




#endif