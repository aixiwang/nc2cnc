#include <windows.h>
#include <stdio.h>
//#include "winio.h"
#include "stepmotor.h"
#include "typedef.h"
#include "subs.h"

dword step_delay1 = 500; 
dword step_delay2 = 500; 
dword step_delay3 = 500; 
dword bPortVal_378;

dword x_steps_per_mm = 80;
dword y_steps_per_mm = 266;
dword z_steps_per_mm = 160;


byte bCmdStr[16];
byte bCmdIdx;


long cur_x=0, cur_y=0, cur_z=0;
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
[07NNNN00]  - x +N step
[08NNNN00]  - x -N step
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

//-----------------------
// wait_uart_ok
//-----------------------

void
wait_uart_ok(void){

byte c;
dword dWaitOkTmout;
byte i;


#if 0
	return; //test
#else

//byte bCmdStr[16];
//byte bCmdIdx;
	c = 0;
	uart_rx(&c);


	myDelayUs(1000);
	while(1){

	dWaitOkTmout = 0;
	
	while( (c != '!') && (dWaitOkTmout<100) ) {

		dWaitOkTmout++;
		myDelayUs(1000);
		uart_rx(&c);	
		printf("........wait counter %d\r", dWaitOkTmout);

	};


	if (dWaitOkTmout>=100)
	{

		
		putchar('\n');
		for(i=0; i<10; i++)
		{	

			uart_tx(bCmdStr[i]);
			putchar(bCmdStr[i]);
		}
			uart_rx(&c);
			printf("\n retry ... ");

	}
	else
		return;

	}
#endif


}

//-----------------------
// xyz_enable
//-----------------------
void
xyz_enable(void){

//[02000000]  - open driver

	uart_tx('[');
	uart_tx('0');
	uart_tx('2');
	uart_tx('0');
	uart_tx('0');
	uart_tx('0');
	uart_tx('0');
	uart_tx('0');
	uart_tx('0');
	uart_tx(']');
	
	cur_x = 0;
	cur_y = 0;
	cur_z = 0;

	wait_uart_ok();

}

//-----------------------
// xyz_disable
//-----------------------
void
xyz_disable(void){
	// [01000000] close driver

	uart_tx('[');
	uart_tx('0');
	uart_tx('1');
	uart_tx('0');
	uart_tx('0');
	uart_tx('0');
	uart_tx('0');
	uart_tx('0');
	uart_tx('0');
	uart_tx(']');


	cur_x = 0;
	cur_y = 0;
	cur_z = 0;
	wait_uart_ok();
}

//-----------------------
// zmotor_enable
//-----------------------
void
zmotor_enable(void){


	uart_tx('[');
	uart_tx('0');
	uart_tx('3');
	uart_tx('0');
	uart_tx('0');
	uart_tx('0');
	uart_tx('0');
	uart_tx('0');
	uart_tx('0');
	uart_tx(']');


	//cur_x = 0;
	//cur_y = 0;
	//cur_z = 0;
	wait_uart_ok();
}

//-----------------------
// zmotor_disable
//-----------------------
void
zmotor_disable(void){


	uart_tx('[');
	uart_tx('0');
	uart_tx('4');
	uart_tx('0');
	uart_tx('0');
	uart_tx('0');
	uart_tx('0');
	uart_tx('0');
	uart_tx('0');
	uart_tx(']');


	//cur_x = 0;
	//cur_y = 0;
	//cur_z = 0;
	wait_uart_ok();

}

//---------------------
// x_go_um
//---------------------

void
x_go_um(dword um, byte direction){

dword i;
	


//	printf("x steps = %d\n",um * x_steps_per_mm/1000);

	i = (um * x_steps_per_mm/1000);

//-----------------------------------------------------------	
	uart_tx('[');
	uart_tx('0');
	if (direction)
		uart_tx('7');
	else
		uart_tx('8');

	//uart_tx( hexit((i >> 20)& 0x0f) );
	//uart_tx( hexit((i >> 16)& 0x0f) );
	uart_tx( hexit((i >> 4)& 0x0f) );
	uart_tx( hexit((i >> 0)& 0x0f) );
	uart_tx( hexit((i >> 12)& 0x0f) );
	uart_tx( hexit((i >> 8)& 0x0f) );

	uart_tx( '0');
	uart_tx( '0');
	uart_tx(']');
	wait_uart_ok();


	if (direction)
		cur_x += um;
	else
		cur_x -= um;
//-----------------------------------------------------------

}

//---------------------
// y_go_um
//---------------------

void
y_go_um(dword um, byte direction){

	dword i;


	 i = (um * y_steps_per_mm/1000);
//-----------------------------------------------------------	
	uart_tx('[');
	uart_tx('0');
	if (direction)
		uart_tx('A');
	else
		uart_tx('9');

	//uart_tx( hexit((i >> 20)& 0x0f) );
	//uart_tx( hexit((i >> 16)& 0x0f) );
	uart_tx( hexit((i >> 4)& 0x0f) );
	uart_tx( hexit((i >> 0)& 0x0f) );

	uart_tx( hexit((i >> 12)& 0x0f) );
	uart_tx( hexit((i >> 8)& 0x0f) );

	uart_tx( '0');
	uart_tx( '0');


	uart_tx(']');
	wait_uart_ok();



	if (direction)
		cur_y += um;
	else
		cur_y -= um;
}


//---------------------
// z_go_um
//---------------------

void
z_go_um(dword um, byte direction){

	dword i;


	i = (um * z_steps_per_mm/1000);
	//printf("z_go_um = %d\n", i);


//-----------------------------------------------------------	
	uart_tx('[');
	uart_tx('0');
	if (direction)
		uart_tx('C');
	else
		uart_tx('B');
	//uart_tx( hexit((i >> 20)& 0x0f) );
	//uart_tx( hexit((i >> 16)& 0x0f) );

	uart_tx( hexit((i >> 4)& 0x0f) );
	uart_tx( hexit((i >> 0)& 0x0f) );

	uart_tx( hexit((i >> 12)& 0x0f) );
	uart_tx( hexit((i >> 8)& 0x0f) );

	uart_tx( '0');
	uart_tx( '0');

	uart_tx(']');


//	printf("z go um 1\n");
	wait_uart_ok();



	if (direction)
		cur_z += um;
	else
		cur_z -= um;
}


//---------------------
// set_x_mm_pmin
//---------------------
void 
set_x_mm_pmin(dword mms){

dword i;

	step_delay1 = 60000000/(2*x_steps_per_mm*mms);
//	step_delay2 = 60000/(2*320*mms);	
//	step_delay3 = 60000/(2*160*mms);		
//-----------------------------------------------------------	

//	printf("step_delay_x = %dus/step\n", step_delay1*2);
	
	i = step_delay1 * 2;


	uart_tx('[');
	uart_tx('0');
	uart_tx('D');

	uart_tx( hexit((i >> 4)& 0x0f) );
	uart_tx( hexit((i >> 0)& 0x0f) );
	uart_tx( hexit((i >> 12)& 0x0f) );
	uart_tx( hexit((i >> 8)& 0x0f) );
	uart_tx( hexit((i >> 20)& 0x0f) );
	uart_tx( hexit((i >> 16)& 0x0f) );
	uart_tx(']');

#if 0
	putchar('[');
	putchar('0');
	putchar('D');
	putchar( hexit((i >> 4)& 0x0f) );
	putchar( hexit((i >> 0)& 0x0f) );
	putchar( hexit((i >> 12)& 0x0f) );
	putchar( hexit((i >> 8)& 0x0f) );

	putchar (hexit((i >> 20)& 0x0f) );
	putchar( hexit((i >> 16)& 0x0f) );

	putchar(']');
	putchar('\n');
#endif

	wait_uart_ok();


};


//---------------------
// set_y_mm_pmin
//---------------------
void 
set_y_mm_pmin(dword mms){
	dword i;
//	step_delay1 = 60000/(2*80*mms);
	step_delay2 = 60000000/(2*y_steps_per_mm*mms);	
//	step_delay3 = 60000/(2*160*mms);		
//	printf("step_delay_y = %dus/step\n", step_delay2*2);

//-----------------------------------------------------------	
	i = step_delay2 * 2;

	uart_tx('[');
	uart_tx('0');
	uart_tx('E');

	uart_tx( hexit((i >> 4)& 0x0f) );
	uart_tx( hexit((i >> 0)& 0x0f) );
	uart_tx( hexit((i >> 12)& 0x0f) );
	uart_tx( hexit((i >> 8)& 0x0f) );
	uart_tx( hexit((i >> 20)& 0x0f) );
	uart_tx( hexit((i >> 16)& 0x0f) );
	uart_tx(']');


#if 0
	putchar('[');
	putchar('0');
	putchar('E');
	putchar( hexit((i >> 4)& 0x0f) );
	putchar( hexit((i >> 0)& 0x0f) );
	putchar( hexit((i >> 12)& 0x0f) );
	putchar( hexit((i >> 8)& 0x0f) );

	putchar (hexit((i >> 20)& 0x0f) );
	putchar( hexit((i >> 16)& 0x0f) );

	putchar(']');
	putchar('\n');
#endif


	wait_uart_ok();
};


//---------------------
// set_z_mm_pmin
//---------------------
void 
set_z_mm_pmin(dword mms){
dword i;

///	step_delay1 = 60000/(2*80*mms);
//	step_delay2 = 60000/(2*320*mms);	
	step_delay3 = 60000000/(2*z_steps_per_mm*mms);
	
//	printf("step_delay_z = %dus/step\n", step_delay3*2);
	i = step_delay3 * 2;

	uart_tx('[');
	uart_tx('0');
	uart_tx('F');
	uart_tx( hexit((i >> 4)& 0x0f) );
	uart_tx( hexit((i >> 0)& 0x0f) );
	uart_tx( hexit((i >> 12)& 0x0f) );
	uart_tx( hexit((i >> 8)& 0x0f) );
	uart_tx( hexit((i >> 20)& 0x0f) );
	uart_tx( hexit((i >> 16)& 0x0f) );


	uart_tx(']');


#if 0
	putchar('[');
	putchar('0');
	putchar('F');
	putchar( hexit((i >> 4)& 0x0f) );
	putchar( hexit((i >> 0)& 0x0f) );
	putchar( hexit((i >> 12)& 0x0f) );
	putchar( hexit((i >> 8)& 0x0f) );

	putchar (hexit((i >> 20)& 0x0f) );
	putchar( hexit((i >> 16)& 0x0f) );

	putchar(']');
	putchar('\n');
#endif

	wait_uart_ok();

};



extern long drill_mm_pmin;
extern long travel_mm_pmin;

//---------------------
// goto_abs_axis_um
//---------------------
void 
goto_abs_axis_um(long abs_x, long abs_y, long abs_z){


	printf("goto abs axis: x=%d.%3d mm\t, y=%d.%3d mm\t, z=%d.%3d mm \t\n", abs_x/1000, abs_x%1000,abs_y/1000, abs_y%1000, abs_z/1000, abs(abs_z%1000)); 

	if (abs_z > 0)
	{
		// move z, x, y
		if (abs_z > cur_z)
			z_go_um( (abs_z - cur_z), 1);
		else
			z_go_um( (cur_z - abs_z), 0);


#if 0
		if (abs_z >= 100) // 0.1 mm, fast move
		{
			set_x_mm_pmin(travel_mm_pmin);
			set_y_mm_pmin(travel_mm_pmin);
			set_z_mm_pmin(travel_mm_pmin);
		}
		else
		{
			set_x_mm_pmin(drill_mm_pmin);
			set_y_mm_pmin(drill_mm_pmin);
			set_z_mm_pmin(drill_mm_pmin);
		}
#endif


		if (abs_x > cur_x)
			x_go_um( (abs_x - cur_x), 1);
		else
			x_go_um( (cur_x - abs_x), 0);	


		if (abs_y > cur_y)
			y_go_um( (abs_y - cur_y), 1);
		else
			y_go_um( (cur_y - abs_y), 0);

	}
	else
	{

#if 0
			set_x_mm_pmin(drill_mm_pmin);
			set_y_mm_pmin(drill_mm_pmin);
			set_z_mm_pmin(drill_mm_pmin);
#endif

		// move x, y, z
		if (abs_x > cur_x)
			x_go_um( (abs_x - cur_x), 1);
		else
			x_go_um( (cur_x - abs_x), 0);	


		if (abs_y > cur_y)
			y_go_um( (abs_y - cur_y), 1);
		else
			y_go_um( (cur_y - abs_y), 0);

		if (abs_z > cur_z)
			z_go_um( (abs_z - cur_z), 1);
		else
			z_go_um( (cur_z - abs_z), 0);
	
	}

};


//--------------------------
// goto_abs_pt_pt_um
//--------------------------
void
goto_abs_pt_pt_um(long x1,long y1, long x2, long y2){
long i,j;

	if ( (abs(x1 - x2) <= 100) || (abs(y1 - y2) <= 100) )
	{
		goto_abs_axis_um(x2, y2, cur_z);
		return;
	}

	printf("---------pt to pt x1=%d, y1=%d,              x2=%d, y2=%d\n", x1,y1,x2, y2);

		if (x2 > x1)
		{
			for(i=1; i<=(x2-x1)/100; i++)
			{
				goto_abs_axis_um( x1 + 100*i, y1 + i * 100 *(y2-y1)/(x2-x1), cur_z);	

			}
		}
		else
		{
			for(i=1; i<=(x1-x2)/100; i++)
			{
				goto_abs_axis_um( x1 - 100*i , y1 - i * 100 *(y2-y1)/(x2-x1), cur_z);	

			}
		}


		goto_abs_axis_um(x2,y2, cur_z);

	//getchar();
}



//--------------
// myDelay1Us
//--------------

void myDelay1Us(void){


	  LARGE_INTEGER litmp; 
       LONGLONG QPart1,QPart2;
       double dfMinus, dfFreq, dfTim; 
       QueryPerformanceFrequency(&litmp);
       dfFreq = (double)litmp.QuadPart;// 获得计数器的时钟频率
       QueryPerformanceCounter(&litmp);
       QPart1 = litmp.QuadPart;// 获得初始值
       do
       {
          QueryPerformanceCounter(&litmp);
          QPart2 = litmp.QuadPart;//获得中止值
          dfMinus = (double)(QPart2-QPart1);
         dfTim = dfMinus / dfFreq;// 获得对应的时间值，单位为秒
       }while(dfTim<0.000001);

}

//--------------
// myDelayUs
//--------------

void myDelayUs(long n_us){

	long i;

	for(i=0; i<n_us; i++)
		myDelay1Us();

}
