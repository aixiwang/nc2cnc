#ifndef	M8_H
#define	M8_H

#include "typedef.h"

#define BIT_50MS_TASK		BIT0
#define BIT_1SEC_TASK		BIT1
#define BIT_UPDATE_LCD		BIT2
#define BIT_5MS_TASK 		BIT3
#define BIT_SPK				BIT4
#define BIT_PROCESS_CMD		BIT5

//#define FLASH_LED   		PORTD	^= BIT2    			/* Toggle P3.7 each time we print */
//#define FLASH_LED			UDR = 0x0f
void InputFromHostStateMachine(unsigned char c);
void port_init(void);
void watchdog_init(void);
void timer0_init(void);
void timer0_ovf_isr(void);
void uart0_init(void);
void uart0_rx_isr(void);
void init_devices(void);
void show_new_version(void);
void t5ms(void);
void ProcessCommand(void);
void x_steps(dword d1, byte direction);
void y_steps(dword d1, byte direction);
void z_steps(dword d1, byte direction);



#endif
