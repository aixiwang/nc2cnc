#include <windows.h>
#include <stdio.h>
//#include "winio.h"
#include "stepmotor.h"
#include "typedef.h"

dword step_delay1 = 500; 
dword step_delay2 = 500; 
dword step_delay3 = 500; 
dword bPortVal_378;

dword x_steps_per_mm = 80;
dword y_steps_per_mm = 266;
dword z_steps_per_mm = 160;


long cur_x=0, cur_y=0, cur_z=0;


//	  if (wPortAddr == 0x378)
//	  {
//=========================================		  
// 2008-07-02
// ----------------------------------------
// 0x378 bit6  -- y_enable#     pin 8
// 0x378 bit7  -- z_enable#     pin 9
// 0x37a bit1  -- x_enable       pin 14
//		  
//=========================================		  
#if 0
		  
		   b_temp = _inp(0x378);
		   b_temp &= ~0x40;          // bit6
		   b_temp &= ~0x80;      // bit7
		    _outp(0x378, b_temp);

		   _outp(0x37a, 0x02); // x enable

#endif



//-----------------------
// x_enable
//-----------------------

void
x_enable(void){

	dword bPortVal;
    GetPortVal(0x37a, &bPortVal, 1);
	bPortVal |= BIT1; // bit1, x enable
    SetPortVal(0x37a, bPortVal, 1);

}

//-----------------------
// x_disable
//-----------------------

void
x_disable(void){
	dword bPortVal;
    GetPortVal(0x37a, &bPortVal, 1);
	bPortVal &= ~BIT1; // bit1, x enable
    SetPortVal(0x37a, bPortVal, 1);
}

//-----------------------
// x_enable
//-----------------------

void
y_enable(void){
	dword bPortVal;
    GetPortVal(0x378, &bPortVal, 1);
	bPortVal &= ~BIT6; // bit6, y enable
    SetPortVal(0x378, bPortVal, 1);
}

//-----------------------
// x_disable
//-----------------------

void
y_disable(void){
	dword bPortVal;
    GetPortVal(0x378, &bPortVal, 1);
	bPortVal |= BIT6; // bit6, y enable
    SetPortVal(0x378, bPortVal, 1);
}

//-----------------------
// x_enable
//-----------------------

void
z_enable(void){
	dword bPortVal;
    GetPortVal(0x378, &bPortVal, 1);
	bPortVal &= ~BIT7; // bit7, z enable
    SetPortVal(0x378, bPortVal, 1);
}

//-----------------------
// x_disable
//-----------------------

void
z_disable(void){
	dword bPortVal;
    GetPortVal(0x378, &bPortVal, 1);
	bPortVal |= BIT7; // bit7, y enable
    SetPortVal(0x378, bPortVal, 1);


}



//-----------------------
// xyz_enable
//-----------------------
void
xyz_enable(void){
    SetPortVal(0x378, 0xff, 1);
    SetPortVal(0x37a, 0x00, 1);	

	x_enable();
	y_enable();
	z_enable();

	GetPortVal(0x378, &bPortVal_378, 1);

	cur_x = 0;
	cur_y = 0;
	cur_z = 0;

}

//-----------------------
// xyz_disable
//-----------------------
void
xyz_disable(void){

	x_disable();
	y_disable();
	z_disable();

//    SetPortVal(0x378, 0xff, 1);
//   SetPortVal(0x37a, 0x00, 1);	

	GetPortVal(0x378, &bPortVal_378, 1);

	cur_x = 0;
	cur_y = 0;
	cur_z = 0;

}

//    GetPortVal(0x378, &dwPortVal, 4);
//    SetPortVal(0x378, 10, 4);


//-----------------------
// input:
// 1. direction       -: 1, postive 0, negtive
//-----------------------
void
x_one_step(byte direction){


    bPortVal_378 &= ~BIT0;	// bit0, pin2 = x step
	SetPortVal(0x378, bPortVal_378, 1);
	//Sleep(step_delay1);
	myDelayUs(step_delay1);

    bPortVal_378 |= BIT0;	// bit0, pin2 = x step
    SetPortVal(0x378, bPortVal_378, 1);
	myDelayUs(step_delay1);


}

//-----------------------
// input:
// 1. direction       -: 1, postive 0, negtive
//-----------------------
void
y_one_step(byte direction){

//dword bPortVal;


	bPortVal_378 &= ~BIT2;	// bit2, pin3 = x step


	SetPortVal(0x378, bPortVal_378, 1);
	//Sleep(step_delay2);
	myDelayUs(step_delay2);

    bPortVal_378 |= BIT2;	// bit2, pin3 = y step
    SetPortVal(0x378, bPortVal_378, 1);

	myDelayUs(step_delay2);

}

//-----------------------
// input:
// 1. direction       -: 1, postive 0, negtive
//-----------------------
void
z_one_step(byte direction){

//dword bPortVal;



	bPortVal_378 &= ~BIT4;	// bit4, pin5 = z step


	SetPortVal(0x378, bPortVal_378, 1);
	//Sleep(step_delay3);
	myDelayUs(step_delay3);

    bPortVal_378 |= BIT4;	// bit4, pin5 = z step
    SetPortVal(0x378, bPortVal_378, 1);
	//Sleep(step_delay3);
	myDelayUs(step_delay3);

}

//---------------------
// x_go_um
//---------------------

void
x_go_um(dword um, byte direction){

dword i;
	
	if (direction)
		bPortVal_378 |= BIT1; // bit1, pin3  = x dir
	else
		bPortVal_378 &= ~BIT1; // bit1, pin3  = x dir

	SetPortVal(0x378, bPortVal_378, 1);

//	printf("x steps = %d\n",um * x_steps_per_mm/1000);

 for(i=0; i< (um * x_steps_per_mm/1000); i++)
	x_one_step(direction);

	if (direction)
		cur_x += um;
	else
		cur_x -= um;


}

//---------------------
// y_go_um
//---------------------

void
y_go_um(dword um, byte direction){

	int i,j;
	if (direction)
		bPortVal_378 &= ~BIT3; // bit3, pin4  = y dir
	else
		bPortVal_378 |= BIT3; // bit3, pin4  = y dir

	SetPortVal(0x378, bPortVal_378, 1);

	 j = (um * y_steps_per_mm/1000);
//	 printf("y steps = %d\n",j);
	 for(i=0; i< j; i++)
		y_one_step(direction);

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

	if (direction)
		bPortVal_378 &= ~BIT5; // bit5, pin6  = z dir
	else
		bPortVal_378 |= BIT5; // bit5, pin6  = z dir
	SetPortVal(0x378, bPortVal_378, 1);

//	printf("z steps = %d\n",um * z_steps_per_mm/1000);
	for(i=0; i< (um * z_steps_per_mm/1000); i++)
		z_one_step(direction);

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

	step_delay1 = 60000000/(2*x_steps_per_mm*mms);
//	step_delay2 = 60000/(2*320*mms);	
//	step_delay3 = 60000/(2*160*mms);		

//	printf("step_delay_x = %dus/step\n", step_delay1*2);
};


//---------------------
// set_y_mm_pmin
//---------------------
void 
set_y_mm_pmin(dword mms){

//	step_delay1 = 60000/(2*80*mms);
	step_delay2 = 60000000/(2*y_steps_per_mm*mms);	
//	step_delay3 = 60000/(2*160*mms);		
//	printf("step_delay_y = %dus/step\n", step_delay2*2);
};


//---------------------
// set_z_mm_pmin
//---------------------
void 
set_z_mm_pmin(dword mms){

///	step_delay1 = 60000/(2*80*mms);
//	step_delay2 = 60000/(2*320*mms);	
	step_delay3 = 60000000/(2*z_steps_per_mm*mms);
	
	//printf("step_delay_z = %dus/step\n", step_delay3*2);
};



extern long drill_mm_pmin;
extern long travel_mm_pmin;
//---------------------
// goto_abs_axis_um
//---------------------
void 
goto_abs_axis_um(long abs_x, long abs_y, long abs_z){



	//printf("goto abs axis: x=%d.%3d mm\t, y=%d.%3d mm\t, z=%d.%3d mm \t\n", abs_x/1000, abs_x%1000,abs_y/1000, abs_y%1000, abs_z/1000, abs_z%1000); 

	if (abs_z > 0)
	{
		// move z, x, y
		if (abs_z > cur_z)
			z_go_um( (abs_z - cur_z), 1);
		else
			z_go_um( (cur_z - abs_z), 0);



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
			set_x_mm_pmin(drill_mm_pmin);
			set_y_mm_pmin(drill_mm_pmin);
			set_z_mm_pmin(drill_mm_pmin);

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
