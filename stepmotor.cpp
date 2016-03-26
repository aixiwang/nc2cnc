//-------------------------------------------------
// Copyright by Aixi Wang (aixi.wang@hotmail.com)
//
//-------------------------------------------------


#include <windows.h>
#include <stdio.h>
//#include "winio.h"
#include <math.h>
#include "stepmotor.h"
#include "typedef.h"
#include "subs.h"
#include "aixicnc.h"

dword step_delay1 = 500; 
dword step_delay2 = 500; 
dword step_delay3 = 500; 
dword bPortVal_378;

//dword x_steps_per_mm = 80;
//dword y_steps_per_mm = 266;
//dword z_steps_per_mm = 160;
dword			x_um_per_circle = 20000;
dword			y_um_per_circle = 6000;
dword			z_um_per_circle = 10000;

dword			steps_per_circle = 1600;


dword xyz_delta = 50;
dword	timeout_cnt	= 5000;


byte bCmdStr[16];
byte bCmdIdx;


long cur_x=0, cur_y=0, cur_z=0;

extern byte NO_COM_SIMULATE_SUPPORT;

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


#if REAL_COM_NO_SCNC_SUPPORT == 1
	return; //test

#endif


if (NO_COM_SIMULATE_SUPPORT == 1)
	return; //test
else
{

//byte bCmdStr[16];
//byte bCmdIdx;
	c = 0;
	uart_rx(&c);


	myDelayUs(1000);
	while(1){

	dWaitOkTmout = 0;
	
//	while( (c != '!') && (dWaitOkTmout<1000) ) {		// 10 sec
	while( ( c!= '!') && (dWaitOkTmout < timeout_cnt) ) {
//while( c != '!' ) {		// no timeout
		dWaitOkTmout++;
		myDelayUs(1000);
		//uart_rx(&c);
		if (uart_has_char() ==0)
			printf("........wait counter %d\r", dWaitOkTmout);
		else
		{
			uart_rx(&c);
		}
	};


		return;

	}


} // no_com_simulate_support


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
	dword j;


//	i = (um * x_steps_per_mm/1000);
//	j = um - i*1000/x_steps_per_mm;		// error

	i = um * steps_per_circle/x_um_per_circle;
	j = um - i*x_um_per_circle/steps_per_circle;


if (NO_COM_SIMULATE_SUPPORT == 1)
	printf("x_go_um = %d, error = %d\n", i, j);

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
	{
		cur_x += um;
		cur_x -= j;
	}
	else
	{
		cur_x -= um;
		cur_x += j;
	}
//-----------------------------------------------------------

}

//---------------------
// y_go_um
//---------------------

void
y_go_um(dword um, byte direction){

	dword i;
	dword j;


//	i = (um * y_steps_per_mm/1000);
//	j = um - i*1000/y_steps_per_mm;		// error


	i = um * steps_per_circle/y_um_per_circle;
	j = um - i*y_um_per_circle/steps_per_circle;

if (NO_COM_SIMULATE_SUPPORT == 1)	
	printf("y_go_um = %d, error = %d\n", i, j);

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
	{
		cur_y += um;
		cur_y -= j;
	}
	else
	{
		cur_y -= um;
		cur_y += j;
	}
}


//---------------------
// z_go_um
//---------------------

void
z_go_um(dword um, byte direction){

	dword i;
	dword j;

//	i = (um * z_steps_per_mm/1000);	
//	j = um - i*1000/z_steps_per_mm;		// error

	i = um * steps_per_circle/z_um_per_circle;
	j = um - i*z_um_per_circle/steps_per_circle;


if (NO_COM_SIMULATE_SUPPORT == 1)
	printf("z_go_um = %d, error = %d\n", i, j);


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
	{
		cur_z += um;
		cur_z -= j;
	}
	else
	{
		cur_z -= um;
		cur_z += j;
	}
}


//---------------------
// set_x_mm_pmin
//---------------------
void 
set_x_mm_pmin(dword mms){

	dword i;

	step_delay1 = 60000000/(2*(steps_per_circle*1000/x_um_per_circle)*mms);
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
	step_delay2 = 60000000/(2*(steps_per_circle*1000/y_um_per_circle)*mms);	
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
	step_delay3 = 60000000/(2*(steps_per_circle*1000/z_um_per_circle)*mms);
	
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

float f1,f2,f3;

f1 = abs_x;
f2 = abs_y;
f3 = abs_z;

if (NO_COM_SIMULATE_SUPPORT == 1)
	puts("---------------------------------------------------------------------");

if (NO_COM_SIMULATE_SUPPORT == 1)
	printf("goto abs axis: x=%.3f mm\t, y=%.3f mm\t, z=%.3f mm \n", f1/1000, f2/1000, f3/1000); 
else
	printf("goto abs axis: x=%.3f mm\t, y=%.3f mm\t, z=%.3f mm \r", f1/1000, f2/1000, f3/1000); 

	if (abs_z > 0)
	{
		// move z, x, y
		if (abs_z > cur_z)
			z_go_um( (abs_z - cur_z), 1);
		else
			z_go_um( (cur_z - abs_z), 0);


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

if (NO_COM_SIMULATE_SUPPORT == 1)
	printf("********cur abs axis: x=%d.%3d mm\t, y=%d.%3d mm\t, z=%d.%3d mm \t\n", cur_x/1000, cur_x%1000,cur_y/1000, cur_y%1000, cur_z/1000, abs(cur_z%1000)); 


#if 0
	if (NO_COM_SIMULATE_SUPPORT == 1)
	{
		printf("simulation mode, any key to next step ...\n");
		getchar();
	}

#endif

};


//--------------------------
// goto_abs_pt_pt_um
//--------------------------
void
goto_abs_pt_pt_um(long x1,long y1, long x2, long y2){
	long i,j;

if (NO_COM_SIMULATE_SUPPORT == 1)
	printf("goto_abs_pt_pt_um, abs(x1-x2)=%d, abs(y1-y2)=%d\n",abs(x1 - x2), abs(y1 - y2));

	if ( (abs(x1 - x2) <= xyz_delta) || (abs(y1 - y2) <= xyz_delta) )
	{
		goto_abs_axis_um(x2, y2, cur_z);
		return;
	}


	printf("=== pt to pt x1=%8d, y1=%8d,              x2=%8d, y2=%8d\n", x1,y1,x2, y2);

		if (x2 > x1)
		{
			for(i=1; i<=(x2-x1)/xyz_delta; i++)
			{
				goto_abs_axis_um( x1 + xyz_delta*i, y1 + i * xyz_delta *(y2-y1)/(x2-x1), cur_z);	

			}
		}
		else
		{
			for(i=1; i<=(x1-x2)/xyz_delta; i++)
			{
				goto_abs_axis_um( x1 - xyz_delta*i , y1 - i * xyz_delta *(y2-y1)/(x2-x1), cur_z);	

			}
		}


		goto_abs_axis_um(x2,y2, cur_z);

	//getchar();
}


//--------------------------
// goto_abs_pt_pt_um_3d
//--------------------------
void 
goto_abs_pt_pt_um_3d(long x1,long y1, long z1, long x2,long y2, long z2){
	long i,j, k,d;
	byte btemp = 0;
	float f1,f2,f3, f4, f5, f6, f7;
	int x_flag, y_flag, z_flag;
	double f_ztemp = 0;

	x_flag = 0;
	y_flag = 0;
	z_flag = 0;
	f1 = x1; f2 = y1; f3 = z1;
	f4 = x2; f5 = y2; f6 = z2;



	if (NO_COM_SIMULATE_SUPPORT == 1)
		printf("===goto_abs_pt_pt_um_3d, pt2pt %.3f, %.3f, %.3f ===> %.3f, %.3f, %.3f<<<\n", f1/1000,f2/1000,f3/1000, f4/1000, f5/1000, f6/1000);

	if (abs(x1 - x2) <= xyz_delta)
	{
		btemp++;
		x_flag = 1;
	}
	if (abs(y1 - y2) <= xyz_delta)
	{
		btemp++;
		y_flag = 1;
	}
	if (abs(z1 - z2) <= xyz_delta)
	{
		btemp++;
		z_flag = 1;
	}

	if (btemp >= 2)
	{

	printf(">>>--- pt2pt %.3f, %.3f, %.3f ===> %.3f, %.3f, %.3f<<<\n", f1/1000,f2/1000,f3/1000, f4/1000, f5/1000, f6/1000);


	if (NO_COM_SIMULATE_SUPPORT == 1)
		printf("btemp = %d, x_flag =%d, y_flag  = %d, z_flag = %d\n", btemp, x_flag, y_flag, z_flag);

		if (x_flag == 0)
		{
			// delta x
			if ( x2 > x1)
			{

			if (NO_COM_SIMULATE_SUPPORT == 1)
				printf("delta x > \n");


			  for(i=x1; i<x2; i=i + xyz_delta)
				 goto_abs_axis_um(i, y2, z2);
			}
			else
			{

				if (NO_COM_SIMULATE_SUPPORT == 1)
					printf("delta x < \n");

				for(i=x1; i>x2; i=i - xyz_delta)
				 goto_abs_axis_um(i, y2, z2);
			}
		}
		else
		if (y_flag == 0)
		{
			// delta y
			if ( y2 > y1)
			{

				if (NO_COM_SIMULATE_SUPPORT == 1)
					printf("delta y > \n");

			  for(i=y1; i<y2; i=i + xyz_delta)
				 goto_abs_axis_um(x2, i, z2);
			}
			else
			{

				if (NO_COM_SIMULATE_SUPPORT == 1)
					printf("delta y < \n");

				for(i=y1; i>y2; i=i - xyz_delta)
				 goto_abs_axis_um(x2, i, z2);
			}
		}
		else
		if (z_flag == 0)
		{
			// delta z
			if ( z2 > z1)
			{

				if (NO_COM_SIMULATE_SUPPORT == 1)
					printf("delta z >, z1 = %d, z2 = %d \n", z1, z2);

			  for(i=z1; i<z2; i=i + xyz_delta)
				 goto_abs_axis_um(x2, y2, i);
			}
			else
			{

				if (NO_COM_SIMULATE_SUPPORT == 1)
					printf("delta y < \n");

			  for(i=z1; i>z2; i=i - xyz_delta)
				 goto_abs_axis_um(x2, y2, i);
			}
		}

		return;

	//	goto_abs_axis_um(x2, y2, z2);
	}

	f1 = x1; f2 = y1; f3 = z1;
	f4 = x2; f5 = y2; f6 = z2;

	printf(">>>--- pt2pt %.3f, %.3f, %.3f ===> %.3f, %.3f, %.3f<<<\n", f1/1000,f2/1000,f3/1000, f4/1000, f5/1000, f6/1000);


#if 1  // simple method
	
	long in_x,in_y,in_z;
	float x1_f,y1_f,z1_f;
	float x2_f,y2_f,z2_f;
	float temp_f;

	x1_f = x1; y1_f = y1; z1_f = z1;
	x2_f = x2; y2_f = y2; z2_f = z2;


	

		if (x2_f > x1_f)
		
		{
			for(i=1; i<=(x2_f-x1_f)/xyz_delta; i++)
			{
				d = (long)xyz_delta;

				//if (NO_COM_SIMULATE_SUPPORT == 1)
				//	printf("x2>x1,i=%d,%d,%d,%d,%d,%d,%d,%d,%d\n", i,x1,x2,y1,y2,xyz_delta,i * d *(y2-y1)/(x2-x1),y2-y1,x2-x1);


				temp_f = x1_f + d*i;
				in_x = (long)temp_f;

				temp_f = y1_f + i * d *(y2_f-y1_f)/(x2_f-x1_f);
				in_y = (long)temp_f;

				temp_f = z1_f + i * d *(z2_f-z1_f)/(x2_f-x1_f);
				in_z = (long)temp_f;

				goto_abs_axis_um(in_x,in_y,in_z);	


			}
		}
		else
		{
			for(i=1; i<=(x1_f-x2_f)/xyz_delta; i++)
			{
				d = (long)xyz_delta;

				//if (NO_COM_SIMULATE_SUPPORT == 1)
				//	printf("x2<x1,i=%d, x=%d,y=%d,z=%d\n", i,x1 - d*i,y1 - i * d *(y2-y1)/(x2-x1),z1 - i * d *(z2-z1)/(x2-x1));

				temp_f = x1_f - d*i;
				in_x = (long)temp_f;

				temp_f = y1_f - i * d *(y2_f-y1_f)/(x2_f-x1_f);
				in_y = (long)temp_f;

				temp_f = z1_f - i * d *(z2_f-z1_f)/(x2_f-x1_f);
				in_z = (long)temp_f;

				goto_abs_axis_um(in_x,in_y,in_z);	


			}
		}

#endif


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
